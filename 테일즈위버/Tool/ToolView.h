
// ToolView.h : CToolView 클래스의 인터페이스
//

#pragma once

class CTerrain;
class CToolDoc;
class CMainDialog;
class CToolView : public CScrollView
{
protected: // serialization에서만 만들어집니다.
	CToolView();
	DECLARE_DYNCREATE(CToolView)

// 특성입니다.
public:
	CToolDoc* GetDocument() const;

// 작업입니다.
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

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~CToolView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

public:
	void Update();

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

#ifndef _DEBUG  // ToolView.cpp의 디버그 버전
inline CToolDoc* CToolView::GetDocument() const
   { return reinterpret_cast<CToolDoc*>(m_pDocument); }
#endif

