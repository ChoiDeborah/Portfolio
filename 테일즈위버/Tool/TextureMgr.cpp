#include "stdafx.h"
#include "TextureMgr.h"
#include "SingleTexture.h"
#include "MultiTexture.h"

IMPLEMENT_SINGLETON(CTextureMgr)

CTextureMgr::CTextureMgr()
{
}


CTextureMgr::~CTextureMgr()
{
	Release();
}

const TEX_INFO* CTextureMgr::GetTexInfo(
	const wstring& wstrObjectKey, 
	const wstring& wstrStateKey, 
	int iIndex)
{
	auto iter_find = m_mapTexture.find(wstrObjectKey);

	if (m_mapTexture.end() == iter_find)
		return nullptr;

	return iter_find->second->GetTexInfo(wstrStateKey, iIndex);
}

const int CTextureMgr::GetTexCount(const wstring & wstrObjectKey, const wstring & wstrStateKey)
{
	auto iter_find = m_mapTexture.find(wstrObjectKey);

	if (m_mapTexture.end() != iter_find)
		return iter_find->second->GetTexCount(wstrStateKey);

	return 0;
}

HRESULT CTextureMgr::ReadImagePath(const wstring & wstrFilePath)
{
	wifstream LoadFile;
	LoadFile.open(wstrFilePath, ios::in);

	if (!LoadFile.good())
		return false;

	TCHAR szMaxCount[MAX_STR] = L"";
	LoadFile.getline(szMaxCount, MAX_STR);

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

		float fOffsetX = float(_wtof(szOffsetX));
		float fOffsetY = float(_wtof(szOffsetY));

		int iCount = int(_wtoi(szCount));

		int iMirror = int(_wtoi(szMirror));

		float fFrameDelay = float(_wtof(szFrameDelay));

		DWORD dwBlend = DWORD(_wtoll(szBlend));
		DWORD dwEraseColor = DWORD(_wtoll(szEraseColorColor));

		//TEXTURE_TYPE eType = TEXTURE_TYPE(_wtoi(szTexType));

		HRESULT hr = LoadTexture(MULTI_TEXTURE, szFilePath, szObjKey, szStateKey, fOffsetX, fOffsetY, iCount, iMirror, fFrameDelay, dwBlend, dwEraseColor);
		FAILED_CHECK_MSG_RETURN(hr, szFilePath, E_FAIL);

	}

	return S_OK;
}

HRESULT CTextureMgr::LoadTexture(
	TEXTURE_TYPE eTexType, 
	const wstring& wstrFilePath, 
	const wstring& wstrObjectKey, 
	const wstring& wstrStateKey, 
	float fOffsetX,
	float fOffsetY,
	int iImgCount,
	int iMirror,
	float fFrameDelay,
	DWORD dwBlend,
	DWORD dwEraseColorColor)
{
	auto iter_find = m_mapTexture.find(wstrObjectKey);

	// 오브젝트 키가 없다면 싱글이건 멀티건 최초 할당 후 텍스처 로드 및 맵 삽입.
	if (m_mapTexture.end() == iter_find)
	{
		CTexture* pTexture = nullptr;

		switch (eTexType)
		{
		case SINGLE_TEXTURE:
			pTexture = CSingleTexture::Create(wstrFilePath, wstrStateKey, fOffsetX, fOffsetY, iMirror, fFrameDelay, dwBlend, dwEraseColorColor);
			break;
		case MULTI_TEXTURE:
			pTexture = CMultiTexture::Create(wstrFilePath, wstrStateKey, fOffsetX, fOffsetY, iImgCount, iMirror, fFrameDelay, dwBlend, dwEraseColorColor);
			break;
		}

		NULL_CHECK_RETURN(pTexture, E_FAIL);
		m_mapTexture.insert({ wstrObjectKey, pTexture });
	}
	// 오브젝트 키가 이미 존재하고 멀티라면
	else if (MULTI_TEXTURE == eTexType)
	{
		HRESULT hr = m_mapTexture[wstrObjectKey]->LoadTexture(
			wstrFilePath, wstrStateKey, fOffsetX, fOffsetY, iImgCount, iMirror, fFrameDelay, dwBlend, dwEraseColorColor);
		FAILED_CHECK_RETURN(hr, E_FAIL);
	}

	return S_OK;
}

void CTextureMgr::Release()
{
	for (auto& MyPair : m_mapTexture)
		SafeDelete(MyPair.second);

	m_mapTexture.clear();
}
