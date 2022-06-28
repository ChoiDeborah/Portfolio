
// ToolView.cpp : CToolView Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
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
	// ǥ�� �μ� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CScrollView::OnFilePrintPreview)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_KEYDOWN()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CToolView ����/�Ҹ�
HWND g_hWnd;

CToolView::CToolView()
	: m_pDeviceMgr(CDeviceMgr::GetInstance()),
	m_pTextureMgr(CTextureMgr::GetInstance()),
	m_pTimeMgr(CTimeMgr::GetInstance()),
	m_pViewMgr(CViewMgr::GetInstance()),
	m_pKeyMgr(CKeyMgr::GetInstance()),
	m_pTerrain(nullptr)
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
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
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CScrollView::PreCreateWindow(cs);
}

void CToolView::OnInitialUpdate()
{
	m_pTimeMgr->InitTime();

	CScrollView::OnInitialUpdate();
	CScrollView::SetScrollSizes(MM_TEXT, CSize(VIEWCX, VIEWCY));

	// View ������ 800 * 600���� �����ϱ�.

	// ������ �����츦 ���´�.
	// AfxGetApp: CWinApp�� �����͸� ������ MFC ���� �Լ�.
	// GetMainWnd: MainFrame�� �����͸� ������ CWinApp�� ����Լ�.
	CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	NULL_CHECK(pMainFrm);

	// MainFrame�� ũ�⸦ ���´�.
	// GetWindowRect: �������� ��ü �׵θ��� ������ �簢�� ũ�⸦ ������ �Լ�.
	// �� �� left, top, right, bottom�� ��ũ����ǥ �����̴�.
	RECT rcMain = {};
	pMainFrm->GetWindowRect(&rcMain);
	::SetRect(&rcMain, 0, 0, rcMain.right - rcMain.left, rcMain.bottom - rcMain.top);


	// View�� ũ�⸦ ���´�.
	// GetClientRect: �������� �׵θ��� �������� ���� ���� Ŭ���̾�Ʈ ������ �簢�� ũ�⸦ ������ �Լ�.
	// �� �� left, top, rigth, bottom�� Ŭ���̾�Ʈ ��ǥ �����̴�.
	// ��, left, top�� �׻� 0, 0�̴�.
	RECT rcView = {};
	GetClientRect(&rcView);

	// MainFrame�� View�� ������ ���.
	int iGapX = rcMain.right - rcView.right;
	int iGapY = rcMain.bottom - rcView.bottom;

	// MainFrame�� ������ ��ġ�� ũ�⸦ �ٽ� ����.
	pMainFrm->SetWindowPos(
		nullptr, 0, 0, VIEWCX + 185 + iGapX, VIEWCY + iGapY, SWP_NOZORDER);
	

	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
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

// ������Ʈ
void CToolView::Update()
{
	CTimeMgr::GetInstance()->UpdateTime();
	if (nullptr != m_pMainDialog) {
		switch (m_pMainDialog->m_Tab.GetCurSel())
		{
		case SPRITE_TOOL:	m_pMainDialog->m_SpriteTool.Update();	break;	// ��������Ʈ ��
		case UNIT_TOOL:		m_pMainDialog->m_UnitTool.FrameMove();	break;	// ���� ��
		case MAP_TOOL:			break;	// �� ��
		case EVENT_TOOL:	m_pMainDialog->m_EventTool.Update();	break;	// �̺�Ʈ ��
		}
	}

	m_pTerrain->Update();

	m_pViewMgr->Update();
	m_pKeyMgr->Update();

	Invalidate(FALSE);
}

// CToolView �׸���
// WM_PAINT �޽��� �߻��� �� ȣ���.
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
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CRect rt;
	GetClientRect(rt);
	pDC->FillSolidRect(rt, RGB(50, 50, 50));

	return TRUE;//CScrollView::OnEraseBkgnd(pDC);
}

// CToolView �μ�

BOOL CToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
}


// CToolView ����

#ifdef _DEBUG
void CToolView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CToolView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CToolDoc* CToolView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CToolDoc)));
	return (CToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CToolView �޽��� ó����


