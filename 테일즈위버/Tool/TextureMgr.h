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
		const wstring& wstrFilePath, /* �ҷ��� �̹��� ��� */
		const wstring& wstrObjectKey,	/*������Ʈ Ű*/
		const wstring& wstrStateKey = L"", /* ���� Ű*/
		float fOffsetX = -0.5f,	/* ���� X */
		float fOffsetY = -0.5f, /* ���� Y */
		int iImgCount = 0, /* �̹��� ���� */
		int iMirror = 1, /*�̹��� �¿� ����*/
		float fFrameDelay = 1.f, /*�̹��� ���ǵ�*/
		DWORD dwBlend = D3DCOLOR_ARGB(255, 255, 255, 255), /* ���� ���� ���� */
		DWORD dwEraseColorColor = 0 /* ���� ���� ���� */
	);

private:
	void Release();

private:
	map<wstring, CTexture*>	m_mapTexture;
};

