#pragma once
class CFrameMgr
{
	DECLARE_SINGLETON(CFrameMgr)

private:
	CFrameMgr();
	~CFrameMgr();

public:
	HRESULT InitTime();
	bool	LockFrame(float fFPS);
	void	RenderFPS();

private:
	LARGE_INTEGER	m_OriginTime;
	LARGE_INTEGER	m_OldTime;
	LARGE_INTEGER	m_CurTime;
	LARGE_INTEGER	m_CPUTick;		// CPU의 진동 수

	float	m_fTimeCount = 0.f;
	float	m_fFPSCount = 0.f;

	TCHAR	m_szFPS[MAX_STR];
	int		m_iFPS = 0;
};

