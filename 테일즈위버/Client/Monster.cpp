#include "stdafx.h"
#include "Monster.h"

#include "DarkCow.h"

#include "Player.h"
#include "ComboEffect.h"
#include "DamageEffect.h"

CMonster::CMonster()
{
}

CMonster::~CMonster()
{
	Release();
}


HRESULT CMonster::Initialize()
{
	m_eLayer = LAYER_OBJ;

	m_fSpeed = 200.f;
	m_tMovingAlarm.SetAlarm(0.1f, false);
	m_tAttackAlarm.SetAlarm(0.1f, false);
	m_tHitAlarm.SetAlarm(0.15f, true);

	m_tDebuffDamage.SetAlarm(0.5f, true);

	CGameObject* pEffect = CreateEffect(m_tInfo.vPos, D3DXVECTOR3{ 0.f, 0.f, 0.f }, L"Effect", L"Casting", false, OBJ_EFFECT, this, nullptr, FRAME_ONCE, true);
	pEffect->SetLayer(LAYER_OBJ);

	m_tRevoceryAlarm.SetAlarm(1.f, true);
	m_tEffectDamage.SetAlarm(0.5f, false);

	return S_OK;
}

void CMonster::Release()
{
	if (nullptr != m_pCharacter)
		SafeDelete(m_pCharacter);
}

HRESULT CMonster::LateInit()
{
	const UNIT_INFO* pUnitInfo = m_pObjMgr->GetUnitData(m_wstrName);
	if (nullptr != pUnitInfo)
	{
		m_tUnitData = *pUnitInfo;

		m_tOBB.vPos = m_tInfo.vPos;
		m_tOBB.vCollSize = m_tUnitData.vCollEnd - m_tUnitData.vCollStart;
		m_tOBB.vOffset = -m_tUnitData.vCollStart;
		CGameObject::InitCollVertex();
	}

	if (nullptr != m_pCharacter)
		m_pCharacter->SetIdleState();
	else
	{
		m_wstrObjKey = m_tUnitData.wstrObjKey;
		m_wstrStateKey = m_tUnitData.wstrStateKey;
	}

	m_vStartPos = m_tInfo.vPos;

	return S_OK;
}

int CMonster::Update()
{
	CGameObject::LateInit();

	m_bIsMouseTarget = false;

	if (m_bIsDead)
		return OBJ_DEAD;
	else
	{
		if (m_bIsMove)
			CEventObject::AStarMove();
		else
			FindPlayer();

		if (m_bIsAttack)
			Attack();
	}

	CGameObject* pAttackEffect = nullptr;
	if (CCollisionMgr::CheckOneToN(m_tOBB, m_pObjMgr->GetObjList(OBJ_PLAYER_ATTACK), &pAttackEffect))
	{
		if (!m_bIsHit)
		{
			m_bIsHit = true;
			if (nullptr != pAttackEffect)
			{
				int iCount = dynamic_cast<CAttackEffect*>(pAttackEffect)->GetAttackCount();
				for (int i = 0; i < iCount; ++i)
				{
					InsertDamage(new DAMAGE_DATE(dynamic_cast<CAttackEffect*>(pAttackEffect)->GetDamage(), 1, 0, dynamic_cast<CAttackEffect*>(pAttackEffect)->GetFromObject()));
				}
			}
		}
	}
	else 
	{
		m_bIsHit = false;
	}

	DebuffUpdate();
	Recovery();

	Hit();



	return NO_EVENT;
}

int CMonster::LateUpdate()
{
	CGameObject::UpdateView();
	FrameMove();

	return NO_EVENT;
}

void CMonster::Render()
{
	m_pTexInfo = m_pTextureMgr->GetTexInfo(m_wstrObjKey, m_wstrStateKey, m_tFrame.iFrameCur);
	NULL_CHECK(m_pTexInfo);

	D3DXMATRIX matScale, matTrans;

	D3DXMatrixScaling(&matScale, m_tInfo.vSize.x * m_pTexInfo->m_iMirror, m_tInfo.vSize.y, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);
	m_tInfo.matWorld = matScale * matTrans;
	m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);

	DWORD dwColor = m_pTexInfo->dwBlend;

	CGameObject* pPlayer = m_pObjMgr->GetPlayer();
	if (nullptr != pPlayer)
	{
		if (m_tInfo.vPos.y >= pPlayer->GetInfo().vPos.y
			&& CCollisionMgr::CheckOneToOne(m_tOBB, m_pObjMgr->GetPlayer()->GetOBB()))
			dwColor = D3DCOLOR_ARGB(128, D3DCOLOR_GET_R(dwColor), D3DCOLOR_GET_G(dwColor), D3DCOLOR_GET_B(dwColor));
	}

	// 그림자
	const TEX_INFO* pShadowTexInfo = m_pTextureMgr->GetTexInfo(L"Effect", L"Shadow", 0);
	if (nullptr != pShadowTexInfo) {
		m_pDeviceMgr->GetSprite()->Draw(pShadowTexInfo->pTexture, nullptr,
			&pShadowTexInfo->vOffset, nullptr, D3DCOLOR_ARGB(D3DCOLOR_GET_A(dwColor), 0, 0, 0));
	}

	DWORD	dwblend = m_pTexInfo->dwBlend;
	if (m_dwDebuff & DEBUFF_FREZE)
		dwblend = D3DCOLOR_ARGB(D3DCOLOR_GET_A(dwColor), 127, 200, 255);
	if (m_dwDebuff & DEBUFF_POISION)
		dwblend = D3DCOLOR_ARGB(D3DCOLOR_GET_A(dwColor), 127, 255, 127);
	if (m_dwDebuff & DEBUFF_UNDEAD)
		dwblend = D3DCOLOR_ARGB(D3DCOLOR_GET_A(dwColor), 127, 127, 127);
	if (m_dwDebuff & DEBUFF_ELECTRIC_SHOCK)
		dwblend = D3DCOLOR_ARGB(D3DCOLOR_GET_A(dwColor), 255, 255, 127);
	
	// 렌더 이미지
	m_pDeviceMgr->GetSprite()->Draw(m_pTexInfo->pTexture, nullptr,
		&m_pTexInfo->vOffset, nullptr, dwblend);

	// 타겟 바
	if (m_bIsPlayerFind)
	{
		const TEX_INFO* pTexTarget = m_pTextureMgr->GetTexInfo(L"Effect", L"ObjectTarget", m_tObjectTarget.iFrameCur);
		if (nullptr != pTexTarget)
		{
			D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
			D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y - m_tOBB.vCollSize.y, 0.f);
			m_tInfo.matWorld = matScale * matTrans;
			m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
			m_pDeviceMgr->GetSprite()->Draw(pTexTarget->pTexture, nullptr,
				&pTexTarget->vOffset, nullptr, D3DCOLOR_ARGB(255, 255, 128, 128));
		}
	}

	// 체력 바
	CEventObject::DrawHpBar();

	if (m_bIsMouseTarget) CEventObject::DrawNameBar();
}


void CMonster::FrameOnceEnd()
{
	FindPlayer();

	NULL_CHECK(m_pCharacter);
	m_pCharacter->SetIdleState();
}

void CMonster::FrameEvent()
{
	NULL_CHECK(m_pCharacter);
	m_pCharacter->FrameEvent();
}

void CMonster::AtarMoveEnd()
{
	m_tMovingAlarm.SetAlarm(1.f, false);
	m_bIsMove = false;

	FindPlayer();

	NULL_CHECK(m_pCharacter);
	m_pCharacter->SetIdleState();
}

void CMonster::AstarMoveSetState()
{
	NULL_CHECK(m_pCharacter);
	m_pCharacter->SetRunState();
}

bool CMonster::AstartExitEvent()
{
	if (nullptr != m_pTarget)
	{
		float fDistance = D3DXVec3Length(&(m_pTarget->GetInfo().vPos - m_tInfo.vPos));

		if (fDistance < TILECX)
		{
			m_bIsMove = false;
			m_bIsAttack = true;
			return true;
		}
	}

	return false;
}

void CMonster::Attack()
{
	if (nullptr != m_pTarget)
	{
		if (m_pTarget->GetDead()) {
			m_pTarget = nullptr;
		}
		else
		{
			if (m_tAttackAlarm.CheckAlarm())
			{
				m_tAttackAlarm.SetAlarm(3.f, false);
				m_tInfo.vDir = m_pTarget->GetInfo().vPos - m_tInfo.vPos;
				m_fAngle = CGameObject::GetRadianAngle(m_tInfo.vDir);
				if (nullptr != m_pCharacter)
					m_pCharacter->SetAttackState();
			}
		}
	}
}

void CMonster::FindPlayer()
{
	// 몬스터가 시작 지점 범위 안일 때
	if (CLIENTCX * 0.4f >= D3DXVec3Length(&(m_vStartPos - m_tInfo.vPos)))
	{
		if (nullptr == m_pTarget)
			m_pTarget = m_pObjMgr->GetPlayer();

		// 플레이어를 탐색
		if (nullptr != m_pTarget && 300.f >= D3DXVec3Length(&(m_pTarget->GetInfo().vPos - m_tInfo.vPos)))
		{
			if (!m_bIsAttack)
			{
				m_pTileMgr->AStarStartPos(&m_lstBest, m_tInfo.vPos, m_pTarget->GetInfo().vPos);
				m_bIsMove = true;
				m_bIsPlayerFind = true;
			}
		}

		// 플레이어를 쫒고 있던 경우 쫒지 않고 시작지점으로 돌아간다.
		else if (m_bIsPlayerFind)
		{
			m_pTileMgr->AStarStartPos(&m_lstBest, m_tInfo.vPos, m_vStartPos);
			m_bIsMove = true;
			m_bIsPlayerFind = false;
			m_bIsAttack = false;
		}

		// 주변을 돌아다닌다.
		else if (!m_bIsPlayerFind)
		{
			if (m_tMovingAlarm.CheckAlarm())
			{
				D3DXVECTOR3 vRand = { float(rand() % TILECX * 4 - TILECX * 2), float(rand() % TILECY * 4 - TILECY * 2), 0.f };
				m_pTileMgr->AStarStartPos(&m_lstBest, m_tInfo.vPos, m_tInfo.vPos + vRand);
				m_bIsMove = true;
				m_bIsAttack = false;
			}
		}
	}
	// 몬스터가 시작 지점 범위 밖일 때
	else
	{
		// 플레이어를 쫒고 있던 경우 쫒지 않고 시작지점으로 돌아간다.
		m_pTileMgr->AStarStartPos(&m_lstBest, m_tInfo.vPos, m_vStartPos);
		m_bIsMove = true;
		m_bIsPlayerFind = false;
		m_bIsAttack = false;
	}
}

void CMonster::Hit()
{
	if (m_tHitAlarm.CheckAlarm())
	{
		if (!m_lstDamageQue.empty())
		{
			DAMAGE_DATE* pDamageData = m_lstDamageQue.front();

			if (nullptr != pDamageData && !m_bIsDead)
			{
				if (0 < pDamageData->iDamage) {
					if (STATE::IDLE == m_eCurState && nullptr != m_pCharacter)
					{
						m_pCharacter->SetHitState();
					}


					// 디버프
					if (0 != pDamageData->dwDebuff)
					{
						int iDebuffRand = rand() % 100;
						if (20 > iDebuffRand)
						{
							m_dwDebuff = pDamageData->dwDebuff;
							m_tDebuffAlarm.SetAlarm(10.f, false);
						}
					}


					int iCritical = rand() % 10;
					if (4 > iCritical)
					{
						pDamageData->iDamage += int(pDamageData->iDamage * (rand() % 3 + 2) * 0.1f);
						/*CGameObject* pEffect = */CreateDamageEffect(m_tInfo.vPos + D3DXVECTOR3{ 0.f, -m_tOBB.vCollSize.y, 0.f }, 0, 2);
						//dynamic_cast<CDamageEffect*>(pEffect)->SetDamageType(2);
					}

					pDamageData->iDamage -= (m_tUnitData.tStatInfo.iDef + m_tUnitData.tAddStatInfo.iDef);

					m_pSoundMgr->PlaySound(L"Collision.wav", CHANNEL_ID::CHANNEL_MONSTER_HIT);
					m_tUnitData.iHp -= pDamageData->iDamage;

					CreateDamageEffect(m_tInfo.vPos + D3DXVECTOR3{ 0.f, -m_tOBB.vCollSize.y, 0.f }, pDamageData->iDamage, pDamageData->iDamageType);

					// 콤보 이펙트
					{
						CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pObjMgr->GetPlayer());
						if (nullptr != pPlayer)
						{
							pPlayer->AddCombo(1);
							CGameObject* pInstance = CAbstractFactory<CComboEffect>::CreateObj(pPlayer->GetInfo().vPos - D3DXVECTOR3{ 0.f, 150.f, 0.f });
							if (nullptr != pInstance)
							{
								pInstance->SetObjID(OBJ_EFFECT);
								pInstance->SetFrameKey(L"Effect", L"HitNumber", EFFECT, NO_DIRECTION, FRAME_ONCE);
								pInstance->SetTarget(pPlayer);
								dynamic_cast<CComboEffect*>(pInstance)->SetFollow(true);
								dynamic_cast<CComboEffect*>(pInstance)->SetDamage(pPlayer->GetCombo());
								m_pObjMgr->AddObject(pInstance, OBJ_EFFECT);
							}
						}
					}

					if (0 >= m_tUnitData.iHp) {
						m_bIsMove = false;
						m_bIsAttack = false;
						m_tUnitData.iHp = 0;

						m_pUIMgr->AddExp(m_tUnitData.iExp + rand()%(m_tUnitData.iExp/10+1)-(m_tUnitData.iExp/20+1));

						wstring wstrItemName[5];
						for (int i = 0; i < 2; ++i)
						{
							int itemrand = rand() % 24;
							switch (itemrand)
							{
							case 0: wstrItemName[i] = L"Seed"; break;
							case 1: wstrItemName[i] = L"BlueHub"; break;
							case 2: wstrItemName[i] = L"BluePotion"; break;
							case 3: wstrItemName[i] = L"GreenHub"; break;
							case 4: wstrItemName[i] = L"RedHub"; break;
							case 5: wstrItemName[i] = L"RedPotion"; break;
							case 6: wstrItemName[i] = L"SmallRedPotion"; break;
							case 7: wstrItemName[i] = L"ReturnFeather"; break;
							case 8: wstrItemName[i] = L"HeavyArmor"; break;
							case 9: wstrItemName[i] = L"LightArmor"; break;
							case 10: wstrItemName[i] = L"ClradHat"; break;
							case 11: wstrItemName[i] = L"FabricHood"; break;
							case 12: wstrItemName[i] = L"BloodSword"; break;
							case 13: wstrItemName[i] = L"BlueAx"; break;
							case 14: wstrItemName[i] = L"DevilSword"; break;
							case 15: wstrItemName[i] = L"GreenRing"; break;
							case 16: wstrItemName[i] = L"RedRing"; break;
							case 17: wstrItemName[i] = L"LeatherShoes"; break;
							case 18: wstrItemName[i] = L"RedShoes"; break;
							case 19: wstrItemName[i] = L"LightSword	"; break;
							case 20: wstrItemName[i] = L"Mace"; break;
							case 21: wstrItemName[i] = L"MagicWand"; break;
							case 22: wstrItemName[i] = L"Sword"; break;
							case 23: wstrItemName[i] = L"Phone"; break;
							}

							CGameObject::CreateItem(wstrItemName[i].c_str(), m_tInfo.vPos, D3DXVECTOR3{ m_tInfo.vPos.x + float(rand() % TILECX * 2 - TILECX), m_tInfo.vPos.y + float(rand() % TILECY * 2 - TILECY), 0.f }, 1);
							//CGameObject::CreateItem(wstrItemName[i].c_str(), m_tInfo.vPos, D3DXVECTOR3{ m_tInfo.vPos.x + float(rand() % TILECX * 2 - TILECX), m_tInfo.vPos.y + float(rand() % TILECY * 2 - TILECY), 0.f }, 1);
						}
						

						// 죽는 모션
						m_bIsDead = true;
					}
					else {
						// 때린 대상 쫒아감
						CGameObject* pFrom = pDamageData->pFrom;
						if (nullptr != pFrom && !m_bIsAttack)
						{
							m_pTarget = pFrom;
							m_pTileMgr->AStarStartPos(&m_lstBest, m_tInfo.vPos, m_pTarget->GetInfo().vPos);
							m_bIsMove = true;
							m_bIsPlayerFind = true;
						}
					}
				}
				else
				{
					CreateDamageEffect(m_tInfo.vPos + D3DXVECTOR3{ 0.f, -m_tOBB.vCollSize.y, 0.f }, 0, 1);
				}
			}

			SafeDelete(pDamageData);
			m_lstDamageQue.pop_front();
		}
	}
}