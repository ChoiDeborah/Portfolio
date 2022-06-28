// SpriteTool.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "SpriteTool.h"
#include "afxdialogex.h"

#include "MainDialog.h"
#include "ToolView.h"

#define SPRITETOOL_TIMER 100

// CSpriteTool ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CSpriteTool, CDialog)

CSpriteTool::CSpriteTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_SPRITETOOL, pParent)
	, m_iFrameScroll(0)
	, m_strAnimTime(_T(""))
	, m_bIsFrameRoof(TRUE)
	, m_strImageSize(_T(""))
	, m_strImageCount(_T(""))
	, m_bIsFrameMove(false)
	, m_strTexType(_T(""))
	, m_bIsMirror(FALSE)
{
}

CSpriteTool::~CSpriteTool()
{
	for (auto& TreeItem : m_mapTreeItem) {
		SafeDelete(TreeItem.second);
	}
	m_mapTreeItem.clear();

	for (auto& ListItem : m_mapListItem) {
		SafeDelete(ListItem.second);
	}
	m_mapListItem.clear();
}

void CSpriteTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE2, m_ForderTree);
	DDX_Control(pDX, IDC_SPRITE_PICTURE, m_SpritePicture);
	DDX_Slider(pDX, IDC_SLIDER1, m_iFrameScroll);
	DDX_Control(pDX, IDC_SLIDER1, m_Slider);
	DDX_Control(pDX, IDC_LIST2, m_ListImageData);
	DDX_Check(pDX, IDC_CHECK2, m_bIsMirror);
	DDX_Control(pDX, IDC_SLIDER2, m_SliderARGB[0]);
	DDX_Control(pDX, IDC_SLIDER3, m_SliderARGB[1]);
	DDX_Control(pDX, IDC_SLIDER4, m_SliderARGB[2]);
	DDX_Control(pDX, IDC_SLIDER5, m_SliderARGB[3]);

	DDX_Control(pDX, IDC_SLIDER9, m_SliderEraseColor[0]);
	DDX_Control(pDX, IDC_SLIDER6, m_SliderEraseColor[1]);
	DDX_Control(pDX, IDC_SLIDER7, m_SliderEraseColor[2]);
	DDX_Control(pDX, IDC_SLIDER8, m_SliderEraseColor[3]);

	DDX_Check(pDX, IDC_CHECK1, m_bIsFrameRoof);
	DDX_Control(pDX, IDC_BUTTON1, m_BtPlay);

	DDX_Text(pDX, IDC_TEX_TYPE, m_strTexType);
	DDX_Text(pDX, IDC_ANIM_TIME, m_strAnimTime);
	DDX_Text(pDX, IDC_IMAGE_SIZE, m_strImageSize);
	DDX_Text(pDX, IDC_IMAGE_NUM, m_strImageCount);

	DDX_Text(pDX, IDC_EDIT1, m_tSpriteInfo.fFrameDelay);
	DDX_Text(pDX, IDC_EDIT3, m_tSpriteInfo.vOffset.x);
	DDX_Text(pDX, IDC_EDIT4, m_tSpriteInfo.vOffset.y);
	CString strObjKey = m_wstrObjKey.c_str();
	DDX_Text(pDX, IDC_EDIT5, strObjKey);
	m_wstrObjKey = strObjKey;
	CString strStateKey = m_wstrStateKey.c_str();
	DDX_Text(pDX, IDC_EDIT6, strStateKey);
	m_wstrStateKey = strStateKey;

	DDX_Text(pDX, IDC_EDIT9, m_tSpriteInfo.tBlend.a);
	DDX_Text(pDX, IDC_EDIT2, m_tSpriteInfo.tBlend.r);
	DDX_Text(pDX, IDC_EDIT7, m_tSpriteInfo.tBlend.g);
	DDX_Text(pDX, IDC_EDIT8, m_tSpriteInfo.tBlend.b);
	DDV_MinMaxInt(pDX, m_tSpriteInfo.tBlend.a, 0, 255);
	DDV_MinMaxInt(pDX, m_tSpriteInfo.tBlend.r, 0, 255);
	DDV_MinMaxInt(pDX, m_tSpriteInfo.tBlend.g, 0, 255);
	DDV_MinMaxInt(pDX, m_tSpriteInfo.tBlend.b, 0, 255);

	DDX_Text(pDX, IDC_EDIT20, m_tSpriteInfo.tEraseColor.a);
	DDX_Text(pDX, IDC_EDIT10, m_tSpriteInfo.tEraseColor.r);
	DDX_Text(pDX, IDC_EDIT11, m_tSpriteInfo.tEraseColor.g);
	DDX_Text(pDX, IDC_EDIT12, m_tSpriteInfo.tEraseColor.b);
	DDV_MinMaxInt(pDX, m_tSpriteInfo.tEraseColor.a, 0, 255);
	DDV_MinMaxInt(pDX, m_tSpriteInfo.tEraseColor.r, 0, 255);
	DDV_MinMaxInt(pDX, m_tSpriteInfo.tEraseColor.g, 0, 255);
	DDV_MinMaxInt(pDX, m_tSpriteInfo.tEraseColor.b, 0, 255);
	DDX_Control(pDX, IDC_RADIO2, m_RadioEraseColor);
	DDX_Control(pDX, IDC_CHECK4, m_CheckDrawLine);
}


BEGIN_MESSAGE_MAP(CSpriteTool, CDialog)
	ON_WM_PAINT()
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE2, &CSpriteTool::OnTvnSelchangedTree)
	ON_BN_CLICKED(IDC_BUTTON1, &CSpriteTool::OnBnClickedFrameMoveOn)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER1, &CSpriteTool::OnNMReleasedcaptureSlider1)
	ON_WM_HSCROLL()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON3, &CSpriteTool::OnBnClickedButtonSet)
	ON_BN_CLICKED(IDC_CHECK1, &CSpriteTool::OnBnClickedCheckRoof)
	ON_BN_CLICKED(IDC_BUTTON7, &CSpriteTool::OnBnClickedInsertData)
	ON_BN_CLICKED(IDC_BUTTON9, &CSpriteTool::OnBnClickedDeleteData)
	ON_BN_CLICKED(IDC_BUTTON10, &CSpriteTool::OnBnClickedSaveDataPath)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST2, &CSpriteTool::OnLvnItemchangedSelectData)
	ON_BN_CLICKED(IDC_CHECK2, &CSpriteTool::OnBnClickedCheckMirror)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER2, &CSpriteTool::OnNMReleasedcaptureSliderA)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER3, &CSpriteTool::OnNMReleasedcaptureSliderR)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER4, &CSpriteTool::OnNMReleasedcaptureSliderG)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER5, &CSpriteTool::OnNMReleasedcaptureSliderB)

	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER9, &CSpriteTool::OnNMReleasedcaptureEraseA)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER6, &CSpriteTool::OnNMReleasedcaptureEraseR)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER7, &CSpriteTool::OnNMReleasedcaptureEraseG)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER8, &CSpriteTool::OnNMReleasedcaptureEraseB)

	ON_BN_CLICKED(IDC_BUTTON2, &CSpriteTool::OnBnClickedButtonAutoOffset)
	ON_BN_CLICKED(IDC_BUTTON11, &CSpriteTool::OnBnClickedLoadDataPath)

	ON_STN_CLICKED(IDC_SPRITE_PICTURE, &CSpriteTool::OnStnClickedSpritePicture)
	ON_BN_CLICKED(IDC_RADIO2, &CSpriteTool::OnBnClickedEraseColor)
	ON_BN_CLICKED(IDC_RADIO4, &CSpriteTool::OnBnClickedOffset)
	ON_BN_CLICKED(IDC_CHECK4, &CSpriteTool::OnBnClickedDrawLine)
END_MESSAGE_MAP()


// �ʱ�ȭ �Լ�
BOOL CSpriteTool::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	m_Slider.SetRange(0, 0);	// �ּ�~�ִ�
	m_Slider.SetRangeMin(0);	// �ּ� ũ��
	m_Slider.SetRangeMax(0);	// �ִ� ũ��
	m_Slider.SetPos(0);			// ��ġ
	m_Slider.SetTicFreq(1);		// ���� ����
	m_Slider.SetLineSize(1);	// Ű����� ������ ���� ������ ũ��

	// ����Ʈ ��Ʈ�� ����
	CRect rt;
	m_ListImageData.GetWindowRect(&rt);
	m_ListImageData.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);	// ����Ʈ ��Ʈ�ѿ� �� ǥ�� �� Item ���� �� ���� ��ü ����

	m_ListImageData.InsertColumn(0,  TEXT("ObjectKey"),	LVCFMT_LEFT, 70);
	m_ListImageData.InsertColumn(1,  TEXT("StateKey"),	LVCFMT_LEFT, 100);
	m_ListImageData.InsertColumn(2,  TEXT("S/M"),		LVCFMT_CENTER, 40);
	m_ListImageData.InsertColumn(3,  TEXT("OffsetX"),	LVCFMT_CENTER, 60);
	m_ListImageData.InsertColumn(4,  TEXT("OffsetY"),	LVCFMT_CENTER, 60);
	m_ListImageData.InsertColumn(5,  TEXT("Count"),		LVCFMT_CENTER, 50);
	m_ListImageData.InsertColumn(6,  TEXT("Mirror"),	LVCFMT_CENTER, 50);
	m_ListImageData.InsertColumn(7,  TEXT("Speed"),		LVCFMT_CENTER, 50);
	m_ListImageData.InsertColumn(8,  TEXT("Blend"),		LVCFMT_CENTER, 70);
	m_ListImageData.InsertColumn(9,  TEXT("Clear"),		LVCFMT_CENTER, 70);
	m_ListImageData.InsertColumn(10, TEXT("FilePath"),	LVCFMT_LEFT, int(rt.Width()*1.5f));

	// ���� �÷� �����̵� ��
	for (int i = 0; i < 4; ++i) {
		m_SliderARGB[i].SetRange(0, 255);
		m_SliderARGB[i].SetRangeMin(0);
		m_SliderARGB[i].SetRangeMax(255);
		m_SliderARGB[i].SetPos(m_tSpriteInfo.tBlend.dwColor[i]);
	}

	// ����� ���� �����̵� ��
	for (int i = 0; i < 4; ++i) {
		m_SliderEraseColor[i].SetRange(0, 255);
		m_SliderEraseColor[i].SetRangeMin(0);
		m_SliderEraseColor[i].SetRangeMax(255);
		m_SliderEraseColor[i].SetPos(m_tSpriteInfo.tEraseColor.dwColor[i]);
	}

	LoadData(L"../Data/ImageData.txt");

	// Ʈ�� ��� �߰�
	HTREEITEM Root = m_ForderTree.InsertItem(TEXT("Texture"), 0, 0, TVI_ROOT, TVI_LAST);
	FindDirctory(Root, L"../Texture");
	m_ForderTree.Expand(Root, TVE_EXPAND);

	m_RadioEraseColor.SetCheck(TRUE);
	m_CheckDrawLine.SetCheck(TRUE);

	return TRUE;
}

// ������Ʈ
void CSpriteTool::Update()
{
	if (m_bIsFrameMove)
		FrameMove();
}

// ����, ESC �����ֱ�
BOOL CSpriteTool::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	// EditBox ���ͳ� ESC �� ���� ����
	if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN))
		return TRUE;

	return CDialog::PreTranslateMessage(pMsg);
}

// ������ �̵�
void CSpriteTool::FrameMove()
{
	if (0 > m_tFrame.fFrameDelay)
		return;

	m_tFrame.fFrameTime += CTimeMgr::GetInstance()->GetDeltaTime();
	if (m_tFrame.fFrameDelay < m_tFrame.fFrameTime) {
		m_tFrame.fFrameTime = 0.f;
		++m_tFrame.iFrameCur;

		if (m_tFrame.iFrameCur >= m_tFrame.iFrameEnd) {
			if (m_bIsFrameRoof)
				m_tFrame.iFrameCur = 0;
			else
				m_tFrame.iFrameCur = m_tFrame.iFrameEnd - 1;
		}

		if (m_tFrame.iFrameCur <= m_Slider.GetRangeMax()) {
			m_iFrameScroll = m_tFrame.iFrameCur;
			m_Slider.SetPos(m_iFrameScroll);
			UpdateData(FALSE);
			Invalidate(FALSE);
			
		}
	}
}

// ����
void CSpriteTool::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
					   // �׸��� �޽����� ���ؼ��� CDialog::OnPaint()��(��) ȣ������ ���ʽÿ�.

	const TEX_INFO* pTexInfo = nullptr;

	TCHAR szFullPath[MAX_STR] = L"";

	if (CTextureMgr::SINGLE_TEXTURE == m_tSpriteInfo.iTexType) {
		pTexInfo = m_pTextureMgr->GetTexInfo(m_tSpriteInfo.wstrObjKey, m_tSpriteInfo.wstrStateKey, 0);

		lstrcpy(szFullPath, m_tSpriteInfo.wstrFilePath.c_str());
	}
	else {
		pTexInfo = m_pTextureMgr->GetTexInfo(m_tSpriteInfo.wstrObjKey, m_tSpriteInfo.wstrStateKey, m_tFrame.iFrameCur);
		swprintf_s(szFullPath, m_tSpriteInfo.wstrFilePath.c_str(), m_tFrame.iFrameCur);
	}
	NULL_CHECK(pTexInfo);

	DWORD dwEraseColorColor = D3DCOLOR_ARGB(m_tSpriteInfo.tEraseColor.a, m_tSpriteInfo.tEraseColor.r, m_tSpriteInfo.tEraseColor.g, m_tSpriteInfo.tEraseColor.b);
	LPDIRECT3DTEXTURE9 pTexture = nullptr;

	HRESULT hr = D3DXCreateTextureFromFileEx(
		m_pDeviceMgr->GetDevice(), /* ��ġ */
		szFullPath, /* �̹��� ���� ��� */
		pTexInfo->tImgInfo.Width, /* �ؽ�ó ���� ũ�� */
		pTexInfo->tImgInfo.Height, /* �ؽ�ó ���� ũ�� */
		pTexInfo->tImgInfo.MipLevels, /* �ӷ���, �Ӹ��� �ܰ� */
		0, /* �ؽ�ó ��� ��� */
		pTexInfo->tImgInfo.Format, /* �ؽ�ó  �ȼ� ���� */
		D3DPOOL_MANAGED, /* �޸� ��� ��� */
		D3DX_DEFAULT, /* ���͸� ��� */
		D3DX_DEFAULT, /* �Ӹ� ���͸� ��� */
		dwEraseColorColor, /* ������ ���� */
		nullptr, /* D3DXIMAGE_INFO�� ������ */
		nullptr, /* PALETTEENTRY�� ������ */
		&pTexture /* LPDIRECT3DTEXTURE9�� ������*/);
	FAILED_CHECK_MSG(hr, L"D3DXCreateTextureFromFileEx Failed");

	pTexInfo->pTexture->Release();
	const_cast<TEX_INFO*>(pTexInfo)->pTexture = pTexture;


	////// Erase Color
	/*//////////////////////////////////////////////////////////////////////
	D3DLOCKED_RECT LockRect;

	pTexInfo->pTexture->LockRect(0, &LockRect, 0, D3DLOCK_DISCARD);
	DWORD* pColor = (DWORD*)LockRect.pBits; // ����� �ȼ� �� �޾ƿ���
	int width = pTexInfo->tImgInfo.Width;
	int height = pTexInfo->tImgInfo.Height; // �ȼ� ������
	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			int iIndex = y * width + x;
			D3DXCOLOR xclr = pColor[iIndex];

			if (xclr.a == 0.f)
				continue;

			xclr.r += 0.2f * (1 - xclr.r);
			xclr.g += 0.2f * (1 - xclr.g);
			xclr.b += 0.2f * (1 - xclr.b);

			//if (m_tSpriteInfo.tEraseColor.r / 255.f > xclr.r && m_tSpriteInfo.tEraseColor.g / 255.f > xclr.g && m_tSpriteInfo.tEraseColor.b / 255.f > xclr.b)
			//	xclr.a = 0;
				
			pColor[iIndex] = xclr;
		}
	}

	//pTexInfo->pTexture->UnlockRect(0); //  ��� ����*/
	////////////////////////////////////////////////////////////////////

	// ���� ���� ����

	m_pDeviceMgr->GetSprite()->End();
	m_pDeviceMgr->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);

	// pixel1 = D3DRS_SRCBLEND * D3DBLEND_SRCCOLOR
	// D3DRS_SRCBLEND : ���� �׷����� �̹����� �ȼ�(RGBA)
	// D3DBLEND_SRCCOLOR : D3DRS_SRCBLEND�� RGBA�� 0~1������ ȯ���� ��
	m_pDeviceMgr->GetDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCCOLOR);

	// pixel2 = D3DRS_DESTBLEND * D3DBLEND_INVSRCCOLOR
	// D3DRS_DESTBLEND : �̹� �ĸ���ۿ� �׷��� �̹����� �ȼ� (RGBA)
	// D3DBLEND_INVSRCCOLOR : D3DBLEND_SRCCOLOR�� ���� ��
	m_pDeviceMgr->GetDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCCOLOR);
	m_pDeviceMgr->GetDevice()->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);

	// �׸���

	m_pDeviceMgr->GetSprite()->End();
	m_pDeviceMgr->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);

	////////////////////////////////////////////////////////////////////

	UpdateData(TRUE);

	//m_pDeviceMgr->GetDevice()->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	////m_pDeviceMgr->GetDevice()->SetRenderState(D3DRS_ZENABLE, TRUE);
	//m_pDeviceMgr->GetDevice()->SetRenderState(D3DRS_ALPHAREF, 0);
	//m_pDeviceMgr->GetDevice()->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_NOTEQUAL);

	// Color Key ���
	// �ȼ��� ������ �ִ�
	// ���伥���� ����� ����ä���� �����ؼ� ����
	// TGA, PNG, DDS���� ������ ���
	// SetRenderState�� ���ĸ� ����Ѵٴ� �ɼ��� ����
	//m_pDeviceMgr->GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	//m_pDeviceMgr->GetDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);			//source blend factor
	//m_pDeviceMgr->GetDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);		//dest blend factor

	// SetTextureStage���� FACTOR�� ������ �����Ѵ�
	// �̹����� ������ �ִ� ���İ��� �ٽ� ���İ��� �����Ҽ� �ִ�.
	// �ȼ�Į��(RGB) * �ȼ����İ�(100 % �ΰ��) * ���ϴ� ���İ�(100 % �ΰ��)
	/*m_pDeviceMgr->GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pDeviceMgr->GetDevice()->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pDeviceMgr->GetDevice()->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pDeviceMgr->GetDevice()->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
	m_pDeviceMgr->GetDevice()->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(255, 255, 255, 255));*/


	m_pDeviceMgr->Render_Begin();

	


	D3DXMATRIX matScale, matTrans, matWorld;
	float fScaleW = float(VIEWCX) / pTexInfo->tImgInfo.Width;
	float fScaleH = float(VIEWCY) / pTexInfo->tImgInfo.Height;

	D3DXMatrixScaling(&matScale, fScaleW * m_tSpriteInfo.iMirrorX, fScaleH, 0.f);
	D3DXMatrixTranslation(&matTrans, VIEWCX * 0.5f, VIEWCY * 0.5f, 0.f);
	matWorld = matScale * matTrans;
	m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3{ pTexInfo->tImgInfo.Width * 0.5f, pTexInfo->tImgInfo.Height * 0.5f, 0.f }, nullptr, D3DCOLOR_ARGB(m_tSpriteInfo.tBlend.a, m_tSpriteInfo.tBlend.r, m_tSpriteInfo.tBlend.g, m_tSpriteInfo.tBlend.b));
	m_pDeviceMgr->Render_End(m_SpritePicture.m_hWnd);

	//m_pDeviceMgr->GetDevice()->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	NULL_CHECK(m_pToolView);
	m_pToolView->Invalidate(FALSE);
}

// ���͸� ã��
void CSpriteTool::FindDirctory(HTREEITEM & hParent, wstring wstrFilePath)
{
	wstring wstrFilePathAdd = wstrFilePath;
	wstrFilePathAdd += L"/*.*";

	CFileFind finder;
	BOOL bWorking = finder.FindFile(wstrFilePathAdd.c_str());
	bool bIsSingleTexture = false;
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		if (finder.IsDots())
		{
			continue;
		}

		// ���丮
		if (finder.IsDirectory()) {
			// ��� �߰� + ���
			HTREEITEM hChild = m_ForderTree.InsertItem(finder.GetFileTitle(), 0, 0, hParent, TVI_LAST);
			wstring wstrPath = wstrFilePath;
			wstrPath += L"/";
			wstrPath += finder.GetFileName();
			FindDirctory(hChild, wstrPath);
		}
		// �̹��� ����
		else {
			// Ȯ���� Ȯ��
			wstring wstrExtension = finder.GetFileName();
			wstrExtension = wstrExtension.substr(wstrExtension.size()-4, 50);
			if (lstrcmp(wstrExtension.c_str(), L".png"))
				continue;

			wstring wstrObjKey;
			wstring wstrStateKey;
			wstring wstrFindNum = finder.GetFileTitle();

			bIsSingleTexture = false;

			// ���� �̹��� �� ��
			if (_wtoi(wstrFindNum.c_str()) != 0 || wstrFindNum.compare(L"0") == 0) 
			{
				// ������ƮŰ : �θ��� �θ� ����, ����Ű : �θ� ����
				//wstrFindNum.erase(remove(wstrFindNum.begin(), wstrFindNum.end(), to_wstring(iImageNum)), wstrFindNum.end());

				wstrObjKey = m_ForderTree.GetItemText(m_ForderTree.GetNextItem(hParent, TVGN_PARENT));
				wstrStateKey = m_ForderTree.GetItemText(hParent);
			}
			// ���� �̹����� �ƴ� ��
			else 
			{
				// ������ƮŰ : �θ� ����, ����Ű : ���� ���� �̸�
				wstrObjKey = m_ForderTree.GetItemText(hParent); //m_ForderTree.GetItemText(m_ForderTree.GetNextItem(m_ForderTree.GetNextItem(hParent, TVGN_PARENT), TVGN_PARENT));
				wstrStateKey = finder.GetFileTitle();
				bIsSingleTexture = true;
			}

			wstring wstrCmp = wstrObjKey + wstrStateKey;

			auto iter_find = m_mapTreeItem.find(wstrCmp.c_str());
			if (iter_find == m_mapTreeItem.end()) {
				// �ʿ� ��� wstring �߰�
				SPRITE_INFO* pSpriteInfo = new SPRITE_INFO;

				pSpriteInfo->wstrFilePath = wstrFilePath + L"/";
				pSpriteInfo->tBlend = { 255, 255, 255, 255 };
				pSpriteInfo->tEraseColor = { 255, 0, 0, 0 };
				pSpriteInfo->vOffset = { 0.f, 0.f, 0.f };
				pSpriteInfo->iMirrorX = 1;
				pSpriteInfo->fFrameDelay = 0.1f;
				pSpriteInfo->iCount = 1;
				pSpriteInfo->iTexType = (bIsSingleTexture) ? CTextureMgr::SINGLE_TEXTURE : CTextureMgr::MULTI_TEXTURE;
				
				// �̱� �ؽ���
				if (bIsSingleTexture)
				{
					pSpriteInfo->wstrObjKey = wstrStateKey;
					pSpriteInfo->wstrStateKey = L"";

					pSpriteInfo->wstrFilePath += finder.GetFileName();
					m_pTextureMgr->LoadTexture(CTextureMgr::SINGLE_TEXTURE, pSpriteInfo->wstrFilePath, pSpriteInfo->wstrObjKey, pSpriteInfo->wstrStateKey, 0.f, 0.f, 1);

					wstring wstrName = finder.GetFileTitle();
					// ��� �߰�
					m_ForderTree.InsertItem(wstrName.c_str(), 0, 0, hParent, TVI_LAST);

					m_mapListSingle.insert({ wstrCmp.c_str(), pSpriteInfo });
				}

				// ��Ƽ �ؽ���
				else 
				{
					pSpriteInfo->wstrObjKey = wstrObjKey;
					pSpriteInfo->wstrStateKey = wstrStateKey;
					pSpriteInfo->wstrFilePath += L"%d";
					pSpriteInfo->wstrFilePath += wstrExtension;
				}

				m_mapTreeItem.insert({ wstrCmp.c_str(), pSpriteInfo });
			}
			else if (!bIsSingleTexture){
				++iter_find->second->iCount;
			}
		}
	}
	finder.Close();

	auto iter_begin = m_mapTreeItem.begin();
	auto iter_end = m_mapTreeItem.end();
	for (; iter_begin != iter_end; ++iter_begin) {
		if (CTextureMgr::MULTI_TEXTURE == iter_begin->second->iTexType)
			m_pTextureMgr->LoadTexture(CTextureMgr::MULTI_TEXTURE, iter_begin->second->wstrFilePath, iter_begin->second->wstrObjKey, iter_begin->second->wstrStateKey, -0.5f, -0.5f, iter_begin->second->iCount);
	}
}

// ����Ʈ��Ʈ�ѿ� ���� ������ ����
wstring CSpriteTool::SetListData(SPRITE_INFO* pSpriteInfo)
{
	TCHAR szOffsetX[MAX_STR];
	swprintf_s(szOffsetX, L"%.2f", pSpriteInfo->vOffset.x);
	TCHAR szOffsetY[MAX_STR];
	swprintf_s(szOffsetY, L"%.2f", pSpriteInfo->vOffset.y);
	TCHAR szFrameDelay[MAX_STR];
	swprintf_s(szFrameDelay, L"%.2f", pSpriteInfo->fFrameDelay);

	wstring wstrData = pSpriteInfo->wstrObjKey + L"|";
	wstrData += pSpriteInfo->wstrStateKey + L"|";
	wstrData += to_wstring(pSpriteInfo->iTexType) + L"|";
	wstrData += wstring(szOffsetX) + L"|";
	wstrData += wstring(szOffsetY) + L"|";
	wstrData += to_wstring(pSpriteInfo->iCount) + L"|";
	wstrData += to_wstring(pSpriteInfo->iMirrorX) + L"|";
	wstrData += wstring(szFrameDelay) + L"|";
	wstrData += to_wstring(pSpriteInfo->GetBlendColor()) + L"|";
	wstrData += to_wstring(pSpriteInfo->GetEraseColor()) + L"|";
	wstrData += pSpriteInfo->wstrFilePath;

	return wstrData;
}

// ������ �ҷ�����
void CSpriteTool::LoadData(wstring wstrFilePath)
{
	wifstream LoadFile;
	LoadFile.open(wstrFilePath.c_str(), ios::in);

	if (!LoadFile.good())
		return;

	m_mapListItem.clear();
	m_ListImageData.DeleteAllItems();

	// �̹��� ����
	TCHAR szImageNum[MAX_STR] = L"";
	LoadFile.getline(szImageNum, MAX_STR);

	TCHAR szObjKey[MAX_STR] = L"";
	TCHAR szStateKey[MAX_STR] = L"";
	TCHAR szTexType[MAX_STR] = L"";
	TCHAR szOffsetX[MAX_STR] = L"";
	TCHAR szOffsetY[MAX_STR] = L"";
	TCHAR szCount[MAX_STR] = L"";
	TCHAR szMirror[MAX_STR] = L"";
	TCHAR szFrameDelay[MAX_STR] = L"";
	TCHAR szBlend[MAX_STR] = L"";
	TCHAR szEraseColorColor[MAX_STR] = L"";
	TCHAR szFilePath[MAX_STR] = L"";

	while (!LoadFile.eof()) {
		LoadFile.getline(szObjKey, MAX_STR, '|');
		LoadFile.getline(szStateKey, MAX_STR, '|');
		LoadFile.getline(szTexType, MAX_STR, '|');
		LoadFile.getline(szOffsetX, MAX_STR, '|');
		LoadFile.getline(szOffsetY, MAX_STR, '|');
		LoadFile.getline(szCount, MAX_STR, '|');
		LoadFile.getline(szMirror, MAX_STR, '|');
		LoadFile.getline(szFrameDelay, MAX_STR, '|');
		LoadFile.getline(szBlend, MAX_STR, '|');
		LoadFile.getline(szEraseColorColor, MAX_STR, '|');
		LoadFile.getline(szFilePath, MAX_STR);

		CString strMapKey = szObjKey;
		strMapKey += '|';
		strMapKey += szStateKey;

		auto& iter_find = m_mapListItem.find(strMapKey);
		if (iter_find != m_mapListItem.end())
			continue;

		SPRITE_INFO* pSpriteInfo = new SPRITE_INFO;

		DWORD dwBlend = DWORD(_wtoll(szBlend));
		DWORD dwEraseColor = DWORD(_wtoll(szEraseColorColor));

		pSpriteInfo->wstrObjKey = szObjKey;
		pSpriteInfo->wstrStateKey = szStateKey;
		pSpriteInfo->iTexType = (CTextureMgr::SINGLE_TEXTURE == int(_wtoi(szTexType))) ? CTextureMgr::SINGLE_TEXTURE : CTextureMgr::MULTI_TEXTURE;
		pSpriteInfo->vOffset.x = float(_wtof(szOffsetX));
		pSpriteInfo->vOffset.y = float(_wtof(szOffsetY));
		pSpriteInfo->iCount = int(_wtoi(szCount));
		pSpriteInfo->iMirrorX = int(_wtoi(szMirror));
		pSpriteInfo->fFrameDelay = float(_wtof(szFrameDelay));
		pSpriteInfo->tBlend = TARGB(D3DCOLOR_GET_A(dwBlend), D3DCOLOR_GET_R(dwBlend), D3DCOLOR_GET_G(dwBlend), D3DCOLOR_GET_B(dwBlend));
		pSpriteInfo->tEraseColor = TARGB(D3DCOLOR_GET_A(dwEraseColor), D3DCOLOR_GET_R(dwEraseColor), D3DCOLOR_GET_G(dwEraseColor), D3DCOLOR_GET_B(dwEraseColor));
		pSpriteInfo->wstrFilePath = szFilePath;

		int iNum = m_ListImageData.GetItemCount();

		TCHAR wstrOffsetX[MAX_STR];
		swprintf_s(wstrOffsetX, L"%.2f", pSpriteInfo->vOffset.x);
		TCHAR wstrOffsetY[MAX_STR];
		swprintf_s(wstrOffsetY, L"%.2f", pSpriteInfo->vOffset.y);
		TCHAR wstrFrameSpeed[MAX_STR];
		swprintf_s(wstrFrameSpeed, L"%.2f", pSpriteInfo->fFrameDelay);

		m_ListImageData.InsertItem(iNum, pSpriteInfo->wstrObjKey.c_str());
		m_ListImageData.SetItem(iNum, 1, LVIF_TEXT, pSpriteInfo->wstrStateKey.c_str(), NULL, NULL, NULL, NULL);
		m_ListImageData.SetItem(iNum, 2, LVIF_TEXT, to_wstring(pSpriteInfo->iTexType).c_str(), NULL, NULL, NULL, NULL);
		m_ListImageData.SetItem(iNum, 3, LVIF_TEXT, wstrOffsetX, NULL, NULL, NULL, NULL);
		m_ListImageData.SetItem(iNum, 4, LVIF_TEXT, wstrOffsetY, NULL, NULL, NULL, NULL);
		m_ListImageData.SetItem(iNum, 5, LVIF_TEXT, to_wstring(pSpriteInfo->iCount).c_str(), NULL, NULL, NULL, NULL);
		m_ListImageData.SetItem(iNum, 6, LVIF_TEXT, to_wstring(pSpriteInfo->iMirrorX).c_str(), NULL, NULL, NULL, NULL);
		m_ListImageData.SetItem(iNum, 7, LVIF_TEXT, wstrFrameSpeed, NULL, NULL, NULL, NULL);
		m_ListImageData.SetItem(iNum, 8, LVIF_TEXT, to_wstring(dwBlend).c_str(), NULL, NULL, NULL, NULL);
		m_ListImageData.SetItem(iNum, 9, LVIF_TEXT, to_wstring(dwEraseColor).c_str(), NULL, NULL, NULL, NULL);
		m_ListImageData.SetItem(iNum, 10,LVIF_TEXT, pSpriteInfo->wstrFilePath.c_str(), NULL, NULL, NULL, NULL);
		
		m_pTextureMgr->LoadTexture(CTextureMgr::TEXTURE_TYPE(pSpriteInfo->iTexType), pSpriteInfo->wstrFilePath, pSpriteInfo->wstrObjKey, pSpriteInfo->wstrStateKey, pSpriteInfo->vOffset.x, pSpriteInfo->vOffset.y, pSpriteInfo->iCount, pSpriteInfo->iMirrorX, pSpriteInfo->fFrameDelay, dwBlend, dwEraseColor);

		m_mapListItem.insert({ strMapKey, pSpriteInfo }).first->second;
	}

	LoadFile.close();
}

// �ɼ� ������ ������Ʈ
void CSpriteTool::UpdateSetting(const TEX_INFO* pTexInfo)
{
	NULL_CHECK(pTexInfo);

	// Frame
	m_tFrame.iFrameCur = 0;
	m_tFrame.iFrameEnd = m_pTextureMgr->GetTexCount(m_tSpriteInfo.wstrObjKey, m_tSpriteInfo.wstrStateKey);
	m_tFrame.fFrameDelay = m_tSpriteInfo.fFrameDelay;

	m_Slider.SetRange(0, m_tFrame.iFrameEnd - 1);
	m_Slider.SetRangeMin(0);
	m_Slider.SetRangeMax(m_tFrame.iFrameEnd - 1);
	m_Slider.SetPos(0);
	m_iFrameScroll = 0;

	// ARGB
	for (int i = 0; i < 4; ++i) 
		m_SliderARGB[i].SetPos(m_tSpriteInfo.tBlend.dwColor[i]);

	for (int i = 0; i < 4; ++i)
		m_SliderEraseColor[i].SetPos(m_tSpriteInfo.tEraseColor.dwColor[i]);

	// Texture Type
	m_strTexType = (CTextureMgr::SINGLE_TEXTURE == m_tSpriteInfo.iTexType) ? "SingleTex" : "MultiTex";

	// ImageSize
	m_strImageSize = to_wstring(pTexInfo->tImgInfo.Width).c_str();
	m_strImageSize += " x ";
	m_strImageSize += to_wstring(pTexInfo->tImgInfo.Height).c_str();

	// ImageCount
	m_strImageCount = to_wstring(m_tFrame.iFrameEnd).c_str();
	m_strImageCount += "��";

	m_bIsMirror = (1 == m_tSpriteInfo.iMirrorX) ? false : true;

	// AnimTime
	TCHAR wstrAnimTime[MAX_STR];
	swprintf_s(wstrAnimTime, L"%.2f��", m_tSpriteInfo.fFrameDelay * m_tSpriteInfo.iCount);
	m_strAnimTime = wstrAnimTime;

	UpdateData(FALSE);
	Invalidate(FALSE);
}


// CSpriteTool �޽��� ó�����Դϴ�.
// �ִϸ��̼� ��� ��ư
void CSpriteTool::OnBnClickedFrameMoveOn()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (m_bIsFrameMove ^= true) {
		//SetTimer(SPRITETOOL_TIMER, 1, 0);
		m_BtPlay.SetWindowText(L"||");
	}
	else {
		//KillTimer(SPRITETOOL_TIMER);
		m_BtPlay.SetWindowText(L"��");
	}
}

// �̹��� ��� Ŭ��
void CSpriteTool::OnTvnSelchangedTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	HTREEITEM hStateKey = m_ForderTree.GetNextItem(NULL, TVGN_CARET);
	HTREEITEM hObjKey = m_ForderTree.GetNextItem(hStateKey, TVGN_PARENT);

	wstring wstrObjKey = m_ForderTree.GetItemText(hObjKey);
	wstring wstrStateKey = m_ForderTree.GetItemText(hStateKey);

	auto iter_find = m_mapTreeItem.find(wstring(wstrObjKey + wstrStateKey).c_str());
	if (iter_find != m_mapTreeItem.end()) {
		const TEX_INFO* pTexInfo = nullptr;

		if (CTextureMgr::MULTI_TEXTURE == iter_find->second->iTexType) {
			pTexInfo = m_pTextureMgr->GetTexInfo(iter_find->second->wstrObjKey, iter_find->second->wstrStateKey);
			NULL_CHECK(pTexInfo);

			m_wstrObjKey = wstrObjKey;
			m_wstrStateKey = wstrStateKey;
		}
		else {
			pTexInfo = m_pTextureMgr->GetTexInfo(iter_find->second->wstrObjKey, iter_find->second->wstrStateKey);
			NULL_CHECK(pTexInfo);

			m_wstrObjKey = iter_find->second->wstrObjKey;
			m_wstrStateKey = iter_find->second->wstrStateKey;
		}

		m_tSpriteInfo = *iter_find->second;

		m_pSpriteInfo = nullptr;

		UpdateSetting(pTexInfo);
	}
}

// �ִϸ��̼� �����̵�
void CSpriteTool::OnNMReleasedcaptureSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	m_iFrameScroll = m_Slider.GetPos();
	m_tFrame.iFrameCur = m_iFrameScroll;
	UpdateData(FALSE);
	Invalidate(FALSE);
}

// �����̴�
void CSpriteTool::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	if (pScrollBar)
	{
		// � �����̴����� �˻�
		if (pScrollBar == (CScrollBar*)&m_Slider)
		{
			// �����̴��� ��ġ�� �˻��Ѵ�.
			m_iFrameScroll = m_Slider.GetPos();
			UpdateData(FALSE);
			Invalidate(FALSE);
			
		}
	}
	else
	{
		// CScrollView�� ��ӹ��� ���� ��� �������� ��ũ�ѷ� ���۽� pScrollBar�� NULL�ȴ�.
	}
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

// ������ Update ��ư
void CSpriteTool::OnBnClickedButtonSet()
{
	UpdateData(TRUE);
		

	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(m_tSpriteInfo.wstrObjKey, m_tSpriteInfo.wstrStateKey, 0);
	UpdateSetting(pTexInfo);

	m_tFrame.fFrameDelay = m_tSpriteInfo.fFrameDelay;

	TCHAR wstrAnimTime[MAX_STR];
	swprintf_s(wstrAnimTime, L"%.2f��", m_tSpriteInfo.fFrameDelay * m_tSpriteInfo.iCount);
	m_strAnimTime = wstrAnimTime;

	if (nullptr != m_pSpriteInfo)
		*m_pSpriteInfo = m_tSpriteInfo;

	UpdateData(FALSE);
	Invalidate(FALSE);
}

// �ִϸ��̼� ���� ��ư
void CSpriteTool::OnBnClickedCheckRoof()
{
	UpdateData(TRUE);
}

// ������ ���� ��ư
void CSpriteTool::OnBnClickedInsertData()
{
	UpdateData(TRUE);
	
	CString strMapKey = wstring(m_wstrObjKey + L"|" + m_wstrStateKey).c_str();
	auto& iter_find = m_mapListItem.find(strMapKey);
	if (iter_find != m_mapListItem.end())
		return;

	int iNum = m_ListImageData.GetItemCount();

	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(m_tSpriteInfo.wstrObjKey, m_tSpriteInfo.wstrStateKey, 0);
	NULL_CHECK(pTexInfo);

	/*
	float fOffestX = -m_tSpriteInfo.vOffset.x / pTexInfo->tImgInfo.Width;
	float fOffestY = -m_tSpriteInfo.vOffset.y / pTexInfo->tImgInfo.Height;*/
	float fOffsetX = m_tSpriteInfo.vOffset.x;
	float fOffsetY = m_tSpriteInfo.vOffset.y;

	TCHAR wstrOffsetX[MAX_STR];
	swprintf_s(wstrOffsetX, L"%.2f", fOffsetX);
	TCHAR wstrOffsetY[MAX_STR];
	swprintf_s(wstrOffsetY, L"%.2f", fOffsetY);
	TCHAR wstrFrameSpeed[MAX_STR];
	swprintf_s(wstrFrameSpeed, L"%.2f", m_tSpriteInfo.fFrameDelay);

	DWORD dwBlend = D3DCOLOR_ARGB(m_tSpriteInfo.tBlend.a, m_tSpriteInfo.tBlend.r, m_tSpriteInfo.tBlend.g, m_tSpriteInfo.tBlend.b);
	DWORD dwEraseColor = D3DCOLOR_ARGB(m_tSpriteInfo.tEraseColor.a, m_tSpriteInfo.tEraseColor.r, m_tSpriteInfo.tEraseColor.g, m_tSpriteInfo.tEraseColor.b);

	m_ListImageData.InsertItem(iNum, m_wstrObjKey.c_str());
	m_ListImageData.SetItem(iNum, 1, LVIF_TEXT, m_wstrStateKey.c_str(), NULL, NULL, NULL, NULL);
	m_ListImageData.SetItem(iNum, 2, LVIF_TEXT, to_wstring(m_tSpriteInfo.iTexType).c_str(), NULL, NULL, NULL, NULL);
	m_ListImageData.SetItem(iNum, 3, LVIF_TEXT, wstrOffsetX, NULL, NULL, NULL, NULL);
	m_ListImageData.SetItem(iNum, 4, LVIF_TEXT, wstrOffsetY, NULL, NULL, NULL, NULL);
	m_ListImageData.SetItem(iNum, 5, LVIF_TEXT, to_wstring(m_tSpriteInfo.iCount).c_str(), NULL, NULL, NULL, NULL);
	m_ListImageData.SetItem(iNum, 6, LVIF_TEXT, to_wstring(m_tSpriteInfo.iMirrorX).c_str(), NULL, NULL, NULL, NULL);
	m_ListImageData.SetItem(iNum, 7, LVIF_TEXT, wstrFrameSpeed, NULL, NULL, NULL, NULL);
	m_ListImageData.SetItem(iNum, 8, LVIF_TEXT, to_wstring(dwBlend).c_str(), NULL, NULL, NULL, NULL);
	m_ListImageData.SetItem(iNum, 9, LVIF_TEXT, to_wstring(dwEraseColor).c_str(), NULL, NULL, NULL, NULL);
	m_ListImageData.SetItem(iNum, 10,LVIF_TEXT, m_tSpriteInfo.wstrFilePath.c_str(), NULL, NULL, NULL, NULL);
	
	if (0.f <= fOffsetX)
		fOffsetX = -fOffsetX / pTexInfo->tImgInfo.Width;
	if (0.f <= fOffsetY)
		fOffsetY = -fOffsetY / pTexInfo->tImgInfo.Height;

	m_pTextureMgr->LoadTexture(CTextureMgr::MULTI_TEXTURE, m_tSpriteInfo.wstrFilePath, m_wstrObjKey, m_wstrStateKey, fOffsetX, fOffsetY, m_tSpriteInfo.iCount);

	m_pSpriteInfo = m_mapListItem.insert({ strMapKey, new SPRITE_INFO(m_tSpriteInfo) }).first->second;
	m_pSpriteInfo->wstrObjKey = m_wstrObjKey;
	m_pSpriteInfo->wstrStateKey = m_wstrStateKey;


	m_pToolView->m_pMainDialog->LoadImageData(L"../Data/ImageData.txt");

	UpdateData(FALSE);
	Invalidate(FALSE);
}

// ������ ���� ��ư
void CSpriteTool::OnBnClickedDeleteData()
{
	UpdateData(TRUE);

	int nItemCount = m_ListImageData.GetSelectedCount();
	POSITION pos = m_ListImageData.GetFirstSelectedItemPosition();
	for (int i = 0; i < nItemCount; i++)
	{
		int nIndex = m_ListImageData.GetNextSelectedItem(pos);
		CString strMapKey = m_ListImageData.GetItemText(nIndex, 0) + "|" + m_ListImageData.GetItemText(nIndex, 1);
		auto iter_find = m_mapListItem.find(strMapKey);
		if (iter_find == m_mapListItem.end())
			continue;

		m_mapListItem.erase(iter_find);
		m_ListImageData.DeleteItem(nIndex);
	}

	m_pSpriteInfo = nullptr;

	UpdateData(FALSE);
	Invalidate(FALSE);
	
}

// ����Ʈ ������ ����
void CSpriteTool::OnLvnItemchangedSelectData(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);

	int nItemCount = m_ListImageData.GetSelectedCount();
	POSITION pos = m_ListImageData.GetFirstSelectedItemPosition();
	int nIndex = m_ListImageData.GetNextSelectedItem(pos);
	CString strMapKey = m_ListImageData.GetItemText(nIndex, 0) + "|" + m_ListImageData.GetItemText(nIndex, 1);
	auto iter_find = m_mapListItem.find(strMapKey);
	if (iter_find == m_mapListItem.end())
		return;

	SPRITE_INFO* pSpriteInfo = iter_find->second;
	NULL_CHECK(pSpriteInfo);

	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(pSpriteInfo->wstrObjKey, pSpriteInfo->wstrStateKey);
	NULL_CHECK(pTexInfo);

	m_pSpriteInfo = pSpriteInfo;
	m_tSpriteInfo = *m_pSpriteInfo;
	m_wstrObjKey = pSpriteInfo->wstrObjKey;
	m_wstrStateKey = pSpriteInfo->wstrStateKey;

	UpdateSetting(pTexInfo);
	
	UpdateData(FALSE);
	Invalidate(FALSE);
}

// �¿� ���� üũ �ڽ�
void CSpriteTool::OnBnClickedCheckMirror()
{
	UpdateData(TRUE);

	m_tSpriteInfo.iMirrorX = (m_bIsMirror) ? -1 : 1;

	UpdateData(FALSE);
	Invalidate(FALSE);
}

// �÷� �� �����̵�
void CSpriteTool::OnNMReleasedcaptureSliderA(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	*pResult = 0;

	UpdateData(TRUE);

	for (int i = 0; i<4; ++i)
		m_tSpriteInfo.tBlend.dwColor[i] = m_SliderARGB[i].GetPos();

	UpdateData(FALSE);
	Invalidate(FALSE);
	
}
void CSpriteTool::OnNMReleasedcaptureSliderR(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	*pResult = 0;

	UpdateData(TRUE);

	for (int i = 0; i<4; ++i)
		m_tSpriteInfo.tBlend.dwColor[i] = m_SliderARGB[i].GetPos();

	UpdateData(FALSE);
	Invalidate(FALSE);
	
}
void CSpriteTool::OnNMReleasedcaptureSliderG(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	*pResult = 0;

	UpdateData(TRUE);

	for (int i = 0; i<4; ++i)
		m_tSpriteInfo.tBlend.dwColor[i] = m_SliderARGB[i].GetPos();

	UpdateData(FALSE);
	Invalidate(FALSE);
	
}
void CSpriteTool::OnNMReleasedcaptureSliderB(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	*pResult = 0;

	UpdateData(TRUE);

	for (int i=0; i<4; ++i)
		m_tSpriteInfo.tBlend.dwColor[i] = m_SliderARGB[i].GetPos();

	UpdateData(FALSE);
	Invalidate(FALSE);
	
}

void CSpriteTool::OnNMReleasedcaptureEraseR(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	*pResult = 0;
	UpdateData(TRUE);

	for (int i = 0; i<4; ++i)
		m_tSpriteInfo.tEraseColor.dwColor[i] = m_SliderEraseColor[i].GetPos();

	UpdateData(FALSE);
	Invalidate(FALSE);
}
void CSpriteTool::OnNMReleasedcaptureEraseG(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	*pResult = 0;
	UpdateData(TRUE);

	for (int i = 0; i<4; ++i)
		m_tSpriteInfo.tEraseColor.dwColor[i] = m_SliderEraseColor[i].GetPos();

	UpdateData(FALSE);
	Invalidate(FALSE);
}
void CSpriteTool::OnNMReleasedcaptureEraseB(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	*pResult = 0;

	UpdateData(TRUE);

	for (int i = 0; i<4; ++i)
		m_tSpriteInfo.tEraseColor.dwColor[i] = m_SliderEraseColor[i].GetPos();

	UpdateData(FALSE);
	Invalidate(FALSE);
}
void CSpriteTool::OnNMReleasedcaptureEraseA(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	*pResult = 0;
	UpdateData(TRUE);

	for (int i = 0; i<4; ++i)
		m_tSpriteInfo.tEraseColor.dwColor[i] = m_SliderEraseColor[i].GetPos();

	UpdateData(FALSE);
	Invalidate(FALSE);
}

// ������ �ڵ� ���� ��ư
void CSpriteTool::OnBnClickedButtonAutoOffset()
{
	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(m_tSpriteInfo.wstrObjKey, m_tSpriteInfo.wstrStateKey, m_tFrame.iFrameCur);
	NULL_CHECK(pTexInfo);

	UpdateData(TRUE);

	m_tSpriteInfo.vOffset.x = pTexInfo->tImgInfo.Width * 0.5f;
	m_tSpriteInfo.vOffset.y = pTexInfo->tImgInfo.Height - TILECY * 0.5f;

	UpdateData(FALSE);
	Invalidate(FALSE);
}

// �����͵� ���� ��ư
void CSpriteTool::OnBnClickedSaveDataPath()
{
	UpdateData(TRUE);
	
	CString strPathName;
	/*CString str = _T("All files(*.*)|*.*|");
	CFileDialog dlg(TRUE, _T("*.dat"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, str, this);
	if (dlg.DoModal() == IDOK)
	{
		strPathName = dlg.GetPathName();
	}*/

	strPathName = L"../Data/ImageData.txt";

	wofstream SaveFile;
	SaveFile.open(strPathName.GetString(), ios::out);
	//SaveFile.open(L"../Data/ImageData.txt", ios::out);

	if (!SaveFile.good())
		return;

	size_t iMax = m_mapListItem.size();
	SaveFile << iMax << '\n';

	size_t iCount = 0;
	for (auto& iter : m_mapListItem) {
		++iCount;
		wstring wstrData = SetListData(iter.second);
		
		SaveFile << wstrData;
		if (iCount < iMax)
			SaveFile << '\n';
	}

	SaveFile.close();

	UpdateData(FALSE);
	Invalidate(FALSE);

	m_pToolView->m_pMainDialog->LoadImageData(L"../Data/ImageData.txt");
	m_pToolView->m_pMainDialog->m_UnitTool.UpdateData(FALSE);
	m_pToolView->m_pMainDialog->m_UnitTool.Invalidate(FALSE);
}

// ������ �ҷ����� ��ư
void CSpriteTool::OnBnClickedLoadDataPath()
{
	// ���� �̸� ���
	CFileDialog dlg(TRUE, _T("*.dat"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
		L"All files(*.*)|*.*|", this);
	if (dlg.DoModal() == IDOK)
	{
		CString strPathName = dlg.GetPathName();
		// ���� ��θ� ������ ����� ���, Edit Control�� �� ����
		//SetDlgItemText(IDC_EDIT1, strPathName);
		LoadData(strPathName.GetString());
	}
}

// �̹��� Ŭ�� (Offset ����)
void CSpriteTool::OnStnClickedSpritePicture()
{
	POINT pt = {};
	GetCursorPos(&pt);
	::ScreenToClient(m_SpritePicture.m_hWnd, &pt);

	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(m_tSpriteInfo.wstrObjKey, m_tSpriteInfo.wstrStateKey, 0);
	NULL_CHECK(pTexInfo);

	UpdateData(TRUE);

	CRect rt;
	m_SpritePicture.GetClientRect(&rt);

	float fScaleW = (float)pt.x / float(rt.right);
	float fScaleH = (float)pt.y / float(rt.bottom);

	if (m_RadioEraseColor.GetCheck()) {
		// Erase Color
		//////////////////////////////////////////////////////////////////
		D3DLOCKED_RECT LockRect;

		pTexInfo->pTexture->LockRect(0, &LockRect, 0, D3DLOCK_DISCARD);

		DWORD* pColor = (DWORD*)LockRect.pBits; // ����� �ȼ� �� �޾ƿ���

		int width = pTexInfo->tImgInfo.Width; // �ȼ� ������
		int height = pTexInfo->tImgInfo.Height; // �ȼ� ������

		int iX = int(pTexInfo->tImgInfo.Width * fScaleW);
		int iY = int(pTexInfo->tImgInfo.Height * fScaleH);

		D3DXCOLOR xclr = pColor[iY * pTexInfo->tImgInfo.Width + iX];
		m_tSpriteInfo.tEraseColor.a = DWORD(xclr.a * 255);
		m_tSpriteInfo.tEraseColor.r = DWORD(xclr.r * 255);
		m_tSpriteInfo.tEraseColor.g = DWORD(xclr.g * 255);
		m_tSpriteInfo.tEraseColor.b = DWORD(xclr.b * 255);

		pTexInfo->pTexture->UnlockRect(0); //  ��� ����
										   //////////////////////////////////////////////////////////////////
	}
	else {
		// Offset
		m_tSpriteInfo.vOffset = { -fScaleW, -fScaleH, 0.f };
	}
	
	UpdateData(FALSE);
	Invalidate(FALSE);
}

// �׸� Ŭ���ؼ� �� ����� ���� ��ư
void CSpriteTool::OnBnClickedEraseColor()
{
	UpdateData(TRUE);
	UpdateData(FALSE);
	Invalidate(FALSE);
}

// �׸� Ŭ���ؼ� ������ ���� ���� ��ư
void CSpriteTool::OnBnClickedOffset()
{
	UpdateData(TRUE);
	UpdateData(FALSE);
	Invalidate(FALSE);
}


void CSpriteTool::OnBnClickedDrawLine()
{
	UpdateData(TRUE);
	UpdateData(FALSE);
	Invalidate(FALSE);
}
