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
	// GetTickCount�� ����Ʈ����� ������ ������ �� �Լ��� �ϵ����� ������ �´�.
	// �ϵ����(���κ���)�� �����ִ� ���ػ�(���е�) Ÿ�̸� ���� ������ �Լ�
	// ���⼭ ���ϴ� Ÿ�̸� ���� ������ �ֱ⸦ ������ ���� ���Ѵ�.
	// ������ �ֱ�� CPU ������(Hz)�� ��Եȴ�.
	// CPU �μ�ǰ�鵵 �ӵ��� �ٸ���, �� ���ڸ� ���߱� ���� CPU�� ����(Ŭ��)�Ѵ�.
	QueryPerformanceCounter(&m_OriginTime);
	QueryPerformanceCounter(&m_OldTime);
	QueryPerformanceCounter(&m_CurTime);

	// CPU�� �ʴ� �������� ������ �Լ�
	QueryPerformanceFrequency(&m_CPUTick);

	return S_OK;
}

void CTimeMgr::UpdateTime()
{
	QueryPerformanceCounter(&m_CurTime);

	// �ֱ������� CPU������ ����
	if (m_CurTime.QuadPart - m_OriginTime.QuadPart > m_CPUTick.QuadPart)
	{
		QueryPerformanceFrequency(&m_CPUTick);
		m_OriginTime = m_CurTime;
	}

	// 1������ �� �����ؾ��ϴ� ����
	// 30FPS�� ��� 1/30�� ��ŸŸ�� ��
	// CPU��������4, FPS�� ������ 2�� 1������ �� �� 10�� �̵� * ��Ÿ Ÿ���� ��� 1FPS�� 5�� �̵� (10*(1/2))
	// ��ŸŸ�� = (����ð� - �����ð�) / CPU������
	m_fDeltaTime = float(m_CurTime.QuadPart - m_OldTime.QuadPart) / m_CPUTick.QuadPart;

	m_OldTime = m_CurTime;
}
