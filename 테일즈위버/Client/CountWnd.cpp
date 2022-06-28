#include "stdafx.h"
#include "CountWnd.h"

#include "ItemEquipData.h"
#include "ItemUsableData.h"

#include "QuickWnd.h"
#include "ShopWnd.h"
#include "InvenWnd.h"

CCountWnd::CCountWnd()
{
}


CCountWnd::~CCountWnd()
{
	Release();
}

HRESULT CCountWnd::Initialize()
{
	m_pWndTex = m_pTextureMgr->GetTexInfo(L"UI", L"DropWnd");
	if (nullptr != m_pWndTex) {
		m_tWndInfo.vPos = { float(CLIENTCX * 0.5f - m_pWndTex->tImgInfo.Width * 0.5f), float(CLIENTCY * 0.5f - m_pWndTex->tImgInfo.Height * 0.5f), 0.f };
		m_tWndInfo.vSize = { float(m_pWndTex->tImgInfo.Width), float(m_pWndTex->tImgInfo.Height), 0.f };
		m_tWndInfo.vOffset = m_pWndTex->vOffset;

		m_tMoveInfo.vSize = { float(m_pWndTex->tImgInfo.Width), 20.f, 0.f };
		m_tMoveInfo.vParentPos = m_tWndInfo.vPos;
		m_tMoveInfo.vPos = { 0.f, 0.f, 0.f };
		m_tMoveInfo.vOffset = m_tWndInfo.vOffset;

		m_pMaxBtnTex = m_pTextureMgr->GetTexInfo(L"Button", L"Max");
		if (nullptr != m_pMaxBtnTex)
		{
			m_tMaxBtn.vParentPos	= m_tWndInfo.vPos;
			m_tMaxBtn.vPos			= { 116.f, 17.f, 0.f };
			m_tMaxBtn.vSize			= { 27.f, 14.f, 0.f };
			m_tMaxBtn.vOffset		= { 0.f, 0.f, 0.f };
		}

		m_pOkBtnTex = m_pTextureMgr->GetTexInfo(L"Button", L"Ok");
		if (nullptr != m_pOkBtnTex)
		{
			m_tOkBtn.vParentPos = m_tWndInfo.vPos;
			m_tOkBtn.vPos = { 153.f, 17.f, 0.f };
			m_tOkBtn.vSize = { 35.f, 15.f, 0.f };
			m_tOkBtn.vOffset = { 0.f, 0.f, 0.f };
		}

		m_tTextBox.vParentPos = m_tWndInfo.vPos;
		m_tTextBox.vPos = { 27.f, 13.f, 0.f };
		m_tTextBox.vSize = { 57.f, 21.f, 0.f };
		m_tTextBox.vOffset = { 0.f, 0.f, 0.f };


		m_pScrollLeftTex		= m_pTextureMgr->GetTexInfo(L"Button", L"ScrollLeft");
		if (nullptr != m_pScrollLeftTex)
		{
			m_tScrollLeft.vParentPos = m_tWndInfo.vPos;
			m_tScrollLeft.vPos = { 7.f, 19.f, 0.f };
			m_tScrollLeft.vSize = { 12.f, 17.f, 0.f };
			m_tScrollLeft.vOffset = { 0.f, 0.f, 0.f };
		}

		m_pScrollRightTex	= m_pTextureMgr->GetTexInfo(L"Button", L"ScrollRight");
		if (nullptr != m_pScrollRightTex)
		{
			m_tScrollRight.vParentPos = m_tWndInfo.vPos;
			m_tScrollRight.vPos =  { 94.f, 19.f, 0.f };
			m_tScrollRight.vSize = { 12.f, 17.f, 0.f };
			m_tScrollRight.vOffset = { 0.f, 0.f, 0.f };
		}
	}

	return S_OK;
}

void CCountWnd::Update()
{
	CUIWnd::WindowMove();

	if (m_bIsShow)
	{
		m_tMaxBtn.vParentPos
		= m_tOkBtn.vParentPos
		= m_tTextBox.vParentPos
		= m_tScrollLeft.vParentPos
		= m_tScrollRight.vParentPos 
		= m_tWndInfo.vPos;

		if (nullptr == m_pItemData || WND_ID::WNDID_END == m_eFromWnd || WND_ID::WNDID_END == m_eToWnd)
		{
			m_pFromData = nullptr;
			SafeDelete(m_pItemData);
			m_pItemData = nullptr;
			m_eFromWnd = WND_ID::WNDID_END;
			m_eToWnd = WND_ID::WNDID_END;
			m_iCount = 1;

			m_bIsShow = false;
		}

		D3DXVECTOR3 vMouse = m_pMouseMgr->GetMouse();
		if (0 == m_tMaxBtn.iHover && m_tMaxBtn.IsRectInPoint(vMouse))
			m_pSoundMgr->PlaySound(L"barover.MP3", CHANNEL_ID::CHANNEL_WINDOW);
		if(0 == m_tOkBtn.iHover && m_tOkBtn.IsRectInPoint(vMouse))
			m_pSoundMgr->PlaySound(L"barover.MP3", CHANNEL_ID::CHANNEL_WINDOW);
		if(0 == m_tScrollLeft.iHover && m_tScrollLeft.IsRectInPoint(vMouse))
			m_pSoundMgr->PlaySound(L"barover.MP3", CHANNEL_ID::CHANNEL_WINDOW);
		if(0 == m_tScrollRight.iHover && m_tScrollRight.IsRectInPoint(vMouse))
			m_pSoundMgr->PlaySound(L"barover.MP3", CHANNEL_ID::CHANNEL_WINDOW);
		if(0 == m_tMaxBtn.iHover && m_tMaxBtn.IsRectInPoint(vMouse))
			m_pSoundMgr->PlaySound(L"barover.MP3", CHANNEL_ID::CHANNEL_WINDOW);
	}
}

void CCountWnd::Render()
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

		

		RECT rt;

		// 개수
		{
			D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
			m_pDeviceMgr->GetSprite()->SetTransform(&matScale);
			SetRect(&rt,
				int(m_tWndInfo.vPos.x + m_tTextBox.vPos.x), int(m_tWndInfo.vPos.y + m_tTextBox.vPos.y),
				int(m_tWndInfo.vPos.x + m_tTextBox.vPos.x + m_tTextBox.vSize.x), int(m_tWndInfo.vPos.y + m_tTextBox.vPos.y + m_tTextBox.vSize.y));
			m_pDeviceMgr->DrawFont(to_wstring(m_iCount), &rt, DT_LEFT | DT_VCENTER | DT_NOCLIP, D3DCOLOR_ARGB(255, 255, 255, 255));
		}

		// 스크롤 왼쪽
		if (nullptr != m_pScrollLeftTex)
		{
			D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
			D3DXMatrixTranslation(&matTrans, m_tWndInfo.vPos.x + m_tScrollLeft.vPos.x, m_tWndInfo.vPos.y + m_tScrollLeft.vPos.y, 0.f);
			m_pDeviceMgr->GetSprite()->SetTransform(&(matScale*matTrans));
			m_pDeviceMgr->GetSprite()->Draw(m_pScrollLeftTex->pTexture, nullptr, &m_pScrollLeftTex->vOffset, nullptr, m_pScrollLeftTex->dwBlend);
		}

		// 스크롤 오른쪽
		if (nullptr != m_pScrollRightTex)
		{
			D3DXMatrixTranslation(&matTrans, m_tWndInfo.vPos.x + m_tScrollRight.vPos.x, m_tWndInfo.vPos.y + m_tScrollRight.vPos.y, 0.f);
			m_pDeviceMgr->GetSprite()->SetTransform(&(matScale*matTrans));
			m_pDeviceMgr->GetSprite()->Draw(m_pScrollRightTex->pTexture, nullptr, &m_pScrollRightTex->vOffset, nullptr, m_pScrollRightTex->dwBlend);
		}

		// 맥스 버튼
		if (nullptr != m_pMaxBtnTex)
		{
			D3DXMatrixTranslation(&matTrans, m_tWndInfo.vPos.x + m_tMaxBtn.vPos.x, m_tWndInfo.vPos.y + m_tMaxBtn.vPos.y, 0.f);
			m_pDeviceMgr->GetSprite()->SetTransform(&(matScale*matTrans));
			m_pDeviceMgr->GetSprite()->Draw(m_pMaxBtnTex->pTexture, nullptr, &m_pMaxBtnTex->vOffset, nullptr, m_pMaxBtnTex->dwBlend);
		}

		// 확인 버튼
		if (nullptr != m_pOkBtnTex)
		{
			D3DXMatrixTranslation(&matTrans, m_tWndInfo.vPos.x + m_tOkBtn.vPos.x, m_tWndInfo.vPos.y + m_tOkBtn.vPos.y, 0.f);
			m_pDeviceMgr->GetSprite()->SetTransform(&(matScale*matTrans));
			m_pDeviceMgr->GetSprite()->Draw(m_pOkBtnTex->pTexture, nullptr, &m_pOkBtnTex->vOffset, nullptr, m_pOkBtnTex->dwBlend);
		}
	}
}

void CCountWnd::Release()
{
}

void CCountWnd::LClickDown()
{
	if (!m_bIsShow)
		return;

	D3DXVECTOR3	vMouse = m_pMouseMgr->GetMouse();

	if (!m_tWndInfo.IsRectInPoint(vMouse))
		return;

	if (nullptr == m_pItemData)
		return;

	m_pMouseMgr->SetItemData(WND_ID::WNDID_END, -1, nullptr);
	m_pMouseMgr->SetSkillData(WND_ID::WNDID_END, -1, nullptr);

	if (m_tScrollLeft.IsRectInPoint(vMouse))
	{
		CSoundMgr::GetInstance()->PlaySound(L"BtnClick.wav", CHANNEL_ID::CHANNEL_WINDOW);

		if (0 < m_iCount)
			--m_iCount;
	}
	if (m_tScrollRight.IsRectInPoint(vMouse))
	{
		CSoundMgr::GetInstance()->PlaySound(L"BtnClick.wav", CHANNEL_ID::CHANNEL_WINDOW);

		if (m_pItemData->iCount > int(m_iCount) && m_pUIMgr->GetPlayerIdentity().iSeed >= m_pItemData->iPrice * int(m_iCount) + 1)
		{
			++m_iCount;
		}
	}
	if (m_tMaxBtn.IsRectInPoint(vMouse))
	{
		CSoundMgr::GetInstance()->PlaySound(L"BtnClick.wav", CHANNEL_ID::CHANNEL_WINDOW);

		m_iCount = m_pItemData->iCount;
	}

	// 확인 버튼
	if (m_tOkBtn.IsRectInPoint(vMouse))
	{
		CSoundMgr::GetInstance()->PlaySound(L"BtnClick.wav", CHANNEL_ID::CHANNEL_WINDOW);

		MoveItem();
		m_bIsShow = false;
	}
}

void CCountWnd::LClickUp()
{
}

void CCountWnd::RClickDown()
{
	if (!m_bIsShow)
		return;

	D3DXVECTOR3	vMouse = m_pMouseMgr->GetMouse();

	if (!m_tWndInfo.IsRectInPoint(vMouse))
		return;

	m_pMouseMgr->SetItemData(WND_ID::WNDID_END, -1, nullptr);
	m_pMouseMgr->SetSkillData(WND_ID::WNDID_END, -1, nullptr);
}

void CCountWnd::SetWndData(WND_ID eFrom, WND_ID eTo, CItemData* pItemData)
{
	m_eFromWnd = eFrom; 
	m_eToWnd = eTo; 
	if (nullptr != pItemData)
	{
		m_pFromData = pItemData;

		CItemData* pItem = nullptr;
		switch (pItemData->eItemType)
		{
		case ITEM_TYPE::ITEM_EQUIP:
			pItem = new CItemEquipData(*(dynamic_cast<CItemEquipData*>(pItemData)));
			break;

		case ITEM_TYPE::ITEM_USE:
			pItem = new CItemUsableData(*(dynamic_cast<CItemUsableData*>(pItemData)));
			break;

		case ITEM_TYPE::ITEM_ETC:
			pItem = new CItemData(*(dynamic_cast<CItemData*>(pItemData)));
			break;
		}
		if (nullptr != pItem)
		{
			pItem->SetPrice(pItemData->GetPrice());
			m_pItemData = pItem;
		}
	}
}

void CCountWnd::MoveItem()
{
	if (nullptr == m_pItemData || WND_ID::WNDID_END == m_eFromWnd || WND_ID::WNDID_END == m_eToWnd)
		return;

	if (0 == m_iCount)
	{
		SafeDelete(m_pItemData);
	}
	else
	{
		if (m_eFromWnd != m_eToWnd)
			m_pFromData->iCount -= m_iCount;

		m_pItemData->iCount = m_iCount;

		if (WND_ID::WNDID_INVEN == m_eFromWnd)
		{
			switch (m_eToWnd)
			{
			case WND_ID::WNDID_SHOP:
				dynamic_cast<CShopWnd*>(m_pUIMgr->GetWindow(WND_ID::WNDID_SHOP))->InsertSellItem(m_pItemData);
				m_pUIMgr->GetPlayerIdentity().iSeed += m_pItemData->iPrice * m_iCount;
				m_pUIMgr->UpdatePlayerNotify();
				break;
			}
		}
		else if (WND_ID::WNDID_SHOP == m_eFromWnd)
		{
			switch (m_eToWnd)
			{
			case WND_ID::WNDID_INVEN:
				dynamic_cast<CInvenWnd*>(m_pUIMgr->GetWindow(WND_ID::WNDID_INVEN))->AddItem(m_pItemData);
				m_pUIMgr->GetPlayerIdentity().iSeed -= m_pItemData->iPrice * m_iCount;
				m_pUIMgr->UpdatePlayerNotify();
				break;

			case WND_ID::WNDID_SHOP:
				dynamic_cast<CShopWnd*>(m_pUIMgr->GetWindow(WND_ID::WNDID_SHOP))->InsertTradeItem(m_pItemData);
				break;
			}
		}
		else
		{
			SafeDelete(m_pItemData);
		}
	}

	m_pFromData = nullptr;
	m_pItemData = nullptr;
	m_eFromWnd = WND_ID::WNDID_END;
	m_eToWnd = WND_ID::WNDID_END;
	m_iCount = 1;

	m_bIsShow = false;
}

CCountWnd* CCountWnd::Create()
{
	CCountWnd* pInstance = new CCountWnd;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}