#include "stdafx.h"
#include "CreditWnd.h"

#include "ItemEquipData.h"
#include "ItemUsableData.h"

#include "QuickWnd.h"
#include "ShopWnd.h"
#include "InvenWnd.h"

CCreditWnd::CCreditWnd()
{
}


CCreditWnd::~CCreditWnd()
{
	Release();
}

HRESULT CCreditWnd::Initialize()
{
	m_pWndTex = m_pTextureMgr->GetTexInfo(L"UI", L"HelpWnd0");
	if (nullptr != m_pWndTex) {
		m_tWndInfo.vPos = { float(CLIENTCX * 0.5f - m_pWndTex->tImgInfo.Width * 0.5f), float(CLIENTCY * 0.5f - m_pWndTex->tImgInfo.Height * 0.5f), 0.f };
		m_tWndInfo.vSize = { float(m_pWndTex->tImgInfo.Width), float(m_pWndTex->tImgInfo.Height), 0.f };
		m_tWndInfo.vOffset = m_pWndTex->vOffset;

		m_tMoveInfo.vSize = { float(m_pWndTex->tImgInfo.Width), 20.f, 0.f };
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

	return S_OK;
}

void CCreditWnd::Update()
{
	CUIWnd::WindowMove();

	if (m_bIsShow)
	{
		m_tExitInfo.vParentPos
		= m_tWndInfo.vPos;

		D3DXVECTOR3 vMouse = m_pMouseMgr->GetMouse();
		if (0 == m_tExitInfo.iHover && m_tExitInfo.IsRectInPoint(vMouse))
			m_pSoundMgr->PlaySound(L"barover.MP3", CHANNEL_ID::CHANNEL_WINDOW);
	}
}

void CCreditWnd::Render()
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
	}
}

void CCreditWnd::Release()
{
}

void CCreditWnd::LClickDown()
{
	if (!m_bIsShow)
		return;

	D3DXVECTOR3	vMouse = m_pMouseMgr->GetMouse();

	if (!m_tWndInfo.IsRectInPoint(vMouse))
		return;

}

void CCreditWnd::LClickUp()
{
}

void CCreditWnd::RClickDown()
{
	if (!m_bIsShow)
		return;

	D3DXVECTOR3	vMouse = m_pMouseMgr->GetMouse();

	if (!m_tWndInfo.IsRectInPoint(vMouse))
		return;
}

CCreditWnd* CCreditWnd::Create()
{
	CCreditWnd* pInstance = new CCreditWnd;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}