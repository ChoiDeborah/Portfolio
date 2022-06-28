#include "stdafx.h"
#include "MiniMapWnd.h"

#include "GameObject.h"

CMiniMapWnd::CMiniMapWnd()
{
}


CMiniMapWnd::~CMiniMapWnd()
{
	Release();
}

HRESULT CMiniMapWnd::Initialize()
{
	m_bIsShow = true;

	m_pWndTex = m_pTextureMgr->GetTexInfo(L"UI", L"MiniMap");
	if (nullptr != m_pWndTex) {
		m_tWndInfo.vPos = { float(CLIENTCX) - 233.f, float(CLIENTCY) - 122.f, 0.f };
		m_tWndInfo.vSize = { 233.f, 122.f, 0.f };
		m_tWndInfo.vOffset = m_pWndTex->vOffset;
	}

	m_bIsShow = true;
	return S_OK;
}

void CMiniMapWnd::Update()
{
	CUIWnd::WindowMove();
}

void CMiniMapWnd::Render()
{
	D3DXMATRIX matScale, matTrans, matWorld;

	D3DXVECTOR3	vMouse = m_pMouseMgr->GetMouse();
	float fSize = (m_tWndInfo.IsRectInPoint(vMouse)) ? 2.f : 1.f;
	

	m_pWndTex = m_pTextureMgr->GetTexInfo(L"Background", L"Map", m_pTileMgr->GetMapNum());
	if (nullptr != m_pWndTex) {

		float fPer = 233.f / float(m_pWndTex->tImgInfo.Width) * fSize;

		m_tWndInfo.vPos = { float(CLIENTCX) - float(m_pWndTex->tImgInfo.Width) * fPer, float(CLIENTCY) - float(m_pWndTex->tImgInfo.Height) * fPer, 0.f};
		m_tWndInfo.vSize = { float(m_pWndTex->tImgInfo.Width) * fPer, float(m_pWndTex->tImgInfo.Height) * fPer, 0.f };
		m_tWndInfo.vOffset = m_pWndTex->vOffset;

		D3DXMatrixScaling(&matScale, fPer, fPer, 0.f);
		D3DXMatrixTranslation(&matTrans, float(CLIENTCX) - float(m_pWndTex->tImgInfo.Width) * fPer, float(CLIENTCY) - float(m_pWndTex->tImgInfo.Height) * fPer, 0.f);
		matWorld = matScale * matTrans;

		m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
		m_pDeviceMgr->GetSprite()->Draw(m_pWndTex->pTexture, nullptr,
			&m_tWndInfo.vOffset, nullptr, D3DCOLOR_ARGB(int(191.25f + 63.75f * (fSize - 1.f)), 255, 255, 255));

		m_pUIMgr->DrawRect(m_tWndInfo);

		m_pObjMgr->MiniRender(m_tWndInfo.vPos, fPer);
	}
}

void CMiniMapWnd::Release()
{
}

void CMiniMapWnd::LClickDown()
{
}

void CMiniMapWnd::LClickUp()
{
}

void CMiniMapWnd::RClickDown()
{
}

void CMiniMapWnd::InsertChat(CHAT_DATA * pChatData)
{
}

CMiniMapWnd* CMiniMapWnd::Create()
{
	CMiniMapWnd* pInstance = new CMiniMapWnd;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}
