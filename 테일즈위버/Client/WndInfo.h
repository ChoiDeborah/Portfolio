#pragma once
// Ã¢ Á¤º¸
class CWndInfo
{
public:
	CWndInfo();
	~CWndInfo();

public:
	bool IsRectInPoint(D3DXVECTOR3 vPoint);

private:
	CSoundMgr*	m_pSoundMgr = CSoundMgr::GetInstance();

public:
	D3DXVECTOR3	vParentPos = { 0.f, 0.f, 0.f };
	D3DXVECTOR3	vPos = { 0.f, 0.f, 0.f };
	D3DXVECTOR3	vSize = { 0.f, 0.f, 0.f };
	D3DXVECTOR3	vOffset = { 0.f, 0.f, 0.f };

	int			iHover = 0;
	bool		bIsShow = true;
};

