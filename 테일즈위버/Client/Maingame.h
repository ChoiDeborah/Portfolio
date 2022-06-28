#pragma once

class CMaingame
{
private:
	CMaingame();

public:
	~CMaingame();

public:	
	void Update();
	void LateUpdate();
	void Render();

private:
	HRESULT Initialize();
	void Release();

public:
	static CMaingame* Create();

private:
	CDeviceMgr*			m_pDeviceMgr;
	CTextureMgr*		m_pTextureMgr;
	CSceneMgr*			m_pSceneMgr;
	CTimeMgr*			m_pTimeMgr;
	CFrameMgr*			m_pFrameMgr;
};

