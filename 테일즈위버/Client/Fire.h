#pragma once
#include "EventObject.h"

#define Fire_NUM 30

class CFire :
	public CEventObject
{
public:
	CFire();
	virtual ~CFire();

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
	void	SetPlayer(CGameObject* pPlayer) { m_pPlayer = pPlayer; }
	void	SetMirrol(bool bIsMirrol) { m_bIsMirrol = bIsMirrol; }

protected:
	//vector<D3DXVECTOR3>		m_vecPrevPos;
	vector<CGameObject*>	m_vecTarget;

	//CAlarm			m_tPosAlarm;
	CAlarm				m_tFireAlarm;

protected:
	CGameObject*	m_pPlayer		= nullptr;

protected:
	bool			m_bIsMirrol		= false;
	int				m_iCurTarget	= 1;

	float			m_fSinMove		= 1.f;

	bool			m_bIsGrab		= false;
};

