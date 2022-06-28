#include "stdafx.h"
#include "FrameMgr.h"

IMPLEMENT_SINGLETON(CFrameMgr)

CFrameMgr::CFrameMgr()
{
	ZeroMemory(&m_OriginTime, sizeof(LARGE_INTEGER));
	ZeroMemory(&m_OldTime, sizeof(LARGE_INTEGER));
	ZeroMemory(&m_CurTime, sizeof(LARGE_INTEGER));
	ZeroMemory(&m_CPUTick, sizeof(LARGE_INTEGER));
}


CFrameMgr::~CFrameMgr()
{
}

HRESULT CFrameMgr::InitTime()
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

	m_fTimeCount = 0.f;

	return S_OK;
}

bool CFrameMgr::LockFrame(float fFPS)
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
	m_fTimeCount += float(m_CurTime.QuadPart - m_OldTime.QuadPart) / m_CPUTick.QuadPart;
	m_OldTime = m_CurTime;

	if (1.f / fFPS <= m_fTimeCount) {
		m_fTimeCount = 0.f;
		return true;
	}

	return false;
}

void CFrameMgr::RenderFPS()
{
	m_fFPSCount += CTimeMgr::GetInstance()->GetDeltaTime();

	if (1.f <= m_fFPSCount) {
		swprintf_s(m_szFPS, L"FPS : %d", m_iFPS);
		m_iFPS = 0;
		m_fFPSCount = 0.f;
	}

	// FPS ��� (������)
	/*D3DXMATRIX matTrans;
	D3DXMatrixTranslation(&matTrans, 500.f, 100.f, 0.f);
	CDeviceMgr::GetInstance()->GetSprite()->SetTransform(&matTrans);
	CDeviceMgr::GetInstance()->GetFont()->DrawText(
		CDeviceMgr::GetInstance()->GetSprite(), // ��������Ʈ Com ��ü
		m_szFPS,								// ����� ����
		lstrlen(m_szFPS),						// ���ڿ� ����
		nullptr,								// �簢�� ����
		0,										// �簢�� ���� �ȿ� �׸� ���� �ɼ�
		D3DCOLOR_ARGB(255, 0, 255, 0)			// ��� ����
	);*/
}
