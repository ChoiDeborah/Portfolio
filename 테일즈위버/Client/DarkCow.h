#pragma once
#include "Character.h"
class CDarkCow :
	public CCharacter
{
public:
	CDarkCow();
	virtual ~CDarkCow();

public:
	// CCharacter을(를) 통해 상속됨
	virtual void SetIdleState()		override;
	virtual void SetRunState()		override;
	virtual void SetAttackState()	override;
	virtual void SetHitState()		override;
	virtual void Attack(SKILLID eSkillID)		override;
	virtual void FrameEvent()		override;
};

