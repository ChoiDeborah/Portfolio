// MapTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "afxdialogex.h"
#include "MapTool.h"

#include "ToolView.h"
#include "AddMap.h"
#include "MainDialog.h"
#include "Terrain.h"



// CMapTool 대화 상자입니다.

IMPLEMENT_DYNAMIC(CMapTool, CDialog)

CMapTool::CMapTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_MAPTOOL, pParent)
	, m_pMapTexture(nullptr)
	, m_strTileSize(_T(""))
	, m_iRadio(0)
	, m_strMapSize(_T(""))
{
	
}

CMapTool::~CMapTool()
{
	for (auto& Unit : m_mapUnit) {
		SafeDelete(Unit.second);
	}
	m_mapUnit.clear();

	SafeDelete(m_pMapData);

	SafeDelete(m_pAddMap);
}

void CMapTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_MAPSIZE, m_strTileSize);
	DDX_Control(pDX, IDC_LIST3, m_MapDataListBox);
	DDX_Control(pDX, IDC_TILE_PICTURE, m_Picture);
	DDX_Text(pDX, IDC_MAP_SIZE, m_strMapSize);
	DDX_Control(pDX, IDC_TREE2, m_TreeCtrlObj);
	DDX_Radio(pDX, IDC_RADIO2, m_iRadio);
	DDX_Control(pDX, IDC_LIST1, m_ListBoxTile);
	DDX_Control(pDX, IDC_CHECK1, m_ButtonDrawLine);
	DDX_Control(pDX, IDC_LIST7, m_ListBoxObject);
	DDX_Control(pDX, IDC_CHECK2, m_ButtonIsDark);
}


BEGIN_MESSAGE_MAP(CMapTool, CDialog)
	ON_WM_PAINT()

	ON_BN_CLICKED(IDC_BUTTON1, &CMapTool::OnBnClickedAddMap)
	ON_BN_CLICKED(IDC_BUTTON2, &CMapTool::OnBnClickedRemoveMap)

	ON_LBN_SELCHANGE(IDC_LIST3, &CMapTool::OnLbnSelchangeMap)
	ON_BN_CLICKED(IDC_BUTTON5, &CMapTool::OnBnClickedInitTile)
	ON_BN_CLICKED(IDC_BUTTON4, &CMapTool::OnBnClickedInitObj)
	ON_BN_CLICKED(IDC_BUTTON3, &CMapTool::OnBnClickedSave)
	ON_BN_CLICKED(IDC_RADIO1, &CMapTool::OnBnClickedRadioTile)
	ON_BN_CLICKED(IDC_RADIO2, &CMapTool::OnBnClickedRadioObj)
	
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE2, &CMapTool::OnTvnSelchangedObject)
	ON_LBN_SELCHANGE(IDC_LIST1, &CMapTool::OnLbnSelchangeListTile)
	ON_BN_CLICKED(IDC_CHECK1, &CMapTool::OnBnClickedDrawLine)
	ON_NOTIFY(BCN_HOTITEMCHANGE, IDC_CHECK1, &CMapTool::OnBnHotItemChangeCheck1)
	ON_LBN_SELCHANGE(IDC_LIST7, &CMapTool::OnLbnSelchangeListObject)
	ON_BN_CLICKED(IDC_BUTTON6, &CMapTool::OnBnClickedDeleteObject)
	ON_BN_CLICKED(IDC_CHECK2, &CMapTool::OnBnClickedIsDark)
END_MESSAGE_MAP()


// 초기화
BOOL CMapTool::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	m_pAddMap = new CAddMap;
	if (NULL == m_pAddMap->GetSafeHwnd()) {
		m_pAddMap->Create(IDD_ADDMAP, this);
		m_pAddMap->SetMainDialog(m_pToolView->m_pMainDialog);
		m_pAddMap->SetMapTool(this);
	}

	m_hRoot = m_TreeCtrlObj.InsertItem(TEXT("Object"), 0, 0, TVI_ROOT, TVI_LAST);
	LoadObjData();
	m_TreeCtrlObj.Expand(m_hRoot, TVE_EXPAND);
	
	// 맵 데이터 로드
	wstring wstrFilePath = L"../Data/MapData/";
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

	// 타일 추가
	m_ListBoxTile.InsertString(-1, L"0");
	m_ListBoxTile.InsertString(-1, L"1");
	m_ListBoxTile.InsertString(-1, L"2");
	m_ListBoxTile.InsertString(-1, L"3");
	m_ListBoxTile.InsertString(-1, L"4"); 
	
	m_ListBoxTile.SetCurSel(0);

	UpdateMapData();

	UpdateData(FALSE);
	Invalidate(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


// 렌더
void CMapTool::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: Add your message handler code here
					   // Do not call CDialog::OnPaint() for painting messages


	m_pDeviceMgr->Render_Begin();
	if (nullptr == m_pSelectObj) {
		m_pDeviceMgr->Render_End(m_Picture.m_hWnd);
		m_pToolView->Invalidate(FALSE);
		return;
	}

	D3DXMATRIX matScale, matTrans, matWorld;

	float fScale = 1.f;
	if (m_pSelectObj->tImgInfo.Width <= m_pSelectObj->tImgInfo.Height)
		fScale = float(VIEWCY) / m_pSelectObj->tImgInfo.Height * 0.5f;
	else
		fScale = float(VIEWCX) / m_pSelectObj->tImgInfo.Width * 0.5f;

	D3DXMatrixScaling(&matScale, fScale * m_pSelectObj->m_iMirror, fScale, 0.f);
	D3DXMatrixTranslation(&matTrans, VIEWCX / 2, VIEWCY / 2, 0.f);
	matWorld = matScale * matTrans;
	m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
	m_pDeviceMgr->GetSprite()->Draw(m_pSelectObj->pTexture, nullptr,
		&m_pSelectObj->vOffset, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	m_pDeviceMgr->Render_End(m_Picture.m_hWnd);
	m_pToolView->Invalidate(FALSE);
}


// 맵 데이터 추가
void CMapTool::InsertMapData(MAP_DATA* pMapData)
{
	if (nullptr == pMapData)
		return;

	m_MapDataListBox.InsertString(-1, pMapData->wstrMapName.c_str());
	m_MapDataListBox.SetCurSel(int(m_MapDataListBox.GetCount() - 1));
	SafeDelete(m_pMapData);
	m_pMapData = pMapData;

	SaveMap(pMapData->wstrMapName);

	//LoadMap(pMapData->wstrMapName);

	UpdateMapData();
}

// 맵 정보 업데이트
void CMapTool::UpdateMapData()
{
	UpdateData(TRUE);

	m_strTileSize = to_string(m_pMapData->iTileX).c_str();
	m_strTileSize += " x ";
	m_strTileSize += to_string(m_pMapData->iTileY).c_str();

	const TEX_INFO*	pTexInfo = m_pTextureMgr->GetTexInfo(L"BackGround", L"Map", m_pMapData->iMapImageNum);
	NULL_CHECK(pTexInfo);

	m_strMapSize = to_string(pTexInfo->tImgInfo.Width).c_str();
	m_strMapSize += " x ";
	m_strMapSize += to_string(pTexInfo->tImgInfo.Height).c_str();

	CViewMgr::GetInstance()->SetRoomSize({ float(pTexInfo->tImgInfo.Width), float(pTexInfo->tImgInfo.Height), 0.f });
	CViewMgr::GetInstance()->SetViewObj({ float(pTexInfo->tImgInfo.Width) * 0.5f, float(pTexInfo->tImgInfo.Height) * 0.5f, 0.f });

	/*m_pView->SetScrollSize(m_pBackInfo->tImgInfo.Width, m_pBackInfo->tImgInfo.Height);
	m_pView->SetScrollPos(0, (m_pBackInfo->tImgInfo.Width - VIEWCX) / 2);
	m_pView->SetScrollPos(1, (m_pBackInfo->tImgInfo.Height - VIEWCY) / 2);*/
	
	UpdateData(FALSE);
	Invalidate(FALSE);
}

// 맵 저장
HRESULT CMapTool::SaveMap(const wstring & wstrFileName)
{
	// 저장해야 할 것
	// 각 맵 따로 파일 저장 :: 맵 이미지, 타일X, 타일 Y, 각 타일 DrawID, Option, 각 오브젝트 위치, 오브젝트ID

	wstring wstrFilePath = L"../Data/MapData/";
	wstrFilePath += wstrFileName;
	wstrFilePath += L".txt";

	wofstream SaveFile;
	SaveFile.open(wstrFilePath, ios::out);

	if (!SaveFile.good()) {
		FAILED_CHECK_MSG_RETURN(E_FAIL, L"Save Failed", E_FAIL);
	}

	// 맵 이미지 번호
	SaveFile << m_pMapData->iMapImageNum << '\n';

	// 타일X, 타일Y
	SaveFile << m_pMapData->iTileX << '|' << m_pMapData->iTileY << '\n';

	// 각 타일 DrawID, Option
	size_t iMax = m_pMapData->vecTile.size();
	for (size_t i = 0; i < iMax; ++i)
	{
		SaveFile 
			<< m_pMapData->vecTile[i]->byDrawID << '|' 
			<< m_pMapData->vecTile[i]->byOption;

		SaveFile << '\n';
	}

	// 각 오브젝트 wstrObjKey, wstrStateKey, vPos.x, vPos.y, vPos.z, vScale.x, vScale.y, vScale.z
	iMax = m_pMapData->vecObj.size();
	SaveFile << iMax; // 오브젝트 개수
	if (0 < iMax)
		SaveFile << '\n';
	for (size_t i = 0; i < iMax; ++i) {
		SaveFile 
			<< m_pMapData->vecObj[i]->byOption		<< '|'
			<< m_pMapData->vecObj[i]->wstrName		<< '|'
			<< m_pMapData->vecObj[i]->wstrObjKey	<< '|' 
			<< m_pMapData->vecObj[i]->wstrStateKey	<< '|'
			<< m_pMapData->vecObj[i]->vPos.x		<< '|' 
			<< m_pMapData->vecObj[i]->vPos.y		<< '|' 
			<< m_pMapData->vecObj[i]->vPos.z		<< '|'
			<< m_pMapData->vecObj[i]->vSize.x		<< '|'
			<< m_pMapData->vecObj[i]->vSize.y		<< '|'
			<< m_pMapData->vecObj[i]->vSize.z;

		if (iMax - 1 > i)
			SaveFile << '\n';
	}

	//m_ListBoxObject.ResetContent();
	SaveFile.close();
	return S_OK;
}

// 맵 로드
HRESULT CMapTool::LoadMap(const wstring & wstrFileName)
{
	wstring wstrFilePath = L"../Data/MapData/";
	wstrFilePath += wstrFileName;
	wstrFilePath += L".txt";

	wifstream LoadFile;
	LoadFile.open(wstrFilePath, ios::in);

	if (!LoadFile.good()) {
		FAILED_CHECK_MSG_RETURN(E_FAIL, L"Load Failed", E_FAIL);
	}

	SafeDelete(m_pMapData);
	m_ListBoxObject.ResetContent();

	m_pMapData = new MAP_DATA;
	m_pMapData->wstrMapName = wstrFileName;

	// 맵 이미지 번호
	TCHAR szMapImageNum[MAX_STR] = L"";
	LoadFile.getline(szMapImageNum, MAX_STR);
	m_pMapData->iMapImageNum = int(_wtoi(szMapImageNum));

	// 타일X, 타일Y
	TCHAR szTileX[MAX_STR] = L"";
	TCHAR szTileY[MAX_STR] = L"";
	LoadFile.getline(szTileX, MAX_STR, '|');
	LoadFile.getline(szTileY, MAX_STR);
	m_pMapData->iTileX = int(_wtoi(szTileX));
	m_pMapData->iTileY = int(_wtoi(szTileY));

	// 각 타일 DrawID, Option
	TCHAR szDrawID[MAX_STR] = L"";
	TCHAR szOption[MAX_STR] = L"";
	TILE_INFO* pTile = nullptr;
	for (int i = 0; i < m_pMapData->iTileY; ++i)
	{
		for (int j = 0; j < m_pMapData->iTileX; ++j)
		{
			float fX = j * TILECX + (i % 2) * (TILECX * 0.5f);
			float fY = i * (TILECY * 0.5f);

			pTile = new TILE_INFO;
			pTile->vPos = { fX, fY, 0.f };
			pTile->vSize = { 1.f, 1.f, 0.f };

			LoadFile.getline(szDrawID, MAX_STR, '|');
			LoadFile.getline(szOption, MAX_STR);

			pTile->byDrawID = int(_wtoi(szDrawID));
			pTile->byOption = int(_wtoi(szOption));

			m_pMapData->vecTile.push_back(pTile);
		}
	}

	// 오브젝트 개수
	TCHAR szObjNum[MAX_STR] = L"";
	LoadFile.getline(szObjNum, MAX_STR);
	int iObjNum = int(_wtoi(szObjNum));

	// 각 오브젝트 wstrName, wstrObjKey, wstrStateKey, vPos.x, vPos.y, vPos.z, vScale.x, vScale.y, vScale.z
	TCHAR szObjID[MAX_STR] = L"";
	TCHAR szName[MAX_STR] = L"";
	TCHAR szObjKey[MAX_STR] = L"";
	TCHAR szStateKey[MAX_STR] = L"";
	TCHAR szPosX[MAX_STR] = L"";
	TCHAR szPosY[MAX_STR] = L"";
	TCHAR szPosZ[MAX_STR] = L"";
	TCHAR szScaleX[MAX_STR] = L"";
	TCHAR szScaleY[MAX_STR] = L"";
	TCHAR szScaleZ[MAX_STR] = L"";

	TCHAR szX[MAX_STR] = L"";
	TCHAR szY[MAX_STR] = L"";
	for (int i = 0; i < iObjNum; ++i)
	{
		pTile = new TILE_INFO;

		LoadFile.getline(szObjID, MAX_STR, '|');
		LoadFile.getline(szName, MAX_STR, '|');
		LoadFile.getline(szObjKey, MAX_STR, '|');
		LoadFile.getline(szStateKey, MAX_STR, '|');
		LoadFile.getline(szPosX, MAX_STR, '|');
		LoadFile.getline(szPosY, MAX_STR, '|');
		LoadFile.getline(szPosZ, MAX_STR, '|');
		LoadFile.getline(szScaleX, MAX_STR, '|');
		LoadFile.getline(szScaleY, MAX_STR, '|');
		LoadFile.getline(szScaleZ, MAX_STR);

		pTile->byOption		= int(_wtoi(szObjID));
		pTile->wstrName		= szName;
		pTile->wstrObjKey	= szObjKey;
		pTile->wstrStateKey = szStateKey;
		pTile->vPos			= D3DXVECTOR3(float(_wtof(szPosX)), float(_wtof(szPosY)), float(_wtof(szPosZ)));
		pTile->vSize		= D3DXVECTOR3(float(_wtof(szScaleX)), float(_wtof(szScaleY)), float(_wtof(szScaleZ)));

		swprintf_s(szPosX, L"\t | (%.2f, %.2f)", pTile->vPos.x, pTile->vPos.y);

		m_pMapData->vecObj.push_back(pTile);
		m_ListBoxObject.AddString((pTile->wstrObjKey + szPosX).c_str());
	}
	LoadFile.close();
	return S_OK;
}

// 오브젝트 데이터 로드
void CMapTool::LoadObjData()
{
	for (auto& Unit : m_mapUnit)
		SafeDelete(Unit.second);
	m_mapUnit.clear();

	// 트리 전체 내용 삭제
	if (m_TreeCtrlObj.ItemHasChildren(m_hRoot))
	{
		HTREEITEM hNextItem;
		HTREEITEM hChildItem = m_TreeCtrlObj.GetChildItem(m_hRoot);

		while (hChildItem != NULL)
		{
			hNextItem = m_TreeCtrlObj.GetNextItem(hChildItem, TVGN_NEXT);
			m_TreeCtrlObj.DeleteItem(hChildItem);
			hChildItem = hNextItem;
		}
	}

	HTREEITEM hMonster = m_TreeCtrlObj.InsertItem(L"Monster", 0, 0, m_hRoot, TVI_LAST);
	HTREEITEM hEnviron = m_TreeCtrlObj.InsertItem(L"Environ", 0, 0, m_hRoot, TVI_LAST);

	TILE_INFO* pUnitInfo = nullptr;

	map<CString, UNIT_INFO*>* mapMonster = &m_pToolView->m_pMainDialog->m_UnitTool.m_MonsterSetting.m_mapListObj;
	for (auto Obj : *mapMonster)
	{
		pUnitInfo = new TILE_INFO;

		pUnitInfo->wstrName		= Obj.second->wstrName;
		pUnitInfo->wstrObjKey	= Obj.second->wstrObjKey;
		pUnitInfo->wstrStateKey = Obj.second->wstrStateKey;
		pUnitInfo->byOption		= Obj.second->iObjID;

		m_mapUnit.insert({ pUnitInfo->wstrName.c_str(), pUnitInfo });

		if (lstrcmp(pUnitInfo->wstrObjKey.c_str(), m_TreeCtrlObj.GetItemText(m_hRoot))) {
			m_TreeCtrlObj.InsertItem(pUnitInfo->wstrName.c_str(), 0, 0, hMonster, TVI_LAST);
		}
	}

	vector<TILE_INFO*>* vecEnviron = &m_pToolView->m_pMainDialog->m_UnitTool.m_EnvironSetting.m_vecListObj;
	for (auto Obj : *vecEnviron)
	{
		pUnitInfo = new TILE_INFO;

		pUnitInfo->wstrName		= Obj->wstrObjKey;
		pUnitInfo->wstrObjKey	= Obj->wstrObjKey;
		pUnitInfo->wstrStateKey = Obj->wstrStateKey;
		pUnitInfo->byOption		= OBJ_ENVIRONMENT;

		m_mapUnit.insert({ pUnitInfo->wstrName.c_str(), pUnitInfo });

		if (lstrcmp(pUnitInfo->wstrObjKey.c_str(), m_TreeCtrlObj.GetItemText(m_hRoot))) {
			m_TreeCtrlObj.InsertItem(pUnitInfo->wstrName.c_str(), 0, 0, hEnviron, TVI_LAST);
		}
	}
}


// CMapTool 메시지 처리기입니다.

// 맵 추가 버튼
void CMapTool::OnBnClickedAddMap()
{
	m_pAddMap->ShowWindow(SW_SHOW);
}

// 맵 삭제 버튼
void CMapTool::OnBnClickedRemoveMap()
{
	UpdateData(TRUE);
	wstring wstrFilePath = L"../Data/MapData/";
	CString strFileName;
	m_MapDataListBox.GetText(m_MapDataListBox.GetCurSel(), strFileName);
	wstrFilePath += strFileName;
	wstrFilePath += L".txt";

	CFileFind finder;
	BOOL bWorking = finder.FindFile(wstrFilePath.c_str());
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		if (finder.IsDots())
			continue;

		DeleteFile(wstrFilePath.c_str());
	}
	finder.Close();

	m_MapDataListBox.DeleteString(m_MapDataListBox.GetCurSel());

	UpdateData(FALSE);
	Invalidate(FALSE);
}

// 맵 리스트 선택
void CMapTool::OnLbnSelchangeMap()
{
	// 맵 크기 변경
	/*m_pMapData = m_vecMapData[m_MapDataListBox.GetCurSel()];

	if (nullptr == m_pMapData)
		return;*/

	CString strPath;
	m_MapDataListBox.GetText(m_MapDataListBox.GetCurSel(), strPath);

	wstring wstrFilePath = strPath;
	LoadMap(wstrFilePath);

	UpdateMapData();

	UpdateData(FALSE);
	Invalidate(FALSE);
}

// 타일 초기화 버튼
void CMapTool::OnBnClickedInitTile()
{
	UpdateData(TRUE);
	for (auto& Tile : m_pMapData->vecTile) {
		Tile->byDrawID = 0;
		Tile->byOption = 0;
	}

	m_ListBoxObject.ResetContent();

	UpdateData(FALSE);
	Invalidate(FALSE);
}

// 오브젝트 초기화 버튼
void CMapTool::OnBnClickedInitObj()
{
	m_pMapData->vecObj.clear();
	m_pMapData->vecObj.shrink_to_fit();

	m_ListBoxObject.ResetContent();

	UpdateData(FALSE);
	Invalidate(FALSE);
}

// 저장 버튼
void CMapTool::OnBnClickedSave()
{
	// 맵 전체 저장
	SaveMap(m_pMapData->wstrMapName.c_str());
}

// 타일 라디오 클릭
void CMapTool::OnBnClickedRadioTile()
{
	UpdateData(TRUE);
	UpdateData(FALSE);
	Invalidate(FALSE);
}

// 오브젝트 라디오 클릭
void CMapTool::OnBnClickedRadioObj()
{
	UpdateData(TRUE);
	UpdateData(FALSE);
	Invalidate(FALSE);
}

// 트리 오브젝트 선택
void CMapTool::OnTvnSelchangedObject(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	UpdateData(TRUE);

	m_iRadio = 0;

	HTREEITEM hName = m_TreeCtrlObj.GetNextItem(NULL, TVGN_CARET);

	wstring wstrName = m_TreeCtrlObj.GetItemText(hName);

	auto iter_find = m_mapUnit.find(wstrName.c_str());
	if (iter_find != m_mapUnit.end()) {
		const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(iter_find->second->wstrObjKey, iter_find->second->wstrStateKey);
		NULL_CHECK(pTexInfo);

		m_pSelectObj = pTexInfo;
		m_pUnitInfo = iter_find->second;
	}

	UpdateData(FALSE);
	Invalidate(FALSE);
}


// 리스트에서 타일 선택
void CMapTool::OnLbnSelchangeListTile()
{
	UpdateData(TRUE);
	m_iRadio = 1;
	m_pSelectObj = m_pTextureMgr->GetTexInfo(L"BackGround", L"TileOption", m_ListBoxTile.GetCurSel());

	UpdateData(FALSE);
	Invalidate(FALSE);
}

// 라인 그리기 체크박스
void CMapTool::OnBnClickedDrawLine()
{
	UpdateData(TRUE);
	UpdateData(FALSE);
	Invalidate(FALSE);
}


void CMapTool::OnBnHotItemChangeCheck1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// 이 기능을 사용하려면 Internet Explorer 6 이상이 필요합니다.
	// _WIN32_IE 기호는 0x0600보다 크거나 같아야 합니다.
	LPNMBCHOTITEM pHotItem = reinterpret_cast<LPNMBCHOTITEM>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	UpdateData(TRUE);
	UpdateData(FALSE);
	Invalidate(FALSE);
}


void CMapTool::OnLbnSelchangeListObject()
{
	UpdateData(TRUE);
	UpdateData(FALSE);
	Invalidate(FALSE);
}


void CMapTool::OnBnClickedDeleteObject()
{
	UpdateData(TRUE);

	auto iter_delete = m_pMapData->vecObj.begin() + m_ListBoxObject.GetCurSel();
	m_pMapData->vecObj.erase(iter_delete);
	m_ListBoxObject.DeleteString(m_ListBoxObject.GetCurSel());

	UpdateData(FALSE);
	Invalidate(FALSE);
}

void CMapTool::OnBnClickedIsDark()
{
	UpdateData(TRUE);
	UpdateData(FALSE);
	Invalidate(FALSE);
}
