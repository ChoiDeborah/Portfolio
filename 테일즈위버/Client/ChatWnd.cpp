#include "stdafx.h"
#include "ChatWnd.h"

#include "Player.h"
#include "Buddy.h"

CChatWnd::CChatWnd()
{
}


CChatWnd::~CChatWnd()
{
	Release();
}

HRESULT CChatWnd::Initialize()
{
	m_pWndTex = m_pTextureMgr->GetTexInfo(L"UI", L"ChatWnd");
	if (nullptr != m_pWndTex) {
		m_tWndInfo.vPos = { CLIENTCX * 0.5f - float(m_pWndTex->tImgInfo.Width) * 0.5f, float(CLIENTCY) - float(m_pWndTex->tImgInfo.Height), 0.f };
		m_tWndInfo.vSize = { float(m_pWndTex->tImgInfo.Width), float(m_pWndTex->tImgInfo.Height), 0.f };
		m_tWndInfo.vOffset = m_pWndTex->vOffset;
	}

	m_bIsShow = true;
	return S_OK;
}

void CChatWnd::Update()
{
	CUIWnd::WindowMove();

	if ((SCENEID::SCENEID_STAGE == m_pSceneMgr->GetCurScene() || SCENEID::SCENEID_STAGE == m_pSceneMgr->GetCurScene()) 
		&& m_pKeyMgr->KeyDown(VK_RETURN))
	{
		m_pUIMgr->SetIsChat(!m_pUIMgr->GetIsChat());

		if (m_pUIMgr->GetIsChat())
		{
			SetFocus(g_hChat);
		}
		else
		{
			TCHAR szChatting[MAX_STR];
			GetWindowTextW(g_hChat, szChatting, MAX_STR);

			if (0 < wstring(szChatting).length())
			{
				m_pUIMgr->InsertChat(m_pUIMgr->GetPlayerData().wstrName, szChatting, D3DCOLOR_ARGB(255, 255, 255, 255));

				CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pObjMgr->GetPlayer());
				if (nullptr != pPlayer)
					pPlayer->SetChatting(szChatting);

				Cheat(szChatting);
			}

			SetWindowTextW(g_hChat, L"");

			SetFocus(g_hWnd);
		}
	}
}

void CChatWnd::Render()
{
	D3DXMATRIX matScale, matTrans, matWorld;

	m_pWndTex = m_pTextureMgr->GetTexInfo(L"UI", L"ChatWnd", m_pUIMgr->GetIsChat());
	if (nullptr != m_pWndTex) {
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
		D3DXMatrixTranslation(&matTrans, m_tWndInfo.vPos.x, m_tWndInfo.vPos.y, 0.f);
		matWorld = matScale * matTrans;

		m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
		m_pDeviceMgr->GetSprite()->Draw(m_pWndTex->pTexture, nullptr,
			&m_tWndInfo.vOffset, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

		RECT rt;

		int iCount = 0;
		for (size_t i = m_vecChatData.size() - 1; i >= 0; --i)
		{
			if (0 > i || i >= m_vecChatData.size())
				break;;

			D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
			matWorld = matScale;
			m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
			SetRect(&rt,
				int(m_tWndInfo.vPos.x + 8.f), int(m_tWndInfo.vPos.y + m_tWndInfo.vSize.y - 59.f - 18.f * ((m_vecChatData.size()-1) - i)),
				int(m_tWndInfo.vPos.x + m_tWndInfo.vSize.x - 8.f), int(m_tWndInfo.vPos.y + m_tWndInfo.vSize.y - 41.f - 18.f * ((m_vecChatData.size()-1) - i)));

			wstring wstrChat = L""; 
			if (int(m_vecChatData[i]->wstrName.length()) < 1)
				wstrChat = m_vecChatData[i]->wstrChat;
			else
				wstrChat = m_vecChatData[i]->wstrName + L"  :  " + m_vecChatData[i]->wstrChat;

			m_pDeviceMgr->DrawFont(wstrChat, &rt, DT_LEFT | DT_NOCLIP | DT_VCENTER, m_vecChatData[i]->dwColor);
			++iCount;
			if (4 <= iCount)
				break;
		}

		// 플레이어 이름
		{
			D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
			matWorld = matScale;
			m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
			SetRect(&rt,
				int(m_tWndInfo.vPos.x + 40.f), int(m_tWndInfo.vPos.y + 89.f),
				int(m_tWndInfo.vPos.x + 152.f), int(m_tWndInfo.vPos.y + 109.f));
			m_pDeviceMgr->DrawFont(m_pUIMgr->GetPlayerData().wstrName, &rt, DT_LEFT | DT_NOCLIP | DT_VCENTER, D3DCOLOR_ARGB(255, 255, 255, 255));
		}

		
		// Chat
		if (m_pUIMgr->GetIsChat())
		{
			TCHAR szChatting[MAX_STR];
			GetWindowTextW(g_hChat, szChatting, MAX_STR);

			D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
			matWorld = matScale;
			m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
			SetRect(&rt,
				int(m_tWndInfo.vPos.x + 170.f), int(m_tWndInfo.vPos.y + 89.f),
				int(m_tWndInfo.vPos.x + 495.f), int(m_tWndInfo.vPos.y + 109.f));
			m_pDeviceMgr->DrawFont(szChatting, &rt, DT_LEFT | DT_NOCLIP | DT_VCENTER, D3DCOLOR_ARGB(255, 255, 255, 255));
		}
	}
}

void CChatWnd::Release()
{
	for (auto& Chat : m_vecChatData)
		SafeDelete(Chat);
	m_vecChatData.clear();
	m_vecChatData.shrink_to_fit();
}

void CChatWnd::LClickDown()
{
}

void CChatWnd::LClickUp()
{
}

void CChatWnd::RClickDown()
{
}

void CChatWnd::InsertChat(CHAT_DATA* pChatData)
{
	NULL_CHECK(pChatData);
	m_vecChatData.push_back(pChatData);
}

void CChatWnd::Cheat(const wstring & wstrCheat)
{
	if (!lstrcmp(wstrCheat.c_str(), L"/show me the money"))
	{
		m_pUIMgr->GetPlayerIdentity().iSeed += 100000;
		m_pUIMgr->UpdatePlayerNotify();
		m_pUIMgr->InsertChat(L"", L"<System> 100,000 Seed가 지급되었습니다.", D3DCOLOR_ARGB(255, 255, 255, 0));
	}
	else if (!lstrcmp(wstrCheat.c_str(), L"/buddy don't follow"))
	{
		auto pBuddyList = m_pObjMgr->GetObjList(OBJ_BUDDY);
		for (auto& Buddy : pBuddyList)
		{
			dynamic_cast<CBuddy*>(Buddy)->SetFix(true);
		}

		m_pUIMgr->InsertChat(L"", L"<System> 동료가 따라오지 않습니다.", D3DCOLOR_ARGB(255, 255, 255, 0));
	}
	else if (!lstrcmp(wstrCheat.c_str(), L"/buddy follow"))
	{
		auto pBuddyList = m_pObjMgr->GetObjList(OBJ_BUDDY);
		for (auto& Buddy : pBuddyList)
		{
			dynamic_cast<CBuddy*>(Buddy)->SetFix(false);
		}

		m_pUIMgr->InsertChat(L"", L"<System> 동료가 따라옵니다.", D3DCOLOR_ARGB(255, 255, 255, 0));
	}
	else if (!lstrcmp(wstrCheat.c_str(), L"/넥슨은 젤리삐를 뿌려라!"))
	{
		CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pObjMgr->GetPlayer());
		if (nullptr != pPlayer)
		{
			auto pTileVec = m_pTileMgr->GetTileVec();

			D3DXVECTOR3 vCreatePos
				= { pPlayer->GetInfo().vPos.x + float(rand() % TILECX * 2 - TILECX), pPlayer->GetInfo().vPos.y + float(rand() % TILECY * 2 - TILECY), 0.f };

			int iIndex = m_pTileMgr->GetTileIndex(vCreatePos);

			int iCount = 0;
			do
			{
				vCreatePos = { pPlayer->GetInfo().vPos.x + float(rand() % TILECX * 2 - TILECX), pPlayer->GetInfo().vPos.y + float(rand() % TILECY * 2 - TILECY), 0.f };
				iIndex = m_pTileMgr->GetTileIndex(vCreatePos);
				++iCount;
				if (1000 <= iCount)
					break;
			} while (0 > iIndex || iIndex >= int(pTileVec->size()) || 1 == (*pTileVec)[iIndex]->byOption);

			if (1000 > iCount)
			{
				TILE_INFO tTileInfo;
				INFO tInfo;
				tInfo.vPos = vCreatePos;
				tTileInfo.wstrName = L"Jelly";
				tTileInfo.byOption = OBJ_BUDDY;
				m_pObjMgr->AddObject(CAbstractFactory<CBuddy>::CreateObj(tTileInfo, tInfo), OBJID(tTileInfo.byOption));
			}
		}

		m_pUIMgr->InsertChat(L"", L"<System> 드..드리겠습니다..", D3DCOLOR_ARGB(255, 255, 255, 0));
	}
	else if (!lstrcmp(wstrCheat.c_str(), L"/show me the item"))
	{
		m_pUIMgr->InsertChat(L"", L"<System> 아이템이 지급됩니다.", D3DCOLOR_ARGB(255, 255, 255, 0));

		D3DXVECTOR3 vMouse = m_pMouseMgr->GetMouse() + m_pViewMgr->GetViewPos();

		CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pObjMgr->GetPlayer());
		if (nullptr != pPlayer)
		{
			wstring wstrItemName[5];
			for (int i = 0; i < 5; ++i)
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

				auto pTileVec = m_pTileMgr->GetTileVec();

				D3DXVECTOR3 vCreatePos
					= { pPlayer->GetInfo().vPos.x + float(rand() % TILECX * 2 - TILECX), pPlayer->GetInfo().vPos.y + float(rand() % TILECY * 2 - TILECY), 0.f };

				int iIndex = m_pTileMgr->GetTileIndex(vCreatePos);

				int iCount = 0;
				do
				{
					vCreatePos = { pPlayer->GetInfo().vPos.x + float(rand() % TILECX * 2 - TILECX), pPlayer->GetInfo().vPos.y + float(rand() % TILECY * 2 - TILECY), 0.f };
					iIndex = m_pTileMgr->GetTileIndex(vCreatePos);
					++iCount;
					if (100 <= iCount)
						break;
				} while (0 > iIndex || iIndex >= int(pTileVec->size()) || 1 == (*pTileVec)[iIndex]->byOption);

				if (100 > iCount)
					CGameObject::CreateItem(wstrItemName[i].c_str(), vMouse, vCreatePos, 1, -1);
			}
		}

		/*if (m_pKeyMgr->KeyDown(KEY_CREATE_ITEM))
		{
			
		}*/
	}
}

CChatWnd* CChatWnd::Create()
{
	CChatWnd* pInstance = new CChatWnd;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}
