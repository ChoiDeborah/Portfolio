#pragma once
#include "UIWnd.h"
class CMiniMapWnd :
	public CUIWnd
{
public:
	CMiniMapWnd();
	virtual ~CMiniMapWnd();

public:
	// Inherited via CUIWnd
	virtual HRESULT Initialize()	override;
	virtual void	Update()		override;
	virtual void	Render()		override;
	virtual void	Release()		override;

	virtual void	LClickDown()	override;
	virtual void	LClickUp()		override;

	virtual void	RClickDown()	override;

public:
	void	InsertChat(CHAT_DATA* pChatData);

public:
	static CMiniMapWnd* Create();

private:
	CTileMgr*			m_pTileMgr		= CTileMgr::GetInstance();

	const TEX_INFO*		m_pPlayerTex	= nullptr;		// 플레이어
	const TEX_INFO*		m_pMonsterTex	= nullptr;		// 몬스터
	const TEX_INFO*		m_pNpcTex		= nullptr;		// NPC
	const TEX_INFO*		m_pBuddyTex		= nullptr;		// Buddy
};

