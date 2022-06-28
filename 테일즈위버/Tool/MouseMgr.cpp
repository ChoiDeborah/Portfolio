#include "stdafx.h"
#include "MouseMgr.h"

IMPLEMENT_SINGLETON(CMouseMgr)

CMouseMgr::CMouseMgr()
{
}

CMouseMgr::~CMouseMgr()
{
}

void CMouseMgr::Render()
{
	const TEX_INFO* m_pTexInfo = CTextureMgr::GetInstance()->GetTexInfo(L"UI", L"Mouse");
	NULL_CHECK(m_pTexInfo);

	D3DXMATRIX matTrans, matWorld;
	POINT pt = {};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);
	D3DXMatrixTranslation(&matTrans, (float)pt.x, (float)pt.y, 0.f);
	matWorld = matTrans;

	float fOffsetX = m_pTexInfo->tImgInfo.Width * 0.5f;
	float fOffsetY = m_pTexInfo->tImgInfo.Height * 0.5f;

	CDeviceMgr::GetInstance()->GetSprite()->SetTransform(&matWorld);
	CDeviceMgr::GetInstance()->GetSprite()->Draw(m_pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(fOffsetX, fOffsetY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

D3DXVECTOR3 CMouseMgr::GetMouse()
{
	POINT pt = {};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	return D3DXVECTOR3((float)pt.x, (float)pt.y, 0.f);
}
