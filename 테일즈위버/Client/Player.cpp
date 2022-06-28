#include "stdafx.h"
#include "Player.h"

#include "Kyoko.h"

#include "Item.h"
#include "Npc.h"
#include "Buddy.h"
#include "WarpGate.h"

#include "NormalEffect.h"
#include "AttackEffect.h"
#include "DamageEffect.h"

#include "SkillData.h"

#include "InvenWnd.h"
#include "ComboWnd.h"

CPlayer::CPlayer()
{
}

CPlayer::~CPlayer()
{
	Release();
}

HRESULT CPlayer::Initialize()
{
	m_eLayer = LAYER_OBJ;

	// 기준 스피드
	m_fSpeed = 300.f;
	m_pViewMgr->SetSpeed(-1.f);

	m_pSubject->AddData(CPlayerSubject::PLAYER_UNIT_DATA,	&m_tUnitData);
	m_pSubject->AddData(CPlayerSubject::PLAYER_IDENTITY,	&m_tIdentity);
	m_pSubject->AddData(CPlayerSubject::PLAYER_INFO,		&m_tInfo);

	m_tIdentity.bIsRun = true;

	m_tRevoceryAlarm.SetAlarm(0.05f, true);
	m_tHitAlarm.SetAlarm(0.15f, true);
	m_tDebuffDamage.SetAlarm(0.5f, true);

	m_tComboDelay.SetAlarm(0.1f, false);

	return S_OK;
}

HRESULT CPlayer::LateInit()
{
	const UNIT_INFO* pUnitInfo = m_pObjMgr->GetUnitData(m_wstrName);
	if (nullptr != pUnitInfo)
	{
		m_tUnitData			= *pUnitInfo;

		LoadGame(L"PlayerData");

		m_tOBB.vCollSize	= m_tUnitData.vCollEnd - m_tUnitData.vCollStart;
		m_tOBB.vOffset		= -m_tUnitData.vCollStart;
	}

	SafeDelete(m_pCharacter);

	if (!lstrcmp(m_tUnitData.wstrObjKey.c_str(), L"Kyoko"))
	{
		m_pCharacter = new CKyoko;
		m_pCharacter->SetBody(this);
	}
	else
	{
		m_wstrObjKey = m_tUnitData.wstrObjKey;
		m_wstrStateKey = m_tUnitData.wstrStateKey;
	}

	m_pSubject->Notify(CPlayerSubject::PLAYER_UNIT_DATA);
	m_pSubject->Notify(CPlayerSubject::PLAYER_IDENTITY);
	m_pSubject->Notify(CPlayerSubject::PLAYER_INFO);

	/*
	else if (!lstrcmp(m_wstrObjKey.c_str(), L"Anais"))
	{
		m_pCharacter = new CAnais;
		m_pCharacter->SetPlayer(this);
	}
	*/


	NULL_CHECK_MSG_RETURN(m_pCharacter, L"Character Create Failed", E_FAIL);

	m_eDirection = DIRECTION::LEFT_DOWN;
	m_pCharacter->SetIdleState();


	return S_OK;
}

int CPlayer::Update()
{
	CGameObject::LateInit();

	m_fSpeed = (m_tIdentity.bIsRun) ? 300.f : 200.f;
	
	if (m_bIsChat && m_tChatAlarm.CheckAlarm())
	{
		m_wstrChat = L"";
		m_bIsChat = false;
	}

	m_pUIMgr->AddEquipStat(&m_tUnitData.tAddStatInfo);

	KeyInput();

	if (STATE::SKILL == m_eCurState)
		m_bIsMove = false;

	if (m_bIsMove)
	{
		if (m_tStepAlarm.CheckAlarm())
		{
			m_tStepAlarm.SetAlarm(0.3f, false);
			m_pSoundMgr->PlaySound(L"footstep.wav", CHANNEL_ID::CHANNEL_WINDOW);
		}
		CEventObject::AStarMove();

		if (m_bIsAttack && nullptr == m_pTarget)
			FindMonster();

		if (m_tIdentity.bIsRun)
		{
			m_tUnitData.iSp -= 1;

			if (0 >= m_tUnitData.iSp)
				m_tIdentity.bIsRun = false;
		}
	}

	auto pTileVec = m_pTileMgr->GetTileVec();
	int iIndex = m_pTileMgr->GetTileIndex(m_tInfo.vPos);
	if (0 <= iIndex && iIndex < int(pTileVec->size()))
	{
		if (4 == (*pTileVec)[iIndex]->byOption)
		{
			m_pSceneMgr->SetNextScene(m_pSceneMgr->GetCurScene(), m_pSceneMgr->GetMapName());
		}
	}

	if (m_bIsAttack)
		Attack();

	DebuffUpdate();
	if (m_dwDebuff & DEBUFF_BLINED)
		m_pSceneMgr->SetEvent(EVENT_FADE_DARK, true);
	else
		m_pSceneMgr->SetEvent(EVENT_FADE_DARK, false);

	Recovery();
	Hit();

	return NO_EVENT;
}

int CPlayer::LateUpdate()
{
	m_pSubject->Notify(CPlayerSubject::PLAYER_UNIT_DATA);
	m_pSubject->Notify(CPlayerSubject::PLAYER_INFO);

	//if (!m_pSceneMgr->GetEvent(EVENT_ALL_STOP))
	m_pViewMgr->SetViewObj(m_tInfo.vPos);

	CGameObject::UpdateView();
	FrameMove();

	return NO_EVENT;
}

void CPlayer::Render()
{
	m_pTexInfo = m_pTextureMgr->GetTexInfo(m_wstrObjKey, m_wstrStateKey, m_tFrame.iFrameCur);
	NULL_CHECK(m_pTexInfo);

	D3DXMATRIX matScale, matTrans;

	D3DXMatrixScaling(&matScale, m_tInfo.vSize.x * m_pTexInfo->m_iMirror, m_tInfo.vSize.y, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);
	m_tInfo.matWorld = matScale * matTrans;
	m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);

	// 그림자
	const TEX_INFO* pShadowTexInfo = m_pTextureMgr->GetTexInfo(L"Effect", L"Shadow", 0);
	if (nullptr != pShadowTexInfo) {
		m_pDeviceMgr->GetSprite()->Draw(pShadowTexInfo->pTexture, nullptr,
			&pShadowTexInfo->vOffset, nullptr, D3DCOLOR_ARGB(255, 0, 0, 0));
	}

	DWORD	dwblend = m_pTexInfo->dwBlend;
	if (m_dwDebuff & DEBUFF_FREZE) 
	{
		dwblend = D3DCOLOR_ARGB(255, 127, 200, 255);
	}
	if (m_dwDebuff & DEBUFF_POISION) 
	{
		dwblend = D3DCOLOR_ARGB(255, 127, 255, 127);
	}
	if (m_dwDebuff & DEBUFF_UNDEAD) 
	{
		dwblend = D3DCOLOR_ARGB(255, 127, 127, 127);
	}
	if (m_dwDebuff & DEBUFF_ELECTRIC_SHOCK) 
	{
		dwblend = D3DCOLOR_ARGB(255, 255, 255, 127);
	}

	// 렌더 이미지
	m_pDeviceMgr->GetSprite()->Draw(m_pTexInfo->pTexture, nullptr,
		&m_pTexInfo->vOffset, nullptr, dwblend);

	const TEX_INFO* pTexTarget = m_pTextureMgr->GetTexInfo(L"Effect", L"ObjectTarget", m_tObjectTarget.iFrameCur);
	if (nullptr != pTexTarget)
	{
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
		D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y - m_tOBB.vCollSize.y, 0.f);
		m_tInfo.matWorld = matScale * matTrans;
		m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
		m_pDeviceMgr->GetSprite()->Draw(pTexTarget->pTexture, nullptr,
			&pTexTarget->vOffset, nullptr, pTexTarget->dwBlend);
	}

	//CGameObject::DrawCollRect();
	CEventObject::DrawHpBar2();
	//CEventObject::DrawNameBar();
	CEventObject::DrawPosition();

	if (m_bIsChat)
		CEventObject::DrawChatting();
}

void CPlayer::Release()
{
	SafeDelete(m_pCharacter);
}

// FRAME_ONCE일 때 마지막 장 이벤트
void CPlayer::FrameOnceEnd() {
	if (m_bIsAttack && IDLE != m_eCurState)
	{
		if (nullptr != m_pCharacter)
		{
			//dynamic_cast<CComboWnd*>(m_pUIMgr->GetWindow(WND_ID::WNDID_COMBO))->GetCurComboSkill()

			CComboWnd* pComboWnd = dynamic_cast<CComboWnd*>(m_pUIMgr->GetWindow(WND_ID::WNDID_COMBO));
			m_tComboDelay.SetAlarm((*(pComboWnd->GetVecCurCombo()))[m_iComboAttack]->fColltime, false);
			if (STATE::SKILL != m_eCurState)
			{
				if (nullptr != m_pCharacter)
					m_pCharacter->SetIdleState();
			}
		}

		if (nullptr == m_pTarget || (nullptr != m_pTarget && m_pTarget->GetDead()))
		{
			m_pTarget = nullptr;
			if (!FindMonster())
			{
				m_iCombo = 0;
				m_iComboAttack = 0;
				m_bIsAttack = false;

				NULL_CHECK(m_pCharacter);
				m_pCharacter->SetIdleState();
			}
		}
	}
	else
	{
		NULL_CHECK(m_pCharacter);
		m_pCharacter->SetIdleState();
	}
}

// 프레임 카운트 마다 이벤트
void CPlayer::FrameEvent() 
{
	NULL_CHECK(m_pCharacter);
	m_pCharacter->FrameEvent();
}

void CPlayer::AtarMoveEnd()
{
	m_bIsGate = false;
	NULL_CHECK(m_pCharacter);
	m_pCharacter->SetIdleState();
}

void CPlayer::AstarMoveSetState()
{
	NULL_CHECK(m_pCharacter);
	m_pCharacter->SetRunState();
}

bool CPlayer::AstartExitEvent()
{
	if (nullptr != m_pTarget)
	{
		float fDistance = D3DXVec3Length(&(m_pTarget->GetInfo().vPos - m_tInfo.vPos));

		if (OBJ_MONSTER == m_pTarget->GetObjID())
		{
			if (fDistance < float(TILECX))
			{
				m_bIsAttack = true;
				return true;
			}
		}
		else if (OBJ_ITEM == m_pTarget->GetObjID())
		{
			if (fDistance < float(TILECX * 2.f))
			{
				dynamic_cast<CItem*>(m_pTarget)->SetItemState(2);	// 인벤으로 넣는다.
				return true;
			}
		}
		else if (m_bIsGate)
		{
			float fDistance = D3DXVec3Length(&(m_vEndPos - m_tInfo.vPos));
			if (fDistance < float(TILECX))
			{
				const vector<TILE_INFO*>* pVecTileInfo = CTileMgr::GetInstance()->GetTileVec();
				// 이전 맵
				if (2 == (*pVecTileInfo)[m_iSelectTile]->byDrawID)
				{
					m_pSceneMgr->SetStageNum(m_pSceneMgr->GetStageNum() - 1);
					m_pSceneMgr->SetNextScene(m_pSceneMgr->GetPreScene(), L"Map" + to_wstring(m_pSceneMgr->GetStageNum()));
					true;
				}
				// 다음 맵
				else if (3 == (*pVecTileInfo)[m_iSelectTile]->byDrawID)
				{
					m_pSceneMgr->SetStageNum(m_pSceneMgr->GetStageNum() + 1);
					m_pSceneMgr->SetNextScene(SCENEID::SCENEID_STAGE, L"Map" + to_wstring(m_pSceneMgr->GetStageNum()));
					true;
				}
			}
		}
	}

	return false;
}

void CPlayer::UseSkill(SKILLID eSkillID)
{
	NULL_CHECK(m_pCharacter);
	m_pCharacter->Attack(eSkillID);
}

void CPlayer::UpdateData()
{
	m_tUnitData = m_pUIMgr->GetPlayerData();
	m_tIdentity = m_pUIMgr->GetPlayerIdentity();

	m_pSubject->Notify(CPlayerSubject::PLAYER_UNIT_DATA);
	m_pSubject->Notify(CPlayerSubject::PLAYER_IDENTITY);
	m_pSubject->Notify(CPlayerSubject::PLAYER_INFO);
}

// 키 입력 함수
void CPlayer::KeyInput()
{
	if (m_pKeyMgr->KeyDown(KEY_RUN))
	{
		m_tIdentity.bIsRun ^= true;
	}

	if (m_pKeyMgr->KeyDown(KEY_LEVEL_UP))
	{
		m_pUIMgr->AddExp(m_tIdentity.iMinExp);
	}

	// 아이템 획득
	if (m_pKeyMgr->KeyDown(KEY_GET_ITEM))
	{
		list<CGameObject*> pList = m_pObjMgr->GetObjList(OBJ_ITEM);

		pList.sort([&](CGameObject* pObj1, CGameObject* pObj2) {
			float fDist1 = D3DXVec3Length(&(this->GetInfo().vPos - pObj1->GetInfo().vPos));
			float fDist2 = D3DXVec3Length(&(this->GetInfo().vPos - pObj2->GetInfo().vPos));

			return (fDist1 > fDist2);
		});

		for (auto& Item : pList)
		{
			if (nullptr != Item && 1 == dynamic_cast<CItem*>(Item)->GetItemState()) {
				float fDistance = D3DXVec3Length(&(Item->GetInfo().vPos - m_tInfo.vPos));

				if (fDistance < float(TILECX * 4))
				{
					m_bIsGate		= false;
					m_bIsAttack		= false;
					m_bIsMove		= true;
					m_pTarget		= Item;
					m_pTileMgr->AStarStartPos(&m_lstBest, m_tInfo.vPos, Item->GetInfo().vPos);
				}
				else
				{
					if (nullptr != m_pTarget && OBJ_ITEM == m_pTarget->GetObjID())
						dynamic_cast<CItem*>(m_pTarget)->SetItemState(2);	// 인벤으로 넣는다.
				}
			}
		}
	}

	/*if (m_pKeyMgr->KeyDown(KEY_DASH))
	{
		D3DXVECTOR3 vDir;
		D3DXVec3Normalize(&vDir, &(m_pMouseMgr->GetMouse() - m_tInfo.vPos));

		if (1 != (*m_pTileMgr->GetTileVec())[m_pTileMgr->GetTileIndex(m_tInfo.vPos + vDir * 100.f)]->byDrawID)
		{
			m_tInfo.vPos += vDir * 100.f;
		}
	}*/

	if (m_bIsKeyDown && !m_pKeyMgr->KeyPressing(VK_LBUTTON))
		m_bIsKeyDown = !m_bIsKeyDown;

	if (m_pKeyMgr->KeyPressing(VK_LBUTTON) && !m_pUIMgr->GetIsHover() && !m_bIsKeyDown)
	{
		m_bIsKeyDown = true;
		// 아이템 겟
		if (MOUSE_TYPE::MOUSE_PICK == m_pMouseMgr->GetMouseType())
		{
			m_pSoundMgr->PlaySound(L"BtnClick.wav", CHANNEL_ID::CHANNEL_WINDOW);
			m_pTarget = m_pMouseMgr->GetRayTarget();
			if (nullptr != m_pTarget) {
				m_tInfo.vDir = m_pTarget->GetInfo().vPos - m_tInfo.vPos;
				float fDistance = D3DXVec3Length(&m_tInfo.vDir);
				D3DXVec3Normalize(&m_tInfo.vDir, &m_tInfo.vDir);

				m_bIsGate = false;
				if (fDistance > float(TILECX))
				{
					m_bIsMove = true;
					m_pTileMgr->AStarStartPos(&m_lstBest, m_tInfo.vPos, m_pTarget->GetInfo().vPos);
				}
				else
				{
					m_fAngle = CGameObject::GetRadianAngle(m_tInfo.vDir);
					dynamic_cast<CItem*>(m_pTarget)->SetItemState(2);
				}
			}
		}

		// NPC 대화
		else if (MOUSE_TYPE::MOUSE_CHAT == m_pMouseMgr->GetMouseType())
		{
			m_pTarget = m_pMouseMgr->GetRayTarget();
			if (nullptr != m_pTarget) {
				dynamic_cast<CNpc*>(m_pTarget)->Talk();
			}
		}

		// 동료 Fix
		else if (MOUSE_TYPE::MOUSE_MAGIC == m_pMouseMgr->GetMouseType())
		{
			m_pTarget = m_pMouseMgr->GetRayTarget();
			if (nullptr != m_pTarget)
			{
				dynamic_cast<CBuddy*>(m_pTarget)->SetFix(!dynamic_cast<CBuddy*>(m_pTarget)->GetFix());
				/*CItemData* pItemData = m_pMouseMgr->GetItemData();
				if (nullptr != pItemData && ITEMID_ETC == pItemData->eItemID && 0 < pItemData->iCount)
				{
				--pItemData->iCount;

				}*/
			}
		}
	
		// 어택 땅
		else if (m_pMouseMgr->GetAttackGround())
		{
			if (MOUSE_TYPE::MOUSE_BATTLE == m_pMouseMgr->GetMouseType())
			{
				m_pTarget = m_pMouseMgr->GetRayTarget();
				if (nullptr != m_pTarget) {
					m_tInfo.vDir = m_pTarget->GetInfo().vPos - m_tInfo.vPos;
					float fDistance = D3DXVec3Length(&m_tInfo.vDir);
					D3DXVec3Normalize(&m_tInfo.vDir, &m_tInfo.vDir);

					m_bIsGate = false;
					if (fDistance > float(TILECX))
					{
						m_bIsMove = true;
						m_pTileMgr->AStarStartPos(&m_lstBest, m_tInfo.vPos, m_pTarget->GetInfo().vPos);
					}
					else
					{
						m_fAngle = CGameObject::GetRadianAngle(m_tInfo.vDir);
						m_bIsAttack = true;
					}
				}
			}
			else if (MOUSE_TYPE::MOUSE_ATTACK_GROUND == m_pMouseMgr->GetMouseType())
			{
				m_bIsGate = false;
				m_pTarget = nullptr;
				m_bIsMove = true;
				m_bIsAttack = true;
				m_vEndPos = m_pMouseMgr->GetMouse() + m_pViewMgr->GetViewPos();
				m_pTileMgr->AStarStartPos(&m_lstBest, m_tInfo.vPos, m_vEndPos);
			}
		}
		else
		{
			if (MOUSE_TYPE::MOUSE_BATTLE == m_pMouseMgr->GetMouseType())
			{
				m_bIsGate = false;
				if (nullptr != m_pMouseMgr->GetRayTarget()) {
					m_pTarget = m_pMouseMgr->GetRayTarget();
				}
			}
			else if (MOUSE_TYPE::MOUSE_PICK == m_pMouseMgr->GetMouseType())
			{
				m_bIsGate = false;
				if (nullptr != m_pMouseMgr->GetRayTarget()) {
					m_pTarget = m_pMouseMgr->GetRayTarget();
				}
			}
		}
	}

	
	// 우클릭
	if (!m_bIsHit && STATE::SKILL != m_eCurState && m_pKeyMgr->KeyPressing(VK_RBUTTON) && !m_pUIMgr->GetIsHover())
	{
		if (!m_bIsGate)
		{
			if (MOUSE_TYPE::MOUSE_GATE == m_pMouseMgr->GetMouseType())
			{
				m_vEndPos = m_pMouseMgr->GetMouse() + m_pViewMgr->GetViewPos();
				float fDistance = D3DXVec3Length(&(m_vEndPos - m_tInfo.vPos));
				if (fDistance < float(TILECX))
				{
					m_iSelectTile = m_pTileMgr->GetTileIndex(m_vEndPos);
					m_bIsGate = false;

					const vector<TILE_INFO*>* pVecTileInfo = m_pTileMgr->GetTileVec();
					// 이전 맵
					if (2 == (*pVecTileInfo)[m_iSelectTile]->byOption)
					{
						m_pSceneMgr->SetStageNum(m_pSceneMgr->GetStageNum() - 1);
						m_pSceneMgr->SetNextScene(m_pSceneMgr->GetPreScene(), L"Map" + to_wstring(m_pSceneMgr->GetStageNum()));
					}

					// 다음 맵
					if (3 == (*pVecTileInfo)[m_iSelectTile]->byOption)
					{
						m_pSceneMgr->SetStageNum(m_pSceneMgr->GetStageNum() + 1);
						m_pSceneMgr->SetNextScene(SCENEID::SCENEID_STAGE, L"Map" + to_wstring(m_pSceneMgr->GetStageNum()));
					}
				}
				else
				{
					m_bIsGate = true;
					m_bIsMove = true;
					m_bIsAttack = false;
					m_iSelectTile = m_pTileMgr->GetTileIndex(m_vEndPos);
					m_pTileMgr->AStarStartPos(&m_lstBest, m_tInfo.vPos, m_vEndPos);
					m_pTarget = nullptr;
				}
			}
		}

		if (MOUSE_TYPE::MOUSE_NORMAL == m_pMouseMgr->GetMouseType())
		{
			m_bIsGate = false;
			m_bIsMove = true;
			m_bIsAttack = false;
			m_vEndPos = m_pMouseMgr->GetMouse() + m_pViewMgr->GetViewPos();
			m_pTileMgr->AStarStartPos(&m_lstBest, m_tInfo.vPos, m_vEndPos);
			m_pTarget = nullptr;
		}
		else if (!m_bIsAttack && MOUSE_TYPE::MOUSE_BATTLE == m_pMouseMgr->GetMouseType())
		{
			m_pTarget = m_pMouseMgr->GetRayTarget();
			if (nullptr != m_pTarget) {
				m_tInfo.vDir = m_pTarget->GetInfo().vPos - m_tInfo.vPos;
				float fDistance = D3DXVec3Length(&m_tInfo.vDir);
				D3DXVec3Normalize(&m_tInfo.vDir, &m_tInfo.vDir);

				m_bIsGate = false;

				if (fDistance > float(TILECX))
				{
					m_bIsMove = true;
					m_pTileMgr->AStarStartPos(&m_lstBest, m_tInfo.vPos, m_pTarget->GetInfo().vPos);
				}
				else
				{
					m_fAngle = CGameObject::GetRadianAngle(m_tInfo.vDir);
					m_bIsAttack = true;
				}
			}
		}
	}

	//AttackKey();
}

void CPlayer::AttackKey()
{
	NULL_CHECK(m_pCharacter);
	//m_pCharacter->AttackKey();
}

bool CPlayer::FindMonster()
{
	OBJLIST pList = m_pObjMgr->GetObjList(OBJ_MONSTER);
	for (auto& Monster : pList)
	{
		if (nullptr != Monster) {
			float fDistance = D3DXVec3Length(&(Monster->GetInfo().vPos - m_tInfo.vPos));

			if (fDistance < float(TILECX * 3))
			{
				//m_bIsAttack = false;
				m_bIsMove = true;
				m_pTarget = Monster;
				m_pTileMgr->AStarStartPos(&m_lstBest, m_tInfo.vPos, Monster->GetInfo().vPos);
				return true;
			}
		}
	}

	return false;
}

// 공격 키
void CPlayer::Attack()
{
	if (nullptr != m_pTarget)
	{
		if (m_pTarget->GetDead())
		{
			m_iComboAttack = 0;
			m_pTarget = nullptr;
		}
		else if (CGameObject::IDLE == m_eCurState)
		{
			//if (m_tComboDelay.CheckAlarm())
			//{
				m_tInfo.vDir = m_pTarget->GetInfo().vPos - m_tInfo.vPos;
				m_fAngle = CGameObject::GetRadianAngle(m_tInfo.vDir);
				float fDistance = D3DXVec3Length(&m_tInfo.vDir);

				if (fDistance < float(TILECX))
				{
					CComboWnd* pComboWnd = dynamic_cast<CComboWnd*>(m_pUIMgr->GetWindow(WND_ID::WNDID_COMBO));
					(*(pComboWnd->GetVecCurCombo()))[m_iComboAttack]->UseEvent();
					++m_iComboAttack;
					if (m_iComboAttack >= int((pComboWnd->GetVecCurCombo())->size()))
						m_iComboAttack = 0;

					/*if (nullptr != m_pCharacter)
						m_pCharacter->SetAttackState();*/
				}
			//}
		}
	}
}

void CPlayer::Hit()
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

					////////////////////////////////////////
					// 치트
					//pDamageData->iDamage = 0;
					////////////////////////////////////////

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

					// 크리티컬
					int iCritical = rand() % 10;
					if (4 > iCritical)
					{
						pDamageData->iDamage += int(pDamageData->iDamage * (rand() % 3 + 2) * 0.1f);
						CreateDamageEffect(m_tInfo.vPos + D3DXVECTOR3{ 0.f, -m_tOBB.vCollSize.y, 0.f }, 0, 2);
					}

					pDamageData->iDamage -= (m_tUnitData.tStatInfo.iDef + m_tUnitData.tAddStatInfo.iDef);

					m_pSoundMgr->PlaySound(L"Block.MP3", CHANNEL_ID::CHANNEL_HIT);
					m_tUnitData.iHp -= pDamageData->iDamage;

					CreateDamageEffect(m_tInfo.vPos + D3DXVECTOR3{ 0.f, -m_tOBB.vCollSize.y, 0.f }, pDamageData->iDamage, pDamageData->iDamageType);

					if (0 >= m_tUnitData.iHp) {
						m_tUnitData.iHp = 0;
						//m_bIsMove = false;
						//m_bIsAttack = false;
						// 죽는 모션
						//m_bIsDead = true;
					}

					m_pSubject->Notify(CPlayerSubject::PLAYER_UNIT_DATA);
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


HRESULT CPlayer::SaveGame(const wstring & wstrFileName)
{
	wstring wstrFilePath = L"../Data/GameData/" + wstrFileName + L".txt";

	wofstream SaveFile;
	SaveFile.open(wstrFilePath, ios::out);

	if (!SaveFile.good()) {
		FAILED_CHECK_MSG_RETURN(E_FAIL, L"Load Failed", E_FAIL);
	}

	// 플레이어 스탯 외 정보
	SaveFile
		<< m_tIdentity.wstrType << '|'
		<< m_tIdentity.wstrTitle << '|'

		<< m_tIdentity.iMinExp << '|'
		<< m_tIdentity.iMaxExp << '|'
		<< m_tIdentity.iSeed << '|'
		<< m_tIdentity.iPoint << '|'
		<< m_tIdentity.iNp << '\n';

	// Need Point
	SaveFile
		<< m_tIdentity.tNeedPoint.iHp << '|'
		<< m_tIdentity.tNeedPoint.iMp << '|'
		<< m_tIdentity.tNeedPoint.iSp << '|'

		<< m_tIdentity.tNeedPoint.iStab << '|'
		<< m_tIdentity.tNeedPoint.iHack << '|'
		<< m_tIdentity.tNeedPoint.iInt << '|'
		<< m_tIdentity.tNeedPoint.iDex << '|'
		<< m_tIdentity.tNeedPoint.iDef << '|'
		<< m_tIdentity.tNeedPoint.iMr << '|'
		<< m_tIdentity.tNeedPoint.iAgi << '|'

		<< m_tIdentity.tNeedPoint.iLightning << '|'
		<< m_tIdentity.tNeedPoint.iFire << '|'
		<< m_tIdentity.tNeedPoint.iEarth << '|'
		<< m_tIdentity.tNeedPoint.iLight << '|'

		<< m_tIdentity.tNeedPoint.iIce << '|'
		<< m_tIdentity.tNeedPoint.iWind << '|'
		<< m_tIdentity.tNeedPoint.iDark << '|'
		<< m_tIdentity.tNeedPoint.iNormal << '\n';


	// 플레이어 스탯 정보
	SaveFile
		<< m_tUnitData.vCollStart.x << '|'
		<< m_tUnitData.vCollStart.y << '|'

		<< m_tUnitData.vCollEnd.x << '|'
		<< m_tUnitData.vCollEnd.y << '|'

		<< m_tUnitData.wstrName << '|'
		<< m_tUnitData.wstrObjKey << '|'
		<< m_tUnitData.wstrStateKey << '|'

		<< m_tUnitData.iObjID << '|'

		<< m_tUnitData.iLevel << '|'
		<< m_tUnitData.iExp << '|'

		<< m_tUnitData.iHp << '|'
		<< m_tUnitData.iMp << '|'
		<< m_tUnitData.iSp << '|'

		<< m_tUnitData.iMaxHp << '|'
		<< m_tUnitData.iMaxMp << '|'
		<< m_tUnitData.iMaxSp << '|'

		<< m_tUnitData.iIsFirstAtt << '|'

		<< m_tUnitData.fFindDst << '\n';

	// 스탯
	SaveFile
		<< m_tUnitData.tStatInfo.iHp << '|'
		<< m_tUnitData.tStatInfo.iMp << '|'
		<< m_tUnitData.tStatInfo.iSp << '|'

		<< m_tUnitData.tStatInfo.iStab << '|'
		<< m_tUnitData.tStatInfo.iHack << '|'
		<< m_tUnitData.tStatInfo.iInt << '|'
		<< m_tUnitData.tStatInfo.iDex << '|'
		<< m_tUnitData.tStatInfo.iDef << '|'
		<< m_tUnitData.tStatInfo.iMr << '|'
		<< m_tUnitData.tStatInfo.iAgi << '|'

		<< m_tUnitData.tStatInfo.iLightning << '|'
		<< m_tUnitData.tStatInfo.iFire << '|'
		<< m_tUnitData.tStatInfo.iEarth << '|'
		<< m_tUnitData.tStatInfo.iLight << '|'

		<< m_tUnitData.tStatInfo.iIce << '|'
		<< m_tUnitData.tStatInfo.iWind << '|'
		<< m_tUnitData.tStatInfo.iDark << '|'
		<< m_tUnitData.tStatInfo.iNormal << '\n';

	// 증가 스탯
	SaveFile
		<< m_tUnitData.tAddStatInfo.iHp << '|'
		<< m_tUnitData.tAddStatInfo.iMp << '|'
		<< m_tUnitData.tAddStatInfo.iSp << '|'

		<< m_tUnitData.tAddStatInfo.iStab << '|'
		<< m_tUnitData.tAddStatInfo.iHack << '|'
		<< m_tUnitData.tAddStatInfo.iInt << '|'
		<< m_tUnitData.tAddStatInfo.iDex << '|'
		<< m_tUnitData.tAddStatInfo.iDef << '|'
		<< m_tUnitData.tAddStatInfo.iMr << '|'
		<< m_tUnitData.tAddStatInfo.iAgi << '|'

		<< m_tUnitData.tAddStatInfo.iLightning << '|'
		<< m_tUnitData.tAddStatInfo.iFire << '|'
		<< m_tUnitData.tAddStatInfo.iEarth << '|'
		<< m_tUnitData.tAddStatInfo.iLight << '|'

		<< m_tUnitData.tAddStatInfo.iIce << '|'
		<< m_tUnitData.tAddStatInfo.iWind << '|'
		<< m_tUnitData.tAddStatInfo.iDark << '|'
		<< m_tUnitData.tAddStatInfo.iNormal << '\n';

	SaveFile.close();

	return S_OK;
}


HRESULT CPlayer::LoadGame(const wstring & wstrFileName)
{
	wstring wstrFilePath = L"../Data/GameData/" + wstrFileName + L".txt";

	wifstream LoadFile;
	LoadFile.open(wstrFilePath, ios::in);

	if (!LoadFile.good()) {
		FAILED_CHECK_MSG_RETURN(E_FAIL, L"Load Failed", E_FAIL);
	}


	// 플레이어 스탯 외 정보
	TCHAR szType[MAX_STR] = L"";
	TCHAR szTitle[MAX_STR] = L"";

	TCHAR szMinExp[MAX_STR] = L"";
	TCHAR szMaxExp[MAX_STR] = L"";
	TCHAR szSeed[MAX_STR] = L"";
	TCHAR szPoint[MAX_STR] = L"";
	TCHAR szNp[MAX_STR] = L"";

	LoadFile.getline(szType, MAX_STR, '|');
	LoadFile.getline(szTitle, MAX_STR, '|');

	LoadFile.getline(szMinExp, MAX_STR, '|');
	LoadFile.getline(szMaxExp, MAX_STR, '|');

	LoadFile.getline(szSeed, MAX_STR, '|');
	LoadFile.getline(szPoint, MAX_STR, '|');
	LoadFile.getline(szNp, MAX_STR);

	
	m_tIdentity.wstrType = szType;
	m_tIdentity.wstrTitle = szTitle;

	m_tIdentity.iMinExp = int(_wtoi(szMinExp));
	m_tIdentity.iMaxExp = int(_wtoi(szMaxExp));

	m_tIdentity.iSeed = int(_wtoi(szSeed));
	m_tIdentity.iPoint = int(_wtoi(szPoint));
	m_tIdentity.iNp = int(_wtoi(szNp));
	m_tIdentity.bIsRun = true;
	

	// Need Point
	TCHAR szHP[MAX_STR] = L"";	// 체력
	TCHAR szMP[MAX_STR] = L"";	// 마나
	TCHAR szSP[MAX_STR] = L"";	// 스테미너

	TCHAR szSTAB[MAX_STR] = L"";	// 찌르기
	TCHAR szHACK[MAX_STR] = L"";	// 베기
	TCHAR szINT[MAX_STR] = L"";	// 마법 공격
	TCHAR szDEX[MAX_STR] = L"";	// 명중
	TCHAR szDEF[MAX_STR] = L"";	// 물리 방어
	TCHAR szMR[MAX_STR] = L"";	// 마법 방어
	TCHAR szAGI[MAX_STR] = L"";	// 회피율

	TCHAR szLightning[MAX_STR] = L"";	// 번개 속성
	TCHAR szFire[MAX_STR] = L"";	// 불 속성
	TCHAR szEarth[MAX_STR] = L"";	// 땅 속성
	TCHAR szLight[MAX_STR] = L"";	// 빛 속성

	TCHAR szIce[MAX_STR] = L"";	// 얼음 속성
	TCHAR szWind[MAX_STR] = L"";	// 바람 속성
	TCHAR szDark[MAX_STR] = L"";	// 암흑 속성
	TCHAR szNormal[MAX_STR] = L"";	// 무 속성

	LoadFile.getline(szHP, MAX_STR, '|');	// 체력
	LoadFile.getline(szMP, MAX_STR, '|');	// 마나
	LoadFile.getline(szSP, MAX_STR, '|');	// 스테미너

	LoadFile.getline(szSTAB, MAX_STR, '|');	// 찌르기
	LoadFile.getline(szHACK, MAX_STR, '|');	// 베기
	LoadFile.getline(szINT, MAX_STR, '|');	// 마법 공격
	LoadFile.getline(szDEX, MAX_STR, '|');	// 명중
	LoadFile.getline(szDEF, MAX_STR, '|');	// 물리 방어
	LoadFile.getline(szMR, MAX_STR, '|');	// 마법 방어
	LoadFile.getline(szAGI, MAX_STR, '|');	// 회피율

	LoadFile.getline(szLightning, MAX_STR, '|');	// 찌르기
	LoadFile.getline(szFire, MAX_STR, '|');	// 베기
	LoadFile.getline(szEarth, MAX_STR, '|');	// 마법 공격
	LoadFile.getline(szLight, MAX_STR, '|');	// 명중

	LoadFile.getline(szIce, MAX_STR, '|');	// 물리 방어
	LoadFile.getline(szWind, MAX_STR, '|');	// 마법 방어
	LoadFile.getline(szDark, MAX_STR, '|');	// 회피율
	LoadFile.getline(szNormal, MAX_STR);	// 회피율

	
	m_tIdentity.tNeedPoint.iHp			= int(_wtoi(szHP));
	m_tIdentity.tNeedPoint.iMp			= int(_wtoi(szMP));
	m_tIdentity.tNeedPoint.iSp			= int(_wtoi(szSP));
										
	m_tIdentity.tNeedPoint.iStab		= int(_wtoi(szSTAB));
	m_tIdentity.tNeedPoint.iHack		= int(_wtoi(szHACK));
	m_tIdentity.tNeedPoint.iInt			= int(_wtoi(szINT));
	m_tIdentity.tNeedPoint.iDex			= int(_wtoi(szDEX));
	m_tIdentity.tNeedPoint.iDef			= int(_wtoi(szDEF));
	m_tIdentity.tNeedPoint.iMr			= int(_wtoi(szMR));
	m_tIdentity.tNeedPoint.iAgi			= int(_wtoi(szAGI));

	m_tIdentity.tNeedPoint.iLightning	= int(_wtoi(szLightning));
	m_tIdentity.tNeedPoint.iFire		= int(_wtoi(szFire));
	m_tIdentity.tNeedPoint.iEarth		= int(_wtoi(szEarth));
	m_tIdentity.tNeedPoint.iLight		= int(_wtoi(szLight));

	m_tIdentity.tNeedPoint.iIce			= int(_wtoi(szIce));
	m_tIdentity.tNeedPoint.iWind		= int(_wtoi(szWind));
	m_tIdentity.tNeedPoint.iDark		= int(_wtoi(szDark));
	m_tIdentity.tNeedPoint.iNormal		= int(_wtoi(szNormal));


	// 플레이어 스탯 정보
	TCHAR szCollLeft[MAX_STR] = L"";	// 충돌렉트 Left
	TCHAR szCollTop[MAX_STR] = L"";	// 충돌렉트 Top
	TCHAR szCollRight[MAX_STR] = L"";	// 충돌렉트 Right
	TCHAR szCollBottom[MAX_STR] = L"";	// 충돌렉트 Bottom

	TCHAR szName[MAX_STR] = L"";	// 이름
	TCHAR szObjKey[MAX_STR] = L"";	// 텍스쳐 오브젝트 키
	TCHAR szStateKey[MAX_STR] = L"";	// 텍스쳐 상태 키

	TCHAR szObjID[MAX_STR] = L"";	// 오브젝트 아이디

	TCHAR szLevel[MAX_STR] = L"";	// 레벨
	TCHAR szExp[MAX_STR] = L"";	// 경험치

	TCHAR szMaxHP[MAX_STR] = L"";	// 체력
	TCHAR szMaxMP[MAX_STR] = L"";	// 마나
	TCHAR szMaxSP[MAX_STR] = L"";	// 스테미너

	TCHAR szIsFirst[MAX_STR] = L"";	// 선공 / 후공

	TCHAR szFindDst[MAX_STR] = L"";	// 플레이어 발견 거리

	LoadFile.getline(szCollLeft, MAX_STR, '|');	// 충돌렉트 Left
	LoadFile.getline(szCollTop, MAX_STR, '|');	// 충돌렉트 Top
	LoadFile.getline(szCollRight, MAX_STR, '|');	// 충돌렉트 Right
	LoadFile.getline(szCollBottom, MAX_STR, '|');		// 충돌렉트 Bottom

	LoadFile.getline(szName, MAX_STR, '|');	// 이름
	LoadFile.getline(szObjKey, MAX_STR, '|');	// 텍스쳐 오브젝트 키
	LoadFile.getline(szStateKey, MAX_STR, '|');	// 텍스쳐 상태 키

	LoadFile.getline(szObjID, MAX_STR, '|');	// 오브젝트 아이디

	LoadFile.getline(szLevel, MAX_STR, '|');	// 레벨
	LoadFile.getline(szExp, MAX_STR, '|');	// 경험치

	LoadFile.getline(szHP, MAX_STR, '|');	// 체력
	LoadFile.getline(szMP, MAX_STR, '|');	// 마나
	LoadFile.getline(szSP, MAX_STR, '|');	// 스테미너

	LoadFile.getline(szMaxHP, MAX_STR, '|');	// 체력
	LoadFile.getline(szMaxMP, MAX_STR, '|');	// 마나
	LoadFile.getline(szMaxSP, MAX_STR, '|');	// 스테미너

	LoadFile.getline(szIsFirst, MAX_STR, '|');	// 선공 / 후공

	LoadFile.getline(szFindDst, MAX_STR);	// 적 발견 거리

	m_tUnitData.vCollStart.x = float(_wtof(szCollLeft));
	m_tUnitData.vCollStart.y = float(_wtof(szCollTop));
	m_tUnitData.vCollEnd.x = float(_wtof(szCollRight));
	m_tUnitData.vCollEnd.y = float(_wtof(szCollBottom));

	m_tUnitData.wstrName = szName;
	m_tUnitData.wstrObjKey = szObjKey;
	m_tUnitData.wstrStateKey = szStateKey;

	m_tUnitData.iObjID = int(_wtoi(szObjID));

	m_tUnitData.iLevel = int(_wtoi(szLevel));
	m_tUnitData.iExp = int(_wtoi(szExp));

	m_tUnitData.iHp = int(_wtoi(szHP));
	m_tUnitData.iMp = int(_wtoi(szMP));
	m_tUnitData.iSp = int(_wtoi(szSP));

	m_tUnitData.iMaxHp = int(_wtoi(szMaxHP));
	m_tUnitData.iMaxMp = int(_wtoi(szMaxMP));
	m_tUnitData.iMaxSp = int(_wtoi(szMaxSP));

	m_tUnitData.iIsFirstAtt = int(_wtoi(szIsFirst));
	m_tUnitData.fFindDst = float(_wtof(szFindDst));

	// 스탯
	LoadFile.getline(szHP, MAX_STR, '|');	// 체력
	LoadFile.getline(szMP, MAX_STR, '|');	// 마나
	LoadFile.getline(szSP, MAX_STR, '|');	// 스테미너

	LoadFile.getline(szSTAB, MAX_STR, '|');	// 찌르기
	LoadFile.getline(szHACK, MAX_STR, '|');	// 베기
	LoadFile.getline(szINT, MAX_STR, '|');	// 마법 공격
	LoadFile.getline(szDEX, MAX_STR, '|');	// 명중
	LoadFile.getline(szDEF, MAX_STR, '|');	// 물리 방어
	LoadFile.getline(szMR, MAX_STR, '|');	// 마법 방어
	LoadFile.getline(szAGI, MAX_STR, '|');	// 회피율

	LoadFile.getline(szLightning, MAX_STR, '|');	// 찌르기
	LoadFile.getline(szFire, MAX_STR, '|');	// 베기
	LoadFile.getline(szEarth, MAX_STR, '|');	// 마법 공격
	LoadFile.getline(szLight, MAX_STR, '|');	// 명중

	LoadFile.getline(szIce, MAX_STR, '|');	// 물리 방어
	LoadFile.getline(szWind, MAX_STR, '|');	// 마법 방어
	LoadFile.getline(szDark, MAX_STR, '|');	// 회피율
	LoadFile.getline(szNormal, MAX_STR);	// 회피율

	m_tUnitData.tStatInfo.iHp = int(_wtoi(szHP));
	m_tUnitData.tStatInfo.iMp = int(_wtoi(szMP));
	m_tUnitData.tStatInfo.iSp = int(_wtoi(szSP));

	m_tUnitData.tStatInfo.iStab = int(_wtoi(szSTAB));
	m_tUnitData.tStatInfo.iHack = int(_wtoi(szHACK));
	m_tUnitData.tStatInfo.iInt = int(_wtoi(szINT));
	m_tUnitData.tStatInfo.iDex = int(_wtoi(szDEX));
	m_tUnitData.tStatInfo.iDef = int(_wtoi(szDEF));
	m_tUnitData.tStatInfo.iMr = int(_wtoi(szMR));
	m_tUnitData.tStatInfo.iAgi = int(_wtoi(szAGI));

	m_tUnitData.tStatInfo.iLightning = int(_wtoi(szLightning));
	m_tUnitData.tStatInfo.iFire = int(_wtoi(szFire));
	m_tUnitData.tStatInfo.iEarth = int(_wtoi(szEarth));
	m_tUnitData.tStatInfo.iLight = int(_wtoi(szLight));

	m_tUnitData.tStatInfo.iIce = int(_wtoi(szIce));
	m_tUnitData.tStatInfo.iWind = int(_wtoi(szWind));
	m_tUnitData.tStatInfo.iDark = int(_wtoi(szDark));
	m_tUnitData.tStatInfo.iNormal = int(_wtoi(szNormal));


	// 증가 스탯
	LoadFile.getline(szHP, MAX_STR, '|');	// 체력
	LoadFile.getline(szMP, MAX_STR, '|');	// 마나
	LoadFile.getline(szSP, MAX_STR, '|');	// 스테미너

	LoadFile.getline(szSTAB, MAX_STR, '|');	// 찌르기
	LoadFile.getline(szHACK, MAX_STR, '|');	// 베기
	LoadFile.getline(szINT, MAX_STR, '|');	// 마법 공격
	LoadFile.getline(szDEX, MAX_STR, '|');	// 명중
	LoadFile.getline(szDEF, MAX_STR, '|');	// 물리 방어
	LoadFile.getline(szMR, MAX_STR, '|');	// 마법 방어
	LoadFile.getline(szAGI, MAX_STR, '|');	// 회피율

	LoadFile.getline(szLightning, MAX_STR, '|');	// 찌르기
	LoadFile.getline(szFire, MAX_STR, '|');	// 베기
	LoadFile.getline(szEarth, MAX_STR, '|');	// 마법 공격
	LoadFile.getline(szLight, MAX_STR, '|');	// 명중

	LoadFile.getline(szIce, MAX_STR, '|');	// 물리 방어
	LoadFile.getline(szWind, MAX_STR, '|');	// 마법 방어
	LoadFile.getline(szDark, MAX_STR, '|');	// 회피율
	LoadFile.getline(szNormal, MAX_STR);	// 회피율

	m_tUnitData.tAddStatInfo.iHp = int(_wtoi(szHP));
	m_tUnitData.tAddStatInfo.iMp = int(_wtoi(szMP));
	m_tUnitData.tAddStatInfo.iSp = int(_wtoi(szSP));

	m_tUnitData.tAddStatInfo.iStab = int(_wtoi(szSTAB));
	m_tUnitData.tAddStatInfo.iHack = int(_wtoi(szHACK));
	m_tUnitData.tAddStatInfo.iInt = int(_wtoi(szINT));
	m_tUnitData.tAddStatInfo.iDex = int(_wtoi(szDEX));
	m_tUnitData.tAddStatInfo.iDef = int(_wtoi(szDEF));
	m_tUnitData.tAddStatInfo.iMr = int(_wtoi(szMR));
	m_tUnitData.tAddStatInfo.iAgi = int(_wtoi(szAGI));

	m_tUnitData.tAddStatInfo.iLightning = int(_wtoi(szLightning));
	m_tUnitData.tAddStatInfo.iFire = int(_wtoi(szFire));
	m_tUnitData.tAddStatInfo.iEarth = int(_wtoi(szEarth));
	m_tUnitData.tAddStatInfo.iLight = int(_wtoi(szLight));

	m_tUnitData.tAddStatInfo.iIce = int(_wtoi(szIce));
	m_tUnitData.tAddStatInfo.iWind = int(_wtoi(szWind));
	m_tUnitData.tAddStatInfo.iDark = int(_wtoi(szDark));
	m_tUnitData.tAddStatInfo.iNormal = int(_wtoi(szNormal));

	LoadFile.close();

	return S_OK;
}