#pragma once

#include "WndInfo.h"

class CUIWnd
{
public:
	CUIWnd();
	~CUIWnd();

public:
	virtual HRESULT	Initialize()	PURE;
	virtual void	Update()		PURE;
	virtual void	Render()		PURE;
	virtual void	Release()		PURE;

	virtual void	LClickDown()	PURE;
	virtual void	LClickUp()		PURE;

	virtual void	RClickDown()	PURE;

public:
	void	WindowMove();
	void	WindowLClickDown();
	void	WindowLClickUp();
	void	WindowRClickDown();

	void	ShowWindow(bool bIsShow) { m_bIsShow = bIsShow; }
	bool	IsShow() { return m_bIsShow; }

public:
	void	SetPos(const D3DXVECTOR3& vPos) { m_tWndInfo.vPos = vPos; }


protected:
	CWndInfo		m_tWndInfo;		// ��ü â ũ�� ��ġ ����
	CWndInfo		m_tMoveInfo;	// �̵� ���� ũ�� ��ġ ����
	CWndInfo		m_tExitInfo;	// ���� ��ư ����

	D3DXVECTOR3		m_vStartPos;


protected:
	CDeviceMgr*		m_pDeviceMgr	= CDeviceMgr::GetInstance();
	CTextureMgr*	m_pTextureMgr	= CTextureMgr::GetInstance();
	CViewMgr*		m_pViewMgr		= CViewMgr::GetInstance();
	CMouseMgr*		m_pMouseMgr		= CMouseMgr::GetInstance();
	CKeyMgr*		m_pKeyMgr		= CKeyMgr::GetInstance();
	CUIMgr*			m_pUIMgr		= CUIMgr::GetInstance();
	CObjMgr*		m_pObjMgr		= CObjMgr::GetInstance();
	CSceneMgr*		m_pSceneMgr		= CSceneMgr::GetInstance();
	CSoundMgr*		m_pSoundMgr		= CSoundMgr::GetInstance();

	const TEX_INFO*	m_pWndTex		= nullptr;
	const TEX_INFO*	m_pExitTex		= nullptr;


protected:
	bool			m_bIsMove		= false;
	bool			m_bIsShow		= false;
};

