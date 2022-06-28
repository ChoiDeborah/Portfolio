#pragma once
#include "Texture.h"
class CMultiTexture :
	public CTexture
{
private:
	CMultiTexture();

public:
	virtual ~CMultiTexture();

public:
	// CTexture을(를) 통해 상속됨
	virtual const TEX_INFO* GetTexInfo(
		const wstring& wstrStateKey = L"", 
		int iIndex = 0) override;

	virtual const int GetTexCount(const wstring& wstrStateKey = L"") override;

public:
	virtual HRESULT LoadTexture(
		const wstring& wstrFilePath, 
		const wstring& wstrStateKey = L"", 
		float fOffsetX = -0.5f,
		float fOffsetY = -0.5f,
		int iImgCount = 0,
		int iMirror = 1,
		float fFrameDelay = 1.f,
		DWORD dwBlend= D3DCOLOR_ARGB(255, 255, 255, 255),
		DWORD dwEraseColorColor= 0
	) override;

private:
	virtual void Release() override;

public:
	static CMultiTexture* Create(
		const wstring& wstrFilePath,
		const wstring& wstrStateKey,
		float fOffsetX = -0.5f,
		float fOffsetY = -0.5f,
		int iImgCount = 0,
		int iMirror = 1,
		float fFrameDelay = 1.f,
		DWORD dwBlend = D3DCOLOR_ARGB(255, 255, 255, 255),
		DWORD dwEraseColorColor = 0);

private:
	map<wstring, vector<TEX_INFO*>>	m_mapMultiTexture;
};

