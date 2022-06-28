#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CMonsterSetting 대화 상자입니다.

class CUnitTool;
class CMonsterSetting : public CDialog
{
	DECLARE_DYNAMIC(CMonsterSetting)

public:
	CMonsterSetting(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CMonsterSetting();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MONSTERSETTING };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg void OnBnClickedIsFirstAttack();
	afx_msg void OnBnClickedUpdateSetting();
	afx_msg void OnCbnSelchangeComboObjID();
	afx_msg void OnBnClickedInsertObject();
	afx_msg void OnBnClickedDeleteObject();
	afx_msg void OnLvnItemchangedObject(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedAutoCollBox();

	DECLARE_MESSAGE_MAP()

public:
	wstring SetListData(UNIT_INFO* pUnitInfo);
	void SaveData(wofstream& SaveFile);
	void LoadData(wifstream& LoadFile);

public:
	map<CString, UNIT_INFO*>		m_mapListObj;

public:
	UNIT_INFO		m_tUnitInfo;

public:
	CUnitTool*		m_pUnitTool = nullptr;

public:
	CComboBox		m_ComboObjID;

	CListCtrl		m_ListCtrlObject;
	CButton			m_CheckShowCollRect;
};
