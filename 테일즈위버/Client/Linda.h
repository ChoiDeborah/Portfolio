#pragma once
#include "Character.h"
class CLinda :
	public CCharacter
{
public:
	CLinda();
	virtual ~CLinda();

public:
	// CCharacter을(를) 통해 상속됨
	virtual void SetIdleState()		override;
	virtual void SetRunState()		override;
	virtual void SetAttackState()	override;
	virtual void SetHitState()		override;
	virtual void Attack(SKILLID eSkillID)		override;
	virtual void FrameEvent()		override;

public:
	int		m_iEventCount;
};

