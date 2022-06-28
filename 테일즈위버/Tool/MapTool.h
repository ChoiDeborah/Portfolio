#pragma once
#include "afxcmn.h"
#include "afxwin.h"

// CMapTool 대화 상자입니다.

class CAddMap;
class CTerrain;
class CToolView;
class CMapTool : public CDialog
{
	DECLARE_DYNAMIC(CMapTool)

public:
	CMapTool(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CMapTool();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAPTOOL };
#endif

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()


public:
	void	SetTerrain(CTerrain* pTerrain)		{ m_pTerrain = pTerrain; }
	void	SetToolView(CToolView* pToolView)	{ m_pToolView = pToolView; }
	void	InsertMapData(MAP_DATA* pMapData);
	void	UpdateMapData();

	HRESULT SaveMap(const wstring & wstrFileName);
	HRESULT LoadMap(const wstring & wstrFileName);

	void	LoadObjData();

public:
	afx_msg void OnPaint();

	afx_msg void OnBnClickedAddMap();
	afx_msg void OnBnClickedRemoveMap();

	afx_msg void OnLbnSelchangeMap();
	afx_msg void OnBnClickedInitTile();
	afx_msg void OnBnClickedInitObj();
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedRadioTile();
	afx_msg void OnBnClickedRadioObj();
	afx_msg void OnTvnSelchangedObject(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLbnSelchangeListTile();
	afx_msg void OnBnClickedDrawLine();
	afx_msg void OnBnHotItemChangeCheck1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLbnSelchangeListObject();
	afx_msg void OnBnClickedDeleteObject();
	afx_msg void OnBnClickedIsDark();


public:
	map<CString, TILE_INFO*>	m_mapUnit;
	
	
public:
	CDeviceMgr*			m_pDeviceMgr	= CDeviceMgr::GetInstance();
	CTextureMgr*		m_pTextureMgr	= CTextureMgr::GetInstance();

	CAddMap*			m_pAddMap		= nullptr;
	CToolView*			m_pToolView		= nullptr;
	MAP_DATA*			m_pMapData		= nullptr;
	TILE_INFO*			m_pUnitInfo		= nullptr;

	const TEX_INFO*		m_pMapTexture	= nullptr;
	const TEX_INFO*		m_pSelectObj	= nullptr;
	CTerrain*			m_pTerrain		= nullptr;

public:
	CListBox			m_MapDataListBox;

	CTreeCtrl			m_TreeCtrlObj;

	CListBox			m_ListBoxTile;
	CListBox			m_ListBoxObject;

	CButton				m_ButtonDrawLine;
	CButton				m_ButtonIsDark;

	CString				m_strTileSize; // 타일 개수
	CString				m_strMapSize;
	CStatic				m_Picture;

	HTREEITEM			m_hRoot;

public:
	int					m_iRadio;
	int					m_iSelectTile;
};
