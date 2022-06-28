#include "stdafx.h"
#include "InvenWnd.h"

#include "ItemEquipData.h"
#include "ItemUsableData.h"

#include "QuickWnd.h"
#include "ShopWnd.h"
#include "CountWnd.h"

CInvenWnd::CInvenWnd()
{
}


CInvenWnd::~CInvenWnd()
{
	Release();
}

HRESULT CInvenWnd::Initialize()
{
	m_pWndTex = m_pTextureMgr->GetTexInfo(L"UI", L"InvenWnd");
	if (nullptr != m_pWndTex) {
		m_tWndInfo.vPos = { float(CLIENTCX - m_pWndTex->tImgInfo.Width) - 18.f, 153.f, 0.f };
		m_tWndInfo.vSize = { float(m_pWndTex->tImgInfo.Width), float(m_pWndTex->tImgInfo.Height), 0.f };
		m_tWndInfo.vOffset = m_pWndTex->vOffset;

		m_tMoveInfo.vSize = { float(m_pWndTex->tImgInfo.Width), 30.f, 0.f };
		m_tMoveInfo.vParentPos = m_tWndInfo.vPos;
		m_tMoveInfo.vPos = { 0.f, 0.f, 0.f };
		m_tMoveInfo.vOffset = m_tWndInfo.vOffset;

		m_pExitTex = m_pTextureMgr->GetTexInfo(L"Button", L"XBtn", m_tExitInfo.iHover);
		if (nullptr != m_pExitTex)
		{
			m_tExitInfo.vParentPos = m_tWndInfo.vPos;
			m_tExitInfo.vPos = { float(m_pWndTex->tImgInfo.Width) - 18.f, 11.f, 0.f };
			m_tExitInfo.vSize = { float(m_pExitTex->tImgInfo.Width), float(m_pExitTex->tImgInfo.Height), 0.f };
			m_tExitInfo.vOffset = m_pExitTex->vOffset;
		}

		for (int i = 0; i < INVEN_SIZE; ++i)
		{
			m_tItem[i].vParentPos = m_tWndInfo.vPos;
			m_tItem[i].vPos	= { 11.f, 50.f + 25.f * i, 0.f };
			m_tItem[i].vSize = { 297.f, 15.f, 0.f };
			m_tItem[i].vOffset = {0.f, 0.f, 0.f};
		}

		m_pScrollUpTex		= m_pTextureMgr->GetTexInfo(L"Button", L"ScrollUp");
		if (nullptr != m_pScrollUpTex)
		{
			m_tScrollUp.vParentPos = m_tWndInfo.vPos;
			m_tScrollUp.vPos = { 323.f, 49.f, 0.f };
			m_tScrollUp.vSize = { 12.f, 13.f, 0.f };
			m_tScrollUp.vOffset = { 0.f, 0.f, 0.f };
		}

		m_pScrollDownTex	= m_pTextureMgr->GetTexInfo(L"Button", L"ScrollDown");
		if (nullptr != m_pScrollDownTex)
		{
			m_tScrollDown.vParentPos = m_tWndInfo.vPos;
			m_tScrollDown.vPos = { 323.f, 328.f, 0.f };
			m_tScrollDown.vSize = { 12.f, 13.f, 0.f };
			m_tScrollDown.vOffset = { 0.f, 0.f, 0.f };
		}

		m_pScrollBarTex		= m_pTextureMgr->GetTexInfo(L"Button", L"ScrollBar");
		if (nullptr != m_pScrollBarTex)
		{
			m_tScrollBar.vParentPos = m_tWndInfo.vPos;
			m_tScrollBar.vPos = { 323.f, 62.f, 0.f };
			m_tScrollBar.vSize = { 10.f, 19.f, 0.f };
			m_tScrollBar.vOffset = { 0.f, 0.f, 0.f };
		}
	}

	return S_OK;
}

void CInvenWnd::Update()
{
	CUIWnd::WindowMove();
	m_tScrollUp.vParentPos = m_tScrollDown.vParentPos = m_tWndInfo.vPos;


	if (m_bIsShow)
	{
		D3DXVECTOR3 vMouse = m_pMouseMgr->GetMouse();
		if ((0 == m_tExitInfo.iHover && m_tExitInfo.IsRectInPoint(vMouse)))
			m_pSoundMgr->PlaySound(L"barover.MP3", CHANNEL_ID::CHANNEL_WINDOW);

		for (size_t i = 0; i < m_vecItem.size(); ++i)
		{
			if (nullptr != m_vecItem[i])
			{
				dynamic_cast<CQuickWnd*>(m_pUIMgr->GetWindow(WND_ID::WNDID_QUICK))->UpdateSlot();
				if (0 >= m_vecItem[i]->iCount)
				{
					SafeDelete(m_vecItem[i]);
					m_vecItem.erase(m_vecItem.begin() + i);
				}
			}
		}

		if (0 < int(m_vecItem.size()) - INVEN_SIZE && m_iScroll >= int(m_vecItem.size()) - INVEN_SIZE)
		{
			m_iScroll = m_vecItem.size() - INVEN_SIZE;
		}
	}
}

void CInvenWnd::Render()
{
	if (!m_bIsShow)
		return;

	D3DXMATRIX matScale, matTrans, matWorld;

	if (nullptr != m_pWndTex) {
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
		D3DXMatrixTranslation(&matTrans, m_tWndInfo.vPos.x, m_tWndInfo.vPos.y, 0.f);
		matWorld = matScale * matTrans;

		m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
		m_pDeviceMgr->GetSprite()->Draw(m_pWndTex->pTexture, nullptr,
			&m_tWndInfo.vOffset, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

		// 닫기 버튼
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
		D3DXMatrixTranslation(&matTrans, m_tExitInfo.vParentPos.x + m_tExitInfo.vPos.x, m_tExitInfo.vParentPos.y + m_tExitInfo.vPos.y, 0.f);
		matWorld = matScale * matTrans;
		m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
		m_pDeviceMgr->GetSprite()->Draw(m_pExitTex->pTexture, nullptr,
			&m_tExitInfo.vOffset, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

		//m_pUIMgr->DrawRect(m_tWndInfo);
		//m_pUIMgr->DrawRect(m_tMoveInfo);

		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
		m_pDeviceMgr->GetSprite()->SetTransform(&matScale);

		RECT rt;

		// 슬롯
		SetRect(&rt,
			int(m_tWndInfo.vPos.x + 37.f), int(m_tWndInfo.vPos.y + 381.f),
			int(m_tWndInfo.vPos.x + 90.f), int(m_tWndInfo.vPos.y + 392.f));
		m_pDeviceMgr->DrawFont(to_wstring(m_vecItem.size()), &rt, DT_CENTER, D3DCOLOR_ARGB(255, 255, 255, 255));

		// 시드
		SetRect(&rt,
			int(m_tWndInfo.vPos.x + 241.f), int(m_tWndInfo.vPos.y + 381.f),
			int(m_tWndInfo.vPos.x + 322.f), int(m_tWndInfo.vPos.y + 392.f));
		m_pDeviceMgr->DrawFont(to_wstring(m_pUIMgr->GetPlayerIdentity().iSeed), &rt, DT_CENTER, D3DCOLOR_ARGB(255, 255, 255, 255));


		// 아이템
		for (size_t i = 0; i < INVEN_SIZE; ++i)
		{
			if (0 > i + m_iScroll || i + m_iScroll >= m_vecItem.size())
				continue;

			if (nullptr == m_vecItem[i + m_iScroll])
				continue;

			const TEX_INFO*	pTexInfo = m_pTextureMgr->GetTexInfo(m_vecItem[i + m_iScroll]->wstrObjKey, m_vecItem[i + m_iScroll]->wstrStateKey);
			if (nullptr != pTexInfo)
			{
				// 아이템 그림
				D3DXMatrixTranslation(&matTrans, m_tWndInfo.vPos.x + 24.f, m_tWndInfo.vPos.y + 57.f + 25.f * i, 0.f);
				m_pDeviceMgr->GetSprite()->SetTransform(&matTrans);
				m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
					&pTexInfo->vOffset, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
			}
						
			D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
			m_pDeviceMgr->GetSprite()->SetTransform(&matScale);

			// 아이템 이름
			SetRect(&rt,
				int(m_tWndInfo.vPos.x + 46.f), int(m_tWndInfo.vPos.y + 50.f + 25.f * i),
				int(m_tWndInfo.vPos.x + 257.f), int(m_tWndInfo.vPos.y + 65.f + 25.f * i));
			m_pDeviceMgr->DrawFont(m_vecItem[i + m_iScroll]->wstrStateKey, &rt, DT_CENTER, m_vecItem[i + m_iScroll]->dwColor);

			// 아이템 옵션
			SetRect(&rt,
				int(m_tWndInfo.vPos.x + 273.f), int(m_tWndInfo.vPos.y + 50.f + 25.f * i),
				int(m_tWndInfo.vPos.x + 313.f), int(m_tWndInfo.vPos.y + 65.f + 25.f * i));
			m_pDeviceMgr->DrawFont(m_vecItem[i + m_iScroll]->GetOption(), &rt, DT_CENTER);
		}

		// 스크롤 업
		if (nullptr != m_pScrollUpTex)
		{
			D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
			D3DXMatrixTranslation(&matTrans, m_tWndInfo.vPos.x + m_tScrollUp.vPos.x, m_tWndInfo.vPos.y + m_tScrollUp.vPos.y, 0.f);
			m_pDeviceMgr->GetSprite()->SetTransform(&(matScale*matTrans));
			m_pDeviceMgr->GetSprite()->Draw(m_pScrollUpTex->pTexture, nullptr, &m_pScrollUpTex->vOffset, nullptr, m_pScrollUpTex->dwBlend);
		}

		// 스크롤 다운
		if (nullptr != m_pScrollDownTex)
		{
			D3DXMatrixTranslation(&matTrans, m_tWndInfo.vPos.x + m_tScrollDown.vPos.x, m_tWndInfo.vPos.y + m_tScrollDown.vPos.y, 0.f);
			m_pDeviceMgr->GetSprite()->SetTransform(&(matScale*matTrans));
			m_pDeviceMgr->GetSprite()->Draw(m_pScrollDownTex->pTexture, nullptr, &m_pScrollDownTex->vOffset, nullptr, m_pScrollDownTex->dwBlend);
		}

		// 스크롤
		if (INVEN_SIZE < m_vecItem.size())
		{
			// 스크롤 바
			if (nullptr != m_pScrollBarTex)
			{
				// 62 ~ 328 = 247
				D3DXMatrixScaling(&matScale, m_tScrollDown.vSize.x / m_tScrollBar.vSize.x, 1.f/*((266.f - (25.f * m_vecItem.size() - INVEN_SIZE)) / m_tScrollBar.vSize.y)*/, 0.f);
				D3DXMatrixTranslation(&matTrans, m_tWndInfo.vPos.x + m_tScrollBar.vPos.x, m_tWndInfo.vPos.y + m_tScrollBar.vPos.y + ((m_tScrollDown.vPos.y - m_tScrollUp.vPos.y - m_tScrollUp.vSize.y - m_tScrollBar.vSize.y) / (m_vecItem.size() - INVEN_SIZE)) * m_iScroll, 0.f);
				m_pDeviceMgr->GetSprite()->SetTransform(&(matScale*matTrans));
				m_pDeviceMgr->GetSprite()->Draw(m_pScrollBarTex->pTexture, nullptr, &m_pScrollBarTex->vOffset, nullptr, m_pScrollBarTex->dwBlend);
			}
		}
	}
}

void CInvenWnd::Release()
{
}

void CInvenWnd::LClickDown()
{
	if (!m_bIsShow)
		return;

	D3DXVECTOR3	vMouse = m_pMouseMgr->GetMouse();

	if (!m_tWndInfo.IsRectInPoint(vMouse))
		return;


	if (m_tScrollUp.IsRectInPoint(vMouse))
	{
		m_pSoundMgr->PlaySound(L"BtnClick.wav", CHANNEL_ID::CHANNEL_WINDOW);

		if (0 < m_iScroll)
			--m_iScroll;
	}
	else if (m_tScrollDown.IsRectInPoint(vMouse))
	{
		m_pSoundMgr->PlaySound(L"BtnClick.wav", CHANNEL_ID::CHANNEL_WINDOW);

		if (int(m_vecItem.size() - INVEN_SIZE) > m_iScroll)
			++m_iScroll;
	}

	if (!m_bIsScrollMove)
	{
		if (m_tScrollBar.IsRectInPoint(vMouse))
		{
			m_pSoundMgr->PlaySound(L"BtnClick.wav", CHANNEL_ID::CHANNEL_WINDOW);

			m_vScrollStartPos = vMouse - m_tScrollBar.vPos;
			m_bIsScrollMove = true;
		}
	}

	// 마우스에 아이템 없는 상태
	CItemData* pMouseItemData = m_pMouseMgr->GetItemData();
	if (nullptr == pMouseItemData)
	{
		for (int i = 0; i < INVEN_SIZE; ++i)
		{
			m_tItem[i].vParentPos = m_tWndInfo.vPos;
			if (m_tItem[i].IsRectInPoint(vMouse))
			{
				if (i + m_iScroll >= int(m_vecItem.size()))
					break;

				m_pSoundMgr->PlaySound(L"Drag.wav", CHANNEL_ID::CHANNEL_WINDOW);

				// 아이템 클릭
				m_pMouseMgr->SetItemData(WND_ID::WNDID_INVEN, i+m_iScroll, m_vecItem[i + m_iScroll]);
				break;
			}
		}
		m_pMouseMgr->SetSkillData(WND_ID::WNDID_END, -1, nullptr);
	}
	else
	{
		m_pSoundMgr->PlaySound(L"Drop.wav", CHANNEL_ID::CHANNEL_WINDOW);

		if (WND_ID::WNDID_EQUIP == m_pMouseMgr->GetFromWndID())
		{
			m_pUIMgr->MoveItem(m_pMouseMgr->GetFromWndID(), WND_ID::WNDID_INVEN, m_pMouseMgr->GetFromItemIndex(), m_pMouseMgr->GetItemData()->iCount);
		}
		else if (WND_ID::WNDID_SHOP == m_pMouseMgr->GetFromWndID())
		{
			m_pUIMgr->GetWindow(WND_ID::WNDID_COUNT)->ShowWindow(true);
			dynamic_cast<CCountWnd*>(m_pUIMgr->GetWindow(WND_ID::WNDID_COUNT))->SetWndData(m_pMouseMgr->GetFromWndID(), WND_ID::WNDID_INVEN, m_pMouseMgr->GetItemData());
		}
		m_pMouseMgr->SetItemData(WND_ID::WNDID_END, -1, nullptr);
		m_pMouseMgr->SetSkillData(WND_ID::WNDID_END, -1, nullptr);
	}
}

void CInvenWnd::LClickUp()
{
}

void CInvenWnd::RClickDown()
{
	if (!m_bIsShow)
		return;

	D3DXVECTOR3	vMouse = m_pMouseMgr->GetMouse();

	if (!m_tWndInfo.IsRectInPoint(vMouse))
		return;

	m_pMouseMgr->SetItemData(WND_ID::WNDID_END, -1, nullptr);
	m_pMouseMgr->SetSkillData(WND_ID::WNDID_END, -1, nullptr);


	// 상점 이용중
	if (m_pUIMgr->GetWindow(WND_ID::WNDID_SHOP)->IsShow())
	{
		dynamic_cast<CShopWnd*>(m_pUIMgr->GetWindow(WND_ID::WNDID_SHOP))->SetShopState(1);

		for (int i = 0; i < INVEN_SIZE; ++i)
		{
			m_tItem[i].vParentPos = m_tWndInfo.vPos;
			if (m_tItem[i].IsRectInPoint(vMouse))
			{
				m_pSoundMgr->PlaySound(L"ShopResult.wav", CHANNEL_ID::CHANNEL_WINDOW);

				if (0 > i + m_iScroll || i + m_iScroll >= int(m_vecItem.size()))
					break;

				if (nullptr != m_vecItem[i + m_iScroll])
				{
					m_pUIMgr->GetWindow(WND_ID::WNDID_COUNT)->ShowWindow(true);
					dynamic_cast<CCountWnd*>(m_pUIMgr->GetWindow(WND_ID::WNDID_COUNT))->SetWndData(WND_ID::WNDID_INVEN, WND_ID::WNDID_SHOP, m_vecItem[i + m_iScroll]);
				}
				break;
			}
		}
	}
	else
	{
		for (int i = 0; i < INVEN_SIZE; ++i)
		{
			m_tItem[i].vParentPos = m_tWndInfo.vPos;
			if (m_tItem[i].IsRectInPoint(vMouse))
			{
				m_pSoundMgr->PlaySound(L"BtnClick.wav", CHANNEL_ID::CHANNEL_WINDOW);

				if (i + m_iScroll >= int(m_vecItem.size()))
					break;

				switch (m_vecItem[i + m_iScroll]->eItemType)
				{
				case ITEM_TYPE::ITEM_EQUIP:
					m_pUIMgr->MoveItem(WND_ID::WNDID_INVEN, WND_ID::WNDID_EQUIP, i + m_iScroll);
					break;

				case ITEM_TYPE::ITEM_USE:
					m_pSoundMgr->PlaySound(L"PotionDrug.wav", CHANNEL_ID::CHANNEL_USEITEM);

					dynamic_cast<CItemUsableData*>(m_vecItem[i + m_iScroll])->UseEvent();
					if (0 >= m_vecItem[i + m_iScroll]->iCount)
					{
						dynamic_cast<CQuickWnd*>(m_pUIMgr->GetWindow(WND_ID::WNDID_QUICK))->UpdateSlot();

						SafeDelete(m_vecItem[i + m_iScroll]);
						m_vecItem.erase(m_vecItem.begin() + i + m_iScroll);
					}
					break;
				}
				break;
			}
		}
	}
}

CItemData * CInvenWnd::GetItem(int iIndex)
{
	if (0 > iIndex || iIndex >= int(m_vecItem.size()))
		return nullptr;

	return *(m_vecItem.begin() + iIndex);
}

void CInvenWnd::AddItem(CItemData* pItemData)
{
	NULL_CHECK(pItemData);

	if (pItemData->eItemID >= ITEMID_USE)
	{
		bool bIsFind = false;
		auto iter_find = m_vecItem.begin();
		for (; iter_find != m_vecItem.end(); ++iter_find)
		{
			if (!lstrcmp(pItemData->wstrStateKey.c_str(), (*iter_find)->wstrStateKey.c_str()))
			{
				if (99 >= (*iter_find)->iCount + pItemData->iCount)
				{
					(*iter_find)->iCount += pItemData->iCount;
					SafeDelete(pItemData);
					bIsFind = true;
					break;
				}
			}
		}

		if (!bIsFind)
			m_vecItem.push_back(pItemData);
	}
	else
		m_vecItem.push_back(pItemData);
}

void CInvenWnd::DeleteItem(int iIndex, int iCount)
{
	if (0 > iIndex || iIndex >= int(m_vecItem.size()))
		return;

	auto iter_find = m_vecItem.begin() + iIndex;
	if (int((*iter_find)->iCount) <= iCount)
	{
		(*iter_find)->iCount = 0;
		dynamic_cast<CQuickWnd*>(m_pUIMgr->GetWindow(WND_ID::WNDID_QUICK))->UpdateSlot();
		SafeDelete(*iter_find);
		m_vecItem.erase(iter_find);
	}
	else
		(*iter_find)->iCount -= iCount;
}

void CInvenWnd::UpdateSlot()
{
	for (int i = 0; i < INVEN_SIZE; ++i)
	{
		if (i + m_iScroll >= int(m_vecItem.size()))
			break;

		if (0 >= m_vecItem[i + m_iScroll]->iCount)
		{
			dynamic_cast<CQuickWnd*>(m_pUIMgr->GetWindow(WND_ID::WNDID_QUICK))->UpdateSlot();

			SafeDelete(m_vecItem[i + m_iScroll]);
			m_vecItem.erase(m_vecItem.begin() + i + m_iScroll);
		}
	}
}

CInvenWnd* CInvenWnd::Create()
{
	CInvenWnd* pInstance = new CInvenWnd;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}