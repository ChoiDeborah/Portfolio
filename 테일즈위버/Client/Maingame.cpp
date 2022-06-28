#include "stdafx.h"
#include "Maingame.h"

CMaingame::CMaingame()
	: m_pDeviceMgr(CDeviceMgr::GetInstance()),
	m_pTextureMgr(CTextureMgr::GetInstance()),
	m_pSceneMgr(CSceneMgr::GetInstance()),
	m_pTimeMgr(CTimeMgr::GetInstance()),
	m_pFrameMgr(CFrameMgr::GetInstance())
{
	srand(unsigned(time(NULL)));
}


CMaingame::~CMaingame()
{
	Release();
}

HRESULT CMaingame::Initialize()
{
	ShowCursor(FALSE);

	m_pTimeMgr->InitTime();

	HRESULT hr = m_pDeviceMgr->InitDevice(CDeviceMgr::MODE_FULL); //CDeviceMgr::MODE_WIN); // 
	FAILED_CHECK_MSG_RETURN(hr, L"장치초기화 실패", E_FAIL);

	CObjMgr::GetInstance()->Initialize();
	m_pSceneMgr->ChangeScene(SCENEID::SCENEID_LOGO);

	return S_OK;
}

void CMaingame::Update()
{
	m_pTimeMgr->UpdateTime();
	m_pSceneMgr->Update();
	//system("cls");
}

void CMaingame::LateUpdate()
{
	m_pSceneMgr->LateUpdate();
}

void CMaingame::Render()
{

	// 앞으로 모든 오브젝트는 이 사이에서 렌더링 수행.
	m_pSceneMgr->Render();
}

void CMaingame::Release()
{
	m_pSceneMgr->DestroyInstance();
	m_pTextureMgr->DestroyInstance();
	m_pDeviceMgr->DestroyInstance();
	m_pTimeMgr->DestroyInstance();
	m_pFrameMgr->DestroyInstance();
	CSoundMgr::GetInstance()->DestroyInstance();
}

CMaingame* CMaingame::Create()
{
	CMaingame* pInstance = new CMaingame;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}
