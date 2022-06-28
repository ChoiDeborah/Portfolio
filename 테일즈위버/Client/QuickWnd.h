#pragma once
#include "UIWnd.h"

#define QUICK_SLOT_SIZEX 4
#define QUICK_SLOT_SIZEY 2

class CIconData;
class CQuickWnd :
	public CUIWnd
{
public:
	CQuickWnd();
	virtual ~CQuickWnd();

public:
	// Inherited via CUIWnd
	virtual HRESULT Initialize() override;
	virtual void	Update() override;
	virtual void	Render() override;
	virtual void	Release() override;

	virtual void	LClickDown()	override;
	virtual void	LClickUp()		override;

	virtual void	RClickDown()	override;

public:
	void		SetIcon(int iIndex, CIconData* pIconData);
	void		DeleteItem(int iIndex);

	void		UpdateSlot();

public:
	static CQuickWnd* Create();

private:
	CIconData*		m_pIcon[2][QUICK_SLOT_SIZEX * QUICK_SLOT_SIZEY];

	CWndInfo		m_tIcon[QUICK_SLOT_SIZEX * QUICK_SLOT_SIZEY];

	int				m_iQuickKey[QUICK_SLOT_SIZEX * QUICK_SLOT_SIZEY];

	int				m_iQuickIndex = 0;
};

