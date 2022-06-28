#include "stdafx.h"
#include "Buddy.h"

#include "Anais.h"
#include "Jelly.h"

#include "Player.h"
#include "ComboEffect.h"
#include "DamageEffect.h"

CBuddy::CBuddy()
{
}

CBuddy::~CBuddy()
{
}

void CBuddy::Release()
{
	if (nullptr != m_pCharacter)
		SafeDelete(m_pCharacter);
}

HRESULT CBuddy::Initialize()
{
	m_eLayer = LAYER_OBJ;

	m_fSpeed = 150.f;
	m_tMovingAlarm.SetAlarm(0.1f, false);
	m_tAttackAlarm.SetAlarm(0.1f, false);
	m_tHitAlarm.SetAlarm(0.15f, true);

	m_tDebuffDamage.SetAlarm(0.5f, true);
	m_tRevoceryAlarm.SetAlarm(1.f, true);
	m_tTalkAlarm.SetAlarm(0.1f, true);

	return S_OK;
}

HRESULT CBuddy::LateInit()
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

		if (!lstrcmp(m_wstrObjKey.c_str(), L"Anais"))
			SetCharacter(new CAnais);
		else if (!lstrcmp(m_wstrObjKey.c_str(), L"Jelly"))
			SetCharacter(new CJelly);
	}

	m_vStartPos = m_tInfo.vPos;

	return S_OK;
}

int CBuddy::Update()
{
	CGameObject::LateInit();

	m_bIsMouseTarget = false;

	if (m_bIsDead)
		return OBJ_DEAD;
	else
	{	
		if (!m_bIsFix)
		{
			if (m_bIsMove)
				CEventObject::AStarMove();
			else
				FindMonster();

			if (m_bIsAttack)
				Attack();
		}
	}

	DebuffUpdate();
	Recovery();

	CGameObject* pAttackEffect = nullptr;
	if (CCollisionMgr::CheckOneToN(m_tOBB, m_pObjMgr->GetObjList(OBJ_ATTACK), &pAttackEffect))
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
	else if (m_bIsHit)
	{
		m_bIsHit = false;
	}

	Hit();


	if (0.5f > float(m_tUnitData.iHp) / m_tUnitData.iMaxHp)
	{
		if (m_tTalkAlarm.CheckAlarm())
		{
			m_pUIMgr->InsertChat(m_wstrObjKey, L"젤리삐약! 살려삐약!", D3DCOLOR_ARGB(255, 255, 255, 255));
		}
	}

	return NO_EVENT;
}

int CBuddy::LateUpdate()
{
	CGameObject::UpdateView();
	FrameMove();

	return NO_EVENT;
}

void CBuddy::Render()
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

	// 타겟
	const TEX_INFO* pTexTarget = m_pTextureMgr->GetTexInfo(L"Effect", L"ObjectTarget", m_tObjectTarget.iFrameCur);
	if (nullptr != pTexTarget)
	{
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
		D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y - m_tOBB.vCollSize.y, 0.f);
		m_tInfo.matWorld = matScale * matTrans;
		m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
		m_pDeviceMgr->GetSprite()->Draw(pTexTarget->pTexture, nullptr,
			&pTexTarget->vOffset, nullptr, D3DCOLOR_ARGB(255, 128, 255, 128));
	}

	// 체력 바
	CEventObject::DrawHpBar();

	if (m_bIsMouseTarget) CEventObject::DrawNameBar();
}


void CBuddy::FrameOnceEnd()
{
	FindMonster();

	NULL_CHECK(m_pCharacter);
	m_pCharacter->SetIdleState();
}

void CBuddy::FrameEvent()
{
	NULL_CHECK(m_pCharacter);
	m_pCharacter->FrameEvent();
}

void CBuddy::AtarMoveEnd()
{
	m_tMovingAlarm.SetAlarm(1.f, false);
	m_bIsMove = false;

	FindMonster();

	NULL_CHECK(m_pCharacter);
	m_pCharacter->SetIdleState();
}

void CBuddy::AstarMoveSetState()
{
	NULL_CHECK(m_pCharacter);
	CGameObject::ChangeDirToAngle(m_fAngle);
	m_pCharacter->SetRunState();
}

bool CBuddy::AstartExitEvent()
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

void CBuddy::Attack()
{
	if (nullptr != m_pTarget)
	{
		if (m_pTarget->GetDead()) 
		{
			m_pTarget = nullptr;
			m_bIsActive = false;
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

void CBuddy::FindMonster()
{
	if (nullptr == m_pPlayer)
		m_pPlayer = m_pObjMgr->GetPlayer();

	m_vStartPos = m_pPlayer->GetInfo().vPos + D3DXVECTOR3{ float(rand()%TILECX*4 - TILECX * 2), float(rand() % TILECY * 4 - TILECY * 2), 0.f };

	if (CLIENTCX * 0.4f >= D3DXVec3Length(&(m_vStartPos - m_tInfo.vPos)))
	{
		if (nullptr == m_pTarget)
		{
			OBJLIST pMonsterList = m_pObjMgr->GetObjList(OBJ_MONSTER);
			for (auto& Monster : pMonsterList)
			{
				if (nullptr != Monster && 500.f >= D3DXVec3Length(&(Monster->GetInfo().vPos - m_tInfo.vPos)))
				{
					if (!m_bIsAttack)
					{
						m_pTileMgr->AStarStartPos(&m_lstBest, m_tInfo.vPos, Monster->GetInfo().vPos);
						m_bIsMove = true;
						m_bIsAttack = true;
						m_pTarget = Monster;
					}
				}
			}

			if (nullptr == m_pTarget)
			{
				m_bIsAttack = false;

				if (float(TILECX) <= D3DXVec3Length(&(m_vStartPos - m_tInfo.vPos)))
				{
					m_pTileMgr->AStarStartPos(&m_lstBest, m_tInfo.vPos, m_vStartPos);
					m_bIsMove = true;
				}
			}
		}
		else if (!m_pTarget->GetDead())
		{
			if (!m_bIsAttack)
			{
				m_pTileMgr->AStarStartPos(&m_lstBest, m_tInfo.vPos, m_pTarget->GetInfo().vPos);
				m_bIsMove = true;
				m_bIsAttack = false;
			}
		}
	}
	else
	{
		m_pTileMgr->AStarStartPos(&m_lstBest, m_tInfo.vPos, m_vStartPos);
		m_bIsMove = true;
		m_bIsAttack = false;
	}
}

void CBuddy::Hit()
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
						if (10 > iDebuffRand)
						{
							m_dwDebuff = pDamageData->dwDebuff;
						}
					}

					// 크리티컬
					int iCritical = rand() % 10;
					if (4 > iCritical)
					{
						pDamageData->iDamage += int(pDamageData->iDamage * (rand() % 3 + 2) * 0.1f);
						CGameObject* pEffect = CreateDamageEffect(m_tInfo.vPos + D3DXVECTOR3{ 0.f, -m_tOBB.vCollSize.y, 0.f }, 0, 0);
						dynamic_cast<CDamageEffect*>(pEffect)->SetDamageType(2);
					}

					pDamageData->iDamage -= (m_tUnitData.tStatInfo.iDef + m_tUnitData.tAddStatInfo.iDef);

					m_tUnitData.iHp -= pDamageData->iDamage;

					CreateDamageEffect(m_tInfo.vPos + D3DXVECTOR3{ 0.f, -m_tOBB.vCollSize.y, 0.f }, pDamageData->iDamage, pDamageData->iDamageType);
					
					if (0 >= m_tUnitData.iHp) {
						//m_bIsMove = false;
						//m_bIsAttack = false;
						m_tUnitData.iHp = 0;
						
						// 죽는 모션
						//m_bIsDead = true;
					}
					else {
						// 때린 대상 쫒아감
						CGameObject* pFrom = pDamageData->pFrom;
						if (nullptr != pFrom && !m_bIsAttack)
						{
							m_pTarget = pFrom;
							m_pTileMgr->AStarStartPos(&m_lstBest, m_tInfo.vPos, m_pTarget->GetInfo().vPos);
							m_bIsMove = true;
						}
					}
				}
				else
				{
					CreateDamageEffect(m_tInfo.vPos + D3DXVECTOR3{ 0.f, -m_tOBB.vCollSize.y, 0.f }, 0, 0);
				}
			}

			SafeDelete(pDamageData);
			m_lstDamageQue.pop_front();
		}
	}
}