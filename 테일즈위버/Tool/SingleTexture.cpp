#include "stdafx.h"
#include "SingleTexture.h"


CSingleTexture::CSingleTexture()
	: m_pTexInfo(nullptr)
{
}


CSingleTexture::~CSingleTexture()
{
	Release();
}

const TEX_INFO* CSingleTexture::GetTexInfo(
	const wstring& wstrStateKey, int iIndex)
{
	return m_pTexInfo;
}

HRESULT CSingleTexture::LoadTexture(
	const wstring& wstrFilePath,
	const wstring& wstrStateKey/* = L""*/,
	float fOffsetX, /* = -0.5f*/
	float fOffsetY, /* = -0.5f*/
	int iImgCount,/* = 0*/
	int iMirror, /* = 1*/
	float fFrameDelay, /* = 1.f*/
	DWORD dwBlend,/*= D3DCOLOR_ARGB(255, 255, 255, 255)*/
	DWORD dwEraseColorColor/*= 0*/)
{
	HRESULT hr = 0;

	LPDIRECT3DDEVICE9 pDevice = m_pDeviceMgr->GetDevice();
	NULL_CHECK_RETURN(pDevice, E_FAIL);

	D3DXIMAGE_INFO	tImgInfo;
	ZeroMemory(&tImgInfo, sizeof(D3DXIMAGE_INFO));

	// ���Ϸκ��� �̹��� ������ ������ �Լ�.
	hr = D3DXGetImageInfoFromFile(wstrFilePath.c_str(), &tImgInfo);
	FAILED_CHECK_RETURN(hr, E_FAIL);

	LPDIRECT3DTEXTURE9 pTexture = nullptr;

	
	// ������ ���� �ؽ�ó�� �ҷ��� IDirect3DTexture9��ü�� �����ϴ� �Լ�.
	hr = D3DXCreateTextureFromFileEx(
		pDevice, /* ��ġ */
		wstrFilePath.c_str(), /* �̹��� ���� ��� */
		tImgInfo.Width, /* �ؽ�ó ���� ũ�� */
		tImgInfo.Height, /* �ؽ�ó ���� ũ�� */
		tImgInfo.MipLevels, /* �ӷ���, �Ӹ��� �ܰ� */
		0, /* �ؽ�ó ��� ��� */
		tImgInfo.Format, /* �ؽ�ó  �ȼ� ���� */
		D3DPOOL_MANAGED, /* �޸� ��� ��� */
		D3DX_DEFAULT, /* ���͸� ��� */
		D3DX_DEFAULT, /* �Ӹ� ���͸� ��� */
		dwEraseColorColor, /* ������ ���� */
		nullptr, /* D3DXIMAGE_INFO�� ������ */
		nullptr, /* PALETTEENTRY�� ������ */
		&pTexture /* LPDIRECT3DTEXTURE9�� ������*/);
	FAILED_CHECK_MSG_RETURN(hr, L"D3DXCreateTextureFromFileEx Failed", E_FAIL);

	m_pTexInfo = new TEX_INFO;
	m_pTexInfo->pTexture = pTexture;
	m_pTexInfo->tImgInfo = tImgInfo;

	m_pTexInfo->vOffset.x = (0.f > fOffsetX) ? tImgInfo.Width	* -fOffsetX : fOffsetX;
	m_pTexInfo->vOffset.y = (0.f > fOffsetY) ? tImgInfo.Height	* -fOffsetY : fOffsetY;
	m_pTexInfo->vOffset.z = 0.f;

	m_pTexInfo->m_iMirror = iMirror;

	m_pTexInfo->m_fFrameDelay = fFrameDelay;

	m_pTexInfo->dwBlend = dwBlend;

	return S_OK;
}

void CSingleTexture::Release()
{
	if (nullptr != m_pTexInfo)
	{
		m_pTexInfo->pTexture->Release();
		SafeDelete(m_pTexInfo);
	}
}

CSingleTexture* CSingleTexture::Create(
	const wstring & wstrFilePath, 
	const wstring& wstrStateKey, 
	float fOffsetX, 
	float fOffsetY, 
	int iMirror, 
	float fFrameDelay,
	DWORD dwBlend,
	DWORD dwEraseColorColor)
{
	CSingleTexture* pInstance = new CSingleTexture;

	if (FAILED(pInstance->LoadTexture(wstrFilePath, wstrStateKey, fOffsetX, fOffsetY, 1, iMirror, fFrameDelay, dwBlend, dwEraseColorColor)))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}
