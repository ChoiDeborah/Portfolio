#pragma once
#include "Scene.h"

class CEventObj;
class CEventScene :
	public CScene
{
public:
	CEventScene();
	virtual ~CEventScene();

public:
	// CScene을(를) 통해 상속됨
	virtual HRESULT Initialize() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual void Release() override;

private:
	HRESULT	LoadEvent(const wstring & wstrFileName);


private:
	vector<CEventObj*>	m_vecEventObj;	// 이벤트 오브젝트
	vector<WORD_EVENT*>	m_vecEventWord;	// 이벤트 텍스트 대화

	FRAME				m_tCurTime;
	EVENT_NOTIFY		m_tCurNotify;

	MAP_DATA			m_tMapData;

	float				m_fRectY = 0.f;

	int					m_iRectState = 0;
};

