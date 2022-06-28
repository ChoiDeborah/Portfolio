#pragma once
#include "Character.h"
class CDarkCow :
	public CCharacter
{
public:
	CDarkCow();
	virtual ~CDarkCow();

public:
	// CCharacter��(��) ���� ��ӵ�
	virtual void SetIdleState()		override;
	virtual void SetRunState()		override;
	virtual void SetAttackState()	override;
	virtual void SetHitState()		override;
	virtual void Attack(SKILLID eSkillID)		override;
	virtual void FrameEvent()		override;
};

