// MainDialog.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "MainDialog.h"
#include "afxdialogex.h"

#include "ToolView.h"
#include "MainFrm.h"
#include "MyForm.h"


// CMainDialog 대화 상자입니다.

IMPLEMENT_DYNAMIC(CMainDialog, CDialog)

CMainDialog::CMainDialog(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_MAINDIALOG, pParent)
	, m_pWnd(NULL)
	, m_pTextureMgr(CTextureMgr::GetInstance())
{

}

CMainDialog::~CMainDialog()
{
}

void CMainDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_Tab);
}


BEGIN_MESSAGE_MAP(CMainDialog, CDialog)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CMainDialog::OnTcnSelchangeTab)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CMainDialog 메시지 처리기입니다.


BOOL CMainDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	CMainFrame* pFrameWnd = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	NULL_CHECK_MSG_RETURN(pFrameWnd, L"MainFrame Get Failed", FALSE);

	m_pToolView = dynamic_cast<CToolView*>(pFrameWnd->m_MainSplitter.GetPane(0, 0));
	NULL_CHECK_MSG_RETURN(m_pToolView, L"ToolView Get Failed", FALSE);

	m_Tab.InsertItem(SPRITE_TOOL + 1,	_T("Sprite"));
	m_Tab.InsertItem(UNIT_TOOL + 1,		_T("Object"));
	m_Tab.InsertItem(MAP_TOOL + 1,		_T("Map"));
	m_Tab.InsertItem(WORD_TOOL + 1,		_T("Word"));
	m_Tab.InsertItem(EVENT_TOOL + 1,	_T("EVENT"));

	CRect rt;
	m_Tab.GetClientRect(&rt);

	if (NULL == m_SpriteTool.GetSafeHwnd()) {
		m_SpriteTool.SetToolView(m_pToolView);
		m_SpriteTool.Create(IDD_SPRITETOOL, &m_Tab); // 다이얼로그 생성.
		m_SpriteTool.SetWindowPos(NULL, 4, 25, rt.Width() - 10, rt.Height() - 30, SWP_SHOWWINDOW | SWP_NOZORDER);
		m_pWnd = &m_SpriteTool;
	}

	if (NULL == m_UnitTool.GetSafeHwnd()) {
		m_UnitTool.SetToolView(m_pToolView);
		m_UnitTool.Create(IDD_UNITTOOL, &m_Tab); // 다이얼로그 생성.
		m_UnitTool.SetWindowPos(NULL, 4, 25, rt.Width() - 10, rt.Height() - 30, SWP_SHOWWINDOW | SWP_NOZORDER);
		m_UnitTool.ShowWindow(SW_HIDE);
		m_UnitTool.m_pImageMap = &m_mapTreeItem;
	}

	if (NULL == m_MapTool.GetSafeHwnd()) {
		m_MapTool.SetToolView(m_pToolView);
		m_MapTool.Create(IDD_MAPTOOL, &m_Tab); // 다이얼로그 생성.
		m_MapTool.SetWindowPos(NULL, 4, 25, rt.Width() - 10, rt.Height() - 30, SWP_SHOWWINDOW | SWP_NOZORDER);
		m_MapTool.ShowWindow(SW_HIDE);
	}

	if (NULL == m_WordTool.GetSafeHwnd()) {
		m_WordTool.SetToolView(m_pToolView);
		m_WordTool.Create(IDD_WORDTOOL, &m_Tab); // 다이얼로그 생성.
		m_WordTool.SetWindowPos(NULL, 4, 25, rt.Width() - 10, rt.Height() - 30, SWP_SHOWWINDOW | SWP_NOZORDER);
		m_WordTool.ShowWindow(SW_HIDE);
		m_WordTool.m_pImageMap = &m_mapTreeItem;
	}

	if (NULL == m_EventTool.GetSafeHwnd()) {
		m_EventTool.SetToolView(m_pToolView);
		m_EventTool.Create(IDD_EVENTTOOL, &m_Tab); // 다이얼로그 생성.
		m_EventTool.SetWindowPos(NULL, 4, 25, rt.Width() - 10, rt.Height() - 30, SWP_SHOWWINDOW | SWP_NOZORDER);
		m_EventTool.ShowWindow(SW_HIDE);
		m_EventTool.m_pImageMap = &m_mapTreeItem;
	}

	LoadImageData(L"../Data/ImageData.txt");

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CMainDialog::OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (nullptr != m_pWnd) {
		m_pWnd->ShowWindow(SW_HIDE);
		m_pWnd = nullptr;
	}

	switch (m_Tab.GetCurSel()) {
	case SPRITE_TOOL:
		m_SpriteTool.ShowWindow(SW_SHOW);
		m_pWnd = &m_SpriteTool;
		break;

	case UNIT_TOOL:
		m_UnitTool.ShowWindow(SW_SHOW);
		m_pWnd = &m_UnitTool;
		break;

	case MAP_TOOL:
	{
		const TEX_INFO*	pTexInfo = m_pTextureMgr->GetTexInfo(L"BackGround", L"Map", m_MapTool.m_pMapData->iMapImageNum);
		NULL_CHECK(pTexInfo);
		CViewMgr::GetInstance()->SetRoomSize({ float(pTexInfo->tImgInfo.Width), float(pTexInfo->tImgInfo.Height), 0.f });
		CViewMgr::GetInstance()->SetViewObj({ float(pTexInfo->tImgInfo.Width) * 0.5f, float(pTexInfo->tImgInfo.Height) * 0.5f, 0.f });

		m_MapTool.ShowWindow(SW_SHOW);
		m_pWnd = &m_MapTool;
	}
		break;

	case WORD_TOOL:
		m_WordTool.ShowWindow(SW_SHOW);
		m_pWnd = &m_WordTool;
		break;

	case EVENT_TOOL:
	{
		if (nullptr != m_EventTool.m_pMapData)
		{
			const TEX_INFO*	pTexInfo = m_pTextureMgr->GetTexInfo(L"BackGround", L"Map", m_EventTool.m_pMapData->iMapImageNum);
			NULL_CHECK(pTexInfo);
			CViewMgr::GetInstance()->SetRoomSize({ float(pTexInfo->tImgInfo.Width), float(pTexInfo->tImgInfo.Height), 0.f });
			CViewMgr::GetInstance()->SetViewObj({ float(pTexInfo->tImgInfo.Width) * 0.5f, float(pTexInfo->tImgInfo.Height) * 0.5f, 0.f });
		}
		m_EventTool.ShowWindow(SW_SHOW);
		m_pWnd = &m_EventTool;
	}
		break;
	}

	NULL_CHECK(m_pToolView);
	m_pToolView->Invalidate(FALSE);
}


BOOL CMainDialog::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CRect rt;
	GetClientRect(rt);
	pDC->FillSolidRect(rt, RGB(50, 50, 50));

	return TRUE;
}


// 이미지 데이터 불러오기
void CMainDialog::LoadImageData(wstring wstrFilePath)
{
	wifstream LoadFile;
	LoadFile.open(wstrFilePath.c_str(), ios::in);

	if (!LoadFile.good())
		return;

	m_mapTreeItem.clear();

	m_UnitTool.ResetImageTree();
	m_WordTool.ResetImageTree();
	m_EventTool.ResetImageTree();
	
	// 유닛
	HTREEITEM hSingle_Unit = m_UnitTool.m_TreeCtrlImage.InsertItem(L"Single", 0, 0, m_UnitTool.m_hRoot, TVI_LAST);
	HTREEITEM hMulti_Unit = m_UnitTool.m_TreeCtrlImage.InsertItem(L"Multi", 0, 0, m_UnitTool.m_hRoot, TVI_LAST);

	// 워드
	HTREEITEM hSingle_Word = m_WordTool.m_TreeCtrlImage.InsertItem(L"Single", 0, 0, m_WordTool.m_hRoot, TVI_LAST);
	HTREEITEM hMulti_Word = m_WordTool.m_TreeCtrlImage.InsertItem(L"Multi", 0, 0, m_WordTool.m_hRoot, TVI_LAST);

	// 이벤트
	HTREEITEM hSingle_Event = m_EventTool.m_TreeCtrlImage.InsertItem(L"Single", 0, 0, m_EventTool.m_hRoot, TVI_LAST);
	HTREEITEM hMulti_Event = m_EventTool.m_TreeCtrlImage.InsertItem(L"Multi", 0, 0, m_EventTool.m_hRoot, TVI_LAST);


	// 이미지 개수
	TCHAR szImageNum[MAX_STR] = L"";
	LoadFile.getline(szImageNum, MAX_STR);


	TCHAR szObjKey[MAX_STR] = L"";
	TCHAR szStateKey[MAX_STR] = L"";
	TCHAR szTexType[MAX_STR] = L"";
	TCHAR szOffsetX[MAX_STR] = L"";
	TCHAR szOffsetY[MAX_STR] = L"";
	TCHAR szCount[MAX_STR] = L"";
	TCHAR szMirror[MAX_STR] = L"";
	TCHAR szFrameDelay[MAX_STR] = L"";
	TCHAR szBlend[MAX_STR] = L"";
	TCHAR szEraseColorColor[MAX_STR] = L"";
	TCHAR szFilePath[MAX_STR] = L"";

	SPRITE_INFO* pSpriteInfo;

	while (!LoadFile.eof()) {
		LoadFile.getline(szObjKey, MAX_STR, '|');
		LoadFile.getline(szStateKey, MAX_STR, '|');
		LoadFile.getline(szTexType, MAX_STR, '|');
		LoadFile.getline(szOffsetX, MAX_STR, '|');
		LoadFile.getline(szOffsetY, MAX_STR, '|');
		LoadFile.getline(szCount, MAX_STR, '|');
		LoadFile.getline(szMirror, MAX_STR, '|');
		LoadFile.getline(szFrameDelay, MAX_STR, '|');
		LoadFile.getline(szBlend, MAX_STR, '|');
		LoadFile.getline(szEraseColorColor, MAX_STR, '|');
		LoadFile.getline(szFilePath, MAX_STR);

		CString strMapKey = szObjKey;
		strMapKey += '|';
		strMapKey += szStateKey;

		auto& iter_find = m_mapTreeItem.find(strMapKey);
		if (iter_find != m_mapTreeItem.end())
			continue;

		DWORD dwBlend = DWORD(_wtol(szBlend));
		DWORD dwEraseColor = DWORD(_wtol(szEraseColorColor));

		pSpriteInfo = new SPRITE_INFO;

		pSpriteInfo->wstrObjKey = szObjKey;
		pSpriteInfo->wstrStateKey = szStateKey;
		pSpriteInfo->iTexType = (CTextureMgr::SINGLE_TEXTURE == int(_wtoi(szTexType))) ? CTextureMgr::SINGLE_TEXTURE : CTextureMgr::MULTI_TEXTURE;
		pSpriteInfo->vOffset.x = float(_wtof(szOffsetX));
		pSpriteInfo->vOffset.y = float(_wtof(szOffsetY));
		pSpriteInfo->iCount = int(_wtoi(szCount));
		pSpriteInfo->iMirrorX = int(_wtoi(szMirror));
		pSpriteInfo->fFrameDelay = float(_wtof(szFrameDelay));
		pSpriteInfo->tBlend = TARGB(D3DCOLOR_GET_A(dwBlend), D3DCOLOR_GET_R(dwBlend), D3DCOLOR_GET_G(dwBlend), D3DCOLOR_GET_B(dwBlend));
		pSpriteInfo->tEraseColor = TARGB(D3DCOLOR_GET_A(dwEraseColor), D3DCOLOR_GET_R(dwEraseColor), D3DCOLOR_GET_G(dwEraseColor), D3DCOLOR_GET_B(dwEraseColor));
		pSpriteInfo->wstrFilePath = szFilePath;

		m_UnitTool.InsertImageTree(*pSpriteInfo, hSingle_Unit, hMulti_Unit);
		m_WordTool.InsertImageTree(*pSpriteInfo, hSingle_Word, hMulti_Word);
		m_EventTool.InsertImageTree(*pSpriteInfo, hSingle_Event, hMulti_Event);

		m_pTextureMgr->LoadTexture(CTextureMgr::TEXTURE_TYPE(pSpriteInfo->iTexType), pSpriteInfo->wstrFilePath, pSpriteInfo->wstrObjKey, pSpriteInfo->wstrStateKey, pSpriteInfo->vOffset.x, pSpriteInfo->vOffset.y, pSpriteInfo->iCount, pSpriteInfo->iMirrorX, pSpriteInfo->fFrameDelay, dwBlend, dwEraseColor);

		m_EventTool.m_pSpriteInfo 
			= m_WordTool.m_pSpriteInfo
			= m_UnitTool.m_pSpriteInfo 
			= m_mapTreeItem.insert({ strMapKey, pSpriteInfo }).first->second;
	}

	LoadFile.close();
}