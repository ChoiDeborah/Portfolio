#pragma once
#include "Alarm.h"

class CWordMgr
{
	DECLARE_SINGLETON(CWordMgr)

public:
	CWordMgr();
	~CWordMgr();

public:
	HRESULT	Initialize();
	void	Update();
	void	Render();

private:
	void	Release();
	HRESULT	LoadWordData(const wstring& wstrFileName);

public:
	void	StartWord(bool bIsLoadFile, const wstring& wstrFileName = L"");
	void	NextWord();
	void	AddWord(WORD_DATA* pWordData);
	void	ClearWord();

public:
	bool	GetWord() { return m_bIsWord; }

private:
	list<WORD_DATA*>	m_lstWord;			// 텍스트 정보 리스트
	
	WORD_DATA			m_tLeftWord;		// 좌측 정보
	WORD_DATA			m_tRightWord;		// 우측 정보

	WORD_DATA			m_tCurWord;			// 현재 텍스트 정보
	CAlarm				m_tWordAlarm;		// 글자 타이핑 알람

	D3DXVECTOR3			m_vRightCharPos		= { float(CLIENTCX + 200.f), float(CLIENTCY), 0.f };	// 이전 이미지 위치
	D3DXVECTOR3			m_vLeftCharPos		= { -200.f, float(CLIENTCY), 0.f };	// 캐릭터 이미지 위치

	wstring				m_wstrCurWord	= L"";

private:
	CDeviceMgr*			m_pDeviceMgr	= nullptr;
	CTextureMgr*		m_pTextureMgr	= nullptr;
	CKeyMgr*			m_pKeyMgr		= nullptr;
	CSceneMgr*			m_pSceneMgr		= nullptr;
	CSoundMgr*			m_pSoundMgr		= nullptr;

	const TEX_INFO*		m_pWordBoxTex	= nullptr;
	const TEX_INFO*		m_pWordNextTex	= nullptr;

private:
	int					m_iTalkCount	= 1;

	int					m_iCurSize		= 0;

	float				m_fRectY		= 0.f;
	int					m_iRectState	= 0;

	bool				m_bIsWord		= false;
	bool				m_bIsTexMove	= true;
};

