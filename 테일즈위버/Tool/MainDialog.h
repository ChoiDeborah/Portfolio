#pragma once
#include "afxcmn.h"
#include "MapTool.h"
#include "SpriteTool.h"
#include "UnitTool.h"
#include "WordTool.h"
#include "EventTool.h"

// CMainDialog ��ȭ �����Դϴ�.

class CToolView;
class CMainDialog : public CDialog
{
	DECLARE_DYNAMIC(CMainDialog)

public:
	CMainDialog(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CMainDialog();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAINDIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();

public:
	afx_msg void OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

public:
	void SetToolView(CToolView* pToolView) { m_pToolView = pToolView; }
	void LoadImageData(wstring wstrFilePath);

public:
	map<CString, SPRITE_INFO*>		m_mapTreeItem;		// �̹���

public:
	CMapTool		m_MapTool;
	CSpriteTool		m_SpriteTool;
	CUnitTool		m_UnitTool;
	CWordTool		m_WordTool;
	CEventTool		m_EventTool;

public:
	CWnd*			m_pWnd			= nullptr;
	CToolView*		m_pToolView		= nullptr;
	CTextureMgr*	m_pTextureMgr	= nullptr;

public:
	CTabCtrl		m_Tab;
};
