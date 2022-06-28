#pragma once
#include "EventObject.h"
class CBuddy :
	public CEventObject
{
public:
	CBuddy();
	virtual ~CBuddy();

public:
	// CEventObject을(를) 통해 상속됨
	virtual HRESULT Initialize()	override;
	virtual int		Update()		override;
	virtual int		LateUpdate()	override;
	virtual void	Render()		override;

protected:
	virtual void	Release()		override;
	virtual HRESULT LateInit()		override;
	virtual void	FrameOnceEnd()	override;
	virtual void	FrameEvent()	override;

	virtual void	AtarMoveEnd();
	virtual void	AstarMoveSetState();
	virtual bool	AstartExitEvent();

	virtual	void	Attack();

	void			FindMonster();

public:
	virtual void	Hit();


	void			SetFix(bool bIsFix) { m_bIsFix = bIsFix; }
	bool			GetFix() { return m_bIsFix; }

protected:
	CAlarm			m_tMovingAlarm;
	CAlarm			m_tAttackAlarm;

	CAlarm			m_tTalkAlarm;

	D3DXVECTOR3		m_vStartPos;

protected:
	CGameObject*	m_pPlayer		= nullptr;

protected:
	bool			m_bIsFix		= true;
};

