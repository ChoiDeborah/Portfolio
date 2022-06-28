#pragma once
#include "Alarm.h"
#include "WndInfo.h"

class CUIWnd;
class CPlayerObserver;
class CUIMgr
{
	DECLARE_SINGLETON(CUIMgr)

private:
	CUIMgr();
	~CUIMgr();

public:
	HRESULT	Initialize();
	void	Update();
	void	Render();

private:
	void	Release();
	HRESULT	LoadExpData(const wstring& wstrFilePath);

public:
	void	ShowShop();
	void	SetDead(bool bIsDead);
	void	SetHover(bool bIsHover)			{ m_bIsMouseHover = bIsHover;	}
	void	SetIsChat(bool bIsChat)			{ m_bIsChat = bIsChat;			}

	void	AddEquipStat(STAT_INFO* pStatInfo);
	void	AddExp(int iAddExp);
	void	AddHealth(int iHp = 0, int iMp = 0, int iSp = 0);
	void	AddItem(WND_ID eWndID, CItemData* pItemData);
	void	AddItem(WND_ID eWndID, wstring wstrName, int iPrice = -1);

	void	InsertChat(const wstring& wstrName, const wstring& wstrChat, DWORD dwFontColor = D3DCOLOR_ARGB(255, 238, 239, 163));

public:
	UNIT_INFO&			GetPlayerData()		{ return m_tPlayerData;			}
	IDENTITY_DATA&		GetPlayerIdentity() { return m_tPlayerIdentity;		}
	void				UpdatePlayerNotify();

public:
	bool	GetIsHover()					{ return m_bIsMouseHover;	}
	bool	GetDead()						{ return m_bIsDead;			}
	bool	GetIsChat()						{ return m_bIsChat;			}

	CUIWnd* GetWindow(WND_ID eWndID);

public:
	void	DrawRect(CWndInfo& tInfo);
	void	MoveItem(WND_ID eFrom, WND_ID eTo, int iItemSlotIndex, int iCount = 1);

private:
	void	KeyInput();

// 사용자 정의
private:
	vector<EXP_DATA*>	m_vecExpData;

	vector<WND_ID>		m_vecWndRender;

	CWndInfo			m_tMiniInfo;

	CWndInfo			m_tExpansion;

	CWndInfo			m_tInfoBar;
	CWndInfo			m_tInvenBar;
	CWndInfo			m_tEquipBar;
	CWndInfo			m_tSkillBar;
	CWndInfo			m_tComboBar;

	CWndInfo			m_tItemShop;


	UNIT_INFO			m_tPlayerData;
	IDENTITY_DATA		m_tPlayerIdentity;
	INFO				m_tPlayerInfo;

	CAlarm				m_tChatAlarm;		// 공지사항 알림

// 포인터
private:
	CDeviceMgr*			m_pDeviceMgr		= nullptr;
	CTextureMgr*		m_pTextureMgr		= nullptr;
	CViewMgr*			m_pViewMgr			= nullptr;
	CMouseMgr*			m_pMouseMgr			= nullptr;
	CKeyMgr*			m_pKeyMgr			= nullptr;
	CSoundMgr*			m_pSoundMgr			= nullptr;

	const TEX_INFO*		m_pMiniTabTex		= nullptr;	// 왼쪽 하단
	const TEX_INFO*		m_pChatWndTex		= nullptr;	// 중앙 하단

	const TEX_INFO*		m_pMiniInfoTex		= nullptr;	// 왼쪽 상단
	const TEX_INFO*		m_pHpTex			= nullptr;	// 왼쪽 상단 Hp바
	const TEX_INFO*		m_pMpTex			= nullptr;	// 왼쪽 상단 Mp바
	const TEX_INFO*		m_pSpTex			= nullptr;	// 왼쪽 상단 Sp바

	const TEX_INFO*		m_pExpansionTex		= nullptr;	// 확장 버튼

	const TEX_INFO*		m_pItemShopTex		= nullptr;	// 아이템샵 버튼

	const TEX_INFO*		m_pInfoBarTex		= nullptr;	// 상태 버튼
	const TEX_INFO*		m_pEquipBarTex		= nullptr;	// 장비 버튼
	const TEX_INFO*		m_pInvenBarTex		= nullptr;	// 아이템창 버튼
	const TEX_INFO*		m_pSkillBarTex		= nullptr;	// 스킬 버튼
	const TEX_INFO*		m_pComboBarTex		= nullptr;	// 콤보 버튼

	CUIWnd*				m_pInfoWnd			= nullptr;	// 정보 창
	CUIWnd*				m_pEquipWnd			= nullptr;	// 장비 창
	CUIWnd*				m_pInvenWnd			= nullptr;	// 아이템 창
	CUIWnd*				m_pSkillWnd			= nullptr;	// 스킬 창
	CUIWnd*				m_pChatWnd			= nullptr;	// 채팅 창
	CUIWnd*				m_pQuickWnd			= nullptr;	// 퀵슬롯
	CUIWnd*				m_pComboWnd			= nullptr;	// 스킬 콤보 창
	CUIWnd*				m_pComboSetWnd		= nullptr;	// 스킬 콤보 편집 창
	CUIWnd*				m_pMiniMapWnd		= nullptr;	// 미니맵 창
	CUIWnd*				m_pCountWnd			= nullptr;	// 개수 창

	CUIWnd*				m_pShopWnd			= nullptr;	// 상점 창
	
	CUIWnd*				m_pCurWnd			= nullptr;	// 현재 선택 창

	CPlayerSubject*		m_pSubject			= nullptr;
	CPlayerObserver*	m_pObserver			= nullptr;

// 기본 자료형
private:
	float				m_fFollowHp			= 100.f;
	float				m_fFollowMp			= 100.f;

	bool				m_bIsExpansion		= true;
	bool				m_bIsDead			= false;
	bool				m_bIsMouseHover		= false;
	bool				m_bIsChat			= false;
};