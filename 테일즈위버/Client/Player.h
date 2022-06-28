#pragma once
#include "EventObject.h"

class CCharacter;
class CPlayer :
	public CEventObject
{
public:
	CPlayer();
	virtual ~CPlayer();

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Initialize()		override;
	virtual HRESULT LateInit()			override;
	virtual int		Update()			override;
	virtual int		LateUpdate()		override;
	virtual void	Render()			override;
	
private:
	virtual void	Release()			override;

	virtual void	FrameOnceEnd()		override;
	virtual void	FrameEvent()		override;

	virtual void	AtarMoveEnd()		override;
	virtual void	AstarMoveSetState()	override;
	virtual bool	AstartExitEvent()	override;
	virtual void	Hit();

	

public:
	virtual	void	Attack();
	
public:
	int				GetCombo()		{ return m_iCombo;			}

public:
	void			AddCombo(int iAddCombo) { m_iCombo += iAddCombo;	}

	void			UseSkill(SKILLID eSkillID);
	void			UpdateData();
	IDENTITY_DATA&	GetIdentity() { return m_tIdentity; }


	HRESULT			SaveGame(const wstring& wstrFileName);
	HRESULT			LoadGame(const wstring& wstrFileName);

private:
	void			KeyInput();
	void			AttackKey();
	bool			FindMonster();
	
	//float	GetMouseLookAngle();

// 사용자 정의형
private:
	IDENTITY_DATA	m_tIdentity;
	D3DXVECTOR3		m_vTargetPos;

	CAlarm			m_tComboDelay;
	CAlarm			m_tStepAlarm;

// 포인터
private:
	CKeyMgr*		m_pKeyMgr			= CKeyMgr::GetInstance();
	CPlayerSubject*	m_pSubject			= CPlayerSubject::GetInstance();

// 기본 자료형
private:
	int				m_iSelectTile		= 0;

	int				m_iComboAttack		= 0;

	int				m_iCombo			= 0;
	int				m_iAttack1Damage	= 2;
	float			m_fSkill1Mp			= 10;

	bool			m_bIsKeyDown = false;
	bool			m_bIsAttackGround	= false;
	bool			m_bIsGate = false;
};

