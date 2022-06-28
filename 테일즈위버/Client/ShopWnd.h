#pragma once
#include "UIWnd.h"

#define BUY_SLOT	6
#define SELL_SLOT	13


class CShopWnd :
	public CUIWnd
{
public:
	CShopWnd();
	virtual ~CShopWnd();

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
	void AddItem(CItemData* pItemData);
	void DeleteAllItem();
	void SetShopState(int iState) { m_iShopState = iState; }
	void InsertSellItem(CItemData* pItemData);
	void InsertTradeItem(CItemData* pItemData);

public:
	static CShopWnd* Create();

private:
	vector<CItemData*>	m_vecShopItem;
	vector<CItemData*>	m_vecTradeItem;
	vector<CItemData*>	m_vecSellItem;

	
	CWndInfo			m_tShopItem[BUY_SLOT];	// ���� ��
	CWndInfo			m_tTradeItem[BUY_SLOT];	// ���� ��� ��
	
	CWndInfo			m_tSellItem[SELL_SLOT];	// �Ǹ� ��

	CWndInfo			m_tTradeBtn;		// ���� �� ��ư
	CWndInfo			m_tReBuyBtn;		// �Ǹ� �� ��ư

	CWndInfo			m_tShopScrollUp;
	CWndInfo			m_tShopScrollDown;
	CWndInfo			m_tShopScrollBar;

	CWndInfo			m_tTradeScrollUp;
	CWndInfo			m_tTradeScrollDown;
	CWndInfo			m_tTradeScrollBar;

	CWndInfo			m_tSellScrollUp;
	CWndInfo			m_tSellScrollDown;
	CWndInfo			m_tSellScrollBar;

	CWndInfo			m_tNegotiationBtn;	// ���� ��ư
	CWndInfo			m_tBuyBtn;			// ���� ��ư

	CWndInfo			m_tAllUpBtn;		// ��� ������ ����
	CWndInfo			m_tAllDownBtn;		// ��� ������ �Ʒ���

	D3DXVECTOR3			m_vScrollStartPos;

private:
	const TEX_INFO*		m_pTradeBtnTex = nullptr;
	const TEX_INFO*		m_pRebuyBtnTex = nullptr;

	const TEX_INFO*		m_pScrollUpTex = nullptr;
	const TEX_INFO*		m_pScrollDownTex = nullptr;
	const TEX_INFO*		m_pScrollBarTex = nullptr;

private:
	int					m_iShopState	= 0;	// 0���� 1�Ǹ�

	int					m_iShopScroll	= 0;
	int					m_iTradeScroll	= 0;
	int					m_iSellScroll	= 0;

	int					m_iPayment = 0;

	bool				m_bIsShopScrollMove = false;
	bool				m_bIsTradeScrollMove = false;
	bool				m_bIsSellScrollMove = false;
};

