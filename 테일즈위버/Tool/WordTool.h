#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CWordTool ��ȭ �����Դϴ�.

class CWordTool : public CDialog
{
	DECLARE_DYNAMIC(CWordTool)

public:
	CWordTool(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CWordTool();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_WORDTOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

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
	afx_msg void OnTvnSelchangedImage(NMHDR *pNMHDR, LRESULT *pResult);	// Ʈ�� Ŭ��

	afx_msg void OnBnClickedImageKeyReset();		// �̹��� Ű ����

	afx_msg void OnBnClickedAddWordList();			// ����Ʈ�ڽ� �߰� ��ư
	afx_msg void OnBnClickedButtonDeleteWordList();	// ����Ʈ�ڽ� ���� ��ư
	afx_msg void OnBnClickedUpdateWord();			// ������Ʈ ��ư

	afx_msg void OnBnClickedCreateWordFile();		// ���� ���� ���� ��ư
	afx_msg void OnLbnSelchangeWordFile();			// ���� ���� ����Ʈ �ڽ� Ŭ��

	afx_msg void OnLbnSelchangeWordListBox();		// ���� ���� ����Ʈ �ڽ� Ŭ��

public:
	vector<WORD_INFO*>	m_vecWord;
	
	WORD_INFO			m_tWordInfo;

public:
	map<CString, SPRITE_INFO*>*		
						m_pImageMap = nullptr;			// �̹��� �� ������ :: MainDialog

	CToolView*			m_pToolView = nullptr;
	CDeviceMgr*			m_pDeviceMgr = CDeviceMgr::GetInstance();
	CTextureMgr*		m_pTextureMgr = CTextureMgr::GetInstance();

	SPRITE_INFO*		m_pSpriteInfo = nullptr;

	WORD_INFO*			m_pWordInfo;

public:
	CString				m_strFileName;		// ���� �̸�

	CTreeCtrl			m_TreeCtrlImage;	// �̹��� Ʈ��
	HTREEITEM			m_hRoot;			// �̹��� ��Ʈ
	
	CStatic				m_pPicture;			// ���� ��ü

	CListBox			m_ListBoxWord;		// WORD_INFO ����Ʈ �ڽ�
	
	CListBox			m_ListBoxWordFile;	// ���� ���� ����Ʈ �ڽ�
};
