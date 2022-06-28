// MonsterSetting.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "MonsterSetting.h"
#include "afxdialogex.h"

#include "UnitTool.h"


// CMonsterSetting 대화 상자입니다.

IMPLEMENT_DYNAMIC(CMonsterSetting, CDialog)

CMonsterSetting::CMonsterSetting(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_MONSTERSETTING, pParent)
{

}

CMonsterSetting::~CMonsterSetting()
{
	for (auto& ListItem : m_mapListObj) {
		SafeDelete(ListItem.second);
	}
	m_mapListObj.clear();
}

void CMonsterSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	CString strName = m_tUnitInfo.wstrName.c_str();
	DDX_Text(pDX, IDC_EDIT5, strName);
	m_tUnitInfo.wstrName = strName;
	DDX_Text(pDX, IDC_EDIT10, m_tUnitInfo.iLevel);
	DDX_Text(pDX, IDC_EDIT22, m_tUnitInfo.iExp);
	DDX_Text(pDX, IDC_EDIT7, m_tUnitInfo.iHp);
	DDX_Text(pDX, IDC_EDIT8, m_tUnitInfo.iMp);
	DDX_Text(pDX, IDC_EDIT11, m_tUnitInfo.iSp);
	DDX_Text(pDX, IDC_EDIT19, m_tUnitInfo.fFindDst);
	DDX_Text(pDX, IDC_EDIT12, m_tUnitInfo.tStatInfo.iStab);
	DDX_Text(pDX, IDC_EDIT13, m_tUnitInfo.tStatInfo.iHack);
	DDX_Text(pDX, IDC_EDIT14, m_tUnitInfo.tStatInfo.iInt);
	DDX_Text(pDX, IDC_EDIT15, m_tUnitInfo.tStatInfo.iDex);
	DDX_Text(pDX, IDC_EDIT16, m_tUnitInfo.tStatInfo.iDef);
	DDX_Text(pDX, IDC_EDIT17, m_tUnitInfo.tStatInfo.iMr);
	DDX_Text(pDX, IDC_EDIT18, m_tUnitInfo.tStatInfo.iAgi);
	DDX_Control(pDX, IDC_COMBO1, m_ComboObjID);
	DDX_Control(pDX, IDC_LIST4, m_ListCtrlObject);
	DDX_Text(pDX, IDC_EDIT6, m_tUnitInfo.vCollStart.x);
	DDX_Text(pDX, IDC_EDIT9, m_tUnitInfo.vCollStart.y);
	DDX_Text(pDX, IDC_EDIT20, m_tUnitInfo.vCollEnd.x);
	DDX_Text(pDX, IDC_EDIT21, m_tUnitInfo.vCollEnd.y);
	DDX_Control(pDX, IDC_CHECK5, m_CheckShowCollRect);
	DDX_Text(pDX, IDC_EDIT23, m_tUnitInfo.tStatInfo.iLightning);
	DDX_Text(pDX, IDC_EDIT24, m_tUnitInfo.tStatInfo.iFire);
	DDX_Text(pDX, IDC_EDIT25, m_tUnitInfo.tStatInfo.iEarth);
	DDX_Text(pDX, IDC_EDIT26, m_tUnitInfo.tStatInfo.iLight);
	DDX_Text(pDX, IDC_EDIT27, m_tUnitInfo.tStatInfo.iIce);
	DDX_Text(pDX, IDC_EDIT28, m_tUnitInfo.tStatInfo.iWind);
	DDX_Text(pDX, IDC_EDIT29, m_tUnitInfo.tStatInfo.iDark);
	DDX_Text(pDX, IDC_EDIT30, m_tUnitInfo.tStatInfo.iNormal);
}


BEGIN_MESSAGE_MAP(CMonsterSetting, CDialog)
	ON_BN_CLICKED(IDC_CHECK3, &CMonsterSetting::OnBnClickedIsFirstAttack)
	ON_BN_CLICKED(IDC_BUTTON8, &CMonsterSetting::OnBnClickedUpdateSetting)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CMonsterSetting::OnCbnSelchangeComboObjID)
	ON_BN_CLICKED(IDC_BUTTON7, &CMonsterSetting::OnBnClickedInsertObject)
	ON_BN_CLICKED(IDC_BUTTON9, &CMonsterSetting::OnBnClickedDeleteObject)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST4, &CMonsterSetting::OnLvnItemchangedObject)
	ON_BN_CLICKED(IDC_BUTTON16, &CMonsterSetting::OnBnClickedAutoCollBox)
END_MESSAGE_MAP()


// CMonsterSetting 메시지 처리기입니다.
BOOL CMonsterSetting::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	// 리스트 컨트롤 세팅
	CRect rt;
	m_ListCtrlObject.GetWindowRect(&rt);
	m_ListCtrlObject.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);	// 리스트 컨트롤에 선 표시 및 Item 선택 시 한행 전체 선택

	m_ListCtrlObject.InsertColumn(0, TEXT("Name"), LVCFMT_CENTER, int(rt.Width() * 0.95f));

	// 콤보 박스 세팅
	m_ComboObjID.InsertString(-1, _T("OBJ_ENVIRONMENT"));
	m_ComboObjID.InsertString(-1, _T("OBJ_COLLISION"));
	m_ComboObjID.InsertString(-1, _T("OBJ_EVENT"));
	m_ComboObjID.InsertString(-1, _T("OBJ_PLAYER"));
	m_ComboObjID.InsertString(-1, _T("OBJ_MONSTER"));
	m_ComboObjID.InsertString(-1, _T("OBJ_NPC"));
	m_ComboObjID.InsertString(-1, _T("OBJ_BUDDY"));
	m_ComboObjID.InsertString(-1, _T("OBJ_GATE"));
	m_ComboObjID.InsertString(-1, _T("OBJ_CREATOR"));
	m_ComboObjID.InsertString(-1, _T("OBJ_ITEM"));
	

	m_CheckShowCollRect.SetCheck(TRUE);

	m_tUnitInfo.iObjID = OBJ_MONSTER;
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

// 엔터 막기
BOOL CMonsterSetting::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	// EditBox 엔터나 ESC 때 종료 막기
	if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN))
		return TRUE;

	return CDialog::PreTranslateMessage(pMsg);
}

// 저장 데이터 글자 조합
wstring CMonsterSetting::SetListData(UNIT_INFO * pUnitInfo)
{
	TCHAR szFindDst[MAX_STR];
	swprintf_s(szFindDst, L"%.2f", pUnitInfo->fFindDst);

	wstring wstrData = pUnitInfo->wstrName + L"|";
	wstrData += pUnitInfo->wstrObjKey + L"|";
	wstrData += pUnitInfo->wstrStateKey + L"|";

	wstrData += to_wstring(pUnitInfo->iObjID) + L"|";

	wstrData += to_wstring(pUnitInfo->iLevel) + L"|";
	wstrData += to_wstring(pUnitInfo->iExp) + L"|";

	wstrData += to_wstring(pUnitInfo->iHp) + L"|";
	wstrData += to_wstring(pUnitInfo->iMp) + L"|";
	wstrData += to_wstring(pUnitInfo->iSp) + L"|";

	wstrData += to_wstring(pUnitInfo->tStatInfo.iStab) + L"|";
	wstrData += to_wstring(pUnitInfo->tStatInfo.iHack) + L"|";
	wstrData += to_wstring(pUnitInfo->tStatInfo.iInt) + L"|";
	wstrData += to_wstring(pUnitInfo->tStatInfo.iDex) + L"|";
	wstrData += to_wstring(pUnitInfo->tStatInfo.iDef) + L"|";
	wstrData += to_wstring(pUnitInfo->tStatInfo.iMr) + L"|";
	wstrData += to_wstring(pUnitInfo->tStatInfo.iAgi) + L"|";

	wstrData += to_wstring(pUnitInfo->tStatInfo.iLightning) + L"|";
	wstrData += to_wstring(pUnitInfo->tStatInfo.iFire) + L"|";
	wstrData += to_wstring(pUnitInfo->tStatInfo.iEarth) + L"|";
	wstrData += to_wstring(pUnitInfo->tStatInfo.iLight) + L"|";

	wstrData += to_wstring(pUnitInfo->tStatInfo.iIce) + L"|";
	wstrData += to_wstring(pUnitInfo->tStatInfo.iWind) + L"|";
	wstrData += to_wstring(pUnitInfo->tStatInfo.iDark) + L"|";
	wstrData += to_wstring(pUnitInfo->tStatInfo.iNormal) + L"|";

	wstrData += to_wstring(pUnitInfo->iIsFirstAtt) + L"|";
	wstrData += wstring(szFindDst) + L"|";
	
	wstrData += to_wstring(pUnitInfo->vCollStart.x) + L"|";
	wstrData += to_wstring(pUnitInfo->vCollStart.y) + L"|";
	wstrData += to_wstring(pUnitInfo->vCollEnd.x) + L"|";
	wstrData += to_wstring(pUnitInfo->vCollEnd.y);

	return wstrData;
}

// 오브젝트 저장
void CMonsterSetting::SaveData(wofstream & SaveFile)
{
	size_t iMax = m_mapListObj.size();
	size_t iCount = 0;

	// 개수
	SaveFile << m_mapListObj.size() << '\n';

	// 데이터 저장
	for (auto& iter : m_mapListObj) {
		++iCount;
		wstring wstrData = SetListData(iter.second);

		SaveFile << wstrData << '\n';
	}
}

// 오브젝트 로드
void CMonsterSetting::LoadData(wifstream& LoadFile)
{
	for (auto& ListItem : m_mapListObj)
		SafeDelete(ListItem.second);
	m_mapListObj.clear();

	m_ListCtrlObject.DeleteAllItems();

	TCHAR szCount[MAX_STR] = L"";
	LoadFile.getline(szCount, MAX_STR);
	int iCount = int(_wtoi(szCount));
	if (0 >= iCount)
		return;

	TCHAR szName[MAX_STR] = L"";	// 이름
	TCHAR szObjKey[MAX_STR] = L"";	// 텍스쳐 오브젝트 키
	TCHAR szStateKey[MAX_STR] = L"";	// 텍스쳐 상태 키

	TCHAR szObjID[MAX_STR] = L"";	// 오브젝트 아이디

	TCHAR szLevel[MAX_STR] = L"";	// 레벨
	TCHAR szExp[MAX_STR] = L"";	// 경험치

	TCHAR szHP[MAX_STR] = L"";	// 체력
	TCHAR szMP[MAX_STR] = L"";	// 마나
	TCHAR szSP[MAX_STR] = L"";	// 스테미너

	TCHAR szSTAB[MAX_STR] = L"";	// 찌르기
	TCHAR szHACK[MAX_STR] = L"";	// 베기
	TCHAR szINT[MAX_STR] = L"";	// 마법 공격
	TCHAR szDEX[MAX_STR] = L"";	// 명중
	TCHAR szDEF[MAX_STR] = L"";	// 물리 방어
	TCHAR szMR[MAX_STR] = L"";	// 마법 방어
	TCHAR szAGI[MAX_STR] = L"";	// 회피율

	TCHAR szLightning[MAX_STR] = L"";	// 번개 속성
	TCHAR szFire[MAX_STR] = L"";	// 불 속성
	TCHAR szEarth[MAX_STR] = L"";	// 땅 속성
	TCHAR szLight[MAX_STR] = L"";	// 빛 속성

	TCHAR szIce[MAX_STR] = L"";	// 얼음 속성
	TCHAR szWind[MAX_STR] = L"";	// 바람 속성
	TCHAR szDark[MAX_STR] = L"";	// 암흑 속성
	TCHAR szNormal[MAX_STR] = L"";	// 무 속성

	TCHAR szIsFirst[MAX_STR] = L"";	// 선공 / 후공

	TCHAR szFindDst[MAX_STR] = L"";	// 플레이어 발견 거리

	TCHAR szCollLeft[MAX_STR] = L"";	// 충돌렉트 Left
	TCHAR szCollTop[MAX_STR] = L"";	// 충돌렉트 Top
	TCHAR szCollRight[MAX_STR] = L"";	// 충돌렉트 Right
	TCHAR szCollBottom[MAX_STR] = L"";	// 충돌렉트 Bottom

	UNIT_INFO tUnitInfo;
	for (int i = 0; i < iCount; ++i) {
		LoadFile.getline(szName, MAX_STR, '|');	// 이름
		LoadFile.getline(szObjKey, MAX_STR, '|');	// 텍스쳐 오브젝트 키
		LoadFile.getline(szStateKey, MAX_STR, '|');	// 텍스쳐 상태 키

		LoadFile.getline(szObjID, MAX_STR, '|');	// 오브젝트 아이디

		LoadFile.getline(szLevel, MAX_STR, '|');	// 레벨
		LoadFile.getline(szExp, MAX_STR, '|');	// 경험치

		LoadFile.getline(szHP, MAX_STR, '|');	// 체력
		LoadFile.getline(szMP, MAX_STR, '|');	// 마나
		LoadFile.getline(szSP, MAX_STR, '|');	// 스테미너

		LoadFile.getline(szSTAB, MAX_STR, '|');	// 찌르기
		LoadFile.getline(szHACK, MAX_STR, '|');	// 베기
		LoadFile.getline(szINT, MAX_STR, '|');	// 마법 공격
		LoadFile.getline(szDEX, MAX_STR, '|');	// 명중
		LoadFile.getline(szDEF, MAX_STR, '|');	// 물리 방어
		LoadFile.getline(szMR, MAX_STR, '|');	// 마법 방어
		LoadFile.getline(szAGI, MAX_STR, '|');	// 회피율

		LoadFile.getline(szLightning, MAX_STR, '|');	// 찌르기
		LoadFile.getline(szFire, MAX_STR, '|');	// 베기
		LoadFile.getline(szEarth, MAX_STR, '|');	// 마법 공격
		LoadFile.getline(szLight, MAX_STR, '|');	// 명중

		LoadFile.getline(szIce, MAX_STR, '|');	// 물리 방어
		LoadFile.getline(szWind, MAX_STR, '|');	// 마법 방어
		LoadFile.getline(szDark, MAX_STR, '|');	// 회피율
		LoadFile.getline(szNormal, MAX_STR, '|');	// 회피율

		LoadFile.getline(szIsFirst, MAX_STR, '|');	// 선공 / 후공

		LoadFile.getline(szFindDst, MAX_STR, '|');	// 적 발견 거리

		LoadFile.getline(szCollLeft, MAX_STR, '|');	// 충돌렉트 Left
		LoadFile.getline(szCollTop, MAX_STR, '|');	// 충돌렉트 Top
		LoadFile.getline(szCollRight, MAX_STR, '|');	// 충돌렉트 Right
		LoadFile.getline(szCollBottom, MAX_STR);		// 충돌렉트 Bottom

		auto& iter_find = m_mapListObj.find(szName);
		if (iter_find != m_mapListObj.end())
			continue;

		tUnitInfo.wstrName = szName;
		tUnitInfo.wstrObjKey = szObjKey;
		tUnitInfo.wstrStateKey = szStateKey;

		tUnitInfo.iObjID = int(_wtoi(szObjID));

		tUnitInfo.iLevel = int(_wtoi(szLevel));
		tUnitInfo.iExp = int(_wtoi(szExp));

		tUnitInfo.iMaxHp =tUnitInfo.iHp = int(_wtoi(szHP));
		tUnitInfo.iMaxMp =tUnitInfo.iMp = int(_wtoi(szMP));
		tUnitInfo.iMaxSp =tUnitInfo.iSp = int(_wtoi(szSP));

		tUnitInfo.tStatInfo.iStab = int(_wtoi(szSTAB));
		tUnitInfo.tStatInfo.iHack = int(_wtoi(szHACK));
		tUnitInfo.tStatInfo.iInt = int(_wtoi(szINT));
		tUnitInfo.tStatInfo.iDex = int(_wtoi(szDEX));
		tUnitInfo.tStatInfo.iDef = int(_wtoi(szDEF));
		tUnitInfo.tStatInfo.iMr = int(_wtoi(szMR));
		tUnitInfo.tStatInfo.iAgi = int(_wtoi(szAGI));

		tUnitInfo.tStatInfo.iLightning = int(_wtoi(szLightning));
		tUnitInfo.tStatInfo.iFire = int(_wtoi(szFire));
		tUnitInfo.tStatInfo.iEarth = int(_wtoi(szEarth));
		tUnitInfo.tStatInfo.iLight = int(_wtoi(szLight));

		tUnitInfo.tStatInfo.iIce = int(_wtoi(szIce));
		tUnitInfo.tStatInfo.iWind = int(_wtoi(szWind));
		tUnitInfo.tStatInfo.iDark = int(_wtoi(szDark));
		tUnitInfo.tStatInfo.iNormal = int(_wtoi(szNormal));

		tUnitInfo.iIsFirstAtt = int(_wtoi(szIsFirst));
		tUnitInfo.fFindDst = float(_wtof(szFindDst));

		tUnitInfo.vCollStart.x = float(_wtof(szCollLeft));
		tUnitInfo.vCollStart.y = float(_wtof(szCollTop));
		tUnitInfo.vCollEnd.x = float(_wtof(szCollRight));
		tUnitInfo.vCollEnd.y = float(_wtof(szCollBottom));

		int iNum = m_ListCtrlObject.GetItemCount();

		TCHAR szFindDst[MAX_STR];
		swprintf_s(szFindDst, L"%.2f", tUnitInfo.fFindDst);

		m_ListCtrlObject.InsertItem(iNum, tUnitInfo.wstrName.c_str());


		m_mapListObj.insert({ szName, new UNIT_INFO(tUnitInfo) }).first->second;
	}
}


// 선공 여부 체크박스
void CMonsterSetting::OnBnClickedIsFirstAttack()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	UpdateData(FALSE);
	Invalidate(FALSE);
}

// 업데이트 버튼
void CMonsterSetting::OnBnClickedUpdateSetting()
{
	UpdateData(TRUE);

	TCHAR szFindDst[MAX_STR];
	swprintf_s(szFindDst, L"%.2f", m_tUnitInfo.fFindDst);

	POSITION pos = m_ListCtrlObject.GetFirstSelectedItemPosition();
	int nIndex = m_ListCtrlObject.GetNextSelectedItem(pos);
	m_ListCtrlObject.SetItem(nIndex, 0, LVIF_TEXT, m_tUnitInfo.wstrName.c_str(), NULL, NULL, NULL, NULL);

	UpdateData(FALSE);
}

// Object ID 콤보박스
void CMonsterSetting::OnCbnSelchangeComboObjID()
{
	UpdateData(TRUE);

	m_tUnitInfo.iObjID = m_ComboObjID.GetCurSel();

	/*CString strObjID;
	m_ComboObjID.GetLBText(m_ComboObjID.GetCurSel(), strObjID);

	if (!lstrcmp(strObjID, L"OBJ_ENVIRONMENT"))
		OBJ_ENVIRONMENT;
	else if (!lstrcmp(strObjID, L"OBJ_COLLISION"))
		m_tUnitInfo.iObjID = OBJ_COLLISION;
	else if (!lstrcmp(strObjID, L"OBJ_EVENT"))
		m_tUnitInfo.iObjID = OBJ_EVENT;
	else if (!lstrcmp(strObjID, L"OBJ_PLAYER"))
		m_tUnitInfo.iObjID = OBJ_PLAYER;
	else if (!lstrcmp(strObjID, L"OBJ_MONSTER"))
		m_tUnitInfo.iObjID = OBJ_MONSTER;
	else if (!lstrcmp(strObjID, L"OBJ_NPC"))
		m_tUnitInfo.iObjID = OBJ_NPC;*/

	UpdateData(FALSE);
}

// 오브젝트 리스트에 추가 버튼
void CMonsterSetting::OnBnClickedInsertObject()
{
	if (nullptr == m_pUnitTool->m_pSpriteInfo)
		return;

	UpdateData(TRUE);

	CString strMapKey = m_tUnitInfo.wstrName.c_str();

	auto& iter_find = m_mapListObj.find(strMapKey);
	if (iter_find != m_mapListObj.end())
		return;

	int iNum = m_ListCtrlObject.GetItemCount();

	TCHAR szFindDst[MAX_STR];
	swprintf_s(szFindDst, L"%.2f", m_tUnitInfo.fFindDst);

	m_ListCtrlObject.InsertItem(iNum, m_tUnitInfo.wstrName.c_str());

	UNIT_INFO tUnitInfo = m_tUnitInfo;
	tUnitInfo.wstrObjKey = m_pUnitTool->m_tObjectInfo.wstrObjKey;
	tUnitInfo.wstrStateKey = m_pUnitTool->m_tObjectInfo.wstrStateKey;

	m_mapListObj.insert({ strMapKey, new UNIT_INFO(tUnitInfo) });

	UpdateData(FALSE);
	Invalidate(FALSE);
}

// 오브젝트 리스트에서 삭제 버튼
void CMonsterSetting::OnBnClickedDeleteObject()
{
	UpdateData(TRUE);

	int nItemCount = m_ListCtrlObject.GetSelectedCount();
	POSITION pos = m_ListCtrlObject.GetFirstSelectedItemPosition();
	for (int i = 0; i < nItemCount; i++)
	{
		int nIndex = m_ListCtrlObject.GetNextSelectedItem(pos);
		CString strMapKey = m_ListCtrlObject.GetItemText(nIndex, 0);
		auto iter_find = m_mapListObj.find(strMapKey);
		if (iter_find == m_mapListObj.end())
			continue;

		SafeDelete(iter_find->second);
		m_mapListObj.erase(iter_find);
		m_ListCtrlObject.DeleteItem(nIndex);
	}

	UpdateData(FALSE);
	m_pUnitTool->Invalidate(FALSE);
}

// 리스트에서 오브젝트 선택
void CMonsterSetting::OnLvnItemchangedObject(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	UpdateData(TRUE);
	
	HTREEITEM hStateKey = m_pUnitTool->m_TreeCtrlImage.GetNextItem(NULL, TVGN_CARET);
	UINT nState = m_pUnitTool->m_TreeCtrlImage.GetItemState(hStateKey, TVIF_STATE); //아이템의 상태를 가져온다.
	nState &= ~TVIS_SELECTED; //안 선택된것 처럼...
	m_pUnitTool->m_TreeCtrlImage.SetItemState(hStateKey, nState, -1); //안 선택된것처럼 세팅.


	POSITION pos = m_ListCtrlObject.GetFirstSelectedItemPosition();
	int nIndex = m_ListCtrlObject.GetNextSelectedItem(pos);
	if (-1 >= nIndex)
		return;

	CString strMapKey = m_ListCtrlObject.GetItemText(nIndex, 0);
	auto iter_find = m_mapListObj.find(strMapKey);
	if (iter_find == m_mapListObj.end())
		return;

	m_tUnitInfo = *(iter_find->second);
	const TEX_INFO* pTexInfo = CTextureMgr::GetInstance()->GetTexInfo(m_tUnitInfo.wstrObjKey, m_tUnitInfo.wstrStateKey);
	NULL_CHECK(pTexInfo);

	m_pUnitTool->m_tObjectInfo.wstrObjKey = m_tUnitInfo.wstrObjKey;
	m_pUnitTool->m_tObjectInfo.wstrStateKey = m_tUnitInfo.wstrStateKey;

	
	// Frame
	m_pUnitTool->m_tFrame.iFrameCur = 0;
	m_pUnitTool->m_tFrame.iFrameEnd = CTextureMgr::GetInstance()->GetTexCount(m_tUnitInfo.wstrObjKey, m_tUnitInfo.wstrStateKey);
	m_pUnitTool->m_tFrame.fFrameDelay = pTexInfo->m_fFrameDelay;


	m_ComboObjID.SetCurSel(m_tUnitInfo.iObjID);

	UpdateData(FALSE);
	m_pUnitTool->Invalidate(FALSE);
}


void CMonsterSetting::OnBnClickedAutoCollBox()
{
	const TEX_INFO* pTexInfo = CTextureMgr::GetInstance()->GetTexInfo(m_tUnitInfo.wstrObjKey, m_tUnitInfo.wstrStateKey);
	NULL_CHECK(pTexInfo);

	UpdateData(TRUE);

	m_tUnitInfo.vCollStart = { -pTexInfo->vOffset.x, -pTexInfo->vOffset.y, 0.f };
	m_tUnitInfo.vCollEnd = { pTexInfo->tImgInfo.Width - pTexInfo->vOffset.x, pTexInfo->tImgInfo.Height - pTexInfo->vOffset.y, 0.f };

	UpdateData(FALSE);
	m_pUnitTool->Invalidate(FALSE);
}
