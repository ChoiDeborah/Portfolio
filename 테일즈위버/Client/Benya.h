#pragma once
#include "Character.h"
class CShopWnd;
class CBenya :
	public CCharacter
{
public:
	CBenya();
	virtual ~CBenya();

public:
	// CCharacter을(를) 통해 상속됨
	virtual void SetIdleState()		override;
	virtual void SetRunState()		override;
	virtual void SetAttackState()	override;
	virtual void SetHitState()		override;
	virtual void Attack(SKILLID eSkillID)		override;
	virtual void FrameEvent()		override;

};

