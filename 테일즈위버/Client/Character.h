#pragma once
class CEventObject;
class CCharacter
{
public:
	enum PLAYER_SKILL { NONE_SKILL, SKILL1, SKILL2, SKILL3, SKILL4, SKILL5, SKILL6, SKILL7, SKILL8 };

public:
	CCharacter();
	~CCharacter();

public:
	virtual void SetIdleState()		PURE;
	virtual void SetRunState()		PURE;
	virtual void SetAttackState()	PURE;
	virtual void SetHitState()		PURE;

	virtual void Attack(SKILLID eSkillID)		PURE;

	virtual void FrameEvent()		PURE;

	virtual void Update();

public:
	void	SetBody(CEventObject* pBody) { m_pBody = pBody; }

protected:
	CViewMgr*		m_pViewMgr	= CViewMgr::GetInstance();
	CTimeMgr*		m_pTimeMgr	= CTimeMgr::GetInstance();
	CSceneMgr*		m_pSceneMgr = CSceneMgr::GetInstance();
	CObjMgr*		m_pObjMgr	= CObjMgr::GetInstance();
	CKeyMgr*		m_pKeyMgr	= CKeyMgr::GetInstance();
	CWordMgr*		m_pWordMgr	= CWordMgr::GetInstance();
	CSoundMgr*		m_pSoundMgr = CSoundMgr::GetInstance();

	CEventObject*	m_pBody		= nullptr;

protected:
	PLAYER_SKILL	m_eSkillNum = NONE_SKILL;
};

