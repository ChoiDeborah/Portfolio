#pragma once
class CTimeMgr
{
	DECLARE_SINGLETON(CTimeMgr)

private:
	CTimeMgr();
	~CTimeMgr();

public:
	float GetDeltaTime();

public:
	HRESULT	InitTime();
	void UpdateTime();

private:
	LARGE_INTEGER	m_OriginTime;
	LARGE_INTEGER	m_OldTime;
	LARGE_INTEGER	m_CurTime;
	LARGE_INTEGER	m_CPUTick;		// CPU�� ���� ��

	float	m_fDeltaTime;			// 1�������� �����ϴ� �ð�
};

