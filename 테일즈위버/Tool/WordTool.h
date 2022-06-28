#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CWordTool 대화 상자입니다.

class CWordTool : public CDialog
{
	DECLARE_DYNAMIC(CWordTool)

public:
	CWordTool(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CWordTool();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_WORDTOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	void	SetToolView(CToolView* pToolView) { m_pToolView = pToolView; }

	void	ResetImageTree();
	void	InsertImageTree(SPRITE_INFO& tSpriteInfo, HTREEITEM hSingle, HTREEITEM hMulti);
	wstring SetListData(WORD_INFO* pWordInfo);
	void	SaveData(const wstring& wstrFileName);
	void	LoadData(const wstring& wstrFileName);

public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnPaint();
	afx_msg void OnTvnSelchangedImage(NMHDR *pNMHDR, LRESULT *pResult);	// 트리 클릭

	afx_msg void OnBnClickedImageKeyReset();		// 이미지 키 리셋

	afx_msg void OnBnClickedAddWordList();			// 리스트박스 추가 버튼
	afx_msg void OnBnClickedButtonDeleteWordList();	// 리스트박스 삭제 버튼
	afx_msg void OnBnClickedUpdateWord();			// 업데이트 버튼

	afx_msg void OnBnClickedCreateWordFile();		// 워드 파일 생성 버튼
	afx_msg void OnLbnSelchangeWordFile();			// 워드 파일 리스트 박스 클릭

	afx_msg void OnLbnSelchangeWordListBox();		// 워드 정보 리스트 박스 클릭

public:
	vector<WORD_INFO*>	m_vecWord;
	
	WORD_INFO			m_tWordInfo;

public:
	map<CString, SPRITE_INFO*>*		
						m_pImageMap = nullptr;			// 이미지 맵 포인터 :: MainDialog

	CToolView*			m_pToolView = nullptr;
	CDeviceMgr*			m_pDeviceMgr = CDeviceMgr::GetInstance();
	CTextureMgr*		m_pTextureMgr = CTextureMgr::GetInstance();

	SPRITE_INFO*		m_pSpriteInfo = nullptr;

	WORD_INFO*			m_pWordInfo;

public:
	CString				m_strFileName;		// 파일 이름

	CTreeCtrl			m_TreeCtrlImage;	// 이미지 트리
	HTREEITEM			m_hRoot;			// 이미지 루트
	
	CStatic				m_pPicture;			// 픽쳐 객체

	CListBox			m_ListBoxWord;		// WORD_INFO 리스트 박스
	
	CListBox			m_ListBoxWordFile;	// 워드 파일 리스트 박스
};
