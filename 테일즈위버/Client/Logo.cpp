#include "stdafx.h"
#include <process.h>
#include "Logo.h"


CLogo::CLogo()
{
}


CLogo::~CLogo()
{
	Release();
}

HRESULT CLogo::Initialize() {
	m_pViewMgr->Initialize();

	m_hLoadingThread = (HANDLE)_beginthreadex(nullptr, 0, LoadingThreadFunc, this, 0, nullptr);
	NULL_CHECK_RETURN(m_hLoadingThread, E_FAIL);
	
	InitializeCriticalSection(&m_CriticalSection);

	HRESULT hr = m_pTextureMgr->LoadTexture(
		CTextureMgr::MULTI_TEXTURE, L"../Texture/UI/Loading/Loading/%d.png", L"Logo", L"Loading", 0, 0, 67);
	FAILED_CHECK_MSG_RETURN(hr, L"Logo Texture Load Failed", E_FAIL);

	hr = m_pTextureMgr->LoadTexture(
		CTextureMgr::SINGLE_TEXTURE, L"../Texture/UI/Loading/NowLoading.png", L"NowLoading", L"", 0.f, -0.65f);
	FAILED_CHECK_MSG_RETURN(hr, L"NowLoading Texture Load Failed", E_FAIL);

	hr = m_pTextureMgr->LoadTexture(
		CTextureMgr::SINGLE_TEXTURE, L"../Texture/UI/Loading/NowLoadingBar.png", L"NowLoadingBar", L"", 0.f, -0.5f);
	FAILED_CHECK_MSG_RETURN(hr, L"NowLoading Texture Load Failed", E_FAIL);

	m_pLoadingTex		= m_pTextureMgr->GetTexInfo(L"NowLoading");
	m_pLoadingBarTex	= m_pTextureMgr->GetTexInfo(L"NowLoadingBar");


	m_pTexInfo = m_pTextureMgr->GetTexInfo(L"Logo", L"Loading", 0);
	m_tFrame.fFrameTime		= 0.f;
	m_tFrame.iFrameCur		= 0;
	m_tFrame.fFrameDelay	= 0.1f;
	m_tFrame.iFrameEnd		= m_pTextureMgr->GetTexCount(L"Logo", L"Loading");

	return S_OK;
}

void CLogo::Update() {
	if (m_bIsLoadingComplete)
	{
		WaitForSingleObject(m_hLoadingThread, INFINITE);

		DWORD dwResult = 0;
		GetExitCodeThread(m_hLoadingThread, &dwResult);

		if (1 == dwResult)
		{
			// 로딩 실패
			::MessageBox(0, L"Load Failed", L"System Error", MB_OK);
		}


		m_pSceneMgr->SetNextScene(SCENEID::SCENEID_TITLE, L"");
	}
}

void CLogo::LateUpdate()
{
}

void CLogo::Render() {
	m_pViewMgr->Update();

	D3DXMATRIX matScale, matTrans, matWorld;


	m_pTexInfo = m_pTextureMgr->GetTexInfo(L"Logo", L"Loading", m_tFrame.iFrameCur);
	if (nullptr != m_pTexInfo)
	{
		D3DXMatrixScaling(&matScale, float(CLIENTCX) / m_pTexInfo->tImgInfo.Width, float(CLIENTCY) / m_pTexInfo->tImgInfo.Height, 0.f);
		D3DXMatrixTranslation(&matTrans, 0.f, 0.f, 0.f);
		matWorld = matScale * matTrans;

		m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
		m_pDeviceMgr->GetSprite()->Draw(m_pTexInfo->pTexture, nullptr,
			&m_pTexInfo->vOffset, nullptr, m_pTexInfo->dwBlend);

		// 프레임 이동
		{
			m_tFrame.fFrameTime += CTimeMgr::GetInstance()->GetDeltaTime();
			if (m_tFrame.fFrameDelay < m_tFrame.fFrameTime) {
				m_tFrame.fFrameTime = 0.f;
				++m_tFrame.iFrameCur;

				if (m_tFrame.iFrameCur >= m_tFrame.iFrameEnd) {
					m_tFrame.iFrameCur = 0;//m_tFrame.iFrameEnd - 1;
				}
			}
		}
	}

	if (nullptr != m_pLoadingTex && nullptr != m_pLoadingBarTex)
	{
		// 로딩
		D3DXMatrixTranslation(&matTrans, CLIENTCX - m_pLoadingTex->tImgInfo.Width * 1.1f, CLIENTCY * 0.9f, 0.f);
		matWorld = matTrans;
		m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
		m_pDeviceMgr->GetSprite()->Draw(m_pLoadingTex->pTexture, nullptr,
			&m_pLoadingTex->vOffset, nullptr, m_pLoadingTex->dwBlend);

		// 로딩 바
		if (0 < m_pSceneMgr->m_iMaxCount)
		{
			D3DXMatrixTranslation(&matTrans, CLIENTCX - m_pLoadingTex->tImgInfo.Width * 1.1f + 3.f, CLIENTCY * 0.9f + 1.f, 0.f);
			D3DXMatrixScaling(&matScale, float(m_pSceneMgr->m_iCount) / m_pSceneMgr->m_iMaxCount, 1.f, 0.f);

			matWorld = matScale * matTrans;
			m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
			m_pDeviceMgr->GetSprite()->Draw(m_pLoadingBarTex->pTexture, nullptr,
				&m_pLoadingBarTex->vOffset, nullptr, m_pLoadingBarTex->dwBlend);
		}
	}
}

void CLogo::Release() 
{
}

unsigned CLogo::LoadingThreadFunc(void* pParam)
{
	CLogo* pLogo = reinterpret_cast<CLogo*>(pParam);

	NULL_CHECK_MSG_RETURN(pLogo, L"Logo Arg Failed", 1);

	EnterCriticalSection(&(pLogo->m_CriticalSection));

	CSoundMgr::GetInstance()->Initialize();

	HRESULT hr = pLogo->m_pTextureMgr->LoadTexture(CTextureMgr::MULTI_TEXTURE,
			L"../Texture/BackGround/Tile/TileOption%d.png",
			L"Background", L"TileOption", -0.5f, -0.5f, 5);
		FAILED_CHECK_MSG_RETURN(hr, L"Terrain Tile Texture Load Failed", 1);

	hr = pLogo->m_pTextureMgr->LoadTexture(CTextureMgr::MULTI_TEXTURE,
			L"../Texture/BackGround/Map/Map%d.png",
			L"Background", L"Map", 0.f, 0.f, 11);
	FAILED_CHECK_MSG_RETURN(hr, L"Terrain Map Texture Load Failed", 1);

	hr = pLogo->m_pTextureMgr->ReadImagePath(L"../Data/ImageData.txt");
	FAILED_CHECK_MSG_RETURN(hr, L"ReadImagePath Load Failed", 1);

	LeaveCriticalSection(&(pLogo->m_CriticalSection));

	pLogo->m_bIsLoadingComplete = true;

	return 0;
}
