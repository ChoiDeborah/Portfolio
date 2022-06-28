#pragma once

class CTexture;
class CTextureMgr
{
	DECLARE_SINGLETON(CTextureMgr)

public:
	enum TEXTURE_TYPE { SINGLE_TEXTURE, MULTI_TEXTURE };

private:
	CTextureMgr();
	~CTextureMgr();

public:
	const TEX_INFO* GetTexInfo(
		const wstring& wstrObjectKey,
		const wstring& wstrStateKey = L"",
		int iIndex = 0);

	const int	GetTexCount(const wstring& wstrObjectKey,
		const wstring& wstrStateKey = L"");

public:
	HRESULT ReadImagePath(const wstring& wstrFilePath);

	HRESULT LoadTexture
	(
		TEXTURE_TYPE eTexType,
		const wstring& wstrFilePath, /* 불러올 이미지 경로 */
		const wstring& wstrObjectKey,	/*오브젝트 키*/
		const wstring& wstrStateKey = L"", /* 상태 키*/
		float fOffsetX = -0.5f,	/* 중점 X */
		float fOffsetY = -0.5f, /* 중점 Y */
		int iImgCount = 0, /* 이미지 개수 */
		int iMirror = 1, /*이미지 좌우 반전*/
		float fFrameDelay = 1.f, /*이미지 스피드*/
		DWORD dwBlend = D3DCOLOR_ARGB(255, 255, 255, 255), /* 블랜딩 색상 정보 */
		DWORD dwEraseColorColor = 0 /* 지울 색상 정보 */
	);

private:
	void Release();

private:
	map<wstring, CTexture*>	m_mapTexture;
};

