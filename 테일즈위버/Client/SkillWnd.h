#pragma once
#include "UIWnd.h"

#define SKILL_SLOT_X	10
#define SKILL_SLOT_Y	3


class CSkillData;
class CSkillWnd :
	public CUIWnd
{
public:
	CSkillWnd();
	virtual ~CSkillWnd();

public:
	// Inherited via CUIWnd
	virtual HRESULT Initialize()	override;
	virtual void	Update()		override;
	virtual void	Render()		override;
	virtual void	Release()		override;

	virtual void	LClickDown()	override;
	virtual void	LClickUp()		override;

	virtual void	RClickDown()	override;

public:
	CSkillData*		GetSkillData(SKILLID eSkillID);
	vector<CSkillData*>* GetVecSkill() { return &m_vecSkillData; }

public:
	static CSkillWnd* Create();

private:
	CWndInfo		m_tSkillIcon[SKILL_SLOT_X*SKILL_SLOT_Y];

private:
	vector<CSkillData*>	m_vecSkillData;
};

