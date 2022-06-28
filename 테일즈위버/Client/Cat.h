#pragma once
#include "Character.h"
class CCat :
	public CCharacter
{
public:
	CCat();
	virtual ~CCat();

public:
	// CCharacter��(��) ���� ��ӵ�
	virtual void SetIdleState()		override;
	virtual void SetRunState()		override;
	virtual void SetAttackState()	override;
	virtual void SetHitState()		override;
	virtual void Attack(SKILLID eSkillID)		override;
	virtual void FrameEvent()		override;
};

