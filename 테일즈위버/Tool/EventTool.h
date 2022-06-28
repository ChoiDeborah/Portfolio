#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CEventTool ��ȭ �����Դϴ�.

class CAddMap;
class CTerrain;
class CToolView;
class CEventObj;
class CEventTool : public CDialog
{
	DECLARE_DYNAMIC(CEventTool)

public:
	CEventTool(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CEventTool();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EVENTTOOL };
#endif

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

public:
	void	Update();
	void	Release();
	void	SetTerrain(CTerrain* pTerrain) { m_pTerrain = pTerrain; }
	void	SetToolView(CToolView* pToolView) { m_pToolView = pToolView; }

public:
	afx_msg void OnPaint();

	afx_msg void OnBnClickedUpdate();	// ������Ʈ ��ư
	afx_msg void OnTvnSelchangedImage(NMHDR *pNMHDR, LRESULT *pResult);	// Ʈ�� �̹��� ����

	afx_msg void OnBnClickedPlay();				// �÷��� ��ư
	afx_msg void OnLbnSelchangeEventObject();	// ����Ʈ ������Ʈ ����
	afx_msg void OnLbnSelchangeEventPath();		// ����Ʈ ��Ʈ ����
	afx_msg void OnBnClickedNotifyUpdate();		// ��Ƽ���� ������Ʈ ��ư

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
	vector<CEventObj*>	m_vecEventObj;	// �̺�Ʈ ������Ʈ

public:
	FRAME				m_tCurTime;		// �̺�Ʈ �ִϸ��̼� ������
	EVENT_NOTIFY		m_tCurNotify;	// 

	CString				m_strObjName;	// ���� ������Ʈ �̸�

public:
	map<CString, SPRITE_INFO*>*			m_pImageMap = nullptr;			// �̹��� �� ������ :: MainDialog

	CDeviceMgr*			m_pDeviceMgr	= CDeviceMgr::GetInstance();
	CTextureMgr*		m_pTextureMgr	= CTextureMgr::GetInstance();
	CKeyMgr*			m_pKeyMgr		= CKeyMgr::GetInstance();

	CAddMap*			m_pAddMap		= nullptr;
	CToolView*			m_pToolView		= nullptr;
	CTerrain*			m_pTerrain		= nullptr;

	MAP_DATA*			m_pMapData		= nullptr;	// �� ����

	CEventObj*			m_pEventObj		= nullptr;	// ������ �̺�Ʈ ������Ʈ

	EVENT_NOTIFY*		m_pSeleteNotify = nullptr;	// ���� ���õ� ��Ƽ����

	SPRITE_INFO*		m_pSpriteInfo	= nullptr;	// ������ �̹���


public:
	CListBox			m_MapDataListBox;	// �� ������ ����Ʈ �ڽ�
	CListBox			m_ListBoxObject;		// ������Ʈ ����Ʈ
	CListBox			m_ListBoxEventPath;	// �̺�Ʈ Path

	CTreeCtrl			m_TreeCtrlImage;	// �̹��� Ʈ��

	CButton				m_ButtonPlay;		// �÷��� ��ư

	CStatic				m_Picture;			// ��������Ʈ �̹���

	CButton				m_CheckPathRender;	// Path ���� üũ�ڽ�

	CStatic				m_TimeSlider;		// �ð� �����̵� �̹���

	HTREEITEM			m_hRoot;			// Ʈ�� ��Ʈ


public:
	bool				m_bIsFrameMove	= false;
	bool				m_bIsPathRender = true;
	
	int					m_bIsViewObj;
};
