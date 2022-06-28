#include "stdafx.h"
#include "MouseMgr.h"
#include "GameObject.h"

#include "Item.h"

#include "ItemEquipData.h"
#include "ItemUsableData.h"

#include "SkillData.h"

#include "EventObject.h"

IMPLEMENT_SINGLETON(CMouseMgr)

CMouseMgr::CMouseMgr()
	: m_pDeviceMgr(CDeviceMgr::GetInstance()),
	m_pTextureMgr(CTextureMgr::GetInstance()),
	m_pTimeMgr(CTimeMgr::GetInstance()),
	m_pKeyMgr(CKeyMgr::GetInstance()),
	m_pSoundMgr(CSoundMgr::GetInstance())
{
	Initialize();
}

CMouseMgr::~CMouseMgr()
{
}

void CMouseMgr::Initialize()
{
	//SetFrame(L"Mouse", L"Normal", MOUSE_NORMAL);
	m_pItemData = nullptr;
	m_pSkillData = nullptr;
}

void CMouseMgr::Update()
{
	if (nullptr == m_pSceneMgr)	m_pSceneMgr = CSceneMgr::GetInstance();

	if (SCENEID::SCENEID_LOGO == m_pSceneMgr->GetCurScene() || SCENEID::SCENEID_EVENT == m_pSceneMgr->GetCurScene())
		return;

	if (nullptr == m_pUIMgr)	m_pUIMgr = CUIMgr::GetInstance();
	if (nullptr == m_pObjMgr)	m_pObjMgr = CObjMgr::GetInstance();
	if (nullptr == m_pViewMgr)	m_pViewMgr = CViewMgr::GetInstance();

	D3DXVECTOR3 vMouse = GetMouse() + m_pViewMgr->GetViewPos();

	if (SCENEID::SCENEID_TITLE == m_pSceneMgr->GetCurScene())
		SetFrame(L"Mouse", L"Normal", MOUSE_NORMAL);
	else if (SCENEID::SCENEID_EVENT == m_pSceneMgr->GetCurScene())
	{
		SetFrame(L"Mouse", L"Delay", MOUSE_DELAY);
	}
	else if (SCENEID::SCENEID_STAGE == m_pSceneMgr->GetCurScene() ||
		SCENEID::SCENEID_STAGE == m_pSceneMgr->GetCurScene())
	{

		// UI상태 일 경우
		if (m_pUIMgr->GetIsHover())
		{
			SetFrame(L"Mouse", L"Normal", MOUSE_NORMAL);
		}
		else
		{
			bool bIsMouseHover = false;
			m_pRayTarget = nullptr;

			
			/*OBJLIST pVecGate = m_pObjMgr->GetObjList(OBJ_GATE);
			for (auto& Gate : pVecGate)
			{
				if (Gate->GetInfo().vPos.x + Gate->GetOBB().vVertex[0].x <= vMouse.x &&
					Gate->GetInfo().vPos.x + Gate->GetOBB().vVertex[2].x >= vMouse.x &&
					Gate->GetInfo().vPos.y + Gate->GetOBB().vVertex[0].y <= vMouse.y &&
					Gate->GetInfo().vPos.y + Gate->GetOBB().vVertex[2].y >= vMouse.y)
				{
					SetFrame(L"Mouse", L"Gate", MOUSE_GATE);
					m_pRayTarget = Gate;
					dynamic_cast<CEventObject*>(m_pRayTarget)->DrawNameBar();
					bIsMouseHover = true;
					break;
				}
			}*/

			if (!bIsMouseHover)
			{
				OBJLIST pVecMonster = m_pObjMgr->GetObjList(OBJ_MONSTER);
				for (auto& Monster : pVecMonster)
				{
					if (Monster->GetInfo().vPos.x + Monster->GetOBB().vVertex[0].x <= vMouse.x &&
						Monster->GetInfo().vPos.x + Monster->GetOBB().vVertex[2].x >= vMouse.x &&
						Monster->GetInfo().vPos.y + Monster->GetOBB().vVertex[0].y <= vMouse.y &&
						Monster->GetInfo().vPos.y + Monster->GetOBB().vVertex[2].y >= vMouse.y)
					{
						if (MOUSE_BATTLE != m_eMouseType)
							m_pSoundMgr->PlaySound(L"BtnClick.wav", CHANNEL_ID::CHANNEL_EFFECT);

						SetFrame(L"Mouse", L"Battle", MOUSE_BATTLE);
						m_pRayTarget = Monster;
						m_pRayTarget->SetMouseTarget(true);
						bIsMouseHover = true;
						break;
					}
				}
			}

			if (!bIsMouseHover)
			{
				OBJLIST pVecNPC = m_pObjMgr->GetObjList(OBJ_NPC);
				for (auto& NPC : pVecNPC)
				{
					if (NPC->GetInfo().vPos.x + NPC->GetOBB().vVertex[0].x <= vMouse.x &&
						NPC->GetInfo().vPos.x + NPC->GetOBB().vVertex[2].x >= vMouse.x &&
						NPC->GetInfo().vPos.y + NPC->GetOBB().vVertex[0].y <= vMouse.y &&
						NPC->GetInfo().vPos.y + NPC->GetOBB().vVertex[2].y >= vMouse.y)
					{
						if (MOUSE_CHAT != m_eMouseType)
							m_pSoundMgr->PlaySound(L"BtnClick.wav", CHANNEL_ID::CHANNEL_EFFECT);

						SetFrame(L"Mouse", L"Chat", MOUSE_CHAT);
						m_pRayTarget = NPC; 
						m_pRayTarget->SetMouseTarget(true);
						bIsMouseHover = true;
						break;
					}
				}
			}

			if (!bIsMouseHover)
			{
				OBJLIST pVecItem = m_pObjMgr->GetObjList(OBJ_ITEM);
				for (auto& Item : pVecItem)
				{
					if (Item->GetInfo().vPos.x + Item->GetOBB().vVertex[0].x <= vMouse.x &&
						Item->GetInfo().vPos.x + Item->GetOBB().vVertex[2].x >= vMouse.x &&
						Item->GetInfo().vPos.y + Item->GetOBB().vVertex[0].y <= vMouse.y &&
						Item->GetInfo().vPos.y + Item->GetOBB().vVertex[2].y >= vMouse.y)
					{
						if (MOUSE_PICK != m_eMouseType)
							m_pSoundMgr->PlaySound(L"BtnClick.wav", CHANNEL_ID::CHANNEL_EFFECT);

						SetFrame(L"Mouse", L"Pick", MOUSE_PICK);
						m_pRayTarget = Item;
						m_pRayTarget->SetMouseTarget(true);
						bIsMouseHover = true;
						break;
					}
				}
			}

			if (!bIsMouseHover)
			{
				OBJLIST pVecBuddy = m_pObjMgr->GetObjList(OBJ_BUDDY);
				for (auto& Buddy : pVecBuddy)
				{
					if (Buddy->GetInfo().vPos.x + Buddy->GetOBB().vVertex[0].x <= vMouse.x &&
						Buddy->GetInfo().vPos.x + Buddy->GetOBB().vVertex[2].x >= vMouse.x &&
						Buddy->GetInfo().vPos.y + Buddy->GetOBB().vVertex[0].y <= vMouse.y &&
						Buddy->GetInfo().vPos.y + Buddy->GetOBB().vVertex[2].y >= vMouse.y)
					{
						if (MOUSE_MAGIC != m_eMouseType)
							m_pSoundMgr->PlaySound(L"BtnClick.wav", CHANNEL_ID::CHANNEL_EFFECT);

						SetFrame(L"Mouse", L"Magic", MOUSE_MAGIC);
						m_pRayTarget = Buddy;
						m_pRayTarget->SetMouseTarget(true);
						bIsMouseHover = true;
						break;
					}
				}
			}

			if (!bIsMouseHover)
			{
				if (m_pKeyMgr->KeyPressing(KEY_ATTACK_GROUND))
				{
					// 어택 땅
					m_bIsAttackGround = true;
					SetFrame(L"Mouse", L"Battle", MOUSE_ATTACK_GROUND);
				}
				else
				{
					m_bIsAttackGround = false;
					const vector<TILE_INFO*>* pVecTileInfo = CTileMgr::GetInstance()->GetTileVec();


					int iIndex = CTileMgr::GetInstance()->GetTileIndex(vMouse);
					if (0 <= iIndex && int((*pVecTileInfo).size()) > iIndex)
					{
						if (nullptr != (*pVecTileInfo)[iIndex])
						{
							if (1 == (*pVecTileInfo)[iIndex]->byDrawID)
								SetFrame(L"Mouse", L"NoPass", MOUSE_NOPASS);
							else if (2 == (*pVecTileInfo)[iIndex]->byOption || 3 == (*pVecTileInfo)[iIndex]->byOption)
								SetFrame(L"Mouse", L"Gate", MOUSE_GATE);
							else
								SetFrame(L"Mouse", L"Normal", MOUSE_NORMAL);
						}
						else
							SetFrame(L"Mouse", L"Normal", MOUSE_NORMAL);
					}
					else
						SetFrame(L"Mouse", L"Normal", MOUSE_NORMAL);
				}
			}

			// 아이템 드랍
			if (m_pKeyMgr->KeyPressing(VK_LBUTTON))
			{
				if (nullptr != m_pItemData)
				{
					if (WND_ID::WNDID_INVEN == m_eWndID || WND_ID::WNDID_EQUIP == m_eWndID)
					{
						CSoundMgr::GetInstance()->PlaySound(L"Drop.wav", CHANNEL_ID::CHANNEL_USEITEM);

						CGameObject* pPlayer = m_pObjMgr->GetPlayer();
						CGameObject::CreateItem(m_pItemData->wstrStateKey, pPlayer->GetInfo().vPos, vMouse, m_pItemData->iCount, -1);
					}

					// 빼는 코드
					m_pUIMgr->MoveItem(m_eWndID, WND_ID::WNDID_END, m_iItemSlotIndex, m_pItemData->iCount);
				}

				m_pSkillData = nullptr;
				m_pItemData = nullptr;
				return;
			}
		}
	}
	FrameMove();
}

void CMouseMgr::Render()
{
	m_pTexInfo = m_pTextureMgr->GetTexInfo(m_wstrObjKey, m_wstrStateKey, m_tFrame.iFrameCur);
	NULL_CHECK(m_pTexInfo);

	m_pDeviceMgr->GetSprite()->End();
	m_pDeviceMgr->GetDevice()->EndScene();

	m_pDeviceMgr->GetDevice()->BeginScene();
	m_pDeviceMgr->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);
	D3DXMATRIX matTrans, matWorld;
	POINT pt = {};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);
	D3DXMatrixTranslation(&matTrans, (float)pt.x, (float)pt.y, 0.f);
	matWorld = matTrans;
	m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);

	// 아이템
	if (nullptr != m_pItemData)
	{
		const TEX_INFO* pItemTex = m_pTextureMgr->GetTexInfo(m_pItemData->wstrObjKey, m_pItemData->wstrStateKey);
		if (nullptr != pItemTex)
			m_pDeviceMgr->GetSprite()->Draw(pItemTex->pTexture, nullptr, &pItemTex->vOffset, nullptr, pItemTex->dwBlend);
	}
	// 스킬
	else if (nullptr != m_pSkillData)
	{
		const TEX_INFO* pSkillTex = m_pTextureMgr->GetTexInfo(m_pSkillData->wstrObjKey, m_pSkillData->wstrStateKey);
		if (nullptr != pSkillTex)
			m_pDeviceMgr->GetSprite()->Draw(pSkillTex->pTexture, nullptr, &pSkillTex->vOffset, nullptr, pSkillTex->dwBlend);
	}

	// 마우스 포인트
	m_pDeviceMgr->GetSprite()->Draw(m_pTexInfo->pTexture, nullptr,
		&m_pTexInfo->vOffset, nullptr, m_pTexInfo->dwBlend);

	m_pDeviceMgr->GetSprite()->End();
	m_pDeviceMgr->GetDevice()->EndScene();

	m_pDeviceMgr->GetDevice()->BeginScene();
	m_pDeviceMgr->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);
}

D3DXVECTOR3 CMouseMgr::GetMouse()
{
	POINT pt = {};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	return D3DXVECTOR3((float)pt.x, (float)pt.y, 0.f);
}

void CMouseMgr::SetFrame(const wstring& wstrObjKey, const wstring& wstrStateKey, MOUSE_TYPE eType)
{
	if (m_eMouseType != eType)
	{
		m_eMouseType = eType;

		m_wstrObjKey = wstrObjKey;
		m_wstrStateKey = wstrStateKey;

		m_tFrame.iFrameCur = 0;

		const TEX_INFO*	pTexInfo = m_pTextureMgr->GetTexInfo(m_wstrObjKey, m_wstrStateKey);
		NULL_CHECK_MSG(pTexInfo, L"pTexInfo Get Failed");

		m_tFrame.iFrameEnd = m_pTextureMgr->GetTexCount(m_wstrObjKey, m_wstrStateKey);
		m_tFrame.fFrameDelay = pTexInfo->m_fFrameDelay;
		m_tFrame.fFrameTime = m_pTimeMgr->GetDeltaTime();
	}
}

void CMouseMgr::SetItemData(WND_ID eWnd, int iIndex, CItemData* pItemData)
{
	if (nullptr != pItemData)
	{
		m_pSkillData = nullptr;

		m_eWndID = eWnd;
		m_iItemSlotIndex = iIndex;
	}
	m_pItemData = pItemData;
}

void CMouseMgr::SetSkillData(WND_ID eWnd, int iIndex, CSkillData* pSkillData)
{
	if (nullptr != pSkillData)
	{
		m_pItemData = nullptr;
		m_eWndID = eWnd;
		m_iItemSlotIndex = iIndex;
	}
	m_pSkillData = pSkillData;
}


void CMouseMgr::FrameMove() 
{
	if (0 > m_tFrame.fFrameDelay)
		return;

	m_tFrame.fFrameTime += m_pTimeMgr->GetDeltaTime();
	if (m_tFrame.fFrameDelay < m_tFrame.fFrameTime) {
		m_tFrame.fFrameTime = 0.f;
		++m_tFrame.iFrameCur;

		if (m_tFrame.iFrameCur >= m_tFrame.iFrameEnd) {
			m_tFrame.iFrameCur = 0;
		}
	}
}