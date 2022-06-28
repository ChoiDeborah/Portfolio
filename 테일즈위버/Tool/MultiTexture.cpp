#include "stdafx.h"
#include "MultiTexture.h"


CMultiTexture::CMultiTexture()
{
}


CMultiTexture::~CMultiTexture()
{
	Release();
}

const TEX_INFO* CMultiTexture::GetTexInfo(
	const wstring& wstrStateKey/* = L""*/,
	int iIndex/* = 0*/)
{
	auto iter_find = m_mapMultiTexture.find(wstrStateKey);

	if (m_mapMultiTexture.end() == iter_find)
		return nullptr;

	return iter_find->second[iIndex];
}

const int CMultiTexture::GetTexCount(const wstring & wstrStateKey)
{
	auto iter_find = m_mapMultiTexture.find(wstrStateKey);
	if (m_mapMultiTexture.end() != iter_find)
		return int(iter_find->second.size());

	return 0;
}

// wstrFilePath:
// D:\������\99C\4������\Framework99C\Texture\Stage\Terrain\Tile\Tile%d.png
HRESULT CMultiTexture::LoadTexture(
	const wstring& wstrFilePath,
	const wstring& wstrStateKey/* = L""*/,
	float fOffsetX,	/* ���� X */
	float fOffsetY, /* ���� Y */
	int iImgCount, /* = 0*/
	int iMirror, /* = 1*/
	float fFrameDelay, /*= 1.f*/
	DWORD dwBlend,/*= D3DCOLOR_ARGB(255, 255, 255, 255)*/
	DWORD dwEraseColorColor/*= 0*/)
{
	auto iter_find = m_mapMultiTexture.find(wstrStateKey);

	if (m_mapMultiTexture.end() != iter_find)
		return E_FAIL;

	HRESULT hr = 0;
	TEX_INFO* pTexInfo = nullptr;

	LPDIRECT3DDEVICE9 pDevice = m_pDeviceMgr->GetDevice();
	NULL_CHECK_RETURN(pDevice, E_FAIL);

	D3DXIMAGE_INFO	tImgInfo;
	ZeroMemory(&tImgInfo, sizeof(D3DXIMAGE_INFO));

	TCHAR szFullPath[MAX_STR] = L"";

	for (int i = 0; i < iImgCount; ++i)
	{
		swprintf_s(szFullPath, wstrFilePath.c_str(), i);

		// ���Ϸκ��� �̹��� ������ ������ �Լ�.
		hr = D3DXGetImageInfoFromFile(szFullPath, &tImgInfo);
		FAILED_CHECK_RETURN(hr, E_FAIL);

		LPDIRECT3DTEXTURE9 pTexture = nullptr;

		// ������ ���� �ؽ�ó�� �ҷ��� IDirect3DTexture9��ü�� �����ϴ� �Լ�.
		hr = D3DXCreateTextureFromFileEx(
			pDevice, /* ��ġ */
			szFullPath, /* �̹��� ���� ��� */
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
		FAILED_CHECK_RETURN(hr, E_FAIL);

		pTexInfo = new TEX_INFO;
		pTexInfo->pTexture = pTexture;
		pTexInfo->tImgInfo = tImgInfo;

		pTexInfo->vOffset.x = (0.f > fOffsetX) ? tImgInfo.Width		* -fOffsetX : fOffsetX;
		pTexInfo->vOffset.y = (0.f > fOffsetY) ? tImgInfo.Height	* -fOffsetY : fOffsetY;
		pTexInfo->vOffset.z = 0.f;

		pTexInfo->m_iMirror = iMirror;

		pTexInfo->m_fFrameDelay = fFrameDelay;

		pTexInfo->dwBlend = dwBlend;

		m_mapMultiTexture[wstrStateKey].push_back(pTexInfo);
	}

	return S_OK;
}

void CMultiTexture::Release()
{
	for (auto& MyPair : m_mapMultiTexture)
	{
		for (auto& pTexInfo : MyPair.second)
		{
			pTexInfo->pTexture->Release();
			SafeDelete(pTexInfo);
		}

		MyPair.second.clear();
		MyPair.second.shrink_to_fit();
	}

	m_mapMultiTexture.clear();
}

CMultiTexture* CMultiTexture::Create(
	const wstring& wstrFilePath,
	const wstring& wstrStateKey,
	float fOffsetX,	/* ���� X */
	float fOffsetY, /* ���� Y */
	int iImgCount,
	int iMirror,
	float fFrameDelay,
	DWORD dwBlend,
	DWORD dwEraseColorColor)
{
	CMultiTexture* pInstance = new CMultiTexture;

	if (FAILED(pInstance->LoadTexture(wstrFilePath, wstrStateKey, fOffsetX, fOffsetY, iImgCount, iMirror, fFrameDelay, dwBlend, dwEraseColorColor)))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}
