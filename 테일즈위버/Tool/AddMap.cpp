// AddMap.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "AddMap.h"
#include "MainDialog.h"
#include "MapTool.h"
#include "EventTool.h"
#include "afxdialogex.h"


// CAddMap ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CAddMap, CDialog)

CAddMap::CAddMap(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_ADDMAP, pParent)
	, m_strMapName(_T(""))
	, m_strMapSize(_T(""))
{

}

CAddMap::~CAddMap()
{
}

void CAddMap::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_MAP, m_MapListBox);
	DDX_Control(pDX, IDC_PICTURE_MAP, m_Picture);
	DDX_Text(pDX, IDC_EDIT3, m_strMapName);
	DDX_Text(pDX, IDC_MAPSIZE, m_strMapSize);
}


BEGIN_MESSAGE_MAP(CAddMap, CDialog)
	ON_LBN_SELCHANGE(IDC_LIST_MAP, &CAddMap::OnLbnSelchangeMapTexture)
	ON_BN_CLICKED(IDOK, &CAddMap::OnBnClickedOk)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CAddMap �޽��� ó�����Դϴ�.


BOOL CAddMap::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	CRect rect;
	m_Picture.GetClientRect(rect);

	m_iBoxWidth = rect.Width();
	m_iBoxHeight = rect.Height();

	wstring wstrName;
	int iCount = CTextureMgr::GetInstance()->GetTexCount(L"BackGround", L"Map");

	for (int i = 0; i < iCount; ++i) {
		const TEX_INFO* pTexInfo = CTextureMgr::GetInstance()->GetTexInfo(L"BackGround", L"Map", i);
		if (nullptr != pTexInfo) {
			if (0 == i) {
				m_pCurMapTex = pTexInfo;
				m_iMapWidth = m_pCurMapTex->tImgInfo.Width;
				m_iMapHeight = m_pCurMapTex->tImgInfo.Height;
				m_strMapSize = to_string(m_iMapWidth).c_str();
				m_strMapSize += " x "; 
				m_strMapSize += to_string(m_iMapHeight).c_str();
			}
			wstrName = L"Map" + to_wstring(i);
			m_MapListBox.InsertString(-1, wstrName.c_str());
		}
	}

	Invalidate(FALSE);
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CAddMap::OnLbnSelchangeMapTexture()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);

	if (m_MapListBox.GetCurSel() < m_MapListBox.GetCount()) {
		m_pCurMapTex = CTextureMgr::GetInstance()->GetTexInfo(L"BackGround", L"Map", m_MapListBox.GetCurSel());
		if (nullptr == m_pCurMapTex)
			return;

		m_iMapWidth = m_pCurMapTex->tImgInfo.Width;
		m_iMapHeight = m_pCurMapTex->tImgInfo.Height;
		m_strMapSize = to_string(m_iMapWidth).c_str();
		m_strMapSize += " x ";
		m_strMapSize += to_string(m_iMapHeight).c_str();

		Invalidate(FALSE);
	}
	UpdateData(FALSE);
}


void CAddMap::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CDialog::OnOK();

	UpdateData(TRUE);

	// �� ���� ���� �߰�
	int iTileX = m_iMapWidth / TILECX + 1;
	int iTileY = m_iMapHeight / (TILECY / 2) + 1;
	MAP_DATA* pMapData = new MAP_DATA(wstring(m_strMapName), m_MapListBox.GetCurSel(), iTileX, iTileY);

	pMapData->vecTile.reserve(iTileX * iTileY);

	// �� ������ �о�ͼ� ToolView�� Terrain�� ����
	TILE_INFO* pTile = nullptr;
	for (int i = 0; i < iTileY; ++i)
	{
		for (int j = 0; j < iTileX; ++j)
		{
			float fX = j * TILECX + (i % 2) * (TILECX * 0.5f);
			float fY = i * (TILECY * 0.5f);

			pTile = new TILE_INFO;
			pTile->vPos = { fX, fY, 0.f };
			pTile->vSize = { 1.f, 1.f, 0.f };
			pTile->byDrawID = 0;
			pTile->byOption = 0;

			pMapData->vecTile.push_back(pTile);
		}
	}

	if (MAP_TOOL == m_pMainDialog->m_Tab.GetCurSel())
		m_pMapTool->InsertMapData(pMapData);
	else if (EVENT_TOOL == m_pMainDialog->m_Tab.GetCurSel())
		m_pEventTool->InsertMapData(pMapData);
	else
		SafeDelete(pMapData);

	m_strMapName = _T("");

	UpdateData(FALSE);
}


void CAddMap::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
					   // �׸��� �޽����� ���ؼ��� CDialog::OnPaint()��(��) ȣ������ ���ʽÿ�.

	if (nullptr == m_pCurMapTex)
		return;

	CDeviceMgr::GetInstance()->Render_Begin();
	D3DXMATRIX matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, float(VIEWCX) / m_pCurMapTex->tImgInfo.Width, float(VIEWCY) / m_pCurMapTex->tImgInfo.Height, 0.f);
	matWorld = matScale;
	CDeviceMgr::GetInstance()->GetSprite()->SetTransform(&matWorld);
	CDeviceMgr::GetInstance()->GetSprite()->Draw(m_pCurMapTex->pTexture, nullptr,
		&m_pCurMapTex->vOffset, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	CDeviceMgr::GetInstance()->Render_End(m_Picture.m_hWnd);
}
