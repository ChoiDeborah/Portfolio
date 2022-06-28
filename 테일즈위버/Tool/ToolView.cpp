
// ToolView.cpp : CToolView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "Tool.h"
#endif

#include "ToolDoc.h"
#include "ToolView.h"
#include "MainFrm.h"
#include "MyForm.h"
#include "Terrain.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CToolView
IMPLEMENT_DYNCREATE(CToolView, CScrollView)

BEGIN_MESSAGE_MAP(CToolView, CScrollView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CScrollView::OnFilePrintPreview)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_KEYDOWN()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CToolView 생성/소멸
HWND g_hWnd;

CToolView::CToolView()
	: m_pDeviceMgr(CDeviceMgr::GetInstance()),
	m_pTextureMgr(CTextureMgr::GetInstance()),
	m_pTimeMgr(CTimeMgr::GetInstance()),
	m_pViewMgr(CViewMgr::GetInstance()),
	m_pKeyMgr(CKeyMgr::GetInstance()),
	m_pTerrain(nullptr)
{
	// TODO: 여기에 생성 코드를 추가합니다.
}

void CToolView::SetScrollSize(int cx, int cy)
{
	CScrollView::SetScrollSizes(MM_TEXT, CSize(cx, cy));
}

CToolView::~CToolView()
{
	SafeDelete(m_pTerrain);
	m_pTextureMgr->DestroyInstance();
	m_pDeviceMgr->DestroyInstance();
	m_pTimeMgr->DestroyInstance();
}

BOOL CToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CScrollView::PreCreateWindow(cs);
}

void CToolView::OnInitialUpdate()
{
	m_pTimeMgr->InitTime();

	CScrollView::OnInitialUpdate();
	CScrollView::SetScrollSizes(MM_TEXT, CSize(VIEWCX, VIEWCY));

	// View 사이즈 800 * 600으로 조정하기.

	// 프레임 윈도우를 얻어온다.
	// AfxGetApp: CWinApp의 포인터를 얻어오는 MFC 전역 함수.
	// GetMainWnd: MainFrame의 포인터를 얻어오는 CWinApp의 멤버함수.
	CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	NULL_CHECK(pMainFrm);

	// MainFrame의 크기를 얻어온다.
	// GetWindowRect: 윈도우의 전체 테두리를 포함한 사각형 크기를 얻어오는 함수.
	// 이 때 left, top, right, bottom은 스크린좌표 기준이다.
	RECT rcMain = {};
	pMainFrm->GetWindowRect(&rcMain);
	::SetRect(&rcMain, 0, 0, rcMain.right - rcMain.left, rcMain.bottom - rcMain.top);


	// View의 크기를 얻어온다.
	// GetClientRect: 윈도우의 테두리를 포함하지 않은 순수 클라이언트 영역의 사각형 크기를 얻어오는 함수.
	// 이 때 left, top, rigth, bottom은 클라이언트 좌표 기준이다.
	// 즉, left, top은 항상 0, 0이다.
	RECT rcView = {};
	GetClientRect(&rcView);

	// MainFrame과 View의 간격을 계산.
	int iGapX = rcMain.right - rcView.right;
	int iGapY = rcMain.bottom - rcView.bottom;

	// MainFrame의 윈도우 위치와 크기를 다시 조정.
	pMainFrm->SetWindowPos(
		nullptr, 0, 0, VIEWCX + 185 + iGapX, VIEWCY + iGapY, SWP_NOZORDER);
	

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	g_hWnd = m_hWnd;

	HRESULT hr = 0;

	hr = m_pDeviceMgr->InitDevice(CDeviceMgr::MODE_WIN);
	FAILED_CHECK_MSG(hr, L"InitDevice Failed");

	m_pTerrain = CTerrain::Create(this);
	NULL_CHECK_MSG(m_pTerrain, L"Terrain Create Failed");

	CMainFrame* pFrameWnd = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	NULL_CHECK(pFrameWnd);

	CMyForm* pMyForm = dynamic_cast<CMyForm*>(pFrameWnd->m_MainSplitter.GetPane(0, 1));
	NULL_CHECK(pMyForm);

	m_pMainDialog = &(pMyForm->m_MainDialog);
	NULL_CHECK(m_pMainDialog);

	m_pTerrain->SetMainDialog(m_pMainDialog);
	m_pMainDialog->m_MapTool.m_pTerrain = m_pTerrain;
	m_pMainDialog->m_EventTool.m_pTerrain = m_pTerrain;
}

// 업데이트
void CToolView::Update()
{
	CTimeMgr::GetInstance()->UpdateTime();
	if (nullptr != m_pMainDialog) {
		switch (m_pMainDialog->m_Tab.GetCurSel())
		{
		case SPRITE_TOOL:	m_pMainDialog->m_SpriteTool.Update();	break;	// 스프라이트 툴
		case UNIT_TOOL:		m_pMainDialog->m_UnitTool.FrameMove();	break;	// 유닛 툴
		case MAP_TOOL:			break;	// 맵 툴
		case EVENT_TOOL:	m_pMainDialog->m_EventTool.Update();	break;	// 이벤트 툴
		}
	}

	m_pTerrain->Update();

	m_pViewMgr->Update();
	m_pKeyMgr->Update();

	Invalidate(FALSE);
}

// CToolView 그리기
// WM_PAINT 메시지 발생할 때 호출됨.
void CToolView::OnDraw(CDC* pDC)
{
	CToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	NULL_CHECK(m_pTerrain);

	m_pViewMgr->Render();
	m_pDeviceMgr->Render_Begin();
	m_pTerrain->Render();
	m_pDeviceMgr->Render_End();
}

BOOL CToolView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CRect rt;
	GetClientRect(rt);
	pDC->FillSolidRect(rt, RGB(50, 50, 50));

	return TRUE;//CScrollView::OnEraseBkgnd(pDC);
}

// CToolView 인쇄

BOOL CToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CToolView 진단

#ifdef _DEBUG
void CToolView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CToolView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CToolDoc* CToolView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CToolDoc)));
	return (CToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CToolView 메시지 처리기


