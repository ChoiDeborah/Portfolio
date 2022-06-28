#include "stdafx.h"
#include "TimeMgr.h"

IMPLEMENT_SINGLETON(CTimeMgr)

CTimeMgr::CTimeMgr()
{
	ZeroMemory(&m_OriginTime, sizeof(LARGE_INTEGER));
	ZeroMemory(&m_OldTime, sizeof(LARGE_INTEGER));
	ZeroMemory(&m_CurTime, sizeof(LARGE_INTEGER));
	ZeroMemory(&m_CPUTick, sizeof(LARGE_INTEGER));
}


CTimeMgr::~CTimeMgr()
{
}

float CTimeMgr::GetDeltaTime()
{
	return m_fDeltaTime;
}

HRESULT CTimeMgr::InitTime()
{
	// GetTickCount는 소프트웨어에서 가지고 오지만 이 함수는 하드웨어에서 가지고 온다.
	// 하드웨어(메인보드)가 갖고있는 고해상도(정밀도) 타이머 값을 얻어오는 함수
	// 여기서 말하는 타이머 값은 일정한 주기를 누적한 값을 말한다.
	// 일정한 주기는 CPU 진동수(Hz)에 비롯된다.
	// CPU 부속품들도 속도가 다른데, 이 박자를 맞추기 위해 CPU가 진동(클럭)한다.
	QueryPerformanceCounter(&m_OriginTime);
	QueryPerformanceCounter(&m_OldTime);
	QueryPerformanceCounter(&m_CurTime);

	// CPU의 초당 진동수를 얻어오는 함수
	QueryPerformanceFrequency(&m_CPUTick);

	return S_OK;
}

void CTimeMgr::UpdateTime()
{
	QueryPerformanceCounter(&m_CurTime);

	// 주기적으로 CPU진동수 갱신
	if (m_CurTime.QuadPart - m_OriginTime.QuadPart > m_CPUTick.QuadPart)
	{
		QueryPerformanceFrequency(&m_CPUTick);
		m_OriginTime = m_CurTime;
	}

	// 1프레임 당 진행해야하는 비율
	// 30FPS일 경우 1/30이 델타타임 값
	// CPU진동수가4, FPS가 진동수 2당 1프레임 일 때 10을 이동 * 델타 타임일 경우 1FPS당 5를 이동 (10*(1/2))
	// 델타타임 = (현재시간 - 지난시간) / CPU진동수
	m_fDeltaTime = float(m_CurTime.QuadPart - m_OldTime.QuadPart) / m_CPUTick.QuadPart;

	m_OldTime = m_CurTime;
}
