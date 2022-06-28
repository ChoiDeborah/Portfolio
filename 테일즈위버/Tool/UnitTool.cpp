// UnitTool.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "UnitTool.h"
#include "afxdialogex.h"

#include "MainDialog.h"
#include "ToolView.h"


// CUnitTool ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CUnitTool, CDialog)

CUnitTool::CUnitTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_UNITTOOL, pParent)
{

}

CUnitTool::~CUnitTool()
{
}

// ��Ʈ�� ID�� ������ ���������ִ� �Լ�.
void CUnitTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_TreeCtrlImage);
	DDX_Control(pDX, IDC_SPRITE_PICTURE, m_pPicture);
	DDX_Control(pDX, IDC_TAB1, m_Tab);
}


BEGIN_MESSAGE_MAP(CUnitTool, CDialog)
	ON_WM_PAINT()
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, &CUnitTool::OnTvnSelchangedImage)
	ON_BN_CLICKED(IDC_BUTTON10, &CUnitTool::OnBnClickedSaveData)
	ON_BN_CLICKED(IDC_BUTTON13, &CUnitTool::OnBnClickedLoadData)
	ON_WM_DROPFILES()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CUnitTool::OnTcnSelchangeTab)
END_MESSAGE_MAP()


// �ʱ�ȭ
BOOL CUnitTool::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	// Tab ����
	m_Tab.InsertItem(UNITTOOL_TAP::MONSTER_SETTING + 1, _T("MONSTER"));
	m_Tab.InsertItem(UNITTOOL_TAP::ENVIRON_SETTING + 1, _T("ENVIRON"));

	CRect rt;
	m_Tab.GetClientRect(&rt);

	if (NULL == m_MonsterSetting.GetSafeHwnd()) {
		m_MonsterSetting.m_pUnitTool = this;
		m_MonsterSetting.Create(IDD_MONSTERSETTING, &m_Tab); // ���̾�α� ����.
		m_MonsterSetting.SetWindowPos(NULL, 4, 25, rt.Width() - 10, rt.Height() - 30, SWP_SHOWWINDOW | SWP_NOZORDER);
		m_pWnd = &m_MonsterSetting;
	}

	if (NULL == m_EnvironSetting.GetSafeHwnd()) {
		m_EnvironSetting.m_pUnitTool = this;
		m_EnvironSetting.Create(IDD_ENVIRONSETTING, &m_Tab); // ���̾�α� ����.
		m_EnvironSetting.SetWindowPos(NULL, 4, 25, rt.Width() - 10, rt.Height() - 30, SWP_SHOWWINDOW | SWP_NOZORDER);
		m_EnvironSetting.ShowWindow(SW_HIDE);
	}

	// �ҷ�����
	OnBnClickedLoadData();


	// Ʈ�� ����
	m_hRoot = m_TreeCtrlImage.InsertItem(TEXT("Texture"), 0, 0, TVI_ROOT, TVI_LAST);
	m_TreeCtrlImage.Expand(m_hRoot, TVE_EXPAND);

	UpdateData(FALSE);
	Invalidate(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


// ����, ESC �����ֱ�
BOOL CUnitTool::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	// EditBox ���ͳ� ESC �� ���� ����
	if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN))
		return TRUE;

	return CDialog::PreTranslateMessage(pMsg);
}


// ����
void CUnitTool::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
					   // �׸��� �޽����� ���ؼ��� CDialog::OnPaint()��(��) ȣ������ ���ʽÿ�.

	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(m_tObjectInfo.wstrObjKey, m_tObjectInfo.wstrStateKey, m_tFrame.iFrameCur);
	NULL_CHECK(pTexInfo);

	

	m_pDeviceMgr->Render_Begin();
	D3DXMATRIX matScale, matTrans, matWorld;
	float fScaleW = float(VIEWCX) / pTexInfo->tImgInfo.Width;
	float fScaleH = float(VIEWCY) / pTexInfo->tImgInfo.Height;
		
	D3DXMatrixScaling(&matScale, fScaleW * pTexInfo->m_iMirror, fScaleH, 0.f);
	D3DXMatrixTranslation(&matTrans, VIEWCX / 2, VIEWCY / 2, 0.f);
	matWorld = matScale * matTrans;
	m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3{ pTexInfo->tImgInfo.Width * 0.5f, pTexInfo->tImgInfo.Height * 0.5f, 0.f }, nullptr, pTexInfo->dwBlend);
	m_pDeviceMgr->Render_End(m_pPicture.m_hWnd);

	NULL_CHECK(m_pToolView);
	m_pToolView->Invalidate(FALSE);
}

// ������ �̵�
void CUnitTool::FrameMove()
{
	if (0 > m_tFrame.fFrameDelay)
		return;

	m_tFrame.fFrameTime += CTimeMgr::GetInstance()->GetDeltaTime();
	if (m_tFrame.fFrameDelay < m_tFrame.fFrameTime) {
		m_tFrame.fFrameTime = 0.f;
		++m_tFrame.iFrameCur;

		if (m_tFrame.iFrameCur >= m_tFrame.iFrameEnd) {
			m_tFrame.iFrameCur = 0;
		}

		UpdateData(TRUE);
		NULL_CHECK(m_pToolView);
		m_pToolView->Invalidate(FALSE);
	}
}

// �̹��� Ʈ�� �ʱ�ȭ
void CUnitTool::ResetImageTree()
{
	// Ʈ�� ��ü ���� ����
	if (m_TreeCtrlImage.ItemHasChildren(m_hRoot))
	{
		HTREEITEM hNextItem;
		HTREEITEM hChildItem = m_TreeCtrlImage.GetChildItem(m_hRoot);

		while (hChildItem != NULL)
		{
			hNextItem = m_TreeCtrlImage.GetNextItem(hChildItem, TVGN_NEXT);
			m_TreeCtrlImage.DeleteItem(hChildItem);
			hChildItem = hNextItem;
		}
	}
}

// �̹��� Ʈ�� ����
void CUnitTool::InsertImageTree(SPRITE_INFO & tSpriteInfo, HTREEITEM hSingle, HTREEITEM hMulti)
{
	// Ʈ������ �θ� ������ ã��
	bool bIsFind = false;

	HTREEITEM hType = (CTextureMgr::SINGLE_TEXTURE == tSpriteInfo.iTexType) ? hSingle : hMulti;

	HTREEITEM hNextItem;
	HTREEITEM hChildItem = m_TreeCtrlImage.GetChildItem(hType);

	if (NULL != hType && !lstrcmp(tSpriteInfo.wstrObjKey.c_str(), m_TreeCtrlImage.GetItemText(hType))) {
		if (lstrcmp(tSpriteInfo.wstrStateKey.c_str(), m_TreeCtrlImage.GetItemText(hChildItem))) {
			m_TreeCtrlImage.InsertItem(tSpriteInfo.wstrStateKey.c_str(), 0, 0, hType, TVI_LAST);
			bIsFind = true;
		}
		else {
			bIsFind = true;
		}
	}
	else {
		while (hChildItem != NULL)
		{
			hNextItem = m_TreeCtrlImage.GetNextItem(hChildItem, TVGN_NEXT);

			if (!lstrcmp(tSpriteInfo.wstrObjKey.c_str(), m_TreeCtrlImage.GetItemText(hChildItem))) {
				if (lstrcmp(tSpriteInfo.wstrStateKey.c_str(), m_TreeCtrlImage.GetItemText(hNextItem))) {
					m_TreeCtrlImage.InsertItem(tSpriteInfo.wstrStateKey.c_str(), 0, 0, hChildItem, TVI_LAST);
					bIsFind = true;
					break;
				}
				else {
					bIsFind = true;
					break;
				}
			}

			hChildItem = hNextItem;
		}
		if (!bIsFind) {
			switch (tSpriteInfo.iTexType) {
			case CTextureMgr::SINGLE_TEXTURE:
				hNextItem = m_TreeCtrlImage.InsertItem(tSpriteInfo.wstrObjKey.c_str(), 0, 0, hSingle, TVI_LAST);
				m_TreeCtrlImage.InsertItem(tSpriteInfo.wstrStateKey.c_str(), 0, 0, hNextItem, TVI_LAST);
				break;

			case CTextureMgr::MULTI_TEXTURE:
				hNextItem = m_TreeCtrlImage.InsertItem(tSpriteInfo.wstrObjKey.c_str(), 0, 0, hMulti, TVI_LAST);
				m_TreeCtrlImage.InsertItem(tSpriteInfo.wstrStateKey.c_str(), 0, 0, hNextItem, TVI_LAST);
				break;
			}
		}
	}
}


// CUnitTool �޽��� ó�����Դϴ�.

// Ʈ�� Ŭ��, �̹��� ����
void CUnitTool::OnTvnSelchangedImage(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	*pResult = 0;
	
	UpdateData(TRUE);

	NULL_CHECK(m_pImageMap);

	HTREEITEM hStateKey		= m_TreeCtrlImage.GetNextItem(NULL, TVGN_CARET);
	HTREEITEM hObjKey		= m_TreeCtrlImage.GetNextItem(hStateKey, TVGN_PARENT);

	wstring wstrObjKey		= m_TreeCtrlImage.GetItemText(hObjKey);
	wstring wstrStateKey	= m_TreeCtrlImage.GetItemText(hStateKey);
	wstring wstrCmp			= wstrObjKey + L"|" + wstrStateKey;

	auto iter_find = m_pImageMap->find(wstrCmp.c_str());
	if (iter_find != m_pImageMap->end()) {
		const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(iter_find->second->wstrObjKey, iter_find->second->wstrStateKey);
		NULL_CHECK(pTexInfo);

		m_tObjectInfo.wstrObjKey	= wstrObjKey;
		m_tObjectInfo.wstrStateKey	= wstrStateKey;

		if (0 == m_Tab.GetCurSel())
		{
			m_MonsterSetting.m_tUnitInfo.wstrObjKey = m_tObjectInfo.wstrObjKey;
			m_MonsterSetting.m_tUnitInfo.wstrStateKey = m_tObjectInfo.wstrStateKey;

			m_MonsterSetting.m_tUnitInfo.vCollStart = { -pTexInfo->vOffset.x, -pTexInfo->vOffset.y, 0.f };
			m_MonsterSetting.m_tUnitInfo.vCollEnd = { pTexInfo->tImgInfo.Width - pTexInfo->vOffset.x, pTexInfo->tImgInfo.Height - pTexInfo->vOffset.y, 0.f };

			m_MonsterSetting.UpdateData(FALSE);
		}

		// Frame
		m_tFrame.iFrameCur = 0;
		m_tFrame.iFrameEnd = m_pTextureMgr->GetTexCount(wstrObjKey, wstrStateKey);
		m_tFrame.fFrameDelay = pTexInfo->m_fFrameDelay;
	}

	UpdateData(FALSE);
	Invalidate(FALSE);
}

// ������Ʈ ����Ʈ ���� ��ư
void CUnitTool::OnBnClickedSaveData()
{
	UpdateData(TRUE);

	CString strPathName;
	//CString str = _T("All files(*.*)|*.*|");
	//CFileDialog dlg(TRUE, _T("*.dat"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, str, this);
	//if (dlg.DoModal() == IDOK)
	//{
	//	strPathName = dlg.GetPathName();
	//	// ���� ��θ� ������ ����� ���, Edit Control�� �� ����
	//	//SetDlgItemText(IDC_EDIT1, strPathName);
	//	//LoadData(strPathName.GetString());
	//}

	strPathName = L"../Data/ObjData/ObjData.txt";

	wofstream SaveFile;
	//SaveFile.open(L"../Data/ObjData/ObjData.txt", ios::out);
	SaveFile.open(strPathName.GetString(), ios::out);

	if (!SaveFile.good())
		return;

	// Monster Setting SaveData();
	m_MonsterSetting.SaveData(SaveFile);
	m_EnvironSetting.SaveData(SaveFile);

	// NPC Setting SaveData();

	SaveFile.close();

	UpdateData(FALSE);
	Invalidate(FALSE);

	m_pToolView->m_pMainDialog->m_MapTool.LoadObjData();
	m_pToolView->m_pMainDialog->m_MapTool.UpdateData(FALSE);
	m_pToolView->m_pMainDialog->m_MapTool.Invalidate(FALSE);
}

// ������Ʈ ����Ʈ �ҷ����� ��ư
void CUnitTool::OnBnClickedLoadData()
{
	//// ���� �̸� ���
	//CString str = _T("All files(*.*)|*.*|");
	//CFileDialog dlg(TRUE, _T("*.dat"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, str, this);
	//if (dlg.DoModal() == IDOK)
	//{
	//	CString strPathName = dlg.GetPathName();
	//	// ���� ��θ� ������ ����� ���, Edit Control�� �� ����
	//	//SetDlgItemText(IDC_EDIT1, strPathName);
	//	LoadData(strPathName.GetString());
	//}

	CString strPathName = L"../Data/ObjData/ObjData.txt";
	wifstream LoadFile;
	LoadFile.open(strPathName, ios::in);

	if (!LoadFile.good())
		return;

	m_MonsterSetting.LoadData(LoadFile);
	m_EnvironSetting.LoadData(LoadFile);

	LoadFile.close();

	UpdateData(FALSE);
	Invalidate(FALSE);
}

// 1. �����̸��� �����
// 2. ��ŷ�ǰ� �����
// 3. ���̺� �ε�

void CUnitTool::OnDropFiles(HDROP hDropInfo)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CDialog::OnDropFiles(hDropInfo);

	// hDropInfo : �巡�� �� ������ ������ �Ǵ� �ڵ�
	// ::DragQueryFile(hDropInfo, index, FileName(out), BufferLength);
	// �巡�� �ڵ�, �巡�� ������ �ε���(��->�� ����), ������ �ּ�, ���� ������

	//// �� ��° ���ڰ� -1�� ��� �巡�� �� ��ӵ� ����� ��ü ������ ��ȯ
	//int iCount = ::DragQueryFile(hDropInfo, -1, nullptr, 0);

	//TCHAR szFilePath[MAX_STR] = L"";
	//for (int i = 0; i < iCount; ++i) {
	//	::DragQueryFile(hDropInfo, i, szFilePath, MAX_STR);
	//	m_ListBoxItemIndex.AddString(szFilePath);
	//}
}

void CUnitTool::OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	*pResult = 0;

	if (nullptr != m_pWnd) {
		m_pWnd->ShowWindow(SW_HIDE);
		m_pWnd = nullptr;
	}

	switch (m_Tab.GetCurSel()) {
	case MONSTER_SETTING:
		m_MonsterSetting.ShowWindow(SW_SHOW);
		m_pWnd = &m_MonsterSetting;
		break;

	case ENVIRON_SETTING:
		m_EnvironSetting.ShowWindow(SW_SHOW);
		m_pWnd = &m_EnvironSetting;
		break;
	}
}
