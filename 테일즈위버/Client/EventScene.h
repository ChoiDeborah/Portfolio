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
	// CScene��(��) ���� ��ӵ�
	virtual HRESULT Initialize() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual void Release() override;

private:
	HRESULT	LoadEvent(const wstring & wstrFileName);


private:
	vector<CEventObj*>	m_vecEventObj;	// �̺�Ʈ ������Ʈ
	vector<WORD_EVENT*>	m_vecEventWord;	// �̺�Ʈ �ؽ�Ʈ ��ȭ

	FRAME				m_tCurTime;
	EVENT_NOTIFY		m_tCurNotify;

	MAP_DATA			m_tMapData;

	float				m_fRectY = 0.f;

	int					m_iRectState = 0;
};

