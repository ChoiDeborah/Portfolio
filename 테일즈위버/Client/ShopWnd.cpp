#include "stdafx.h"
#include "ShopWnd.h"

#include "ItemEquipData.h"
#include "ItemUsableData.h"

#include "QuickWnd.h"
#include "InvenWnd.h"

#include "CountWnd.h"

CShopWnd::CShopWnd()
{
}


CShopWnd::~CShopWnd()
{
	Release();
}

HRESULT CShopWnd::Initialize()
{
	m_pWndTex = m_pTextureMgr->GetTexInfo(L"UI", L"ShopWnd", 0);
	if (nullptr != m_pWndTex) {
		m_tWndInfo.vPos = { float(CLIENTCX * 0.5f - m_pWndTex->tImgInfo.Width) - 18.f, 102.f, 0.f };
		m_tWndInfo.vSize = { float(m_pWndTex->tImgInfo.Width), float(m_pWndTex->tImgInfo.Height), 0.f };
		m_tWndInfo.vOffset = m_pWndTex->vOffset;

		m_tMoveInfo.vSize = { float(m_pWndTex->tImgInfo.Width), 30.f, 0.f };
		m_tMoveInfo.vParentPos = m_tWndInfo.vPos;
		m_tMoveInfo.vPos = { 0.f, 0.f, 0.f };
		m_tMoveInfo.vOffset = m_tWndInfo.vOffset;

		// 나가기 버튼
		m_pExitTex = m_pTextureMgr->GetTexInfo(L"Button", L"Exit", 0);
		if (nullptr != m_pExitTex)
		{
			m_tExitInfo.vParentPos = m_tWndInfo.vPos;
			m_tExitInfo.vPos = { 350.f, 5.f, 0.f };
			m_tExitInfo.vSize = { float(m_pExitTex->tImgInfo.Width), float(m_pExitTex->tImgInfo.Height), 0.f };
			m_tExitInfo.vOffset = m_pExitTex->vOffset;
		}
		
		// 구매 상점 슬롯
		for (int i = 0; i < BUY_SLOT; ++i)
		{
			m_tShopItem[i].vParentPos = m_tWndInfo.vPos;
			m_tShopItem[i].vPos = { 20.f, 52.f + 25.f * i, 0.f };
			m_tShopItem[i].vSize = { 352.f, 25.f, 0.f };
			m_tShopItem[i].vOffset = { 0.f, 0.f, 0.f };
		}

		// 구매 대기 슬롯
		for (int i = 0; i < BUY_SLOT; ++i)
		{
			m_tTradeItem[i].vParentPos = m_tWndInfo.vPos;
			m_tTradeItem[i].vPos = { 27.f, 236.f + 23.f * i, 0.f };
			m_tTradeItem[i].vSize = { 345.f, 23.f, 0.f };
			m_tTradeItem[i].vOffset = { 0.f, 0.f, 0.f };
		}

		// 판매 슬롯
		for (int i=0; i<SELL_SLOT; ++i)
		{
			m_tSellItem[i].vParentPos = m_tWndInfo.vPos;
			m_tSellItem[i].vPos	= { 23.f, 60.f + 25.f * i, 0.f };
			m_tSellItem[i].vSize = { 344.f, 24.f, 0.f };
			m_tSellItem[i].vOffset = {0.f, 0.f, 0.f};
		}

		
		// 구입탭 버튼
		m_pTradeBtnTex = m_pTextureMgr->GetTexInfo(L"Button", L"Trade", m_tTradeBtn.iHover);
		if (nullptr != m_pTradeBtnTex)
		{
			m_tTradeBtn.vParentPos = m_tWndInfo.vPos;
			m_tTradeBtn.vPos = { 0.f, 26.f, 0.f };
			m_tTradeBtn.vSize = { float(m_pTradeBtnTex->tImgInfo.Width), 60.f, 0.f };
			m_tTradeBtn.vOffset = m_pTradeBtnTex->vOffset;
		}

		// 판매탭 버튼
		m_pRebuyBtnTex = m_pTextureMgr->GetTexInfo(L"Button", L"Rebuy", m_tReBuyBtn.iHover);
		if (nullptr != m_pRebuyBtnTex)
		{
			m_tReBuyBtn.vParentPos = m_tWndInfo.vPos;
			m_tReBuyBtn.vPos = { 0.f, 26.f + 60.f, 0.f };
			m_tReBuyBtn.vSize = { float(m_pRebuyBtnTex->tImgInfo.Width), 60.f, 0.f };
			m_tReBuyBtn.vOffset = m_pRebuyBtnTex->vOffset;
		}

		// 협상 버튼
		m_tNegotiationBtn.vParentPos = m_tWndInfo.vPos;
		m_tNegotiationBtn.vPos = { 347.f, 398.f, 0.f };
		m_tNegotiationBtn.vSize = { 42.f, 16.f, 0.f };
		m_tNegotiationBtn.vOffset = { 0.f, 0.f, 0.f };

		// 결제 버튼
		m_tBuyBtn.vParentPos = m_tWndInfo.vPos;
		m_tBuyBtn.vPos = { 347.f, 421.f, 0.f };
		m_tBuyBtn.vSize = { 42.f, 16.f, 0.f };
		m_tBuyBtn.vOffset = { 0.f, 0.f, 0.f };

		// 모든 아이템 위로
		m_tAllUpBtn.vParentPos = m_tWndInfo.vPos;
		m_tAllUpBtn.vPos = { 269.f, 398.f, 0.f };
		m_tAllUpBtn.vSize = { 16.f, 16.f, 0.f };
		m_tAllUpBtn.vOffset = { 0.f, 0.f, 0.f };

		// 모든 아이템 아래로
		m_tAllDownBtn.vParentPos = m_tWndInfo.vPos;
		m_tAllDownBtn.vPos = { 295.f, 398.f, 0.f };
		m_tAllDownBtn.vSize = { 16.f, 16.f, 0.f };
		m_tAllDownBtn.vOffset = { 0.f, 0.f, 0.f };


		// 스크롤
		{
			m_pScrollUpTex = m_pTextureMgr->GetTexInfo(L"Button", L"ScrollUp");
			if (nullptr != m_pScrollUpTex)
			{
				// UP

				// Shop Scroll
				m_tShopScrollUp.vParentPos = m_tWndInfo.vPos;
				m_tShopScrollUp.vPos = { 380.f, 45.f, 0.f };
				m_tShopScrollUp.vSize = { 12.f, 13.f, 0.f };
				m_tShopScrollUp.vOffset = { 0.f, 0.f, 0.f };

				// Trade Scroll
				m_tTradeScrollUp.vParentPos = m_tWndInfo.vPos;
				m_tTradeScrollUp.vPos = { 375.f, 225.f, 0.f };
				m_tTradeScrollUp.vSize = { 12.f, 13.f, 0.f };
				m_tTradeScrollUp.vOffset = { 0.f, 0.f, 0.f };

				// Sell Scroll
				m_tSellScrollUp.vParentPos = m_tWndInfo.vPos;
				m_tSellScrollUp.vPos = { 373.f, 60.f, 0.f };
				m_tSellScrollUp.vSize = { 12.f, 13.f, 0.f };
				m_tSellScrollUp.vOffset = { 0.f, 0.f, 0.f };
			}

			m_pScrollDownTex = m_pTextureMgr->GetTexInfo(L"Button", L"ScrollDown");
			if (nullptr != m_pScrollDownTex)
			{
				// Down

				// Shop Scroll
				m_tShopScrollDown.vParentPos = m_tWndInfo.vPos;
				m_tShopScrollDown.vPos = { 380.f, 185.f, 0.f };
				m_tShopScrollDown.vSize = { 12.f, 13.f, 0.f };
				m_tShopScrollDown.vOffset = { 0.f, 0.f, 0.f };

				// Trade Scroll
				m_tTradeScrollDown.vParentPos = m_tWndInfo.vPos;
				m_tTradeScrollDown.vPos = { 375.f, 370.f, 0.f };
				m_tTradeScrollDown.vSize = { 12.f, 13.f, 0.f };
				m_tTradeScrollDown.vOffset = { 0.f, 0.f, 0.f };

				// Sell Scroll
				m_tSellScrollDown.vParentPos = m_tWndInfo.vPos;
				m_tSellScrollDown.vPos = { 373.f, 370.f, 0.f };
				m_tSellScrollDown.vSize = { 12.f, 13.f, 0.f };
				m_tSellScrollDown.vOffset = { 0.f, 0.f, 0.f };
			}

			m_pScrollBarTex = m_pTextureMgr->GetTexInfo(L"Button", L"ScrollBar");
			if (nullptr != m_pScrollBarTex)
			{
				// Bar

				// Shop Scroll
				m_tShopScrollBar.vParentPos = m_tWndInfo.vPos;
				m_tShopScrollBar.vPos = { 380.f, 58.f, 0.f };
				m_tShopScrollBar.vSize = { 10.f, 19.f, 0.f };
				m_tShopScrollBar.vOffset = { 0.f, 0.f, 0.f };

				// Trade Scroll
				m_tTradeScrollBar.vParentPos = m_tWndInfo.vPos;
				m_tTradeScrollBar.vPos = { 375.f, 238.f, 0.f };
				m_tTradeScrollBar.vSize = { 10.f, 19.f, 0.f };
				m_tTradeScrollBar.vOffset = { 0.f, 0.f, 0.f };

				// Sell Scroll
				m_tSellScrollBar.vParentPos = m_tWndInfo.vPos;
				m_tSellScrollBar.vPos = { 373.f, 73.f, 0.f };
				m_tSellScrollBar.vSize = { 10.f, 19.f, 0.f };
				m_tSellScrollBar.vOffset = { 0.f, 0.f, 0.f };
			}
		}		
	}

	return S_OK;
}

void CShopWnd::Update()
{
	CUIWnd::WindowMove();

	m_tNegotiationBtn.vParentPos = m_tBuyBtn.vParentPos = m_tExitInfo.vParentPos = m_tTradeBtn.vParentPos = m_tReBuyBtn.vParentPos 
		= m_tShopScrollUp.vParentPos = m_tTradeScrollUp.vParentPos = m_tSellScrollUp.vParentPos 
		= m_tShopScrollDown.vParentPos = m_tTradeScrollDown.vParentPos = m_tSellScrollDown.vParentPos 
		= m_tShopScrollBar.vParentPos = m_tTradeScrollBar.vParentPos = m_tSellScrollBar.vParentPos 
		= m_tWndInfo.vPos;
	
	D3DXVECTOR3 vMouse = m_pMouseMgr->GetMouse();
	if (0 == m_tNegotiationBtn.iHover && m_tNegotiationBtn.IsRectInPoint(vMouse))
		m_pSoundMgr->PlaySound(L"barover.MP3", CHANNEL_ID::CHANNEL_WINDOW);
	if(0 == m_tBuyBtn.iHover && m_tBuyBtn.IsRectInPoint(vMouse))
		m_pSoundMgr->PlaySound(L"barover.MP3", CHANNEL_ID::CHANNEL_WINDOW);
	if(0 == m_tTradeBtn.iHover && m_tTradeBtn.IsRectInPoint(vMouse))
		m_pSoundMgr->PlaySound(L"barover.MP3", CHANNEL_ID::CHANNEL_WINDOW);
	if(0 == m_tReBuyBtn.iHover && m_tReBuyBtn.IsRectInPoint(vMouse))
		m_pSoundMgr->PlaySound(L"barover.MP3", CHANNEL_ID::CHANNEL_WINDOW);
	if(0 == m_tExitInfo.iHover && m_tExitInfo.IsRectInPoint(vMouse))
		m_pSoundMgr->PlaySound(L"barover.MP3", CHANNEL_ID::CHANNEL_WINDOW);

	for (size_t i = 0; i < m_vecShopItem.size(); ++i)
	{
		if (nullptr != m_vecShopItem[i])
		{
			if (0 >= m_vecShopItem[i]->iCount)
			{
				SafeDelete(m_vecShopItem[i]);
				m_vecShopItem.erase(m_vecShopItem.begin() + i);
			}
		}
	}
	if (m_vecShopItem.size() > BUY_SLOT && m_iShopScroll >= int(m_vecShopItem.size()) - BUY_SLOT)
	{
		m_iShopScroll = m_vecShopItem.size() - BUY_SLOT;
	}


	for (size_t i = 0; i < m_vecTradeItem.size(); ++i)
	{
		if (nullptr != m_vecTradeItem[i])
		{
			if (0 >= m_vecTradeItem[i]->iCount)
			{
				SafeDelete(m_vecTradeItem[i]);
				m_vecTradeItem.erase(m_vecTradeItem.begin() + i);
			}
		}
	}
	if (m_vecTradeItem.size() > BUY_SLOT && m_iTradeScroll >= int(m_vecTradeItem.size()) - SELL_SLOT)
	{
		m_iTradeScroll = m_vecTradeItem.size() - SELL_SLOT;
	}


	for (size_t i = 0; i < m_vecSellItem.size(); ++i)
	{
		if (nullptr != m_vecSellItem[i])
		{
			if (0 >= m_vecSellItem[i]->iCount)
			{
				SafeDelete(m_vecSellItem[i]);
				m_vecSellItem.erase(m_vecSellItem.begin() + i);
			}
		}
	}
	if (m_vecSellItem.size() > SELL_SLOT && m_iSellScroll >= int(m_vecSellItem.size()) - SELL_SLOT)
	{
		m_iSellScroll = m_vecSellItem.size() - SELL_SLOT;
	}
}

void CShopWnd::Render()
{
	if (!m_bIsShow)
		return;

	D3DXMATRIX matScale, matTrans, matWorld;

	m_pWndTex = m_pTextureMgr->GetTexInfo(L"UI", L"ShopWnd", m_iShopState);
	if (nullptr != m_pWndTex) {
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
		D3DXMatrixTranslation(&matTrans, m_tWndInfo.vPos.x, m_tWndInfo.vPos.y, 0.f);
		matWorld = matScale * matTrans;

		m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
		m_pDeviceMgr->GetSprite()->Draw(m_pWndTex->pTexture, nullptr,
			&m_tWndInfo.vOffset, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

		// 나가기 버튼
		m_tExitInfo.IsRectInPoint(m_pMouseMgr->GetMouse());
		m_pExitTex = m_pTextureMgr->GetTexInfo(L"Button", L"Exit", m_tExitInfo.iHover);

		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
		D3DXMatrixTranslation(&matTrans, m_tExitInfo.vParentPos.x + m_tExitInfo.vPos.x, m_tExitInfo.vParentPos.y + m_tExitInfo.vPos.y, 0.f);
		matWorld = matScale * matTrans;
		m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
		m_pDeviceMgr->GetSprite()->Draw(m_pExitTex->pTexture, nullptr, &m_tExitInfo.vOffset, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

		// 구매 탭 버튼
		m_tTradeBtn.IsRectInPoint(m_pMouseMgr->GetMouse());
		m_pTradeBtnTex = m_pTextureMgr->GetTexInfo(L"Button", L"Trade", m_tTradeBtn.iHover);
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
		D3DXMatrixTranslation(&matTrans, m_tTradeBtn.vParentPos.x + m_tTradeBtn.vPos.x, m_tTradeBtn.vParentPos.y + m_tTradeBtn.vPos.y, 0.f);
		matWorld = matScale * matTrans;
		m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
		m_pDeviceMgr->GetSprite()->Draw(m_pTradeBtnTex->pTexture, nullptr, &m_pTradeBtnTex->vOffset, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

		// 판매 탭 버튼
		m_tReBuyBtn.IsRectInPoint(m_pMouseMgr->GetMouse());
		m_pRebuyBtnTex = m_pTextureMgr->GetTexInfo(L"Button", L"Rebuy", m_tReBuyBtn.iHover);
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
		D3DXMatrixTranslation(&matTrans, m_tReBuyBtn.vParentPos.x + m_tReBuyBtn.vPos.x, m_tReBuyBtn.vParentPos.y + m_tReBuyBtn.vPos.y, 0.f);
		matWorld = matScale * matTrans;
		m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
		m_pDeviceMgr->GetSprite()->Draw(m_pRebuyBtnTex->pTexture, nullptr, &m_pRebuyBtnTex->vOffset, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));


		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
		m_pDeviceMgr->GetSprite()->SetTransform(&matScale);

		RECT rt;

		//// 슬롯
		//SetRect(&rt,
		//	int(m_tWndInfo.vPos.x + 37.f), int(m_tWndInfo.vPos.y + 381.f),
		//	int(m_tWndInfo.vPos.x + 90.f), int(m_tWndInfo.vPos.y + 392.f));
		//m_pDeviceMgr->DrawFont(to_wstring(m_vecItem.size()), &rt, DT_CENTER, D3DCOLOR_ARGB(255, 255, 255, 255));


		// 여분 슬롯
		SetRect(&rt,
			int(m_tWndInfo.vPos.x + 77.f), int(m_tWndInfo.vPos.y + 5.f),
			int(m_tWndInfo.vPos.x + 158.f), int(m_tWndInfo.vPos.y + 19.f));
		m_pDeviceMgr->DrawFont(L"-", &rt, DT_CENTER, D3DCOLOR_ARGB(255, 255, 255, 255));

		// 여유 무게
		SetRect(&rt,
			int(m_tWndInfo.vPos.x + 238.f), int(m_tWndInfo.vPos.y + 5.f),
			int(m_tWndInfo.vPos.x + 318.f), int(m_tWndInfo.vPos.y + 19.f));
		m_pDeviceMgr->DrawFont(L"-", &rt, DT_CENTER, D3DCOLOR_ARGB(255, 255, 255, 255));

		// 시드
		SetRect(&rt,
			int(m_tWndInfo.vPos.x + 63.f), int(m_tWndInfo.vPos.y + 400.f),
			int(m_tWndInfo.vPos.x + 207.f), int(m_tWndInfo.vPos.y + 413.f));
		m_pDeviceMgr->DrawFont(to_wstring(m_pUIMgr->GetPlayerIdentity().iSeed), &rt, DT_CENTER, D3DCOLOR_ARGB(255, 255, 255, 255));
		

		// 아이템 슬롯
		switch (m_iShopState)
		{
		// 구매
		case 0:
			// Shop Item
			for (int i = 0; i < BUY_SLOT; ++i)
			{
				if (i + m_iShopScroll >= int(m_vecShopItem.size()))
					break;

				const TEX_INFO*	pTexInfo = m_pTextureMgr->GetTexInfo(m_vecShopItem[i + m_iShopScroll]->wstrObjKey, m_vecShopItem[i + m_iShopScroll]->wstrStateKey);
				if (nullptr != pTexInfo)
				{
					// 아이템 그림
					D3DXMatrixTranslation(&matTrans, m_tWndInfo.vPos.x + 43.f, m_tWndInfo.vPos.y + 64.f + 25.f * i, 0.f);
					m_pDeviceMgr->GetSprite()->SetTransform(&matTrans);
					m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
						&pTexInfo->vOffset, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
				}

				D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
				m_pDeviceMgr->GetSprite()->SetTransform(&matScale);

				// 아이템 이름
				SetRect(&rt,
					int(m_tWndInfo.vPos.x + 60.f), int(m_tWndInfo.vPos.y + 57.f + 25.f * i),
					int(m_tWndInfo.vPos.x + 288.f), int(m_tWndInfo.vPos.y + 71.f + 25.f * i));
				m_pDeviceMgr->DrawFont(m_vecShopItem[i + m_iShopScroll]->wstrStateKey, &rt, DT_CENTER, m_vecShopItem[i + m_iShopScroll]->dwColor);

				SetRect(&rt,
					int(m_tWndInfo.vPos.x + 294.f), int(m_tWndInfo.vPos.y + 57.f + 25.f * i),
					int(m_tWndInfo.vPos.x + 372.f), int(m_tWndInfo.vPos.y + 71.f + 25.f * i));
				m_pDeviceMgr->DrawFont(to_wstring(m_vecShopItem[i + m_iShopScroll]->GetPrice()), &rt, DT_CENTER);
			}

			// Buy Scroll
			// 스크롤 업
			if (nullptr != m_pScrollUpTex)
			{
				D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
				D3DXMatrixTranslation(&matTrans, m_tWndInfo.vPos.x + m_tShopScrollUp.vPos.x, m_tWndInfo.vPos.y + m_tShopScrollUp.vPos.y, 0.f);
				m_pDeviceMgr->GetSprite()->SetTransform(&(matScale*matTrans));
				m_pDeviceMgr->GetSprite()->Draw(m_pScrollUpTex->pTexture, nullptr, &m_pScrollUpTex->vOffset, nullptr, m_pScrollUpTex->dwBlend);
			}

			// 스크롤 다운
			if (nullptr != m_pScrollDownTex)
			{
				D3DXMatrixTranslation(&matTrans, m_tWndInfo.vPos.x + m_tShopScrollDown.vPos.x, m_tWndInfo.vPos.y + m_tShopScrollDown.vPos.y, 0.f);
				m_pDeviceMgr->GetSprite()->SetTransform(&(matScale*matTrans));
				m_pDeviceMgr->GetSprite()->Draw(m_pScrollDownTex->pTexture, nullptr, &m_pScrollDownTex->vOffset, nullptr, m_pScrollDownTex->dwBlend);
			}

			// 스크롤
			if (BUY_SLOT < m_vecShopItem.size())
			{
				// 스크롤 바
				if (nullptr != m_pScrollBarTex)
				{
					// 62 ~ 328 = 247
					D3DXMatrixScaling(&matScale, m_tShopScrollDown.vSize.x / m_tShopScrollBar.vSize.x, 1.f/*((266.f - (25.f * m_vecItem.size() - INVEN_SIZE)) / m_tScrollBar.vSize.y)*/, 0.f);
					D3DXMatrixTranslation(&matTrans, m_tWndInfo.vPos.x + m_tShopScrollBar.vPos.x, m_tWndInfo.vPos.y + m_tShopScrollBar.vPos.y + ((m_tShopScrollDown.vPos.y - m_tShopScrollUp.vPos.y - m_tShopScrollUp.vSize.y - m_tShopScrollBar.vSize.y) / (m_vecShopItem.size() - BUY_SLOT)) * m_iShopScroll, 0.f);
					m_pDeviceMgr->GetSprite()->SetTransform(&(matScale*matTrans));
					m_pDeviceMgr->GetSprite()->Draw(m_pScrollBarTex->pTexture, nullptr, &m_pScrollBarTex->vOffset, nullptr, m_pScrollBarTex->dwBlend);
				}
			}

			// Trade Item
			for (int i = 0; i < BUY_SLOT; ++i)
			{
				if (i + m_iTradeScroll >= int(m_vecTradeItem.size()))
					break;

				const TEX_INFO*	pTexInfo = m_pTextureMgr->GetTexInfo(m_vecTradeItem[i + m_iTradeScroll]->wstrObjKey, m_vecTradeItem[i + m_iTradeScroll]->wstrStateKey);
				if (nullptr != pTexInfo)
				{
					// 아이템 그림
					D3DXMatrixTranslation(&matTrans, m_tWndInfo.vPos.x + 43.f, m_tWndInfo.vPos.y + 248.f + 25.f * i, 0.f);
					m_pDeviceMgr->GetSprite()->SetTransform(&matTrans);
					m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
						&pTexInfo->vOffset, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
				}

				D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
				m_pDeviceMgr->GetSprite()->SetTransform(&matScale);

				// 아이템 이름
				SetRect(&rt,
					int(m_tWndInfo.vPos.x + 61.f), int(m_tWndInfo.vPos.y + 241.f + 25.f * i),
					int(m_tWndInfo.vPos.x + 298.f), int(m_tWndInfo.vPos.y + 255.f + 25.f * i));
				m_pDeviceMgr->DrawFont(m_vecTradeItem[i + m_iTradeScroll]->wstrStateKey, &rt, DT_CENTER, m_vecTradeItem[i + m_iTradeScroll]->dwColor);

				// 아이템 개수
				SetRect(&rt,
					int(m_tWndInfo.vPos.x + 295.f), int(m_tWndInfo.vPos.y + 241.f + 25.f * i),
					int(m_tWndInfo.vPos.x + 373.f), int(m_tWndInfo.vPos.y + 255.f + 25.f * i));
				m_pDeviceMgr->DrawFont(to_wstring(m_vecTradeItem[i + m_iTradeScroll]->iCount) + L" / " + to_wstring(m_vecTradeItem[i + m_iTradeScroll]->iPrice), &rt, DT_CENTER | DT_NOCLIP);
			}

			// 스크롤
			// 스크롤 업
			if (nullptr != m_pScrollUpTex)
			{
				D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
				D3DXMatrixTranslation(&matTrans, m_tWndInfo.vPos.x + m_tTradeScrollUp.vPos.x, m_tWndInfo.vPos.y + m_tTradeScrollUp.vPos.y, 0.f);
				m_pDeviceMgr->GetSprite()->SetTransform(&(matScale*matTrans));
				m_pDeviceMgr->GetSprite()->Draw(m_pScrollUpTex->pTexture, nullptr, &m_pScrollUpTex->vOffset, nullptr, m_pScrollUpTex->dwBlend);
			}

			// 스크롤 다운
			if (nullptr != m_pScrollDownTex)
			{
				D3DXMatrixTranslation(&matTrans, m_tWndInfo.vPos.x + m_tTradeScrollDown.vPos.x, m_tWndInfo.vPos.y + m_tTradeScrollDown.vPos.y, 0.f);
				m_pDeviceMgr->GetSprite()->SetTransform(&(matScale*matTrans));
				m_pDeviceMgr->GetSprite()->Draw(m_pScrollDownTex->pTexture, nullptr, &m_pScrollDownTex->vOffset, nullptr, m_pScrollDownTex->dwBlend);
			}
			if (BUY_SLOT < m_vecTradeItem.size())
			{
				// 스크롤 바
				if (nullptr != m_pScrollBarTex)
				{
					// 62 ~ 328 = 247
					D3DXMatrixScaling(&matScale, m_tTradeScrollDown.vSize.x / m_tTradeScrollBar.vSize.x, 1.f/*((266.f - (25.f * m_vecItem.size() - INVEN_SIZE)) / m_tScrollBar.vSize.y)*/, 0.f);
					D3DXMatrixTranslation(&matTrans, m_tWndInfo.vPos.x + m_tTradeScrollBar.vPos.x, m_tWndInfo.vPos.y + m_tTradeScrollBar.vPos.y + ((m_tTradeScrollDown.vPos.y - m_tTradeScrollUp.vPos.y - m_tTradeScrollUp.vSize.y - m_tTradeScrollBar.vSize.y) / (m_vecTradeItem.size() - BUY_SLOT)) * m_iTradeScroll, 0.f);
					m_pDeviceMgr->GetSprite()->SetTransform(&(matScale*matTrans));
					m_pDeviceMgr->GetSprite()->Draw(m_pScrollBarTex->pTexture, nullptr, &m_pScrollBarTex->vOffset, nullptr, m_pScrollBarTex->dwBlend);
				}
			}
			{
				D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
				m_pDeviceMgr->GetSprite()->SetTransform(&matScale);
				int iAllPrice = 0;

				auto iter_find = m_vecTradeItem.begin();
				for (; iter_find != m_vecTradeItem.end(); ++iter_find)
				{
					iAllPrice += (*iter_find)->iPrice * (*iter_find)->iCount;
				}
				// 결제 금액
				SetRect(&rt,
					int(m_tWndInfo.vPos.x + 82.f), int(m_tWndInfo.vPos.y + 423.f),
					int(m_tWndInfo.vPos.x + 227.f), int(m_tWndInfo.vPos.y + 437.f));
				m_pDeviceMgr->DrawFont(to_wstring(iAllPrice), &rt, DT_CENTER | DT_NOCLIP, D3DCOLOR_ARGB(255, 255, 255, 255));
			}
			break;

		// 판매
		case 1:
			// Sell Item
			for (int i = 0; i < SELL_SLOT; ++i)
			{
				if (i + m_iSellScroll >= int(m_vecSellItem.size()))
					break;

				const TEX_INFO*	pTexInfo = m_pTextureMgr->GetTexInfo(m_vecSellItem[i + m_iSellScroll]->wstrObjKey, m_vecSellItem[i + m_iSellScroll]->wstrStateKey);
				if (nullptr != pTexInfo)
				{
					// 아이템 그림
					D3DXMatrixTranslation(&matTrans, m_tWndInfo.vPos.x + 42.f, m_tWndInfo.vPos.y + 73.f + 25.f * i, 0.f);
					m_pDeviceMgr->GetSprite()->SetTransform(&matTrans);
					m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
						&pTexInfo->vOffset, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
				}

				D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
				m_pDeviceMgr->GetSprite()->SetTransform(&matScale);

				// 아이템 이름
				SetRect(&rt,
					int(m_tWndInfo.vPos.x + 60.f), int(m_tWndInfo.vPos.y + 67.f + 25.f * i),
					int(m_tWndInfo.vPos.x + 306.f), int(m_tWndInfo.vPos.y + 80.f + 25.f * i));
				m_pDeviceMgr->DrawFont(m_vecSellItem[i + m_iSellScroll]->wstrStateKey, &rt, DT_CENTER, m_vecSellItem[i + m_iSellScroll]->dwColor);

				// 아이템 옵션
				SetRect(&rt,
					int(m_tWndInfo.vPos.x + 312.f), int(m_tWndInfo.vPos.y + 67.f + 25.f * i),
					int(m_tWndInfo.vPos.x + 368.f), int(m_tWndInfo.vPos.y + 60.f + 25.f * i));
				m_pDeviceMgr->DrawFont(to_wstring(m_vecSellItem[i + m_iSellScroll]->iCount) + L" / " + to_wstring(m_vecSellItem[i + m_iSellScroll]->iPrice), &rt, DT_CENTER | DT_NOCLIP);
			}

			// Sell Scroll
			// 스크롤 업
			if (nullptr != m_pScrollUpTex)
			{
				D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
				D3DXMatrixTranslation(&matTrans, m_tWndInfo.vPos.x + m_tSellScrollUp.vPos.x, m_tWndInfo.vPos.y + m_tSellScrollUp.vPos.y, 0.f);
				m_pDeviceMgr->GetSprite()->SetTransform(&(matScale*matTrans));
				m_pDeviceMgr->GetSprite()->Draw(m_pScrollUpTex->pTexture, nullptr, &m_pScrollUpTex->vOffset, nullptr, m_pScrollUpTex->dwBlend);
			}

			// 스크롤 다운
			if (nullptr != m_pScrollDownTex)
			{
				D3DXMatrixTranslation(&matTrans, m_tWndInfo.vPos.x + m_tSellScrollDown.vPos.x, m_tWndInfo.vPos.y + m_tSellScrollDown.vPos.y, 0.f);
				m_pDeviceMgr->GetSprite()->SetTransform(&(matScale*matTrans));
				m_pDeviceMgr->GetSprite()->Draw(m_pScrollDownTex->pTexture, nullptr, &m_pScrollDownTex->vOffset, nullptr, m_pScrollDownTex->dwBlend);
			}
			// 스크롤
			if (SELL_SLOT < m_vecSellItem.size())
			{
				// 스크롤 바
				if (nullptr != m_pScrollBarTex)
				{
					// 62 ~ 328 = 247
					D3DXMatrixScaling(&matScale, m_tSellScrollDown.vSize.x / m_tSellScrollBar.vSize.x, 1.f/*((266.f - (25.f * m_vecItem.size() - INVEN_SIZE)) / m_tScrollBar.vSize.y)*/, 0.f);
					D3DXMatrixTranslation(&matTrans, m_tWndInfo.vPos.x + m_tSellScrollBar.vPos.x, m_tWndInfo.vPos.y + m_tSellScrollBar.vPos.y + ((m_tSellScrollDown.vPos.y - m_tSellScrollUp.vPos.y - m_tSellScrollUp.vSize.y - m_tSellScrollBar.vSize.y) / (m_vecSellItem.size() - SELL_SLOT)) * m_iSellScroll, 0.f);
					m_pDeviceMgr->GetSprite()->SetTransform(&(matScale*matTrans));
					m_pDeviceMgr->GetSprite()->Draw(m_pScrollBarTex->pTexture, nullptr, &m_pScrollBarTex->vOffset, nullptr, m_pScrollBarTex->dwBlend);
				}
			}

			break;
		}		
	}
}

void CShopWnd::Release()
{
	for (auto& Shop : m_vecShopItem)
		SafeDelete(Shop);
	m_vecShopItem.clear();
	m_vecShopItem.shrink_to_fit();

	for (auto& Trade : m_vecTradeItem)
		SafeDelete(Trade);
	m_vecTradeItem.clear();
	m_vecTradeItem.shrink_to_fit();

	for (auto& Sell : m_vecSellItem)
		SafeDelete(Sell);
	m_vecSellItem.clear();
	m_vecSellItem.shrink_to_fit();
}

void CShopWnd::LClickDown()
{
	if (!m_bIsShow)
		return;

	D3DXVECTOR3	vMouse = m_pMouseMgr->GetMouse();

	if (!m_tWndInfo.IsRectInPoint(vMouse))
		return;

	// 왼쪽 버튼
	if (m_tTradeBtn.IsRectInPoint(vMouse))
	{
		m_iShopState = 0;
	}
	if (m_tReBuyBtn.IsRectInPoint(vMouse))
	{
		m_iShopState = 1;
	}

	// 모든 아이템 없애기
	if (m_tAllDownBtn.IsRectInPoint(vMouse))
	{
		for (auto& Item : m_vecTradeItem)
		{
			SafeDelete(Item);
		}
		m_vecTradeItem.clear();
		m_vecTradeItem.shrink_to_fit();
	}

	// 모든 아이템 아래로
	if (m_tAllUpBtn.IsRectInPoint(vMouse))
	{
		// Shop -> Trade
		for (auto& Item : m_vecShopItem)
		{
			bool BisTradeFind = false;
			for (auto& Trade : m_vecTradeItem)
			{
				if (!lstrcmp(Trade->wstrStateKey.c_str(), Item->wstrStateKey.c_str()))
				{
					if (ITEM_TYPE::ITEM_EQUIP != Item->eItemType)
					{
						BisTradeFind = true;
						++Trade->iCount;
					}
					break;
				}
			}

			if (!BisTradeFind)
			{
				CItemData* pItemData = nullptr;
				switch (Item->eItemType)
				{
				case ITEM_TYPE::ITEM_EQUIP:
					pItemData = new CItemEquipData(*(dynamic_cast<CItemEquipData*>(Item)));
					break;

				case ITEM_TYPE::ITEM_USE:
					pItemData = new CItemUsableData(*(dynamic_cast<CItemUsableData*>(Item)));
					break;

				case ITEM_TYPE::ITEM_ETC:
					pItemData = new CItemData(*(dynamic_cast<CItemData*>(Item)));
					break;
				}

				// Trade에 담기
				if (nullptr != pItemData)
				{
					pItemData->SetPrice(Item->GetPrice());
					m_vecTradeItem.push_back(pItemData);
				}
			}
		}
	}

	
	// 스크롤
	switch (m_iShopState)
	{
		case 0:
		{
			// Shop
			if (m_tShopScrollUp.IsRectInPoint(vMouse))
			{
				CSoundMgr::GetInstance()->PlaySound(L"BtnClick.wav", CHANNEL_ID::CHANNEL_WINDOW);

				if (0 < m_iShopScroll)
					--m_iShopScroll;
			}
			if (m_tShopScrollDown.IsRectInPoint(vMouse))
			{
				CSoundMgr::GetInstance()->PlaySound(L"BtnClick.wav", CHANNEL_ID::CHANNEL_WINDOW);

				if (int(m_vecShopItem.size() - BUY_SLOT) > m_iShopScroll)
					++m_iShopScroll;
			}

			if (!m_bIsShopScrollMove)
			{
				if (m_tShopScrollBar.IsRectInPoint(vMouse))
				{
					m_vScrollStartPos = vMouse - m_tShopScrollBar.vPos;
					m_bIsShopScrollMove = true;
				}
			}

			// Trade
			if (m_tTradeScrollUp.IsRectInPoint(vMouse))
			{
				CSoundMgr::GetInstance()->PlaySound(L"BtnClick.wav", CHANNEL_ID::CHANNEL_WINDOW);

				if (0 < m_iTradeScroll)
					--m_iTradeScroll;
			}
			if (m_tTradeScrollDown.IsRectInPoint(vMouse))
			{
				CSoundMgr::GetInstance()->PlaySound(L"BtnClick.wav", CHANNEL_ID::CHANNEL_WINDOW);

				if (int(m_vecTradeItem.size() - BUY_SLOT) > m_iTradeScroll)
					++m_iTradeScroll;
			}

			if (!m_bIsTradeScrollMove)
			{
				if (m_tTradeScrollBar.IsRectInPoint(vMouse))
				{
					m_vScrollStartPos = vMouse - m_tTradeScrollBar.vPos;
					m_bIsTradeScrollMove = true;
				}
			}
			
			// 협상 버튼
			if (m_tNegotiationBtn.IsRectInPoint(vMouse))
			{

			}

			// 결제 버튼
			if (m_tBuyBtn.IsRectInPoint(vMouse))
			{
				CSoundMgr::GetInstance()->PlaySound(L"BtnClick.wav", CHANNEL_ID::CHANNEL_WINDOW);

				int iAllPrice = 0;

				auto iter_find = m_vecTradeItem.begin();
				for (; iter_find != m_vecTradeItem.end(); ++iter_find)
				{
					iAllPrice += (*iter_find)->iPrice * (*iter_find)->iCount;
				}

				if (iAllPrice <= m_pUIMgr->GetPlayerIdentity().iSeed)
				{
					auto iter_find = m_vecTradeItem.begin();
					for (; iter_find != m_vecTradeItem.end();)
					{
						dynamic_cast<CInvenWnd*>(m_pUIMgr->GetWindow(WND_ID::WNDID_INVEN))->AddItem(*iter_find);
						iter_find = m_vecTradeItem.erase(iter_find);
					}
					m_vecTradeItem.shrink_to_fit();

					m_pUIMgr->GetPlayerIdentity().iSeed -= iAllPrice;
					m_pUIMgr->UpdatePlayerNotify();
				}
			}
		}
		break;

		case 1:
		{
			// Sell
			if (m_tSellScrollUp.IsRectInPoint(vMouse))
			{
				CSoundMgr::GetInstance()->PlaySound(L"BtnClick.wav", CHANNEL_ID::CHANNEL_WINDOW);

				if (0 < m_iSellScroll)
					--m_iSellScroll;
			}
			if (m_tSellScrollDown.IsRectInPoint(vMouse))
			{
				CSoundMgr::GetInstance()->PlaySound(L"BtnClick.wav", CHANNEL_ID::CHANNEL_WINDOW);

				if (int(m_vecSellItem.size() - BUY_SLOT) > m_iSellScroll)
					++m_iSellScroll;
			}

			if (!m_bIsSellScrollMove)
			{
				if (m_tSellScrollBar.IsRectInPoint(vMouse))
				{
					m_vScrollStartPos = vMouse - m_tSellScrollBar.vPos;
					m_bIsSellScrollMove = true;
				}
			}

			// 인벤 -> 샵
			// 마우스에 아이템 없는 상태
			CItemData* pMouseItemData = m_pMouseMgr->GetItemData();
			if (nullptr == pMouseItemData)
			{
				for (int i = 0; i < INVEN_SIZE; ++i)
				{
					m_tSellItem[i].vParentPos = m_tWndInfo.vPos;
					if (m_tSellItem[i].IsRectInPoint(vMouse))
					{
						CSoundMgr::GetInstance()->PlaySound(L"BtnClick.wav", CHANNEL_ID::CHANNEL_WINDOW);

						if (i + m_iSellScroll >= int(m_vecSellItem.size()))
							break;

						// 아이템 클릭
						m_pMouseMgr->SetItemData(WND_ID::WNDID_SHOP, i + m_iSellScroll, m_vecSellItem[i + m_iSellScroll]);
						break;
					}
				}
				m_pMouseMgr->SetSkillData(WND_ID::WNDID_END, -1, nullptr);
			}
			else
			{
				if (WND_ID::WNDID_INVEN == m_pMouseMgr->GetFromWndID())
				{
					m_pUIMgr->GetWindow(WND_ID::WNDID_COUNT)->ShowWindow(true);
					dynamic_cast<CCountWnd*>(m_pUIMgr->GetWindow(WND_ID::WNDID_COUNT))->SetWndData(m_pMouseMgr->GetFromWndID(), WND_ID::WNDID_SHOP, m_pMouseMgr->GetItemData());
				}

				m_pMouseMgr->SetItemData(WND_ID::WNDID_END, -1, nullptr);
				m_pMouseMgr->SetSkillData(WND_ID::WNDID_END, -1, nullptr);
			}
		}
		break;
	}

	m_pMouseMgr->SetItemData(WND_ID::WNDID_END, -1, nullptr);
	m_pMouseMgr->SetSkillData(WND_ID::WNDID_END, -1, nullptr);
}

void CShopWnd::LClickUp()
{
}

void CShopWnd::RClickDown()
{
	if (!m_bIsShow)
		return;

	D3DXVECTOR3	vMouse = m_pMouseMgr->GetMouse();

	if (!m_tWndInfo.IsRectInPoint(vMouse))
		return;

	m_pMouseMgr->SetItemData(WND_ID::WNDID_END, -1, nullptr);
	m_pMouseMgr->SetSkillData(WND_ID::WNDID_END, -1, nullptr);

	switch (m_iShopState)
	{
	case 0:
	{
		bool bIsFind = false;
		// Shop -> Trade
		for (int i = 0; i < BUY_SLOT; ++i)
		{
			m_tShopItem[i].vParentPos = m_tWndInfo.vPos;
			if (m_tShopItem[i].IsRectInPoint(vMouse))
			{
				CSoundMgr::GetInstance()->PlaySound(L"ShopResult.wav", CHANNEL_ID::CHANNEL_WINDOW);

				bIsFind = true;
				if (0 > i + m_iShopScroll || i + m_iShopScroll >= int(m_vecShopItem.size()))
					break;

				m_pUIMgr->GetWindow(WND_ID::WNDID_COUNT)->ShowWindow(true);
				dynamic_cast<CCountWnd*>(m_pUIMgr->GetWindow(WND_ID::WNDID_COUNT))->SetWndData(WND_ID::WNDID_SHOP, WND_ID::WNDID_SHOP, m_vecShopItem[i + m_iShopScroll]);


				/*int iAllPrice = 0;

				auto iter_find = m_vecTradeItem.begin();
				for (; iter_find != m_vecTradeItem.end(); ++iter_find)
				{
					iAllPrice += (*iter_find)->iPrice * (*iter_find)->iCount;
				}

				if (iAllPrice + m_vecShopItem[i + m_iShopScroll]->iPrice <= m_pUIMgr->GetPlayerIdentity().iSeed)
				{
					// 찾기
					bool BisTradeFind = false;
					for (auto& Trade : m_vecTradeItem)
					{
						if (!lstrcmp(Trade->wstrStateKey.c_str(), m_vecShopItem[i + m_iShopScroll]->wstrStateKey.c_str()))
						{
							if (ITEM_TYPE::ITEM_EQUIP != Trade->eItemType)
							{
								BisTradeFind = true;
								++Trade->iCount;
							}
							break;
						}
					}

					if (!BisTradeFind)
					{
						CItemData* pItemData = nullptr;
						switch (m_vecShopItem[i + m_iShopScroll]->eItemType)
						{
						case ITEM_TYPE::ITEM_EQUIP:
							pItemData = new CItemEquipData(*(dynamic_cast<CItemEquipData*>(m_vecShopItem[i + m_iShopScroll])));
							break;

						case ITEM_TYPE::ITEM_USE:
							pItemData = new CItemUsableData(*(dynamic_cast<CItemUsableData*>(m_vecShopItem[i + m_iShopScroll])));
							break;

						case ITEM_TYPE::ITEM_ETC:
							pItemData = new CItemData(*(dynamic_cast<CItemData*>(m_vecShopItem[i + m_iShopScroll])));
							break;
						}

						// Trade에 담기
						if (nullptr != pItemData)
						{
							pItemData->SetPrice(m_vecShopItem[i + m_iShopScroll]->GetPrice());
							m_vecTradeItem.push_back(pItemData);
						}
					}
				}*/

				break;
			}
		}
		/*if (!bIsFind)
		{
			// Trade -> Shop
			for (int i = 0; i < BUY_SLOT; ++i)
			{
				m_tTradeItem[i].vParentPos = m_tWndInfo.vPos;
				if (m_tTradeItem[i].IsRectInPoint(vMouse))
				{
					if (i + m_iTradeScroll >= int(m_vecTradeItem.size()))
						break;

					if (nullptr != m_vecTradeItem[i + m_iTradeScroll])
					{
						m_pUIMgr->GetWindow(WND_ID::WNDID_COUNT)->ShowWindow(true);
						dynamic_cast<CCountWnd*>(m_pUIMgr->GetWindow(WND_ID::WNDID_COUNT))->SetWndData(WND_ID::WNDID_SHOP, WND_ID::WNDID_SHOP, m_vecTradeItem[i + m_iTradeScroll]);
					}
					break;
				}
			}
		}*/
	}
		break;

	case 1:
		for (int i = 0; i < SELL_SLOT; ++i)
		{
			m_tSellItem[i].vParentPos = m_tWndInfo.vPos;
			if (m_tSellItem[i].IsRectInPoint(vMouse))
			{
				CSoundMgr::GetInstance()->PlaySound(L"ShopResult.wav", CHANNEL_ID::CHANNEL_WINDOW);

				if (i + m_iSellScroll >= int(m_vecSellItem.size()))
					break;

				m_pUIMgr->GetWindow(WND_ID::WNDID_COUNT)->ShowWindow(true);
				dynamic_cast<CCountWnd*>(m_pUIMgr->GetWindow(WND_ID::WNDID_COUNT))->SetWndData(WND_ID::WNDID_SHOP, WND_ID::WNDID_INVEN, m_vecSellItem[i + m_iSellScroll]);
				break;
			}
		}
		break;
	}
}

void CShopWnd::AddItem(CItemData* pItemData)
{
	NULL_CHECK(pItemData);

	pItemData->iCount = 99;

	bool bIsFind = false;
	auto iter_find = m_vecShopItem.begin();
	for (; iter_find != m_vecShopItem.end(); ++iter_find)
	{
		if (!lstrcmp(pItemData->wstrStateKey.c_str(), (*iter_find)->wstrStateKey.c_str()))
		{
			bIsFind = true;
			if (99 >= (*iter_find)->iCount + pItemData->iCount)
			{
				SafeDelete(pItemData);
				break;
			}
			else
				m_vecShopItem.push_back(pItemData);
		}
	}

	if (!bIsFind)
		m_vecShopItem.push_back(pItemData);
}

void CShopWnd::DeleteAllItem()
{
	for (auto& Shop : m_vecShopItem)
		SafeDelete(Shop);
	m_vecShopItem.clear();
	m_vecShopItem.shrink_to_fit();

	for (auto& Trade : m_vecTradeItem)
		SafeDelete(Trade);
	m_vecTradeItem.clear();
	m_vecTradeItem.shrink_to_fit();
}

void CShopWnd::InsertSellItem(CItemData * pItemData)
{
	NULL_CHECK(pItemData);

	bool bIsFind = false;
	auto iter_find = m_vecSellItem.begin();
	for (; iter_find != m_vecSellItem.end(); ++iter_find)
	{
		if (!lstrcmp(pItemData->wstrStateKey.c_str(), (*iter_find)->wstrStateKey.c_str()))
		{
			bIsFind = true;
			if (99 >= (*iter_find)->iCount + pItemData->iCount)
			{
				(*iter_find)->iCount += pItemData->iCount;
				SafeDelete(pItemData);
				break;
			}
			else
				m_vecSellItem.push_back(pItemData);
		}
	}

	if (!bIsFind)
		m_vecSellItem.push_back(pItemData);
}

void CShopWnd::InsertTradeItem(CItemData * pItemData)
{
	bool bIsFind = false;
	auto iter_find = m_vecTradeItem.begin();
	for (; iter_find != m_vecTradeItem.end(); ++iter_find)
	{
		if (!lstrcmp(pItemData->wstrStateKey.c_str(), (*iter_find)->wstrStateKey.c_str()))
		{
			bIsFind = true;
			if (99 >= (*iter_find)->iCount + pItemData->iCount)
			{
				(*iter_find)->iCount += pItemData->iCount;
				SafeDelete(pItemData);
				break;
			}
			else
				m_vecTradeItem.push_back(pItemData);
		}
	}

	if (!bIsFind)
		m_vecTradeItem.push_back(pItemData);
}


CShopWnd* CShopWnd::Create()
{
	CShopWnd* pInstance = new CShopWnd;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}