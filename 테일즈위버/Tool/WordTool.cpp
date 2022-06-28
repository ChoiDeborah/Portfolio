// WordTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "WordTool.h"
#include "afxdialogex.h"

#include "MainDialog.h"
#include "ToolView.h"

#include <locale>
#include <codecvt>

// CWordTool 대화 상자입니다.

IMPLEMENT_DYNAMIC(CWordTool, CDialog)

CWordTool::CWordTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_WORDTOOL, pParent)
	, m_strFileName(_T(""))
{

}

CWordTool::~CWordTool()
{
	for (auto& Word : m_vecWord)
	{
		SafeDelete(Word);
	}
	m_vecWord.clear();
	m_vecWord.shrink_to_fit();
}

void CWordTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_TREE1, m_TreeCtrlImage);		// 트리
	DDX_Control(pDX, IDC_SPRITE_PICTURE, m_pPicture);	// 이미지

														// 이름
	CString strName = m_tWordInfo.wstrName.c_str();
	DDX_Text(pDX, IDC_EDIT2, strName);
	m_tWordInfo.wstrName = strName;

	// 워드
	CString strWord = m_tWordInfo.wstrWord.c_str();
	DDX_Text(pDX, IDC_EDIT1, strWord);
	m_tWordInfo.wstrWord = strWord;

	DDX_Control(pDX, IDC_LIST1, m_ListBoxWord);
	DDX_Text(pDX, IDC_EDIT4, m_strFileName);
	DDX_Control(pDX, IDC_LIST3, m_ListBoxWordFile);

	CString strObjKey = m_tWordInfo.wstrObjKey.c_str();
	DDX_Text(pDX, IDC_OBJKEY, strObjKey);
	m_tWordInfo.wstrObjKey = strObjKey;

	CString strStateKey = m_tWordInfo.wstrStateKey.c_str();
	DDX_Text(pDX, IDC_STATEKEY, strStateKey);
	m_tWordInfo.wstrStateKey = strStateKey;
}


BEGIN_MESSAGE_MAP(CWordTool, CDialog)
	ON_WM_PAINT()
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, &CWordTool::OnTvnSelchangedImage)
	ON_BN_CLICKED(IDC_BUTTON18, &CWordTool::OnBnClickedImageKeyReset)
	ON_BN_CLICKED(IDC_BUTTON12, &CWordTool::OnBnClickedAddWordList)
	ON_BN_CLICKED(IDC_BUTTON10, &CWordTool::OnBnClickedButtonDeleteWordList)
	ON_BN_CLICKED(IDC_BUTTON19, &CWordTool::OnBnClickedUpdateWord)
	ON_BN_CLICKED(IDC_BUTTON1, &CWordTool::OnBnClickedCreateWordFile)
	ON_LBN_SELCHANGE(IDC_LIST3, &CWordTool::OnLbnSelchangeWordFile)
	ON_LBN_SELCHANGE(IDC_LIST1, &CWordTool::OnLbnSelchangeWordListBox)
END_MESSAGE_MAP()



// 초기화
BOOL CWordTool::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	// 워드 파일 찾기
	wstring wstrFilePath = L"../Data/WordData/*.*";
	CFileFind finder;

	BOOL bWorking = finder.FindFile(wstrFilePath.c_str());
	bool bIsSingleTexture = false;
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		if (finder.IsDots())
		{
			continue;
		}

		// 디렉토리
		if (!finder.IsDirectory()) {
			// 확장자 확인
			wstring wstrExtension = finder.GetFileName();
			wstrExtension = wstrExtension.substr(wstrExtension.size() - 4, 50);
			if (lstrcmp(wstrExtension.c_str(), L".txt"))
				continue;

			m_ListBoxWordFile.InsertString(-1, finder.GetFileTitle());
		}
	}
	finder.Close();

	// 트리 세팅
	m_hRoot = m_TreeCtrlImage.InsertItem(TEXT("Texture"), 0, 0, TVI_ROOT, TVI_LAST);
	m_TreeCtrlImage.Expand(m_hRoot, TVE_EXPAND);

	UpdateData(FALSE);
	Invalidate(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

// 엔터, ESC 막아주기
BOOL CWordTool::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	// EditBox 엔터나 ESC 때 종료 막기
	if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN))
		return TRUE;

	return CDialog::PreTranslateMessage(pMsg);
}

// 렌더
void CWordTool::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CDialog::OnPaint()을(를) 호출하지 마십시오.

	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(m_tWordInfo.wstrObjKey, m_tWordInfo.wstrStateKey);
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

// 저장 데이터 글자 조합
wstring CWordTool::SetListData(WORD_INFO* pWordInfo)
{
	wstring wstrData = pWordInfo->wstrName + L"|"
		+ pWordInfo->wstrObjKey + L"|"
		+ pWordInfo->wstrStateKey + L"|"
		+ pWordInfo->wstrWord;

	return wstrData;
}

void CWordTool::SaveData(const wstring & wstrFileName)
{
	UpdateData(TRUE);

	wstring wstrFilePath = L"../Data/WordData/" + wstrFileName + L".txt";

	wofstream SaveFile;
	SaveFile.open(wstrFilePath.c_str(), ios::out);
	SaveFile.imbue(std::locale("kor"));

	if (!SaveFile.good())
		return;

	// 개수
	SaveFile << m_vecWord.size() << '\n';

	// 데이터 저장
	for (auto& iter : m_vecWord) {
		SaveFile << SetListData(iter) << '\n';
	}

	SaveFile.close();

	UpdateData(FALSE);
	Invalidate(FALSE);
}

void CWordTool::LoadData(const wstring& wstrFileName)
{
	UpdateData(TRUE);

	wstring wstrFilePath = L"../Data/WordData/" + wstrFileName + L".txt";

	wifstream LoadFile;
	LoadFile.open(wstrFilePath.c_str(), ios::in);
	LoadFile.imbue(std::locale("kor"));

	if (!LoadFile.good())
		return;

	m_ListBoxWord.ResetContent();

	m_tWordInfo.wstrName = L"";
	m_tWordInfo.wstrObjKey = L"";
	m_tWordInfo.wstrStateKey = L"";
	m_tWordInfo.wstrWord = L"";

	for (auto& Word : m_vecWord)
	{
		SafeDelete(Word);
	}
	m_vecWord.clear();
	m_vecWord.shrink_to_fit();

	TCHAR szCount[MAX_STR] = L"";
	LoadFile.getline(szCount, MAX_STR);
	int iCount = int(_wtoi(szCount));
	if (0 >= iCount)
		return;

	m_vecWord.reserve(iCount);

	TCHAR szName[MAX_STR] = L"";		// 이름
	TCHAR szObjKey[MAX_STR] = L"";		// 텍스쳐 오브젝트 키
	TCHAR szStateKey[MAX_STR] = L"";	// 텍스쳐 상태 키
	TCHAR szWord[MAX_STR] = L"";		// 대화

	// 데이터 저장
	for (int i = 0; i < iCount; ++i) {
		LoadFile.getline(szName, MAX_STR, '|');		// 이름
		LoadFile.getline(szObjKey, MAX_STR, '|');	// 텍스쳐 오브젝트 키
		LoadFile.getline(szStateKey, MAX_STR, '|');	// 텍스쳐 상태 키
		LoadFile.getline(szWord, MAX_STR);			// 대화

		WORD_INFO* pWordInfo = new WORD_INFO;
		pWordInfo->wstrName		= szName;
		pWordInfo->wstrObjKey	= szObjKey;
		pWordInfo->wstrStateKey = szStateKey;
		pWordInfo->wstrWord		= szWord;

		m_vecWord.push_back(pWordInfo);
		m_ListBoxWord.InsertString(-1, SetListData(pWordInfo).c_str());
	}

	LoadFile.close();

	UpdateData(FALSE);
	Invalidate(FALSE);
}



// CWordTool 메시지 처리기입니다.

// 이미지 트리 초기화
void CWordTool::ResetImageTree()
{
	// 트리 전체 내용 삭제
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

// 이미지 트리 삽입
void CWordTool::InsertImageTree(SPRITE_INFO & tSpriteInfo, HTREEITEM hSingle, HTREEITEM hMulti)
{
	// 트리에서 부모 아이템 찾기
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

// 트리 클릭, 이미지 변경
void CWordTool::OnTvnSelchangedImage(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	UpdateData(TRUE);

	NULL_CHECK(m_pImageMap);

	HTREEITEM hStateKey = m_TreeCtrlImage.GetNextItem(NULL, TVGN_CARET);
	HTREEITEM hObjKey = m_TreeCtrlImage.GetNextItem(hStateKey, TVGN_PARENT);

	wstring wstrObjKey = m_TreeCtrlImage.GetItemText(hObjKey);
	wstring wstrStateKey = m_TreeCtrlImage.GetItemText(hStateKey);
	wstring wstrCmp = wstrObjKey + L"|" + wstrStateKey;

	auto iter_find = m_pImageMap->find(wstrCmp.c_str());
	if (iter_find != m_pImageMap->end()) {
		const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(iter_find->second->wstrObjKey, iter_find->second->wstrStateKey);
		NULL_CHECK(pTexInfo);

		m_tWordInfo.wstrObjKey = wstrObjKey;
		m_tWordInfo.wstrStateKey = wstrStateKey;
	}

	UpdateData(FALSE);
	Invalidate(FALSE);
}

// 이미지 키 초기화
void CWordTool::OnBnClickedImageKeyReset()
{
	UpdateData(TRUE);

	m_tWordInfo.wstrObjKey = L"";
	m_tWordInfo.wstrStateKey = L"";

	UpdateData(FALSE);
	Invalidate(FALSE);
}

// 워드 리스트 추가
void CWordTool::OnBnClickedAddWordList()
{
	UpdateData(TRUE);

	WORD_INFO* pWordInfo = new WORD_INFO;
	pWordInfo->wstrName = L"Unknown";
	pWordInfo->wstrWord = L"";
	m_vecWord.push_back(pWordInfo);
	m_ListBoxWord.InsertString(-1, L"Unknown|||");

	UpdateData(FALSE);
	Invalidate(FALSE);
}

// 워드 리스트 삭제
void CWordTool::OnBnClickedButtonDeleteWordList()
{
	UpdateData(TRUE);

	int iIndex = m_ListBoxWord.GetCurSel();
	if (-1 >= iIndex || iIndex <= int(m_vecWord.size()))
		return;

	m_ListBoxWord.DeleteString(iIndex);
	auto iter_erase = m_vecWord.begin() + iIndex;
	if (iter_erase != m_vecWord.end())
	{
		SafeDelete(*iter_erase);
		m_vecWord.erase(iter_erase);
		m_vecWord.shrink_to_fit();
	}

	UpdateData(FALSE);
	Invalidate(FALSE);
}

// 업데이트 버튼
void CWordTool::OnBnClickedUpdateWord()
{
	UpdateData(TRUE);

	int iIndex = m_ListBoxWord.GetCurSel();
	if (-1 >= iIndex || iIndex >= int(m_vecWord.size()))
		return;

	auto iter_find = m_vecWord.begin() + iIndex;
	if (iter_find != m_vecWord.end())
	{
		*(*iter_find) = m_tWordInfo;
	}

	m_ListBoxWord.DeleteString(iIndex);
	m_ListBoxWord.InsertString(iIndex, SetListData(&m_tWordInfo).c_str());

	UpdateData(FALSE);
	Invalidate(FALSE);
}

// 워드 파일 생성
void CWordTool::OnBnClickedCreateWordFile()
{
	UpdateData(TRUE);

	m_ListBoxWordFile.InsertString(-1, m_strFileName);

	UpdateData(FALSE);
	Invalidate(FALSE);
}

// 워드 파일 리스트 박스 클릭
void CWordTool::OnLbnSelchangeWordFile()
{
	UpdateData(TRUE);

	int iIndex = m_ListBoxWordFile.GetCurSel();
	if (-1 == iIndex)
		return;

	CString strFileName;
	m_ListBoxWordFile.GetText(iIndex, strFileName);

	LoadData(wstring(strFileName));

	UpdateData(FALSE);
	Invalidate(FALSE);
}

// 워드정보 리스트 박스 클릭
void CWordTool::OnLbnSelchangeWordListBox()
{
	UpdateData(TRUE);

	int iIndex = m_ListBoxWord.GetCurSel();
	if (-1 == iIndex)
		return;

	m_tWordInfo = *m_vecWord[iIndex];

	UpdateData(FALSE);
	Invalidate(FALSE);
}
