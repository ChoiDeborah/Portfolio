#pragma once



// CMyForm �� ���Դϴ�.
#include "MainDialog.h"
#include "afxcmn.h"

class CMyForm : public CFormView
{
	DECLARE_DYNCREATE(CMyForm)

protected:
	CMyForm();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CMyForm();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MYFORM };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	virtual void OnInitialUpdate();
	DECLARE_MESSAGE_MAP()

public:	// Message Functions

public:
	// MFC ���� ��Ʈ
	CFont		m_Font;
	CMainDialog m_MainDialog;
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};


