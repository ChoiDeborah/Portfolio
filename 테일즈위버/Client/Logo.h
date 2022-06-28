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
	// CScene을(를) 통해 상속됨
	virtual HRESULT Initialize() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual void Release() override;

protected:
	static unsigned __stdcall LoadingThreadFunc(void* pParam);

// 사용자정의
protected:
	FRAME				m_tFrame;

	CRITICAL_SECTION	m_CriticalSection;

	HANDLE				m_hLoadingThread;

// 포인터
protected:
	const TEX_INFO*		m_pLoadingTex			= nullptr;
	const TEX_INFO*		m_pLoadingBarTex		= nullptr;

// 기본자료형
protected:
	bool				m_bIsLoadingComplete	= false;
};

