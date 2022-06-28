#pragma once
#include "Scene.h"

#define CIRCLE_NUM 20

class CLogo :
	public CScene
{
public:
	CLogo();
	virtual ~CLogo();

public:
	// CScene��(��) ���� ��ӵ�
	virtual HRESULT Initialize() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual void Release() override;

protected:
	static unsigned __stdcall LoadingThreadFunc(void* pParam);

// ���������
protected:
	FRAME				m_tFrame;

	CRITICAL_SECTION	m_CriticalSection;

	HANDLE				m_hLoadingThread;

// ������
protected:
	const TEX_INFO*		m_pLoadingTex			= nullptr;
	const TEX_INFO*		m_pLoadingBarTex		= nullptr;

// �⺻�ڷ���
protected:
	bool				m_bIsLoadingComplete	= false;
};

