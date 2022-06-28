#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CSpriteTool 대화 상자입니다.

class CToolView;
class CSpriteTool : public CDialog
{
	DECLARE_DYNAMIC(CSpriteTool)

public:
	CSpriteTool(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSpriteTool();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SPRITETOOL };
#endif

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnPaint();

private:
	void FindDirctory(HTREEITEM& hParent, wstring wstrFilePath);

public:
	afx_msg void OnTvnSelchangedTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedFrameMoveOn();
	afx_msg void OnNMReleasedcaptureSlider1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedButtonSet();
	afx_msg void OnBnClickedCheckRoof();
	afx_msg void OnBnClickedInsertData();
	afx_msg void OnBnClickedDeleteData();
	afx_msg void OnBnClickedSaveDataPath();

	afx_msg void OnLvnItemchangedSelectData(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedCheckMirror();

	afx_msg void OnNMReleasedcaptureSliderA(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMReleasedcaptureSliderR(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMReleasedcaptureSliderG(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMReleasedcaptureSliderB(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void OnNMReleasedcaptureEraseA(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMReleasedcaptureEraseR(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMReleasedcaptureEraseG(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMReleasedcaptureEraseB(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void OnBnClickedButtonAutoOffset();
	afx_msg void OnBnClickedLoadDataPath();
	afx_msg void OnStnClickedSpritePicture();
	afx_msg void OnBnClickedEraseColor();
	afx_msg void OnBnClickedOffset();
	afx_msg void OnBnClickedDrawLine();

public:
	void Update();

protected:
	void FrameMove();
	wstring	SetListData(SPRITE_INFO* pSpriteInfo);
	void LoadData(wstring wstrFilePath);
	void UpdateSetting(const TEX_INFO* pTexInfo);

public:
	void SetToolView(CToolView* pToolView) { m_pToolView = pToolView; }

public:
	map<CString, SPRITE_INFO*>		m_mapTreeItem;
	map<CString, SPRITE_INFO*>		m_mapListItem;

	map<CString, SPRITE_INFO*>		m_mapListSingle;

	vector<wstring>					m_vecImageData;

	FRAME			m_tFrame;		// 스프라이트 프레임 키
	SPRITE_INFO		m_tSpriteInfo;
	wstring			m_wstrObjKey = L"";
	wstring			m_wstrStateKey = L"";


public:
	CDeviceMgr*		m_pDeviceMgr	= CDeviceMgr::GetInstance();
	CTextureMgr*	m_pTextureMgr	= CTextureMgr::GetInstance();

	SPRITE_INFO*	m_pSpriteInfo	= nullptr;
	CToolView*		m_pToolView		= nullptr;


public:
	CTreeCtrl		m_ForderTree;
	CListCtrl		m_ListImageData;

	CSliderCtrl		m_Slider;
	CSliderCtrl		m_SliderARGB[4];

	CSliderCtrl		m_SliderEraseColor[4];

	CButton			m_BtPlay;
	CButton			m_RadioEraseColor;
	CButton			m_CheckDrawLine;

	CStatic			m_SpritePicture;
	
	CString			m_strAnimTime;
	CString			m_strImageSize;
	CString			m_strImageCount;
	CString			m_strTexType;

public:
	int				m_iFrameScroll;

	int				m_iImageNum = 0;
	bool			m_bIsFrameMove = false;
	BOOL			m_bIsFrameRoof;
	BOOL			m_bIsMirror;
};
