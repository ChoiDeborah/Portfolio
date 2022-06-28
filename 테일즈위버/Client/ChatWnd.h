#pragma once
#include "UIWnd.h"
class CChatWnd :
	public CUIWnd
{
public:
	CChatWnd();
	virtual ~CChatWnd();

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
	void	Cheat(const wstring& wstrCheat);

public:
	static CChatWnd* Create();

private:
	vector<CHAT_DATA*>	m_vecChatData;

	CTileMgr*	m_pTileMgr = CTileMgr::GetInstance();

	bool		m_bIsChat = false;
};

