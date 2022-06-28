#pragma once
class CTexture
{
protected:
	CTexture();

public:
	virtual ~CTexture();

public:
	virtual const TEX_INFO* GetTexInfo(
		const wstring& wstrStateKey = L"", /* 상태 키*/
		int iIndex = 0	/* 이미지 번호 */) PURE;

	virtual const int GetTexCount(const wstring& wstrStateKey = L"") PURE;

public:
	virtual HRESULT LoadTexture(
		const wstring& wstrFilePath, /* 불러올 이미지 경로 */
		const wstring& wstrStateKey = L"", /* 상태 키*/
		float fOffsetX = -0.5f,	/* 중점 X */
		float fOffsetY = -0.5f, /* 중점 Y */
		int iImgCount = 0, /* 이미지 개수 */
		int iMirror = 1, /*이미지 좌우 반전*/
		float fFrameDelay = 1.f, /*이미지 스피드*/
		DWORD dwBlend = D3DCOLOR_ARGB(255, 255, 255, 255), /* 블랜딩 색상 정보 */
		DWORD dwEraseColorColor = 0 /* 지울 색상 정보 */
		) PURE;

protected:
	virtual void Release() PURE;

protected:
	CDeviceMgr*	m_pDeviceMgr;
};

