#pragma once
#include "Effect.h"
class CNormalEffect :
	public CEffect
{
public:
	CNormalEffect();
	virtual ~CNormalEffect();

public:
	// CEffect��(��) ���� ��ӵ�
	virtual HRESULT Initialize() override;
	virtual int Update() override;
	virtual int LateUpdate() override;
	virtual void Render() override;

protected:
	virtual HRESULT LateInit() override;
	virtual void Release() override;
	virtual void FrameOnceEnd() override;
	virtual void FrameEvent() override;
};

