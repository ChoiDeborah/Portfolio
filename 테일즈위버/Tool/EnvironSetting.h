#pragma once
#include "afxwin.h"


// CEnvironSetting ��ȭ �����Դϴ�.

class CUnitTool;
class CEnvironSetting : public CDialog
{
	DECLARE_DYNAMIC(CEnvironSetting)

public:
	CEnvironSetting(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CEnvironSetting();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ENVIRONSETTING };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

public:
	wstring SetListData(TILE_INFO* pUnitInfo);
	void SaveData(wofstream& SaveFile);
	void LoadData(wifstream& LoadFile);

public:
	vector<TILE_INFO*>		m_vecListObj;

public:
	CUnitTool*		m_pUnitTool = nullptr;

public:
	TILE_INFO		m_tUnitInfo;

public:
	CListBox		m_ListBoxObj;
	afx_msg void OnBnClickedInsert();
	afx_msg void OnBnClickedDelete();
	afx_msg void OnLbnSelchangeObject();
};
