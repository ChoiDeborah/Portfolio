#pragma once
#include "afxwin.h"


// CAddMap 대화 상자입니다.
class CMapTool;
class CEventTool;
class CMainDialog;
class CAddMap : public CDialog
{
	DECLARE_DYNAMIC(CAddMap)

public:
	CAddMap(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CAddMap();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ADDMAP };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnLbnSelchangeMapTexture();
	afx_msg void OnBnClickedOk();
	afx_msg void OnPaint();

public:
	void SetMainDialog(CMainDialog* pMainDialog) {m_pMainDialog = pMainDialog;}
	void SetMapTool(CMapTool* pMapTool)			{ m_pMapTool = pMapTool; }
	void SetEventTool(CEventTool* pEventTool)	{ m_pEventTool = pEventTool; }

private:
	const TEX_INFO*	m_pCurMapTex;
	CMainDialog*	m_pMainDialog;
	CMapTool*		m_pMapTool;
	CEventTool*		m_pEventTool;

	int				m_iMapWidth		= 0;
	int				m_iMapHeight	= 0;

	int				m_iBoxWidth		= 0;
	int				m_iBoxHeight	= 0;

protected:
	CString			m_strMapName;
	CStatic			m_Picture;
	CListBox		m_MapListBox;
	CString			m_strMapSize;
};
