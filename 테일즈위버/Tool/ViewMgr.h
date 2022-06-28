#pragma once

class CGameObject;
class CViewMgr
{
	DECLARE_SINGLETON(CViewMgr)

private:
	CViewMgr();
	~CViewMgr();

public:
	void Initialize();
	void Update();
	void Render();

public:
	D3DXVECTOR3 GetEye() { return m_vEye; }
	D3DXVECTOR3 GetAt() { return m_vAt; }
	D3DXVECTOR3 GetUp() { return m_vUp; }
	D3DXVECTOR3 GetScroll();
	D3DXVECTOR3 GetViewSize() { return m_vViewSize; }

	float GetViewX() { return m_vViewPos.x + m_vShakePos.x; }
	float GetViewY() { return m_vViewPos.y + m_vShakePos.y; }
	float GetRoomW() { return m_vRoomSize.x; }
	float GetRoomH() { return m_vRoomSize.y; }

public:
	void AddViewX(float fX);
	void AddViewY(float fY);

	void SetViewX(float fX);
	void SetViewY(float fY);

	void SetShakeX(float shakeX) { m_vShakePos.x = shakeX; }
	void SetShakeY(float shakeY) { m_vShakePos.y = shakeY; }

	void SetSpeed(float fSpeed) { m_fSpeed = fSpeed; }

	void SetViewObj(D3DXVECTOR3 vPos);
	void SetOffset(D3DXVECTOR3 vOffset);
	void SetRoomSize(D3DXVECTOR3 vRoomSize) { m_vRoomSize = vRoomSize; }

private:
	void ShakeView();
	void KeyInput();


// 사용자 정의 자료형
private:
	D3DXVECTOR3	m_vEye			= { 0.f, 0.f, m_fZoomMax };
	D3DXVECTOR3	m_vAt			= { 0.f, 0.f, 0.f };
	D3DXVECTOR3	m_vUp			= { 0.f, 1.f, 0.f };

	D3DXVECTOR3 m_vRoomSize		= { float(VIEWCX), float(VIEWCY), 0.f };

	D3DXVECTOR3 m_vViewSize		= { float(VIEWCX * 0.5f), float(VIEWCY * 0.5f), 0.f };

	D3DXVECTOR3 m_vViewPos		= { 0.f, 0.f, 0.f };
	D3DXVECTOR3 m_vFollowPos	= { 0.f, 0.f, 0.f };

	D3DXVECTOR3 m_vShakePos		= { 0.f, 0.f, 0.f };

	D3DXVECTOR3 m_vOffset		= { 0.f, 0.f, 0.f };


// 포인터
private:
	CDeviceMgr*	m_pDeviceMgr = CDeviceMgr::GetInstance();

// 기본 자료형
private:
	float m_fZoomMin			= 100.f;
	float m_fZoomMax			= 400.f;

	float m_fViewSize			= 1.f;
	float m_fSpeed				= -1.f;
};

