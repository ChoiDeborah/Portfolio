#pragma once
#include "afxwin.h"
#include "afxcmn.h"

#include "MonsterSetting.h"
#include "EnvironSetting.h"

// CUnitTool 대화 상자입니다.

class CUnitTool : public CDialog
{
	DECLARE_DYNAMIC(CUnitTool)

public:
	CUnitTool(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CUnitTool();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_UNITTOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	void	SetToolView(CToolView* pToolView) { m_pToolView = pToolView; }
	void	FrameMove();
	void	ResetImageTree();
	void	InsertImageTree(SPRITE_INFO& tSpriteInfo, HTREEITEM hSingle, HTREEITEM hMulti);

public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg void OnPaint();
	afx_msg void OnTvnSelchangedImage(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedSaveData();
	afx_msg void OnBnClickedLoadData();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult);


public:
	map<CString, SPRITE_INFO*>*		m_pImageMap = nullptr;			// 이미지 맵 포인터 :: MainDialog

public:
	CMonsterSetting	m_MonsterSetting;
	CEnvironSetting	m_EnvironSetting;

	TILE_INFO		m_tObjectInfo;
	FRAME			m_tFrame;

public:
	CToolView*		m_pToolView		= nullptr;
	CDeviceMgr*		m_pDeviceMgr	= CDeviceMgr::GetInstance();
	CTextureMgr*	m_pTextureMgr	= CTextureMgr::GetInstance();

	SPRITE_INFO*	m_pSpriteInfo	= nullptr;

	CWnd*			m_pWnd			= nullptr;
	
	
public:
	CTreeCtrl		m_TreeCtrlImage;
	CStatic			m_pPicture;

	CTabCtrl		m_Tab;

	HTREEITEM		m_hRoot;
};
