#pragma once
#include "UIWnd.h"

#define COMBOSET_SLOT_X		5
#define COMBOSET_SELECT_Y	4

enum ANIM_STATE { ANIM_IDLE, ANIM_ATTACK };

class CSkillData;
class CComboSetWnd :
	public CUIWnd
{
public:
	CComboSetWnd();
	virtual ~CComboSetWnd();

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
	static CComboSetWnd* Create();

private:
	CWndInfo		m_tSkillIcon[COMBOSET_SLOT_X];	// 스킬 아이콘 (하단)
	CWndInfo		m_tSelected[COMBOSET_SELECT_Y];	// 상단 모든 스킬

	CWndInfo		m_tPauseBtn;
	CWndInfo		m_tStartBtn;

	CWndInfo		m_tScrollUp;
	CWndInfo		m_tScrollDown;
	CWndInfo		m_tScrollBar;

	CWndInfo		m_tCharacter;

	CWndInfo		m_tAnimState;
	
	FRAME			m_tAnimation;
	CAlarm			m_tDelayAlarm;

	wstring			m_wstrObjKey = L"";
	wstring			m_wstrStateKey	= L"";

	ANIM_STATE		m_eAnimState = ANIM_IDLE;

private:
	const TEX_INFO*		m_pScrollUpTex = nullptr;
	const TEX_INFO*		m_pScrollDownTex = nullptr;
	const TEX_INFO*		m_pScrollBarTex = nullptr;

private:
	int				m_iScrollY = 0;
	int				m_iComboCount = 0;

	bool			m_bIsAnim = false;
};

