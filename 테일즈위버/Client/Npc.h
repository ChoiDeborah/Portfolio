#pragma once
#include "EventObject.h"
class CNpc :
	public CEventObject
{
public:
	CNpc();
	virtual ~CNpc();

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

public:
	virtual void	Hit();

public:
	void	Talk();


protected:
	CAlarm			m_tMovingAlarm;
	CAlarm			m_tAttackAlarm;

	D3DXVECTOR3		m_vStartPos;

	bool			m_bIsTalk = false;
};

