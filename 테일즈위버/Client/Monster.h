#pragma once
#include "EventObject.h"
class CMonster :
	public CEventObject
{
public:
	CMonster();
	virtual ~CMonster();

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

	void	FindPlayer();

public:
	virtual void	Hit();

protected:
	CAlarm			m_tMovingAlarm;
	CAlarm			m_tAttackAlarm;

	D3DXVECTOR3		m_vStartPos;

	bool			m_bIsPlayerFind = false;
};

