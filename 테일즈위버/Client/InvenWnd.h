#pragma once
#include "UIWnd.h"

#define INVEN_SIZE 12

class CInvenWnd :
	public CUIWnd
{
public:
	CInvenWnd();
	virtual ~CInvenWnd();

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
	CItemData*	GetItem(int iIndex);
	void AddItem(CItemData* pItemData);
	void DeleteItem(int iIndex, int iCount);

	void UpdateSlot();

public:
	static CInvenWnd* Create();

private:
	vector<CItemData*>	m_vecItem;

	CWndInfo			m_tItem[INVEN_SIZE];

	CWndInfo			m_tScrollUp;
	CWndInfo			m_tScrollDown;
	CWndInfo			m_tScrollBar;

	D3DXVECTOR3			m_vScrollStartPos;

private:
	const TEX_INFO*		m_pScrollUpTex = nullptr;
	const TEX_INFO*		m_pScrollDownTex = nullptr;
	const TEX_INFO*		m_pScrollBarTex = nullptr;

private:
	int					m_iScroll	= 0;
	bool				m_bIsScrollMove = false;
};

