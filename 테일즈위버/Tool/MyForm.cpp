// MyForm.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "MyForm.h"


// CMyForm

IMPLEMENT_DYNCREATE(CMyForm, CFormView)

CMyForm::CMyForm()
	: CFormView(IDD_MYFORM)
{

}

CMyForm::~CMyForm()
{
}

void CMyForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMyForm, CFormView)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CMyForm �����Դϴ�.

#ifdef _DEBUG
void CMyForm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CMyForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMyForm �޽��� ó�����Դϴ�.


void CMyForm::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	//m_Font.CreatePointFont(200, L"consolas"); // (��Ʈũ��, ��ü)

	//GetDlgItem(IDC_BUTTON1)->SetFont(&m_Font);
	//GetDlgItem(IDC_BUTTON2)->SetFont(&m_Font);

	// ���̾�αװ� �����Ǳ� ���� ������ �ڵ��� null�̴�.
	if (nullptr == m_MainDialog.GetSafeHwnd()) {
		m_MainDialog.Create(IDD_MAINDIALOG, this);
		m_MainDialog.ShowWindow(SW_SHOW);
	}
}

BOOL CMyForm::OnEraseBkgnd(CDC* pDC)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	CRect rt;
	GetClientRect(rt);
	pDC->FillSolidRect(rt, RGB(50, 50, 50));

	return TRUE;
}
