// EventTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "afxdialogex.h"
#include "EventTool.h"

#include "ToolView.h"
#include "AddMap.h"
#include "MainDialog.h"
#include "Terrain.h"

#include "EventObj.h"

// CEventTool 대화 상자입니다.

IMPLEMENT_DYNAMIC(CEventTool, CDialog)

CEventTool::CEventTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_EVENTTOOL, pParent)
	, m_strObjName(_T(""))
	, m_bIsViewObj(0)
{
}

CEventTool::~CEventTool()
{
	Release();
	SafeDelete(m_pAddMap);
}

void CEventTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST3, m_MapDataListBox);
	DDX_Control(pDX, IDC_BUTTON17, m_ButtonPlay);
	DDX_Control(pDX, IDC_LIST1, m_ListBoxObject);
	DDX_Control(pDX, IDC_LIST9, m_ListBoxEventPath);
	DDX_Control(pDX, IDC_TREE1, m_TreeCtrlImage);


	DDX_Text(pDX, IDC_EDIT1, m_tCurTime.fFrameDelay);
	DDX_Text(pDX, IDC_CURTIME, m_tCurTime.fFrameTime);
	DDX_Text(pDX, IDC_EDIT3, m_tCurNotify.fTime);
	DDX_Text(pDX, IDC_EDIT4, m_tCurNotify.vPos.x);
	DDX_Text(pDX, IDC_EDIT5, m_tCurNotify.vPos.y);

	CString strObjKey = m_tCurNotify.wstrObjKey.c_str();
	DDX_Text(pDX, IDC_OBJKEY, strObjKey);
	m_tCurNotify.wstrObjKey = strObjKey;

	CString strStateKey = m_tCurNotify.wstrStateKey.c_str();
	DDX_Text(pDX, IDC_STATEKEY, strStateKey);
	m_tCurNotify.wstrStateKey = strStateKey;

	BOOL bIsMove = m_tCurNotify.bIsMove;
	DDX_Check(pDX, IDC_CHECK1, bIsMove);
	m_tCurNotify.bIsMove = (bIsMove) ? true : false;

	BOOL bIsActive = m_tCurNotify.bIsActive;
	DDX_Check(pDX, IDC_CHECK6, bIsActive);
	m_tCurNotify.bIsActive = (bIsActive) ? true : false;
	DDX_Control(pDX, IDC_SPRITE_PICTURE, m_Picture);
	DDX_Control(pDX, IDC_CHECK7, m_CheckPathRender);
	DDX_Control(pDX, IDC_TIMESLIDER, m_TimeSlider);

	DDX_Text(pDX, IDC_EDIT6, m_strObjName);
	DDX_Check(pDX, IDC_CHECK8, m_bIsViewObj);
}


void CEventTool::Release()
{
	for (auto& EventObj : m_vecEventObj)
		SafeDelete(EventObj);

	m_vecEventObj.clear();
	m_vecEventObj.shrink_to_fit();
}


BEGIN_MESSAGE_MAP(CEventTool, CDialog)
	ON_WM_PAINT()

	/*ON_BN_CLICKED(IDC_BUTTON1, &CEventTool::OnBnClickedAddMap)
	ON_BN_CLICKED(IDC_BUTTON2, &CEventTool::OnBnClickedRemoveMap)

	ON_LBN_SELCHANGE(IDC_LIST3, &CEventTool::OnLbnSelchangeMap)*/
	ON_BN_CLICKED(IDC_BUTTON4, &CEventTool::OnBnClickedUpdate)
	ON_BN_CLICKED(IDC_BUTTON17, &CEventTool::OnBnClickedPlay)
	ON_LBN_SELCHANGE(IDC_LIST1, &CEventTool::OnLbnSelchangeEventObject)
	ON_LBN_SELCHANGE(IDC_LIST9, &CEventTool::OnLbnSelchangeEventPath)
	ON_BN_CLICKED(IDC_BUTTON5, &CEventTool::OnBnClickedNotifyUpdate)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, &CEventTool::OnTvnSelchangedImage)
	ON_BN_CLICKED(IDC_BUTTON16, &CEventTool::OnBnClickedAddObject)
	ON_BN_CLICKED(IDC_BUTTON8, &CEventTool::OnBnClickedDeleteObject)
	ON_BN_CLICKED(IDC_BUTTON12, &CEventTool::OnBnClickedAddNotify)
	ON_BN_CLICKED(IDC_BUTTON6, &CEventTool::OnBnClickedDeleteNotify)
	ON_BN_CLICKED(IDC_CHECK1, &CEventTool::OnBnClickedCheckIsMoving)
	ON_BN_CLICKED(IDC_CHECK6, &CEventTool::OnBnClickedCheckActive)
	ON_BN_CLICKED(IDC_BUTTON1, &CEventTool::OnBnClickedCreateScene)
	ON_LBN_SELCHANGE(IDC_LIST3, &CEventTool::OnLbnSelchangeMap)
	ON_BN_CLICKED(IDC_BUTTON3, &CEventTool::OnBnClickedSaveScene)
	ON_BN_CLICKED(IDC_BUTTON9, &CEventTool::OnBnClickedNameSet)
	ON_BN_CLICKED(IDC_CHECK8, &CEventTool::OnBnClickedIsViewObj)
END_MESSAGE_MAP()


// CEventTool 메시지 처리기입니다.
// 초기화
BOOL CEventTool::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_pAddMap = new CAddMap;
	if (NULL == m_pAddMap->GetSafeHwnd()) {
		m_pAddMap->Create(IDD_ADDMAP, this);
		m_pAddMap->SetMainDialog(m_pToolView->m_pMainDialog);
		m_pAddMap->SetEventTool(this);
	}

	// 시간 초기화
	m_tCurTime.fFrameTime	= 0.f;
	m_tCurTime.fFrameDelay	= 30.f;

	// 오브젝트 초기화
	CEventObj* pEventObj = new CEventObj(new EVENT_NOTIFY({ 700.f, 700.f, 0.f }, L"Boss", L"Walk_RD", 0.f, true));
	pEventObj->InsertNotify(new EVENT_NOTIFY({ 900.f, 700.f, 0.f }, L"Boss", L"Walk_RD", 2.f, true));
	pEventObj->InsertNotify(new EVENT_NOTIFY({ 1100.f, 1000.f, 0.f }, L"Boss", L"Attack_RD", 4.f));
	pEventObj->InsertNotify(new EVENT_NOTIFY({ 1100.f, 1000.f, 0.f }, L"Boss", L"Walk_RD", 6.f, true));
	pEventObj->InsertNotify(new EVENT_NOTIFY({ 1300.f, 900.f, 0.f }, L"Boss", L"Idle_RD", 8.f));
	m_vecEventObj.push_back(pEventObj);
	m_ListBoxObject.AddString(L"Boss");	// 리스트에 삽입
	
	// 트리 세팅
	m_hRoot = m_TreeCtrlImage.InsertItem(TEXT("Texture"), 0, 0, TVI_ROOT, TVI_LAST);
	m_TreeCtrlImage.Expand(m_hRoot, TVE_EXPAND);


	// 맵 데이터 로드
	wstring wstrFilePath = L"../Data/EventData/";
	wstrFilePath += L"/*.*";
	CFileFind finder;

	BOOL bWorking = finder.FindFile(wstrFilePath.c_str());
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		if (finder.IsDots())
		{
			continue;
		}
		wstrFilePath = finder.GetFileTitle();
		m_MapDataListBox.InsertString(-1, finder.GetFileTitle());
	}

	m_MapDataListBox.SetCurSel(int(m_MapDataListBox.GetCount() - 1));
	LoadMap(wstrFilePath);

	finder.Close();
	
	UpdateData(FALSE);
	Invalidate(FALSE);
	return TRUE;
}


void CEventTool::Update()
{
	UpdateData(TRUE);
	// 그림 좌클릭
	if (m_pKeyMgr->KeyPressing(VK_LBUTTON))
	{
		POINT point = {};
		GetCursorPos(&point);
		::ScreenToClient(m_TimeSlider.m_hWnd, &point);

		CRect rt;
		m_TimeSlider.GetClientRect(&rt);

		if (0 <= point.x && 0 <= point.y &&
			rt.right >= point.x && rt.bottom >= point.y) {

			m_tCurTime.fFrameTime = m_tCurTime.fFrameDelay * float(point.x) / float(rt.right);

			for (auto& EventObj : m_vecEventObj)
			{
				EventObj->ResetNotify();
				EventObj->UpdateNotify(m_tCurTime.fFrameTime);
			}
		}
	}

	UpdateData(FALSE);
	Invalidate(FALSE);

	FrameMove();
}


// 렌더
void CEventTool::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: Add your message handler code here
					   // Do not call CDialog::OnPaint() for painting messages

	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(m_tCurNotify.wstrObjKey, m_tCurNotify.wstrStateKey, 0);
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
	m_pDeviceMgr->Render_End(m_Picture.m_hWnd);

	NULL_CHECK(m_pToolView);
	m_pToolView->Invalidate(FALSE);
}


// 이미지 트리 초기화
void CEventTool::ResetImageTree()
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
void CEventTool::InsertImageTree(SPRITE_INFO & tSpriteInfo, HTREEITEM hSingle, HTREEITEM hMulti)
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

// 맵 데이터 삽입
void CEventTool::InsertMapData(MAP_DATA * pMapData)
{
	if (nullptr == pMapData)
		return;

	m_MapDataListBox.InsertString(-1, pMapData->wstrMapName.c_str());
	m_MapDataListBox.SetCurSel(int(m_MapDataListBox.GetCount() - 1));
	SafeDelete(m_pMapData);
	m_pMapData = pMapData;

	SaveMap(pMapData->wstrMapName);

	UpdateMapData();
}

// 맵 업데이트
void CEventTool::UpdateMapData()
{
	const TEX_INFO*	pTexInfo = m_pTextureMgr->GetTexInfo(L"BackGround", L"Map", m_pMapData->iMapImageNum);
	NULL_CHECK(pTexInfo);

	CViewMgr::GetInstance()->SetRoomSize({ float(pTexInfo->tImgInfo.Width), float(pTexInfo->tImgInfo.Height), 0.f });
	CViewMgr::GetInstance()->SetViewObj({ float(pTexInfo->tImgInfo.Width) * 0.5f, float(pTexInfo->tImgInfo.Height) * 0.5f, 0.f });

	/*m_pView->SetScrollSize(m_pBackInfo->tImgInfo.Width, m_pBackInfo->tImgInfo.Height);
	m_pView->SetScrollPos(0, (m_pBackInfo->tImgInfo.Width - VIEWCX) / 2);
	m_pView->SetScrollPos(1, (m_pBackInfo->tImgInfo.Height - VIEWCY) / 2);*/

	UpdateData(FALSE);
	Invalidate(FALSE);
}

// 씬 저장
HRESULT CEventTool::SaveMap(const wstring & wstrFileName)
{
	// 저장해야 할 것
	// 각 맵 따로 파일 저장 :: 맵 이미지, 타일X, 타일 Y, 각 타일 DrawID, Option, 각 오브젝트 위치, 오브젝트ID

	wstring wstrFilePath = L"../Data/EventData/";
	wstrFilePath += wstrFileName;
	wstrFilePath += L".txt";

	wofstream SaveFile;
	SaveFile.open(wstrFilePath, ios::out);

	if (!SaveFile.good()) {
		FAILED_CHECK_MSG_RETURN(E_FAIL, L"Save Failed", E_FAIL);
	}
	
	// 맵 이미지 번호
	SaveFile << m_pMapData->iMapImageNum << '\n';
	
	// 씬 시간
	SaveFile << m_tCurTime.fFrameDelay << '\n';

	// 각 오브젝트의 Notify Path 저장
	size_t iMax = m_vecEventObj.size();
	SaveFile << iMax; // 오브젝트 개수
	if (0 < iMax)
		SaveFile << '\n';

	for (size_t i=0; i< iMax; ++i)
	{
		if (nullptr == m_vecEventObj[i])
			continue;

		// 오브젝트 이름
		SaveFile
			<< m_vecEventObj[i]->GetName() << '|'
			<< m_vecEventObj[i]->GetViewObj() << '|';

		// 노티파이 개수
		vector<EVENT_NOTIFY*>* pVecNotify = m_vecEventObj[i]->GetVecNotify();
		size_t iMaxNotify = pVecNotify->size();
		SaveFile << iMaxNotify << '\n';
		for (size_t j = 0; j < iMaxNotify; ++j)
		{
			// 노티파이 저장 : 시간 위치X, Y, 오브젝트키, 상태키, 무빙, 액티브
			SaveFile
				<< (*(pVecNotify))[j]->fTime << '|'
				<< (*(pVecNotify))[j]->vPos.x << '|'
				<< (*(pVecNotify))[j]->vPos.y << '|'
				<< (*(pVecNotify))[j]->wstrObjKey << '|'
				<< (*(pVecNotify))[j]->wstrStateKey << '|'
				<< (((*(pVecNotify))[j]->bIsMove) ? 1 : 0) << '|'
				<< (((*(pVecNotify))[j]->bIsActive) ? 1 : 0);

			if (iMaxNotify - 1 > j)
				SaveFile << '\n';
		}
		if (iMax - 1 > i)
			SaveFile << '\n';
	}

	SaveFile.close();
	return S_OK;
}

// 씬 로드
HRESULT CEventTool::LoadMap(const wstring & wstrFileName)
{
	wstring wstrFilePath = L"../Data/EventData/";
	wstrFilePath += wstrFileName;
	wstrFilePath += L".txt";

	wifstream LoadFile;
	LoadFile.open(wstrFilePath, ios::in);

	if (!LoadFile.good()) {
		FAILED_CHECK_MSG_RETURN(E_FAIL, L"Load Failed", E_FAIL);
	}

	SafeDelete(m_pMapData);


	// 삭제
	for (auto& EventObj : m_vecEventObj) {
		SafeDelete(EventObj);
	}
	m_vecEventObj.clear();
	m_vecEventObj.shrink_to_fit();

	m_ListBoxObject.ResetContent();
	m_ListBoxEventPath.ResetContent();

	m_pMapData = new MAP_DATA;
	m_pMapData->wstrMapName = wstrFileName;
	
	// 맵 이미지 번호
	TCHAR szMapImageNum[MAX_STR] = L"";
	LoadFile.getline(szMapImageNum, MAX_STR);
	m_pMapData->iMapImageNum = int(_wtoi(szMapImageNum));
	

	// 씬 시간
	TCHAR szSceneMaxTime[MAX_STR] = L"";
	LoadFile.getline(szSceneMaxTime, MAX_STR);
	m_tCurTime.fFrameDelay = float(_wtof(szSceneMaxTime));


	// 오브젝트 개수
	TCHAR szObjNum[MAX_STR] = L"";
	LoadFile.getline(szObjNum, MAX_STR);
	int iObjNum = int(_wtoi(szObjNum));

	m_vecEventObj.reserve(iObjNum);

	// 각 오브젝트 wstrObjKey, wstrStateKey, vPos.x, vPos.y, vPos.z, vScale.x, vScale.y, vScale.z
	
	TCHAR szNotifyNum[MAX_STR] = L"";

	TCHAR szObjName[MAX_STR] = L"";
	TCHAR szIsViewObj[MAX_STR] = L"";

	TCHAR szTime[MAX_STR] = L"";
	TCHAR szX[MAX_STR] = L"";
	TCHAR szY[MAX_STR] = L"";
	TCHAR szObjKey[MAX_STR] = L"";
	TCHAR szStateKey[MAX_STR] = L"";
	TCHAR szMoving[MAX_STR] = L"";
	TCHAR szActive[MAX_STR] = L"";

	// 오브젝트
	for (int i = 0; i < iObjNum; ++i)
	{
		CEventObj* pEventObj = nullptr;
		
		// 오브젝트 이름
		LoadFile.getline(szObjName, MAX_STR, '|');
		LoadFile.getline(szIsViewObj, MAX_STR, '|');

		// 노티파이 개수
		LoadFile.getline(szNotifyNum, MAX_STR);
		int iNotifyNum = int(_ttoi(szNotifyNum));
		for (int j = 0; j < iNotifyNum; ++j)
		{
			LoadFile.getline(szTime, MAX_STR, '|');
			LoadFile.getline(szX, MAX_STR, '|');
			LoadFile.getline(szY, MAX_STR, '|');
			LoadFile.getline(szObjKey, MAX_STR, '|');
			LoadFile.getline(szStateKey, MAX_STR, '|');
			LoadFile.getline(szMoving, MAX_STR, '|');
			LoadFile.getline(szActive, MAX_STR);

			bool bIsMove = (1 == _ttoi(szMoving)) ? true : false;
			bool bIsActive = (1 == _ttoi(szActive)) ? true : false;

			// 노티파이 저장 : 시간 위치X, Y, 오브젝트키, 상태키, 무빙, 액티브
			if (0 == j)
			{
				pEventObj = new CEventObj(new EVENT_NOTIFY({float(_ttof(szX)), float(_ttof(szY)), 0.f }, szObjKey, szStateKey, float(_ttof(szTime)), bIsMove, bIsActive));

				pEventObj->SetName(szObjName);
				pEventObj->SetViewObj(int(_wtoi(szIsViewObj)));

				m_ListBoxObject.InsertString(i, szObjName);	// 리스트에 삽입

				m_vecEventObj.push_back(pEventObj);
			}
			else
			{
				if (nullptr == pEventObj)
					break;

				pEventObj->InsertNotify(new EVENT_NOTIFY({ float(_ttof(szX)), float(_ttof(szY)), 0.f }, szObjKey, szStateKey, float(_ttof(szTime)), bIsMove, bIsActive));
			}
		}
	}
	LoadFile.close();

	return S_OK;
}


// 프레임 이동
void CEventTool::FrameMove()
{
	if (!m_bIsFrameMove || 0.f > m_tCurTime.fFrameDelay)
		return;

	UpdateData(TRUE);

	m_tCurTime.fFrameTime += CTimeMgr::GetInstance()->GetDeltaTime();
	if (m_tCurTime.fFrameDelay < m_tCurTime.fFrameTime) {
		m_tCurTime.fFrameTime = 0.f;
		++m_tCurTime.iFrameCur;

		if (m_tCurTime.iFrameCur >= m_tCurTime.iFrameEnd) {
			m_tCurTime.iFrameCur = 0;
			for (auto& EventObj : m_vecEventObj)
				EventObj->ResetNotify();
		}
	}

	for (auto& Notify : m_vecEventObj)
		Notify->UpdateNotify(m_tCurTime.fFrameTime);
	
	UpdateData(FALSE);
	NULL_CHECK(m_pToolView);
	Invalidate(FALSE);
}

// 시간 업데이트 버튼
void CEventTool::OnBnClickedUpdate()
{
	UpdateData(TRUE);
	
	UpdateData(FALSE);
	Invalidate(FALSE);
}

// 플레이 버튼
void CEventTool::OnBnClickedPlay()
{
	UpdateData(TRUE);

	if (m_bIsFrameMove ^= true)
		m_ButtonPlay.SetWindowText(L"||");
	else
		m_ButtonPlay.SetWindowText(L"▶");

	UpdateData(FALSE);
	Invalidate(FALSE);
}

// 리스트에서 오브젝트 선택
void CEventTool::OnLbnSelchangeEventObject()
{
	UpdateData(TRUE);

	int iIndex = m_ListBoxObject.GetCurSel();
	if (0 > iIndex || int(m_vecEventObj.size()) <= iIndex)
	{
		m_pEventObj = nullptr;
		m_pSeleteNotify = nullptr;
		return;
	}

	CEventObj* pEventObj = m_vecEventObj[iIndex];
	NULL_CHECK(pEventObj);
	m_pEventObj = pEventObj;
	m_ListBoxEventPath.ResetContent();
	vector<EVENT_NOTIFY*>* pVecEventPath = pEventObj->GetVecNotify();
	NULL_CHECK(pVecEventPath);
	
	TCHAR szTime[MID_STR] = L"";
	TCHAR szPosX[MID_STR] = L"";
	TCHAR szPosY[MID_STR] = L"";
	

	for (auto& Path : (*pVecEventPath))
	{
		swprintf_s(szTime, L"%.2f", Path->fTime);
		swprintf_s(szPosX, L"%.2f", Path->vPos.x);
		swprintf_s(szPosY, L"%.2f", Path->vPos.y);
		m_ListBoxEventPath.InsertString(-1, (wstring(szTime) + L"초 | (" + szPosX + L", " + szPosY + L")" + Path->wstrObjKey + L"|" + Path->wstrStateKey).c_str());
	}

	if (0 < pVecEventPath->size())
	{
		m_ListBoxEventPath.SetCurSel(0);
		m_pSeleteNotify = (*pVecEventPath)[0];
		m_tCurNotify = *m_pSeleteNotify;
	}

	m_strObjName = m_pEventObj->GetName().c_str();
	m_bIsViewObj = m_pEventObj->GetViewObj();

	UpdateData(FALSE);
	Invalidate(FALSE);
}

// 리스트에서 노티파이 선택
void CEventTool::OnLbnSelchangeEventPath()
{
	UpdateData(TRUE);

	int iIndex = m_ListBoxObject.GetCurSel();
	if (0 > iIndex || int(m_vecEventObj.size()) <= iIndex)
	{
		m_pEventObj = nullptr;
		m_pSeleteNotify = nullptr;
		return;
	}

	vector<EVENT_NOTIFY*>* pVecEventPath = m_vecEventObj[iIndex]->GetVecNotify();
	iIndex = m_ListBoxEventPath.GetCurSel();
	if (0 > iIndex || int(pVecEventPath->size()) <= iIndex)
	{
		m_pSeleteNotify = nullptr;
		return;
	}

	m_pSeleteNotify = (*(pVecEventPath))[m_ListBoxEventPath.GetCurSel()];
	m_tCurNotify = *m_pSeleteNotify;

	UpdateData(FALSE);
	Invalidate(FALSE);
}

// 버튼 노티파이 업데이트
void CEventTool::OnBnClickedNotifyUpdate()
{
	UpdateData(TRUE);

	NULL_CHECK(m_pEventObj);
	NULL_CHECK(m_pSeleteNotify);
	
	int iIndex = m_ListBoxObject.GetCurSel();
	if (0 > iIndex || int(m_vecEventObj.size()) <= iIndex)
	{
		m_pEventObj = nullptr;
		m_pSeleteNotify = nullptr;
		return;
	}

	vector<EVENT_NOTIFY*>* pVecEventPath = m_vecEventObj[iIndex]->GetVecNotify();
	iIndex = m_ListBoxEventPath.GetCurSel();
	if (0 > iIndex || int(pVecEventPath->size()) <= iIndex)
	{
		m_pSeleteNotify = nullptr;
		return;
	}


	*m_pSeleteNotify = m_tCurNotify;

	m_ListBoxEventPath.DeleteString(iIndex);

	TCHAR szTime[MID_STR] = L"";
	TCHAR szPosX[MID_STR] = L"";
	TCHAR szPosY[MID_STR] = L"";
	swprintf_s(szTime, L"%.2f", m_pSeleteNotify->fTime);
	swprintf_s(szPosX, L"%.2f", m_pSeleteNotify->vPos.x);
	swprintf_s(szPosY, L"%.2f", m_pSeleteNotify->vPos.y);
	m_ListBoxEventPath.InsertString(iIndex, (wstring(szTime) + L"초 | (" + szPosX + L", " + szPosY + L")" + m_pSeleteNotify->wstrObjKey + L"|" + m_pSeleteNotify->wstrStateKey).c_str());

	m_ListBoxEventPath.SetCurSel(iIndex);


	UpdateData(FALSE);
	Invalidate(FALSE);
}


// 트리 이미지 선택
void CEventTool::OnTvnSelchangedImage(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	UpdateData(TRUE);

	NULL_CHECK(m_pImageMap);

	HTREEITEM hStateKey = m_TreeCtrlImage.GetNextItem(NULL, TVGN_CARET);
	HTREEITEM hObjKey = m_TreeCtrlImage.GetNextItem(hStateKey, TVGN_PARENT);

	wstring wstrObjKey = m_TreeCtrlImage.GetItemText(hObjKey);
	wstring wstrStateKey = m_TreeCtrlImage.GetItemText(hStateKey);
	wstring wstrCmp = wstrObjKey + L"|" + wstrStateKey;

	auto iter_find = m_pImageMap->find(wstrCmp.c_str());
	if (iter_find != m_pImageMap->end()) {
		const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(iter_find->second->wstrObjKey, iter_find->second->wstrStateKey);
		NULL_CHECK(pTexInfo);

		m_tCurNotify.wstrObjKey = wstrObjKey;
		m_tCurNotify.wstrStateKey = wstrStateKey;
	}

	UpdateData(FALSE);
	Invalidate(FALSE);
}

// 오브젝트 추가
void CEventTool::OnBnClickedAddObject()
{
	UpdateData(TRUE);

	// 오브젝트 초기화
	CEventObj* pEventObj = new CEventObj(new EVENT_NOTIFY({ m_pToolView->m_pViewMgr->GetViewX(), m_pToolView->m_pViewMgr->GetViewY(), 0.f }, L"", L"", 0.f, true));
	m_vecEventObj.push_back(pEventObj);
	m_ListBoxObject.AddString(L"New");	// 리스트에 삽입
	m_ListBoxObject.SetCurSel(int(m_vecEventObj.size()) - 1);

	m_pEventObj = pEventObj;
	m_pEventObj->SetName(L"New");

	m_ListBoxEventPath.SetCurSel(0);
	m_pSeleteNotify = (*m_pEventObj->GetVecNotify())[0];
	m_tCurNotify = *m_pSeleteNotify;

	UpdateData(FALSE);
	Invalidate(FALSE);
}

// 오브젝트 삭제
void CEventTool::OnBnClickedDeleteObject()
{
	UpdateData(TRUE);

	int iIndex = m_ListBoxObject.GetCurSel();
	if (0 > iIndex || int(m_vecEventObj.size()) <= iIndex)
		return;

	auto iter_erase = m_vecEventObj.begin() + iIndex;
	m_vecEventObj.erase(iter_erase);
	m_vecEventObj.shrink_to_fit();
	m_ListBoxObject.DeleteString(iIndex);

	if (0 < int(m_vecEventObj.size()))
	{
		if (0 < iIndex)
		{
			m_ListBoxObject.SetCurSel(iIndex - 1);
			m_pEventObj = m_vecEventObj[iIndex - 1];
		}
		else
		{
			m_ListBoxObject.SetCurSel(0);
			m_pEventObj = m_vecEventObj[0];
		}

		m_ListBoxEventPath.SetCurSel(0);
		m_pSeleteNotify = (*m_pEventObj->GetVecNotify())[0];
		m_tCurNotify = *m_pSeleteNotify;
	}

	UpdateData(FALSE);
	Invalidate(FALSE);
}

// 노티파이 추가
void CEventTool::OnBnClickedAddNotify()
{
	UpdateData(TRUE);

	int iIndex = m_ListBoxObject.GetCurSel();
	if (0 > iIndex || int(m_vecEventObj.size()) <= iIndex)
	{
		m_pEventObj = nullptr;
		m_pSeleteNotify = nullptr;
		return;
	}

	vector<EVENT_NOTIFY*>* pVecEventPath = m_vecEventObj[iIndex]->GetVecNotify();
	m_vecEventObj[iIndex]->InsertNotify(new EVENT_NOTIFY(*((*pVecEventPath)[pVecEventPath->size()-1])));

	int iCurIndex = int(pVecEventPath->size()) - 1;


	TCHAR szTime[MID_STR] = L"";
	TCHAR szPosX[MID_STR] = L"";
	TCHAR szPosY[MID_STR] = L"";

	swprintf_s(szTime, L"%.2f", (*pVecEventPath)[iCurIndex]->fTime);
	swprintf_s(szPosX, L"%.2f", (*pVecEventPath)[iCurIndex]->vPos.x);
	swprintf_s(szPosY, L"%.2f", (*pVecEventPath)[iCurIndex]->vPos.y);
	m_ListBoxEventPath.InsertString(-1, (wstring(szTime) + L"초 | (" + szPosX + L", " + szPosY + L")" + (*pVecEventPath)[iCurIndex]->wstrObjKey + L"|" + (*pVecEventPath)[iCurIndex]->wstrStateKey).c_str());

	m_ListBoxEventPath.SetCurSel(iCurIndex);

	if (0 > iCurIndex)
	{
		m_pSeleteNotify = nullptr;
		return;
	}

	m_pSeleteNotify = (*(m_vecEventObj[iIndex]->GetVecNotify()))[iCurIndex];
	m_tCurNotify = *m_pSeleteNotify;

	UpdateData(FALSE);
	Invalidate(FALSE);
}

// 노티파이 삭제
void CEventTool::OnBnClickedDeleteNotify()
{
	UpdateData(TRUE);

	int iIndex = m_ListBoxObject.GetCurSel();
	if (0 > iIndex || int(m_vecEventObj.size()) <= iIndex)
	{
		m_pSeleteNotify = nullptr;
		return;
	}

	vector<EVENT_NOTIFY*>* pVecEventPath = m_vecEventObj[iIndex]->GetVecNotify(); m_vecEventObj[iIndex]->GetVecNotify();

	iIndex = m_ListBoxEventPath.GetCurSel();

	if (0 > iIndex || int((*pVecEventPath).size()) <= iIndex)
	{
		return;
	}

	auto iter_erase = pVecEventPath->begin() + iIndex;
	pVecEventPath->erase(iter_erase);
	pVecEventPath->shrink_to_fit();
	m_ListBoxEventPath.DeleteString(iIndex);

	if (0 < iIndex)
	{
		m_pSeleteNotify = (*(pVecEventPath))[iIndex-1];
	}
	else
	{
		m_pSeleteNotify = (*(pVecEventPath))[0];
	}

	m_tCurNotify = *m_pSeleteNotify;

	UpdateData(FALSE);
	Invalidate(FALSE);
}

// 무빙 체크 버튼
void CEventTool::OnBnClickedCheckIsMoving()
{
	UpdateData(TRUE);
	
	UpdateData(FALSE);
	Invalidate(FALSE);
}

// 활성화 체크 버튼
void CEventTool::OnBnClickedCheckActive()
{
	UpdateData(TRUE);

	UpdateData(FALSE);
	Invalidate(FALSE);
}

// 씬 추가
void CEventTool::OnBnClickedCreateScene()
{
	m_pAddMap->ShowWindow(SW_SHOW);
}

// 씬 선택
void CEventTool::OnLbnSelchangeMap()
{
	UpdateData(TRUE);

	CString strName;
	m_MapDataListBox.GetText(m_MapDataListBox.GetCurSel(), strName);
	LoadMap(wstring(strName));

	UpdateData(FALSE);
	Invalidate(FALSE);
}

// 씬 저장 버튼 클릭
void CEventTool::OnBnClickedSaveScene()
{
	UpdateData(TRUE);

	CString strName;
	m_MapDataListBox.GetText(m_MapDataListBox.GetCurSel(), strName);
	SaveMap(wstring(strName));

	UpdateData(FALSE);
	Invalidate(FALSE);
}

// 오브젝트 이름 설정 버튼 클릭
void CEventTool::OnBnClickedNameSet()
{
	UpdateData(TRUE);

	int iIndex = m_ListBoxObject.GetCurSel();
	if (0 > iIndex || int(m_vecEventObj.size()) <= iIndex)
	{
		m_pSeleteNotify = nullptr;
		return;
	}

	NULL_CHECK(m_pEventObj);

	m_pEventObj->SetName(wstring(m_strObjName));
	m_pEventObj->SetViewObj(m_bIsViewObj);

	m_ListBoxObject.DeleteString(iIndex);
	m_ListBoxObject.InsertString(iIndex, m_strObjName);	

	UpdateData(FALSE);
	Invalidate(FALSE);
}


void CEventTool::OnBnClickedIsViewObj()
{
	UpdateData(TRUE);
	UpdateData(FALSE);
	Invalidate(FALSE);
}
