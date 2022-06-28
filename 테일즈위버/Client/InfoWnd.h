#pragma once
#include "UIWnd.h"
class CGameObject;
class CInfoWnd :
	public CUIWnd
{
public:
	CInfoWnd();
	virtual ~CInfoWnd();

public:
	// Inherited via CUIWnd
	virtual HRESULT Initialize()	override;
	virtual void	Update()		override;
	virtual void	Render()		override;
	virtual void	Release()		override;

	virtual void	LClickDown()	override;
	virtual void	LClickUp()		override;

	virtual void	RClickDown()	override;

private:
	wstring IsAddStat(int iAddStat, const wstring& wstrBeforeStat);

public:
	void	UpdateState();

private:
	CWndInfo	tStabBtn;
	CWndInfo	tHackBtn;
	CWndInfo	tIntBtn;
	CWndInfo	tDefBtn;

	CWndInfo	tMrBtn;
	CWndInfo	tDexBtn;
	CWndInfo	tAgiBtn;
	

private:
	CGameObject*	m_pPlayer	= nullptr;

	const TEX_INFO*	m_pExpBar	= nullptr;
	const TEX_INFO*	m_pStatusUp = nullptr;

private:
	int		m_iNeedStab = 2;
	int		m_iNeedHack = 2;
	int		m_iNeedInt = 2;
	int		m_iNeedDef = 1;
	int		m_iNeedMr = 1;
	int		m_iNeedDex = 3;
	int		m_iNeedAgi = 3;
};

