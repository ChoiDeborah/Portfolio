#pragma once
#include "afxwin.h"


// CEnvironSetting 대화 상자입니다.

class CUnitTool;
class CEnvironSetting : public CDialog
{
	DECLARE_DYNAMIC(CEnvironSetting)

public:
	CEnvironSetting(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CEnvironSetting();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ENVIRONSETTING };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

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
