#include "stdafx.h"
#include "UIWnd.h"


CUIWnd::CUIWnd()
{
}


CUIWnd::~CUIWnd()
{
}

void CUIWnd::WindowMove()
{
	if (m_bIsShow)
	{
		D3DXVECTOR3 vMouse = m_pMouseMgr->GetMouse();

		if (m_tWndInfo.IsRectInPoint(vMouse))
			m_pUIMgr->SetHover(true);

		m_tExitInfo.vParentPos = m_tMoveInfo.vParentPos = m_tWndInfo.vPos;

		if (!m_bIsMove)
		{
			m_pExitTex = m_pTextureMgr->GetTexInfo(L"Button", L"XBtn", m_tExitInfo.iHover);
		}
		else
		{
			if (m_pKeyMgr->KeyPressing(VK_LBUTTON))
			{
				m_pUIMgr->SetHover(true);
				m_tExitInfo.vParentPos = m_tMoveInfo.vParentPos = m_tWndInfo.vPos = vMouse - m_vStartPos;
			}
		}
	}
}

void CUIWnd::WindowLClickDown()
{
	if (m_bIsShow)
	{
		D3DXVECTOR3 vMouse = m_pMouseMgr->GetMouse();

		if (m_tWndInfo.IsRectInPoint(vMouse))
		{
			m_pUIMgr->SetHover(true);
			m_pMouseMgr->SetUIClick(true);
		}

		m_tExitInfo.vParentPos = m_tMoveInfo.vParentPos = m_tWndInfo.vPos;

		if (!m_bIsMove)
		{
			if (m_tExitInfo.IsRectInPoint(vMouse))
			{
				m_bIsShow = false;
				return;
			}

			if (m_tMoveInfo.IsRectInPoint(vMouse))
			{
				m_vStartPos = vMouse - m_tWndInfo.vPos;
				m_bIsMove = true;
				return;
			}

			this->LClickDown();
		}
	}
}

void CUIWnd::WindowLClickUp()
{
	if (m_bIsMove)
	{
		m_bIsMove = false;
		LClickUp();
	}
}

void CUIWnd::WindowRClickDown()
{
	if (m_bIsShow)
	{
		D3DXVECTOR3 vMouse = m_pMouseMgr->GetMouse();

		if (m_tWndInfo.IsRectInPoint(vMouse))
		{
			m_pUIMgr->SetHover(true);
			m_pMouseMgr->SetUIClick(true);
		}

		m_tExitInfo.vParentPos = m_tMoveInfo.vParentPos = m_tWndInfo.vPos;

		if (!m_bIsMove)
		{
			this->RClickDown();
		}
	}
}
