#pragma once
#include "GameObject.h"
class CStaticObject :
	public CGameObject
{
public:
	CStaticObject();
	virtual ~CStaticObject();

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
};

