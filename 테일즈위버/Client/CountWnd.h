#pragma once
#include "UIWnd.h"

#define INVEN_SIZE 12

class CCountWnd :
	public CUIWnd
{
public:
	CCountWnd();
	virtual ~CCountWnd();

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
	void				SetItemData(CItemData* pItemData) { m_pItemData = pItemData; }

	void				SetWndData(WND_ID eFrom, WND_ID eTo, CItemData* pItemData);

	void				MoveItem();

public:
	static CCountWnd*	Create();

private:
	CWndInfo			m_tMaxBtn;
	CWndInfo			m_tOkBtn;

	CWndInfo			m_tScrollLeft;
	CWndInfo			m_tScrollRight;

	CWndInfo			m_tTextBox;

	WND_ID				m_eFromWnd	= WND_ID::WNDID_END;
	WND_ID				m_eToWnd	= WND_ID::WNDID_END;

private:
	CItemData*			m_pFromData = nullptr;
	CItemData*			m_pItemData = nullptr;

	const TEX_INFO*		m_pMaxBtnTex = nullptr;
	const TEX_INFO*		m_pOkBtnTex = nullptr;

	const TEX_INFO*		m_pScrollLeftTex = nullptr;
	const TEX_INFO*		m_pScrollRightTex = nullptr;

private:
	size_t				m_iCount;
};

