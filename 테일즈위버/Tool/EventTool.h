#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CEventTool 대화 상자입니다.

class CAddMap;
class CTerrain;
class CToolView;
class CEventObj;
class CEventTool : public CDialog
{
	DECLARE_DYNAMIC(CEventTool)

public:
	CEventTool(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CEventTool();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EVENTTOOL };
#endif

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	void	Update();
	void	Release();
	void	SetTerrain(CTerrain* pTerrain) { m_pTerrain = pTerrain; }
	void	SetToolView(CToolView* pToolView) { m_pToolView = pToolView; }

public:
	afx_msg void OnPaint();

	afx_msg void OnBnClickedUpdate();	// 업데이트 버튼
	afx_msg void OnTvnSelchangedImage(NMHDR *pNMHDR, LRESULT *pResult);	// 트리 이미지 선택

	afx_msg void OnBnClickedPlay();				// 플레이 버튼
	afx_msg void OnLbnSelchangeEventObject();	// 리스트 오브젝트 선택
	afx_msg void OnLbnSelchangeEventPath();		// 리스트 패트 선택
	afx_msg void OnBnClickedNotifyUpdate();		// 노티파이 업데이트 버튼

	afx_msg void OnBnClickedAddObject();
	afx_msg void OnBnClickedDeleteObject();
	afx_msg void OnBnClickedAddNotify();
	afx_msg void OnBnClickedDeleteNotify();
	afx_msg void OnBnClickedCheckIsMoving();
	afx_msg void OnBnClickedCheckActive();

	afx_msg void OnBnClickedCreateScene();

	afx_msg void OnLbnSelchangeMap();
	afx_msg void OnBnClickedSaveScene();

	afx_msg void OnBnClickedNameSet();
	afx_msg void OnBnClickedIsViewObj();
	
	//afx_msg void OnBnClickedRemoveMap();

public:
	void	FrameMove();
	void	ResetImageTree();
	void	InsertImageTree(SPRITE_INFO& tSpriteInfo, HTREEITEM hSingle, HTREEITEM hMulti);
	void	InsertMapData(MAP_DATA* pMapData); 
	void	UpdateMapData();

	HRESULT SaveMap(const wstring & wstrFileName);
	HRESULT LoadMap(const wstring & wstrFileName);


public:
	vector<CEventObj*>	m_vecEventObj;	// 이벤트 오브젝트

public:
	FRAME				m_tCurTime;		// 이벤트 애니메이션 프레임
	EVENT_NOTIFY		m_tCurNotify;	// 

	CString				m_strObjName;	// 선택 오브젝트 이름

public:
	map<CString, SPRITE_INFO*>*			m_pImageMap = nullptr;			// 이미지 맵 포인터 :: MainDialog

	CDeviceMgr*			m_pDeviceMgr	= CDeviceMgr::GetInstance();
	CTextureMgr*		m_pTextureMgr	= CTextureMgr::GetInstance();
	CKeyMgr*			m_pKeyMgr		= CKeyMgr::GetInstance();

	CAddMap*			m_pAddMap		= nullptr;
	CToolView*			m_pToolView		= nullptr;
	CTerrain*			m_pTerrain		= nullptr;

	MAP_DATA*			m_pMapData		= nullptr;	// 맵 정보

	CEventObj*			m_pEventObj		= nullptr;	// 선택한 이벤트 오브젝트

	EVENT_NOTIFY*		m_pSeleteNotify = nullptr;	// 현재 선택된 노티파이

	SPRITE_INFO*		m_pSpriteInfo	= nullptr;	// 보여줄 이미지


public:
	CListBox			m_MapDataListBox;	// 맵 데이터 리스트 박스
	CListBox			m_ListBoxObject;		// 오브젝트 리스트
	CListBox			m_ListBoxEventPath;	// 이벤트 Path

	CTreeCtrl			m_TreeCtrlImage;	// 이미지 트리

	CButton				m_ButtonPlay;		// 플레이 버튼

	CStatic				m_Picture;			// 스프라이트 이미지

	CButton				m_CheckPathRender;	// Path 렌더 체크박스

	CStatic				m_TimeSlider;		// 시간 슬라이드 이미지

	HTREEITEM			m_hRoot;			// 트리 루트


public:
	bool				m_bIsFrameMove	= false;
	bool				m_bIsPathRender = true;
	
	int					m_bIsViewObj;
};
