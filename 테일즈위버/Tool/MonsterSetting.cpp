// MonsterSetting.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "MonsterSetting.h"
#include "afxdialogex.h"

#include "UnitTool.h"


// CMonsterSetting ��ȭ �����Դϴ�.

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


// CMonsterSetting �޽��� ó�����Դϴ�.
BOOL CMonsterSetting::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	// ����Ʈ ��Ʈ�� ����
	CRect rt;
	m_ListCtrlObject.GetWindowRect(&rt);
	m_ListCtrlObject.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);	// ����Ʈ ��Ʈ�ѿ� �� ǥ�� �� Item ���� �� ���� ��ü ����

	m_ListCtrlObject.InsertColumn(0, TEXT("Name"), LVCFMT_CENTER, int(rt.Width() * 0.95f));

	// �޺� �ڽ� ����
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
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

// ���� ����
BOOL CMonsterSetting::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	// EditBox ���ͳ� ESC �� ���� ����
	if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN))
		return TRUE;

	return CDialog::PreTranslateMessage(pMsg);
}

// ���� ������ ���� ����
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

// ������Ʈ ����
void CMonsterSetting::SaveData(wofstream & SaveFile)
{
	size_t iMax = m_mapListObj.size();
	size_t iCount = 0;

	// ����
	SaveFile << m_mapListObj.size() << '\n';

	// ������ ����
	for (auto& iter : m_mapListObj) {
		++iCount;
		wstring wstrData = SetListData(iter.second);

		SaveFile << wstrData << '\n';
	}
}

// ������Ʈ �ε�
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

	TCHAR szName[MAX_STR] = L"";	// �̸�
	TCHAR szObjKey[MAX_STR] = L"";	// �ؽ��� ������Ʈ Ű
	TCHAR szStateKey[MAX_STR] = L"";	// �ؽ��� ���� Ű

	TCHAR szObjID[MAX_STR] = L"";	// ������Ʈ ���̵�

	TCHAR szLevel[MAX_STR] = L"";	// ����
	TCHAR szExp[MAX_STR] = L"";	// ����ġ

	TCHAR szHP[MAX_STR] = L"";	// ü��
	TCHAR szMP[MAX_STR] = L"";	// ����
	TCHAR szSP[MAX_STR] = L"";	// ���׹̳�

	TCHAR szSTAB[MAX_STR] = L"";	// ���
	TCHAR szHACK[MAX_STR] = L"";	// ����
	TCHAR szINT[MAX_STR] = L"";	// ���� ����
	TCHAR szDEX[MAX_STR] = L"";	// ����
	TCHAR szDEF[MAX_STR] = L"";	// ���� ���
	TCHAR szMR[MAX_STR] = L"";	// ���� ���
	TCHAR szAGI[MAX_STR] = L"";	// ȸ����

	TCHAR szLightning[MAX_STR] = L"";	// ���� �Ӽ�
	TCHAR szFire[MAX_STR] = L"";	// �� �Ӽ�
	TCHAR szEarth[MAX_STR] = L"";	// �� �Ӽ�
	TCHAR szLight[MAX_STR] = L"";	// �� �Ӽ�

	TCHAR szIce[MAX_STR] = L"";	// ���� �Ӽ�
	TCHAR szWind[MAX_STR] = L"";	// �ٶ� �Ӽ�
	TCHAR szDark[MAX_STR] = L"";	// ���� �Ӽ�
	TCHAR szNormal[MAX_STR] = L"";	// �� �Ӽ�

	TCHAR szIsFirst[MAX_STR] = L"";	// ���� / �İ�

	TCHAR szFindDst[MAX_STR] = L"";	// �÷��̾� �߰� �Ÿ�

	TCHAR szCollLeft[MAX_STR] = L"";	// �浹��Ʈ Left
	TCHAR szCollTop[MAX_STR] = L"";	// �浹��Ʈ Top
	TCHAR szCollRight[MAX_STR] = L"";	// �浹��Ʈ Right
	TCHAR szCollBottom[MAX_STR] = L"";	// �浹��Ʈ Bottom

	UNIT_INFO tUnitInfo;
	for (int i = 0; i < iCount; ++i) {
		LoadFile.getline(szName, MAX_STR, '|');	// �̸�
		LoadFile.getline(szObjKey, MAX_STR, '|');	// �ؽ��� ������Ʈ Ű
		LoadFile.getline(szStateKey, MAX_STR, '|');	// �ؽ��� ���� Ű

		LoadFile.getline(szObjID, MAX_STR, '|');	// ������Ʈ ���̵�

		LoadFile.getline(szLevel, MAX_STR, '|');	// ����
		LoadFile.getline(szExp, MAX_STR, '|');	// ����ġ

		LoadFile.getline(szHP, MAX_STR, '|');	// ü��
		LoadFile.getline(szMP, MAX_STR, '|');	// ����
		LoadFile.getline(szSP, MAX_STR, '|');	// ���׹̳�

		LoadFile.getline(szSTAB, MAX_STR, '|');	// ���
		LoadFile.getline(szHACK, MAX_STR, '|');	// ����
		LoadFile.getline(szINT, MAX_STR, '|');	// ���� ����
		LoadFile.getline(szDEX, MAX_STR, '|');	// ����
		LoadFile.getline(szDEF, MAX_STR, '|');	// ���� ���
		LoadFile.getline(szMR, MAX_STR, '|');	// ���� ���
		LoadFile.getline(szAGI, MAX_STR, '|');	// ȸ����

		LoadFile.getline(szLightning, MAX_STR, '|');	// ���
		LoadFile.getline(szFire, MAX_STR, '|');	// ����
		LoadFile.getline(szEarth, MAX_STR, '|');	// ���� ����
		LoadFile.getline(szLight, MAX_STR, '|');	// ����

		LoadFile.getline(szIce, MAX_STR, '|');	// ���� ���
		LoadFile.getline(szWind, MAX_STR, '|');	// ���� ���
		LoadFile.getline(szDark, MAX_STR, '|');	// ȸ����
		LoadFile.getline(szNormal, MAX_STR, '|');	// ȸ����

		LoadFile.getline(szIsFirst, MAX_STR, '|');	// ���� / �İ�

		LoadFile.getline(szFindDst, MAX_STR, '|');	// �� �߰� �Ÿ�

		LoadFile.getline(szCollLeft, MAX_STR, '|');	// �浹��Ʈ Left
		LoadFile.getline(szCollTop, MAX_STR, '|');	// �浹��Ʈ Top
		LoadFile.getline(szCollRight, MAX_STR, '|');	// �浹��Ʈ Right
		LoadFile.getline(szCollBottom, MAX_STR);		// �浹��Ʈ Bottom

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


// ���� ���� üũ�ڽ�
void CMonsterSetting::OnBnClickedIsFirstAttack()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);

	UpdateData(FALSE);
	Invalidate(FALSE);
}

// ������Ʈ ��ư
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

// Object ID �޺��ڽ�
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

// ������Ʈ ����Ʈ�� �߰� ��ư
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

// ������Ʈ ����Ʈ���� ���� ��ư
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

// ����Ʈ���� ������Ʈ ����
void CMonsterSetting::OnLvnItemchangedObject(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	*pResult = 0;

	UpdateData(TRUE);
	
	HTREEITEM hStateKey = m_pUnitTool->m_TreeCtrlImage.GetNextItem(NULL, TVGN_CARET);
	UINT nState = m_pUnitTool->m_TreeCtrlImage.GetItemState(hStateKey, TVIF_STATE); //�������� ���¸� �����´�.
	nState &= ~TVIS_SELECTED; //�� ���õȰ� ó��...
	m_pUnitTool->m_TreeCtrlImage.SetItemState(hStateKey, nState, -1); //�� ���õȰ�ó�� ����.


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
