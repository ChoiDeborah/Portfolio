#pragma once
#include "Character.h"
class CKyoko :
	public CCharacter
{
public:
	CKyoko();
	virtual ~CKyoko();

public:
	// CCharacter��(��) ���� ��ӵ�
	virtual void SetIdleState()					override;
	virtual void SetRunState()					override;
	virtual void SetAttackState()				override;
	virtual void SetHitState()					override;
	virtual void Attack(SKILLID eSkillID)		override;
	virtual void FrameEvent()					override;

private:
	vector<CGameObject*>	m_vecSkillTarget;

	int		m_iCombo = 0;
};

