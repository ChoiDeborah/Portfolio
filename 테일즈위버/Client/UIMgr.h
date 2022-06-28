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

// ����� ����
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

	CAlarm				m_tChatAlarm;		// �������� �˸�

// ������
private:
	CDeviceMgr*			m_pDeviceMgr		= nullptr;
	CTextureMgr*		m_pTextureMgr		= nullptr;
	CViewMgr*			m_pViewMgr			= nullptr;
	CMouseMgr*			m_pMouseMgr			= nullptr;
	CKeyMgr*			m_pKeyMgr			= nullptr;
	CSoundMgr*			m_pSoundMgr			= nullptr;

	const TEX_INFO*		m_pMiniTabTex		= nullptr;	// ���� �ϴ�
	const TEX_INFO*		m_pChatWndTex		= nullptr;	// �߾� �ϴ�

	const TEX_INFO*		m_pMiniInfoTex		= nullptr;	// ���� ���
	const TEX_INFO*		m_pHpTex			= nullptr;	// ���� ��� Hp��
	const TEX_INFO*		m_pMpTex			= nullptr;	// ���� ��� Mp��
	const TEX_INFO*		m_pSpTex			= nullptr;	// ���� ��� Sp��

	const TEX_INFO*		m_pExpansionTex		= nullptr;	// Ȯ�� ��ư

	const TEX_INFO*		m_pItemShopTex		= nullptr;	// �����ۼ� ��ư

	const TEX_INFO*		m_pInfoBarTex		= nullptr;	// ���� ��ư
	const TEX_INFO*		m_pEquipBarTex		= nullptr;	// ��� ��ư
	const TEX_INFO*		m_pInvenBarTex		= nullptr;	// ������â ��ư
	const TEX_INFO*		m_pSkillBarTex		= nullptr;	// ��ų ��ư
	const TEX_INFO*		m_pComboBarTex		= nullptr;	// �޺� ��ư

	CUIWnd*				m_pInfoWnd			= nullptr;	// ���� â
	CUIWnd*				m_pEquipWnd			= nullptr;	// ��� â
	CUIWnd*				m_pInvenWnd			= nullptr;	// ������ â
	CUIWnd*				m_pSkillWnd			= nullptr;	// ��ų â
	CUIWnd*				m_pChatWnd			= nullptr;	// ä�� â
	CUIWnd*				m_pQuickWnd			= nullptr;	// ������
	CUIWnd*				m_pComboWnd			= nullptr;	// ��ų �޺� â
	CUIWnd*				m_pComboSetWnd		= nullptr;	// ��ų �޺� ���� â
	CUIWnd*				m_pMiniMapWnd		= nullptr;	// �̴ϸ� â
	CUIWnd*				m_pCountWnd			= nullptr;	// ���� â

	CUIWnd*				m_pShopWnd			= nullptr;	// ���� â
	
	CUIWnd*				m_pCurWnd			= nullptr;	// ���� ���� â

	CPlayerSubject*		m_pSubject			= nullptr;
	CPlayerObserver*	m_pObserver			= nullptr;

// �⺻ �ڷ���
private:
	float				m_fFollowHp			= 100.f;
	float				m_fFollowMp			= 100.f;

	bool				m_bIsExpansion		= true;
	bool				m_bIsDead			= false;
	bool				m_bIsMouseHover		= false;
	bool				m_bIsChat			= false;
};