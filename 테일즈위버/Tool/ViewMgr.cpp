#include "stdafx.h"
#include "ViewMgr.h"

IMPLEMENT_SINGLETON(CViewMgr)

CViewMgr::CViewMgr()
{
}


CViewMgr::~CViewMgr()
{
}

D3DXVECTOR3 CViewMgr::GetScroll()
{
	return m_vViewPos + m_vShakePos - m_vViewSize;
}

void CViewMgr::AddViewX(float fX)
{
	float fScrollX = m_vFollowPos.x - fX - m_vViewSize.x;
	m_vFollowPos.x = (0.f > fScrollX) ? 0.f : ((m_vRoomSize.x - VIEWCX) < fScrollX) ? m_vRoomSize.x - VIEWCX : fScrollX;
}

void CViewMgr::AddViewY(float fY)
{
	float fScrollY = m_vFollowPos.y - fY - m_vViewSize.y;
	m_vFollowPos.y = (0.f > fScrollY) ? 0.f : ((m_vRoomSize.y - VIEWCY) < fScrollY) ? m_vRoomSize.y - VIEWCY : fScrollY;
}

void CViewMgr::SetViewX(float fX)
{
	float fScrollX = fX - m_vViewSize.x;
	m_vFollowPos.x = (0.f > fScrollX) ? 0.f : ((m_vRoomSize.x - VIEWCX) < fScrollX) ? m_vRoomSize.x - VIEWCX : fScrollX;
}

void CViewMgr::SetViewY(float fY)
{
	float fScrollY = fY - m_vViewSize.y;
	m_vFollowPos.y = (0.f > fScrollY) ? 0.f : ((m_vRoomSize.y - VIEWCY) < fScrollY) ? m_vRoomSize.y - VIEWCY : fScrollY;
}

void CViewMgr::SetOffset(D3DXVECTOR3 vOffset)
{
	m_vOffset = vOffset;
}

void CViewMgr::SetViewObj(D3DXVECTOR3 vPos)
{
	SetViewX(vPos.x);
	SetViewY(vPos.y);
}

void CViewMgr::ShakeView()
{
	if (0 != m_vShakePos.x)
		m_vShakePos.x = -(m_vShakePos.x - (m_vShakePos.x / abs(m_vShakePos.x) * 2));

	if (0 != m_vShakePos.y)
		m_vShakePos.y = -(m_vShakePos.y - (m_vShakePos.y / abs(m_vShakePos.y) * 2));
}

void CViewMgr::KeyInput()
{
	CKeyMgr*	pKeyMgr = CKeyMgr::GetInstance();

	if (pKeyMgr->KeyPressing('Q')) {
		m_vEye.z -= 10.f;
		if (m_fZoomMin >= m_vEye.z) m_vEye.z = m_fZoomMin;
	}
	if (pKeyMgr->KeyPressing('E')) {
		m_vEye.z += 10.f;
		if (m_fZoomMax <= m_vEye.z) m_vEye.z = m_fZoomMax;
	}

	if (pKeyMgr->KeyPressing('A'))
		m_vFollowPos.x -= 10.f;
	if (pKeyMgr->KeyPressing('D'))
		m_vFollowPos.x += 10.f;
	if (pKeyMgr->KeyPressing('W'))
		m_vFollowPos.y -= 10.f;
	if (pKeyMgr->KeyPressing('S'))
		m_vFollowPos.y += 10.f;
	if (pKeyMgr->KeyPressing(VK_F3))
		Initialize();
}

void CViewMgr::Initialize()
{
	m_vEye			= { 0.f, 0.f, m_fZoomMax };
	m_vAt			= { 0.f, 0.f, 0.f };
	m_vUp			= { 0.f, -1.f, 0.f };

	m_vRoomSize		= { VIEWCX, VIEWCY, 0.f };
	m_vViewSize		= { float(VIEWCX * 0.5f) * (m_vEye.z/m_fZoomMax), float(VIEWCY * 0.5f) * (m_vEye.z / m_fZoomMax), 0.f };

	m_vViewPos		= { 0.f, 0.f, 0.f };
	m_vFollowPos	= { 0.f, 0.f, 0.f };
	m_vOffset		= { 0.f, 0.f, 0.f };
	m_vShakePos		= { 0.f, 0.f, 0.f };
}

void CViewMgr::Update()
{
	KeyInput();

	m_vViewSize = { float(VIEWCX * 0.5f) * (m_vEye.z / m_fZoomMax), float(VIEWCY * 0.5f) * (m_vEye.z / m_fZoomMax), 0.f };

	m_vFollowPos.x = (0.f > m_vFollowPos.x) ? 0.f : ((m_vRoomSize.x - VIEWCX) < m_vFollowPos.x) ? m_vRoomSize.x - VIEWCX : m_vFollowPos.x;
	m_vFollowPos.y = (0.f > m_vFollowPos.y) ? 0.f : ((m_vRoomSize.y - VIEWCY) < m_vFollowPos.y) ? m_vRoomSize.y - VIEWCY : m_vFollowPos.y;

	if (-1 == int(m_fSpeed)) {
		m_vViewPos.x = m_vFollowPos.x;
		m_vViewPos.y = m_vFollowPos.y;
	}
	else {
		float fDstX = m_vFollowPos.x - m_vViewPos.x;
		float fDstY = m_vFollowPos.y - m_vViewPos.y;

		// ÀÌµ¿
		if (m_fSpeed < fabs(fDstX) || m_fSpeed < fabs(fDstY)) {
			float fAngle = atanf(fDstY / fDstX);
			if (0 > fDstX) fAngle += PI;

			if (1.f < fabs(fDstX))
				m_vViewPos.x += cosf(fAngle) * m_fSpeed;// *CTimeMgr::GetInstance()->GetDeltaTime();

			if (1.f < fabs(fDstY))
				m_vViewPos.y += sinf(fAngle) * m_fSpeed;// *CTimeMgr::GetInstance()->GetDeltaTime();
		}
		else
		{
			m_vViewPos.x = m_vFollowPos.x;
			m_vViewPos.y = m_vFollowPos.y;
		}
	}
	
	m_vEye.x = m_vAt.x = m_vViewPos.x + m_vShakePos.x;
	m_vEye.y = m_vAt.y = m_vViewPos.y + m_vShakePos.y;

	/*cout << "X : " << m_vViewPos.x + m_vShakePos.x << ", Y : " << m_vViewPos.y + m_vShakePos.y << endl;
	cout << "------------------------------" << endl;*/

	ShakeView();
}

void CViewMgr::Render()
{
	//D3DXMATRIX matView, matProj;

	//D3DXMatrixLookAtLH(&matView, &m_vEye, &m_vAt, &m_vUp);
	//m_pDeviceMgr->GetDevice()->SetTransform(D3DTS_VIEW, &matView);

	///*D3DXMatrixOrthoLH(&matProj, VIEWCX * m_fViewSize, VIEWCY * m_fViewSize, 1.f, 1000.f);
	//m_pDeviceMgr->GetDevice()->SetTransform(D3DTS_PROJECTION, &matProj);*/

	//D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI * 0.5f, float(VIEWCX) / VIEWCY, 1.f, 1000.f);
	//m_pDeviceMgr->GetDevice()->SetTransform(D3DTS_PROJECTION, &matProj);
}
