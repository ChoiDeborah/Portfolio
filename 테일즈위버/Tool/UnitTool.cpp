// UnitTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "UnitTool.h"
#include "afxdialogex.h"

#include "MainDialog.h"
#include "ToolView.h"


// CUnitTool 대화 상자입니다.

IMPLEMENT_DYNAMIC(CUnitTool, CDialog)

CUnitTool::CUnitTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_UNITTOOL, pParent)
{

}

CUnitTool::~CUnitTool()
{
}

// 컨트롤 ID와 변수를 연동시켜주는 함수.
void CUnitTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_TreeCtrlImage);
	DDX_Control(pDX, IDC_SPRITE_PICTURE, m_pPicture);
	DDX_Control(pDX, IDC_TAB1, m_Tab);
}


BEGIN_MESSAGE_MAP(CUnitTool, CDialog)
	ON_WM_PAINT()
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, &CUnitTool::OnTvnSelchangedImage)
	ON_BN_CLICKED(IDC_BUTTON10, &CUnitTool::OnBnClickedSaveData)
	ON_BN_CLICKED(IDC_BUTTON13, &CUnitTool::OnBnClickedLoadData)
	ON_WM_DROPFILES()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CUnitTool::OnTcnSelchangeTab)
END_MESSAGE_MAP()


// 초기화
BOOL CUnitTool::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	// Tab 세팅
	m_Tab.InsertItem(UNITTOOL_TAP::MONSTER_SETTING + 1, _T("MONSTER"));
	m_Tab.InsertItem(UNITTOOL_TAP::ENVIRON_SETTING + 1, _T("ENVIRON"));

	CRect rt;
	m_Tab.GetClientRect(&rt);

	if (NULL == m_MonsterSetting.GetSafeHwnd()) {
		m_MonsterSetting.m_pUnitTool = this;
		m_MonsterSetting.Create(IDD_MONSTERSETTING, &m_Tab); // 다이얼로그 생성.
		m_MonsterSetting.SetWindowPos(NULL, 4, 25, rt.Width() - 10, rt.Height() - 30, SWP_SHOWWINDOW | SWP_NOZORDER);
		m_pWnd = &m_MonsterSetting;
	}

	if (NULL == m_EnvironSetting.GetSafeHwnd()) {
		m_EnvironSetting.m_pUnitTool = this;
		m_EnvironSetting.Create(IDD_ENVIRONSETTING, &m_Tab); // 다이얼로그 생성.
		m_EnvironSetting.SetWindowPos(NULL, 4, 25, rt.Width() - 10, rt.Height() - 30, SWP_SHOWWINDOW | SWP_NOZORDER);
		m_EnvironSetting.ShowWindow(SW_HIDE);
	}

	// 불러오기
	OnBnClickedLoadData();


	// 트리 세팅
	m_hRoot = m_TreeCtrlImage.InsertItem(TEXT("Texture"), 0, 0, TVI_ROOT, TVI_LAST);
	m_TreeCtrlImage.Expand(m_hRoot, TVE_EXPAND);

	UpdateData(FALSE);
	Invalidate(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


// 엔터, ESC 막아주기
BOOL CUnitTool::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	// EditBox 엔터나 ESC 때 종료 막기
	if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN))
		return TRUE;

	return CDialog::PreTranslateMessage(pMsg);
}


// 렌더
void CUnitTool::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CDialog::OnPaint()을(를) 호출하지 마십시오.

	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(m_tObjectInfo.wstrObjKey, m_tObjectInfo.wstrStateKey, m_tFrame.iFrameCur);
	NULL_CHECK(pTexInfo);

	

	m_pDeviceMgr->Render_Begin();
	D3DXMATRIX matScale, matTrans, matWorld;
	float fScaleW = float(VIEWCX) / pTexInfo->tImgInfo.Width;
	float fScaleH = float(VIEWCY) / pTexInfo->tImgInfo.Height;
		
	D3DXMatrixScaling(&matScale, fScaleW * pTexInfo->m_iMirror, fScaleH, 0.f);
	D3DXMatrixTranslation(&matTrans, VIEWCX / 2, VIEWCY / 2, 0.f);
	matWorld = matScale * matTrans;
	m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3{ pTexInfo->tImgInfo.Width * 0.5f, pTexInfo->tImgInfo.Height * 0.5f, 0.f }, nullptr, pTexInfo->dwBlend);
	m_pDeviceMgr->Render_End(m_pPicture.m_hWnd);

	NULL_CHECK(m_pToolView);
	m_pToolView->Invalidate(FALSE);
}

// 프레임 이동
void CUnitTool::FrameMove()
{
	if (0 > m_tFrame.fFrameDelay)
		return;

	m_tFrame.fFrameTime += CTimeMgr::GetInstance()->GetDeltaTime();
	if (m_tFrame.fFrameDelay < m_tFrame.fFrameTime) {
		m_tFrame.fFrameTime = 0.f;
		++m_tFrame.iFrameCur;

		if (m_tFrame.iFrameCur >= m_tFrame.iFrameEnd) {
			m_tFrame.iFrameCur = 0;
		}

		UpdateData(TRUE);
		NULL_CHECK(m_pToolView);
		m_pToolView->Invalidate(FALSE);
	}
}

// 이미지 트리 초기화
void CUnitTool::ResetImageTree()
{
	// 트리 전체 내용 삭제
	if (m_TreeCtrlImage.ItemHasChildren(m_hRoot))
	{
		HTREEITEM hNextItem;
		HTREEITEM hChildItem = m_TreeCtrlImage.GetChildItem(m_hRoot);

		while (hChildItem != NULL)
		{
			hNextItem = m_TreeCtrlImage.GetNextItem(hChildItem, TVGN_NEXT);
			m_TreeCtrlImage.DeleteItem(hChildItem);
			hChildItem = hNextItem;
		}
	}
}

// 이미지 트리 삽입
void CUnitTool::InsertImageTree(SPRITE_INFO & tSpriteInfo, HTREEITEM hSingle, HTREEITEM hMulti)
{
	// 트리에서 부모 아이템 찾기
	bool bIsFind = false;

	HTREEITEM hType = (CTextureMgr::SINGLE_TEXTURE == tSpriteInfo.iTexType) ? hSingle : hMulti;

	HTREEITEM hNextItem;
	HTREEITEM hChildItem = m_TreeCtrlImage.GetChildItem(hType);

	if (NULL != hType && !lstrcmp(tSpriteInfo.wstrObjKey.c_str(), m_TreeCtrlImage.GetItemText(hType))) {
		if (lstrcmp(tSpriteInfo.wstrStateKey.c_str(), m_TreeCtrlImage.GetItemText(hChildItem))) {
			m_TreeCtrlImage.InsertItem(tSpriteInfo.wstrStateKey.c_str(), 0, 0, hType, TVI_LAST);
			bIsFind = true;
		}
		else {
			bIsFind = true;
		}
	}
	else {
		while (hChildItem != NULL)
		{
			hNextItem = m_TreeCtrlImage.GetNextItem(hChildItem, TVGN_NEXT);

			if (!lstrcmp(tSpriteInfo.wstrObjKey.c_str(), m_TreeCtrlImage.GetItemText(hChildItem))) {
				if (lstrcmp(tSpriteInfo.wstrStateKey.c_str(), m_TreeCtrlImage.GetItemText(hNextItem))) {
					m_TreeCtrlImage.InsertItem(tSpriteInfo.wstrStateKey.c_str(), 0, 0, hChildItem, TVI_LAST);
					bIsFind = true;
					break;
				}
				else {
					bIsFind = true;
					break;
				}
			}

			hChildItem = hNextItem;
		}
		if (!bIsFind) {
			switch (tSpriteInfo.iTexType) {
			case CTextureMgr::SINGLE_TEXTURE:
				hNextItem = m_TreeCtrlImage.InsertItem(tSpriteInfo.wstrObjKey.c_str(), 0, 0, hSingle, TVI_LAST);
				m_TreeCtrlImage.InsertItem(tSpriteInfo.wstrStateKey.c_str(), 0, 0, hNextItem, TVI_LAST);
				break;

			case CTextureMgr::MULTI_TEXTURE:
				hNextItem = m_TreeCtrlImage.InsertItem(tSpriteInfo.wstrObjKey.c_str(), 0, 0, hMulti, TVI_LAST);
				m_TreeCtrlImage.InsertItem(tSpriteInfo.wstrStateKey.c_str(), 0, 0, hNextItem, TVI_LAST);
				break;
			}
		}
	}
}


// CUnitTool 메시지 처리기입니다.

// 트리 클릭, 이미지 변경
void CUnitTool::OnTvnSelchangedImage(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
	
	UpdateData(TRUE);

	NULL_CHECK(m_pImageMap);

	HTREEITEM hStateKey		= m_TreeCtrlImage.GetNextItem(NULL, TVGN_CARET);
	HTREEITEM hObjKey		= m_TreeCtrlImage.GetNextItem(hStateKey, TVGN_PARENT);

	wstring wstrObjKey		= m_TreeCtrlImage.GetItemText(hObjKey);
	wstring wstrStateKey	= m_TreeCtrlImage.GetItemText(hStateKey);
	wstring wstrCmp			= wstrObjKey + L"|" + wstrStateKey;

	auto iter_find = m_pImageMap->find(wstrCmp.c_str());
	if (iter_find != m_pImageMap->end()) {
		const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(iter_find->second->wstrObjKey, iter_find->second->wstrStateKey);
		NULL_CHECK(pTexInfo);

		m_tObjectInfo.wstrObjKey	= wstrObjKey;
		m_tObjectInfo.wstrStateKey	= wstrStateKey;

		if (0 == m_Tab.GetCurSel())
		{
			m_MonsterSetting.m_tUnitInfo.wstrObjKey = m_tObjectInfo.wstrObjKey;
			m_MonsterSetting.m_tUnitInfo.wstrStateKey = m_tObjectInfo.wstrStateKey;

			m_MonsterSetting.m_tUnitInfo.vCollStart = { -pTexInfo->vOffset.x, -pTexInfo->vOffset.y, 0.f };
			m_MonsterSetting.m_tUnitInfo.vCollEnd = { pTexInfo->tImgInfo.Width - pTexInfo->vOffset.x, pTexInfo->tImgInfo.Height - pTexInfo->vOffset.y, 0.f };

			m_MonsterSetting.UpdateData(FALSE);
		}

		// Frame
		m_tFrame.iFrameCur = 0;
		m_tFrame.iFrameEnd = m_pTextureMgr->GetTexCount(wstrObjKey, wstrStateKey);
		m_tFrame.fFrameDelay = pTexInfo->m_fFrameDelay;
	}

	UpdateData(FALSE);
	Invalidate(FALSE);
}

// 오브젝트 리스트 저장 버튼
void CUnitTool::OnBnClickedSaveData()
{
	UpdateData(TRUE);

	CString strPathName;
	//CString str = _T("All files(*.*)|*.*|");
	//CFileDialog dlg(TRUE, _T("*.dat"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, str, this);
	//if (dlg.DoModal() == IDOK)
	//{
	//	strPathName = dlg.GetPathName();
	//	// 파일 경로를 가져와 사용할 경우, Edit Control에 값 저장
	//	//SetDlgItemText(IDC_EDIT1, strPathName);
	//	//LoadData(strPathName.GetString());
	//}

	strPathName = L"../Data/ObjData/ObjData.txt";

	wofstream SaveFile;
	//SaveFile.open(L"../Data/ObjData/ObjData.txt", ios::out);
	SaveFile.open(strPathName.GetString(), ios::out);

	if (!SaveFile.good())
		return;

	// Monster Setting SaveData();
	m_MonsterSetting.SaveData(SaveFile);
	m_EnvironSetting.SaveData(SaveFile);

	// NPC Setting SaveData();

	SaveFile.close();

	UpdateData(FALSE);
	Invalidate(FALSE);

	m_pToolView->m_pMainDialog->m_MapTool.LoadObjData();
	m_pToolView->m_pMainDialog->m_MapTool.UpdateData(FALSE);
	m_pToolView->m_pMainDialog->m_MapTool.Invalidate(FALSE);
}

// 오브젝트 리스트 불러오기 버튼
void CUnitTool::OnBnClickedLoadData()
{
	//// 파일 이름 얻기
	//CString str = _T("All files(*.*)|*.*|");
	//CFileDialog dlg(TRUE, _T("*.dat"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, str, this);
	//if (dlg.DoModal() == IDOK)
	//{
	//	CString strPathName = dlg.GetPathName();
	//	// 파일 경로를 가져와 사용할 경우, Edit Control에 값 저장
	//	//SetDlgItemText(IDC_EDIT1, strPathName);
	//	LoadData(strPathName.GetString());
	//}

	CString strPathName = L"../Data/ObjData/ObjData.txt";
	wifstream LoadFile;
	LoadFile.open(strPathName, ios::in);

	if (!LoadFile.good())
		return;

	m_MonsterSetting.LoadData(LoadFile);
	m_EnvironSetting.LoadData(LoadFile);

	LoadFile.close();

	UpdateData(FALSE);
	Invalidate(FALSE);
}

// 1. 파일이름만 남기기
// 2. 피킹되게 만들기
// 3. 세이브 로드

void CUnitTool::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialog::OnDropFiles(hDropInfo);

	// hDropInfo : 드래그 된 대상들의 기준이 되는 핸들
	// ::DragQueryFile(hDropInfo, index, FileName(out), BufferLength);
	// 드래그 핸들, 드래그 대상들의 인덱스(좌->우 방향), 저장할 주소, 버퍼 사이즈

	//// 두 번째 인자가 -1일 경우 드래그 앤 드롭된 대상의 전체 개수를 반환
	//int iCount = ::DragQueryFile(hDropInfo, -1, nullptr, 0);

	//TCHAR szFilePath[MAX_STR] = L"";
	//for (int i = 0; i < iCount; ++i) {
	//	::DragQueryFile(hDropInfo, i, szFilePath, MAX_STR);
	//	m_ListBoxItemIndex.AddString(szFilePath);
	//}
}

void CUnitTool::OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	if (nullptr != m_pWnd) {
		m_pWnd->ShowWindow(SW_HIDE);
		m_pWnd = nullptr;
	}

	switch (m_Tab.GetCurSel()) {
	case MONSTER_SETTING:
		m_MonsterSetting.ShowWindow(SW_SHOW);
		m_pWnd = &m_MonsterSetting;
		break;

	case ENVIRON_SETTING:
		m_EnvironSetting.ShowWindow(SW_SHOW);
		m_pWnd = &m_EnvironSetting;
		break;
	}
}
