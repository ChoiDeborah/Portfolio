#pragma once
#include "StaticObject.h"
class CWarpGate :
	public CStaticObject
{
public:
	CWarpGate();
	virtual ~CWarpGate();

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
	void ChangeScene();

private:
	wstring		m_wstrSceneName = L"";
	SCENEID		m_eSceneID = SCENEID_STAGE;
};

