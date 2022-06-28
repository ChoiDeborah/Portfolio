#pragma once
#include "Texture.h"
class CSingleTexture :
	public CTexture
{
private:
	CSingleTexture();

public:
	virtual ~CSingleTexture();

public:
	virtual const TEX_INFO* GetTexInfo(
		const wstring& wstrStateKey = L"",
		int iIndex = 0)override;

	virtual const int GetTexCount(const wstring& wstrStateKey = L"") override { return 1; }

public:
	// CTexture을(를) 통해 상속됨
	virtual HRESULT LoadTexture(
		const wstring& wstrFilePath, 
		const wstring & wstrStateKey = L"", 
		float fOffsetX = -0.5f,
		float fOffsetY = -0.f,
		int iImgCount = 1,
		int iMirror = 1,
		float fFrameDelay = 1.f, 
		DWORD dwBlend = D3DCOLOR_ARGB(255, 255, 255, 255),
		DWORD dwEraseColorColor = 0) override;

private:
	virtual void Release() override;

public:
	static CSingleTexture* Create(
		const wstring& wstrFilePath, 
		const wstring& wstrStateKey = L"", 
		float fOffsetX = -0.5f, 
		float fOffsetY = -0.5f, 
		int iMirror = 1, 
		float fFrameDelay = 1.f,
		DWORD dwBlend = D3DCOLOR_ARGB(255, 255, 255, 255),
		DWORD dwEraseColorColor = 0);

private:
	TEX_INFO*	m_pTexInfo;
};

