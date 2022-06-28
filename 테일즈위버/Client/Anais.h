#pragma once
#include "Character.h"
class CAnais :
	public CCharacter
{
public:
	CAnais();
	virtual ~CAnais();

public:
	// CCharacter을(를) 통해 상속됨
	virtual void SetIdleState()		override;
	virtual void SetRunState()		override;
	virtual void SetAttackState()	override;
	virtual void SetHitState()		override;
	virtual void Attack(SKILLID eSkillID)		override;
	virtual void FrameEvent()		override;
};

