// EnvironSetting.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "EnvironSetting.h"
#include "afxdialogex.h"

#include "UnitTool.h"

// CEnvironSetting ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CEnvironSetting, CDialog)

CEnvironSetting::CEnvironSetting(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_ENVIRONSETTING, pParent)
{

}

CEnvironSetting::~CEnvironSetting()
{
	for (auto& Obj : m_vecListObj)
		SafeDelete(Obj);
	m_vecListObj.clear();
	m_vecListObj.shrink_to_fit();
}

void CEnvironSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListBoxObj);
}

// ���� ������ ���� ����
wstring CEnvironSetting::SetListData(TILE_INFO * pUnitInfo)
{
	return pUnitInfo->wstrName + L"|" + pUnitInfo->wstrObjKey + L"|" + pUnitInfo->wstrStateKey;
}

// ������Ʈ ����
void CEnvironSetting::SaveData(wofstream & SaveFile)
{
	size_t iMax = m_vecListObj.size();
	size_t iCount = 0;

	// ����
	SaveFile << m_vecListObj.size() << '\n';

	// ������ ����
	for (auto& Obj : m_vecListObj) {
		++iCount;
		wstring wstrData = SetListData(Obj);

		SaveFile << wstrData;
		if (iCount < iMax)
			SaveFile << '\n';
	}
}

// ������Ʈ �ε�
void CEnvironSetting::LoadData(wifstream & LoadFile)
{
	for (auto& Obj : m_vecListObj)
		SafeDelete(Obj);
	m_vecListObj.clear();
	m_vecListObj.shrink_to_fit();
	m_ListBoxObj.ResetContent();

	TCHAR szCount[MAX_STR] = L"";
	LoadFile.getline(szCount, MAX_STR);
	int iCount = int(_wtoi(szCount));
	if (0 >= iCount)
		return;

	m_vecListObj.reserve(size_t(iCount));

	TCHAR szName[MAX_STR] = L"";
	TCHAR szObjKey[MAX_STR] = L"";
	TCHAR szStateKey[MAX_STR] = L"";

	TILE_INFO tUnitInfo;
	for (int i = 0; i < iCount; ++i) {
		LoadFile.getline(szName, MAX_STR, '|');
		LoadFile.getline(szObjKey, MAX_STR, '|');
		LoadFile.getline(szStateKey, MAX_STR);

		CString strKey = szName;
			
		tUnitInfo.wstrName		= szName;
		tUnitInfo.wstrObjKey	= szObjKey;
		tUnitInfo.wstrStateKey	= szStateKey;

		m_ListBoxObj.AddString(szName);

		m_vecListObj.push_back(new TILE_INFO(tUnitInfo));
	}
}


BEGIN_MESSAGE_MAP(CEnvironSetting, CDialog)
	ON_BN_CLICKED(IDC_BUTTON7, &CEnvironSetting::OnBnClickedInsert)
	ON_BN_CLICKED(IDC_BUTTON9, &CEnvironSetting::OnBnClickedDelete)
	ON_LBN_SELCHANGE(IDC_LIST1, &CEnvironSetting::OnLbnSelchangeObject)
END_MESSAGE_MAP()


// CEnvironSetting �޽��� ó�����Դϴ�.


// Environ ������Ʈ �߰�
void CEnvironSetting::OnBnClickedInsert()
{
	UpdateData(TRUE);

	CString strKey = m_pUnitTool->m_tObjectInfo.wstrObjKey.c_str();
		

	m_ListBoxObj.InsertString(-1, strKey);
	m_vecListObj.push_back(new TILE_INFO(m_pUnitTool->m_tObjectInfo));
	
	UpdateData(FALSE);
	Invalidate(FALSE);
}

// Environ ������Ʈ ����
void CEnvironSetting::OnBnClickedDelete()
{
	UpdateData(TRUE);

	int iIndex = m_ListBoxObj.GetCurSel();
	SafeDelete(m_vecListObj[iIndex]);
	m_vecListObj.erase(m_vecListObj.begin() + iIndex);
	m_vecListObj.shrink_to_fit();
	m_ListBoxObj.DeleteString(iIndex);
	
	UpdateData(FALSE);
	m_pUnitTool->Invalidate(FALSE);
}

// ������Ʈ ����
void CEnvironSetting::OnLbnSelchangeObject()
{
	UpdateData(TRUE);

	int iIndex = m_ListBoxObj.GetCurSel();
	if (-1 >= iIndex || iIndex >= int(m_vecListObj.size()))
		return;

	m_pUnitTool->m_tObjectInfo = *m_vecListObj[iIndex];

	m_pUnitTool->m_tFrame.iFrameCur = 0;
	m_pUnitTool->m_tFrame.iFrameEnd = CTextureMgr::GetInstance()->GetTexCount(m_pUnitTool->m_tObjectInfo.wstrObjKey, m_pUnitTool->m_tObjectInfo.wstrStateKey);
	m_pUnitTool->m_tFrame.fFrameDelay = CTextureMgr::GetInstance()->GetTexInfo(m_pUnitTool->m_tObjectInfo.wstrObjKey, m_pUnitTool->m_tObjectInfo.wstrStateKey)->m_fFrameDelay;

	UpdateData(FALSE);
	m_pUnitTool->Invalidate(FALSE);
}