#pragma once
#include "UIWnd.h"

#define COMBO_SLOT_X	5
#define COMBO_SLOT_Y	3


class CSkillData;
class CComboWnd :
	public CUIWnd
{
public:
	CComboWnd();
	virtual ~CComboWnd();

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
	int				GetCurComboSkill() { return m_iCurSlot; }
	void			SetComboSkill(int iSlotY, int iSlotX, CSkillData* pSkillData);
	void			DeleteComboSkill(int iSlotY, int iSlotX);

	vector<CSkillData*>*	GetVecCurCombo() { return &m_vecSkillData[m_iCurSlot]; }

public:
	static CComboWnd* Create();

private:
	vector<CSkillData*>		m_vecSkillData[COMBO_SLOT_Y];

private:
	CWndInfo		m_tSkillIcon[COMBO_SLOT_Y][COMBO_SLOT_X];

	CWndInfo		m_tSelected[COMBO_SLOT_Y];

private:
	int				m_iCurSlot = 0;
};

