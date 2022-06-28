#pragma once
#include "Scene.h"
#include "CreditWnd.h"

#define CIRCLE_NUM 20

class CTitle :
	public CScene
{
public:
	CTitle();
	virtual ~CTitle();

public:
	// CScene��(��) ���� ��ӵ�
	virtual HRESULT Initialize() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual void Release() override;

private:
	CWndInfo	m_tGameStart;
	CWndInfo	m_tShowOpenning;
	CWndInfo	m_tCredit;
	CWndInfo	m_tExit;

	CMouseMgr*	m_pMouseMgr = CMouseMgr::GetInstance();

// ���������
protected:
	CCreditWnd			m_tCreditWnd;

	FRAME				m_tFrame;

};

