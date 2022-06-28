#include "stdafx.h"
#include "EquipWnd.h"

#include "ItemEquipData.h"
#include "ItemUsableData.h"


CEquipWnd::CEquipWnd()
{
}


CEquipWnd::~CEquipWnd()
{
	Release();
}

HRESULT CEquipWnd::Initialize()
{
	m_pWndTex = m_pTextureMgr->GetTexInfo(L"UI", L"EquipWnd");
	if (nullptr != m_pWndTex) {
		m_tWndInfo.vPos = { float(CLIENTCX - m_pWndTex->tImgInfo.Width) - 18.f, 102.f, 0.f };
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

		for (int i = 0; i < SLOT_SIZE; ++i)
		{
			m_tItem[i].vParentPos = m_tWndInfo.vPos;
			m_tItem[i].vPos =		{ 6.f, 27.f + 31.f * i, 0.f };
			m_tItem[i].vSize =		{ 185.f, 26.f, 0.f };
			m_tItem[i].vOffset =	{ 0.f, 0.f, 0.f };
		}
	}
	return S_OK;
}

void CEquipWnd::Update()
{
	CUIWnd::WindowMove();

	D3DXVECTOR3 vMouse = m_pMouseMgr->GetMouse();
	if (0 == m_tExitInfo.iHover && m_tExitInfo.IsRectInPoint(vMouse))
		m_pSoundMgr->PlaySound(L"barover.MP3", CHANNEL_ID::CHANNEL_WINDOW);
}

void CEquipWnd::Render()
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

		RECT rt;
		// 아이템
		for (size_t i = 0; i < SLOT_SIZE; ++i)
		{
			if (nullptr == m_pItem[i])
				continue;

			const TEX_INFO*	pTexInfo = m_pTextureMgr->GetTexInfo(m_pItem[i]->wstrObjKey, m_pItem[i]->wstrStateKey);
			if (nullptr != pTexInfo)
			{
				// 아이템 그림
				D3DXMatrixTranslation(&matTrans, m_tWndInfo.vPos.x + 19.f, m_tWndInfo.vPos.y + 40.f + 30.f * i, 0.f);
				m_pDeviceMgr->GetSprite()->SetTransform(&matTrans);
				m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
					&pTexInfo->vOffset, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
			}

			D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
			m_pDeviceMgr->GetSprite()->SetTransform(&matScale);

			// 아이템 이름
			SetRect(&rt,
				int(m_tWndInfo.vPos.x + 37.f), int(m_tWndInfo.vPos.y + 27.f + 31.f * i),
				int(m_tWndInfo.vPos.x + 192.f), int(m_tWndInfo.vPos.y + 54.f + 31.f * i));
			m_pDeviceMgr->DrawFont(m_pItem[i]->wstrStateKey, &rt, DT_CENTER, m_pItem[i]->dwColor);

			// 아이템 옵션
			SetRect(&rt,
				int(m_tWndInfo.vPos.x + 128.f), int(m_tWndInfo.vPos.y + 40.f + 31.f * i),
				int(m_tWndInfo.vPos.x + 192.f), int(m_tWndInfo.vPos.y + 54.f + 31.f * i));
			m_pDeviceMgr->DrawFont(m_pItem[i]->GetOption(), &rt, DT_CENTER);
		}
	}
}

void CEquipWnd::Release()
{
}

void CEquipWnd::LClickDown()
{
	if (!m_bIsShow)
		return;

	D3DXVECTOR3	vMouse = m_pMouseMgr->GetMouse();

	if (!m_tWndInfo.IsRectInPoint(vMouse))
		return;

	// 마우스에 아이템 없는 상태
	CItemData* pMouseItemData = m_pMouseMgr->GetItemData();
	if (nullptr == pMouseItemData)
	{
		for (int i = 0; i < SLOT_SIZE; ++i)
		{
			m_tItem[i].vParentPos = m_tWndInfo.vPos;
			if (m_tItem[i].IsRectInPoint(vMouse))
			{
				CSoundMgr::GetInstance()->PlaySound(L"Drag.wav", CHANNEL_ID::CHANNEL_WINDOW);

				// 아이템 클릭
				m_pMouseMgr->SetItemData(WND_ID::WNDID_EQUIP, i, m_pItem[i]);
			}
		}
		m_pMouseMgr->SetSkillData(WND_ID::WNDID_END, -1, nullptr);
	}
	else
	{
		if (WND_ID::WNDID_EQUIP != m_pMouseMgr->GetFromWndID())
		{
			if (ITEM_TYPE::ITEM_EQUIP == pMouseItemData->eItemType &&
				ITEMID::ITEMID_ACCESSOAY4 > pMouseItemData->eItemID)
			{
				CSoundMgr::GetInstance()->PlaySound(L"Drop.wav", CHANNEL_ID::CHANNEL_WINDOW);
				//m_pItem[pMouseItemData->eItemID] = pMouseItemData;
				m_pUIMgr->MoveItem(m_pMouseMgr->GetFromWndID(), WND_ID::WNDID_EQUIP, m_pMouseMgr->GetFromItemIndex(), pMouseItemData->iCount);
			}
			m_pMouseMgr->SetItemData(WND_ID::WNDID_END, -1, nullptr);
			m_pMouseMgr->SetSkillData(WND_ID::WNDID_END, -1, nullptr);
		}
	}
}

void CEquipWnd::LClickUp()
{
}

void CEquipWnd::RClickDown()
{
	if (!m_bIsShow)
		return;

	D3DXVECTOR3	vMouse = m_pMouseMgr->GetMouse();

	if (!m_tWndInfo.IsRectInPoint(vMouse))
		return;

	m_pMouseMgr->SetItemData(WND_ID::WNDID_END, -1, nullptr);
	m_pMouseMgr->SetSkillData(WND_ID::WNDID_END, -1, nullptr);

	for (int i = 0; i < SLOT_SIZE; ++i)
	{
		m_tItem[i].vParentPos = m_tWndInfo.vPos;
		if (m_tItem[i].IsRectInPoint(vMouse))
		{
			CSoundMgr::GetInstance()->PlaySound(L"BtnClick.wav", CHANNEL_ID::CHANNEL_WINDOW);

			// 아이템 클릭
			m_pUIMgr->MoveItem(WND_ID::WNDID_EQUIP, WND_ID::WNDID_INVEN, i);
			break;
		}
	}
	m_pMouseMgr->SetSkillData(WND_ID::WNDID_END, -1, nullptr);
}

CItemData * CEquipWnd::GetItem(int iIndex)
{
	if (0 > iIndex || iIndex >= SLOT_SIZE)
		return nullptr;

	return m_pItem[iIndex];
}

void CEquipWnd::SetItem(int iIndex, CItemData* pItemData)
{
	if (0 > iIndex || iIndex >= SLOT_SIZE)
		return;

	NULL_CHECK(pItemData);
	if (nullptr != m_pItem[iIndex])
		m_pUIMgr->MoveItem(WND_ID::WNDID_EQUIP, WND_ID::WNDID_INVEN, iIndex);

	m_pItem[iIndex] = pItemData;
}

void CEquipWnd::DeleteItem(int iIndex)
{
	if (0 > iIndex || iIndex >= SLOT_SIZE)
		return;

	SafeDelete(m_pItem[iIndex]);
}

void CEquipWnd::AddStat(STAT_INFO * pStatInfo)
{
	for (int i = 0; i < SLOT_SIZE; ++i)
	{
		if (nullptr != m_pItem[i])
		{
			*pStatInfo += dynamic_cast<CItemEquipData*>(m_pItem[i])->tStatInfo;
		}
	}
}

CEquipWnd* CEquipWnd::Create()
{
	CEquipWnd* pInstance = new CEquipWnd;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}