#pragma once
class CTexture
{
protected:
	CTexture();

public:
	virtual ~CTexture();

public:
	virtual const TEX_INFO* GetTexInfo(
		const wstring& wstrStateKey = L"", /* ���� Ű*/
		int iIndex = 0	/* �̹��� ��ȣ */) PURE;

	virtual const int GetTexCount(const wstring& wstrStateKey = L"") PURE;

public:
	virtual HRESULT LoadTexture(
		const wstring& wstrFilePath, /* �ҷ��� �̹��� ��� */
		const wstring& wstrStateKey = L"", /* ���� Ű*/
		float fOffsetX = -0.5f,	/* ���� X */
		float fOffsetY = -0.5f, /* ���� Y */
		int iImgCount = 0, /* �̹��� ���� */
		int iMirror = 1, /*�̹��� �¿� ����*/
		float fFrameDelay = 1.f, /*�̹��� ���ǵ�*/
		DWORD dwBlend = D3DCOLOR_ARGB(255, 255, 255, 255), /* ���� ���� ���� */
		DWORD dwEraseColorColor = 0 /* ���� ���� ���� */
		) PURE;

protected:
	virtual void Release() PURE;

protected:
	CDeviceMgr*	m_pDeviceMgr;
};

