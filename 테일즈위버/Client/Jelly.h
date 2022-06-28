#pragma once
#include "Character.h"
class CJelly :
	public CCharacter
{
public:
	CJelly();
	virtual ~CJelly();

public:
	// CCharacter��(��) ���� ��ӵ�
	virtual void SetIdleState()		override;
	virtual void SetRunState()		override;
	virtual void SetAttackState()	override;
	virtual void SetHitState()		override;
	virtual void Attack(SKILLID eSkillID)		override;
	virtual void FrameEvent()		override;
};

