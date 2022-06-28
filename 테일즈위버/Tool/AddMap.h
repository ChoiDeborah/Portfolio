#pragma once
#include "afxwin.h"


// CAddMap ��ȭ �����Դϴ�.
class CMapTool;
class CEventTool;
class CMainDialog;
class CAddMap : public CDialog
{
	DECLARE_DYNAMIC(CAddMap)

public:
	CAddMap(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CAddMap();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ADDMAP };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

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
