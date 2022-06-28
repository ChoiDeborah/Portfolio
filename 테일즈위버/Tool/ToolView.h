
// ToolView.h : CToolView Ŭ������ �������̽�
//

#pragma once

class CTerrain;
class CToolDoc;
class CMainDialog;
class CToolView : public CScrollView
{
protected: // serialization������ ��������ϴ�.
	CToolView();
	DECLARE_DYNCREATE(CToolView)

// Ư���Դϴ�.
public:
	CToolDoc* GetDocument() const;

// �۾��Դϴ�.
public:
	void SetScrollSize(int cx, int cy);

public:
	CDeviceMgr*		m_pDeviceMgr	= nullptr;
	CTextureMgr*	m_pTextureMgr	= nullptr;
	CTimeMgr*		m_pTimeMgr		= nullptr;
	CViewMgr*		m_pViewMgr		= nullptr;
	CKeyMgr*		m_pKeyMgr		= nullptr;
	CTerrain*		m_pTerrain		= nullptr;
	CMainDialog*	m_pMainDialog	= nullptr;

// �������Դϴ�.
public:
	virtual void OnDraw(CDC* pDC);  // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// �����Դϴ�.
public:
	virtual ~CToolView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

public:
	void Update();

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

#ifndef _DEBUG  // ToolView.cpp�� ����� ����
inline CToolDoc* CToolView::GetDocument() const
   { return reinterpret_cast<CToolDoc*>(m_pDocument); }
#endif

