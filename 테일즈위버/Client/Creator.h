#pragma once
#include "GameObject.h"
class CCreator :
	public CGameObject
{
public:
	CCreator();
	virtual ~CCreator();

public:
	virtual HRESULT Initialize() override;
	virtual int Update() override;
	virtual int LateUpdate() override;
	virtual void Render() override;

protected:
	virtual HRESULT LateInit();
	virtual void Release() override;
	virtual void FrameOnceEnd() override;
	virtual void FrameEvent() override;

public:
	void		SetCreateData(const TILE_INFO& tTileInfo, INFO& tInfo);
	void		SetIsCreate(bool bIsCreate) { m_bIsCreate = bIsCreate; }

protected:
	TILE_INFO	m_tTileInfo;
	CAlarm		m_tCreateAlarm;

protected:
	bool		m_bIsCreate = false;
	bool		m_bIsSetting = false;
};

