#pragma once
#include "UIWnd.h"

#define INVEN_SIZE 12

class CCreditWnd :
	public CUIWnd
{
public:
	CCreditWnd();
	virtual ~CCreditWnd();

public:
	// Inherited via CUIWnd
	virtual HRESULT		Initialize() override;
	virtual void		Update() override;
	virtual void		Render() override;
	virtual void		Release() override;

	virtual void		LClickDown()	override;
	virtual void		LClickUp()		override;

	virtual void		RClickDown()	override;

public:
	static CCreditWnd*	Create();

private:
	CWndInfo			m_tTextBox;
};

