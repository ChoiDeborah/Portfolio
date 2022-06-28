#include "stdafx.h"
#include "ComboWnd.h"

#include "SkillWnd.h"
#include "ComboSetWnd.h"

#include "SkillData.h"
#include "Player.h"

CComboWnd::CComboWnd()
{
}


CComboWnd::~CComboWnd()
{
	Release();
}

HRESULT CComboWnd::Initialize()
{
	m_pWndTex = m_pTextureMgr->GetTexInfo(L"UI", L"ComboWnd");
	if (nullptr != m_pWndTex) 
	{
		m_tWndInfo.vPos = { float(CLIENTCX - m_pWndTex->tImgInfo.Width) - 18.f, 51.f, 0.f };
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
	}
	
	CSkillData* pSkillData = dynamic_cast<CSkillWnd*>(m_pUIMgr->GetWindow(WND_ID::WNDID_SKILL))->GetSkillData(SKILLID::SKILLID_STAB);

	for (int i = 0; i < COMBO_SLOT_Y; ++i)
	{
		for (int j = 0; j < COMBO_SLOT_X; ++j)
		{
			m_vecSkillData[i].push_back(pSkillData);

			m_tSkillIcon[i][j].vParentPos = m_tWndInfo.vPos;
			m_tSkillIcon[i][j].vPos		= { 14.f + 12.f + 31.f * j, 38.f + 12.f + 45.f * i, 0.f };
			m_tSkillIcon[i][j].vSize	= { 24.f, 24.f, 0.f };
			m_tSkillIcon[i][j].vOffset	= { 12.f, 12.f, 0.f };
		}

		m_tSelected[i].vParentPos = m_tWndInfo.vPos;
		m_tSelected[i].vPos = { 9.f, 23.f + 45.f * i, 0.f };
		m_tSelected[i].vSize = { 256.f, 41.f, 0.f };
		m_tSelected[i].vOffset = { 0.f, 0.f, 0.f };
	}


	return S_OK;
}

void CComboWnd::Update()
{
	CUIWnd::WindowMove();

	for (int i = 0; i < COMBO_SLOT_Y; ++i)
	{
		for (int j = 0; j < COMBO_SLOT_X; ++j)
		{
			m_tSkillIcon[i][j].vParentPos = m_tWndInfo.vPos;
		}

		m_tSelected[i].vParentPos = m_tWndInfo.vPos;
	}

	D3DXVECTOR3 vMouse = m_pMouseMgr->GetMouse();
	if (0 == m_tExitInfo.iHover && m_tExitInfo.IsRectInPoint(vMouse))
		m_pSoundMgr->PlaySound(L"barover.MP3", CHANNEL_ID::CHANNEL_WINDOW);
}

void CComboWnd::Render()
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
		D3DXMatrixTranslation(&matTrans, m_tWndInfo.vPos.x + m_tExitInfo.vPos.x, m_tWndInfo.vPos.y + m_tExitInfo.vPos.y, 0.f);
		matWorld = matScale * matTrans;
		m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
		m_pDeviceMgr->GetSprite()->Draw(m_pExitTex->pTexture, nullptr,
			&m_tExitInfo.vOffset, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));


		//m_pUIMgr->DrawRect(m_tWndInfo);
		//m_pUIMgr->DrawRect(m_tMoveInfo);
	}


	const TEX_INFO* pSkillTex = nullptr;

	for (int i = 0; i < COMBO_SLOT_Y; ++i)
	{
		for (size_t j = 0; j < m_vecSkillData[i].size(); ++j)
		{
			if (nullptr != m_vecSkillData[i][j])
			{
				pSkillTex = m_pTextureMgr->GetTexInfo(m_vecSkillData[i][j]->wstrObjKey, m_vecSkillData[i][j]->wstrStateKey);
				if (nullptr != pSkillTex)
				{
					D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
					D3DXMatrixTranslation(&matTrans, m_tWndInfo.vPos.x + m_tSkillIcon[i][j].vPos.x, m_tWndInfo.vPos.y + m_tSkillIcon[i][j].vPos.y, 0.f);
					matWorld = matScale * matTrans;
					m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
					m_pDeviceMgr->GetSprite()->Draw(pSkillTex->pTexture, nullptr,
						&pSkillTex->vOffset, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
				}
			}
		}
	}
}

void CComboWnd::Release()
{
	for (int i = 0; i < COMBO_SLOT_Y; ++i)
	{
		m_vecSkillData[i].clear();
		m_vecSkillData[i].shrink_to_fit();
	}
}

void CComboWnd::LClickDown()
{
	if (!m_bIsShow)
		return;

	D3DXVECTOR3	vMouse = m_pMouseMgr->GetMouse();

	if (!m_tWndInfo.IsRectInPoint(vMouse))
		return;

	m_pMouseMgr->SetItemData(WND_ID::WNDID_END, -1, nullptr);
	m_pMouseMgr->SetSkillData(WND_ID::WNDID_END, -1, nullptr);

	// 마우스에 스킬이 없는 상태
	for (int i = 0; i < COMBO_SLOT_Y; ++i)
	{
		if (m_tSelected[i].IsRectInPoint(vMouse))
		{
			CSoundMgr::GetInstance()->PlaySound(L"BtnClick.wav", CHANNEL_ID::CHANNEL_WINDOW);

			m_iCurSlot = i;
			m_pUIMgr->GetWindow(WND_ID::WNDID_COMBOSET)->ShowWindow(true);
			break;
		}
	}
}

void CComboWnd::LClickUp()
{
}

void CComboWnd::RClickDown()
{
	if (!m_bIsShow)
		return;

	D3DXVECTOR3	vMouse = m_pMouseMgr->GetMouse();

	if (!m_tWndInfo.IsRectInPoint(vMouse))
		return;

	m_pMouseMgr->SetItemData(WND_ID::WNDID_END, -1, nullptr);
	m_pMouseMgr->SetSkillData(WND_ID::WNDID_END, -1, nullptr);

	/*// 마우스에 스킬이 없는 상태
	CSkillData* pMouseSkillData = m_pMouseMgr->GetSkillData();
	if (nullptr == pMouseSkillData)
	{
		for (int i = 0; i < COMBO_SLOT_Y; ++i)
		{
			for (int j = 0; j < COMBO_SLOT_X; ++j)
			{
				if (m_tSkillIcon[i][j].IsRectInPoint(vMouse))
				{
					if (0 > j || j >= int(m_vecSkillData[i].size()))
						break;

					m_vecSkillData[i].erase(m_vecSkillData[i].begin() + j);
				}
			}
		}
	}*/
}

void CComboWnd::SetComboSkill(int iSlotY, int iSlotX, CSkillData* pSkillData)
{
	if (0 > iSlotY || iSlotY > COMBO_SLOT_Y - 1 ||
		0 > iSlotX || iSlotX > COMBO_SLOT_X - 1)
		return;

	if (iSlotX < int(m_vecSkillData[iSlotY].size()) - 1)
		m_vecSkillData[iSlotY].push_back(pSkillData);
	else
		m_vecSkillData[iSlotY][iSlotX] = pSkillData;
}

void CComboWnd::DeleteComboSkill(int iSlotY, int iSlotX)
{
	if (0 > iSlotY || iSlotY > COMBO_SLOT_Y-1 ||
		0 > iSlotX || iSlotX >= int(m_vecSkillData[iSlotY].size()))
		return;

	if (nullptr != *(m_vecSkillData[iSlotY].begin() + iSlotX))
		m_vecSkillData[iSlotY].erase(m_vecSkillData[iSlotY].begin() + iSlotX);
}

CComboWnd* CComboWnd::Create()
{
	CComboWnd* pInstance = new CComboWnd;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}