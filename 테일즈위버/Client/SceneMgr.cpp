#include "stdafx.h"
#include "SceneMgr.h"

#include "Logo.h"
#include "Title.h"
#include "Stage.h"
#include "EventScene.h"

#include "GameObject.h"

#include "Player.h"

IMPLEMENT_SINGLETON(CSceneMgr)

CSceneMgr::CSceneMgr()
	: m_pDeviceMgr(CDeviceMgr::GetInstance()),
	m_pTextureMgr(CTextureMgr::GetInstance()),
	m_pViewMgr(CViewMgr::GetInstance()),
	m_pKeyMgr(CKeyMgr::GetInstance()),
	m_pMouseMgr(CMouseMgr::GetInstance()),
	m_pTileMgr(CTileMgr::GetInstance()),
	m_pWordMgr(CWordMgr::GetInstance())
{
	__noop;
}

CSceneMgr::~CSceneMgr()
{
	Release();
}

void CSceneMgr::Update() {
	// 일시 정지 시 업데이트 막음
	if (2 != m_iPause && SCENE_STATE::NEXT_SCENE != m_eSceneState)
	{
		m_pScene->Update();
		m_pWordMgr->Update();
	}

	if (SCENEID::SCENEID_LOGO != m_eCurScene) {
		m_pMouseMgr->Update();
	}

	switch (m_eSceneState)
	{
	case SCENE_STATE::START_SCENE:
		if (0.f < m_fFadeAlpha) {
			m_fFadeAlpha -= 10.f;
			if (0.f >= m_fFadeAlpha) {
				m_fFadeAlpha = 0.f;
				m_eSceneState = SCENE_STATE::SCENE_PLAY;
			}
		}
		break;

	case SCENE_STATE::NEXT_SCENE:
		SetEvent(EVENT_ALL_STOP, false);

		if (255.f > m_fFadeAlpha) {
			m_fFadeAlpha += 10.f;
			if (255.f <= m_fFadeAlpha) {
				m_fFadeAlpha = 255.f;

				CPlayer* pPlayer = dynamic_cast<CPlayer*>(CObjMgr::GetInstance()->GetPlayer());
				if (nullptr != pPlayer)
					pPlayer->SaveGame(L"PlayerData");

				ChangeScene(m_eNextScene);
				m_eSceneState = SCENE_STATE::START_SCENE;
			}
		}
		break;

	case SCENE_STATE::SCENE_PLAY:
		if (SCENEID::SCENEID_LOGO != m_eCurScene) {
			if (!CUIMgr::GetInstance()->GetIsChat())
			{
				// 씬 재시작
				if (m_pKeyMgr->KeyDown(KEY_ROOM_RESTART))
				{
					SetNextScene(m_eCurScene, m_wstrCurMap);
				}

				// 앞 씬
				if (m_pKeyMgr->KeyDown(KEY_ROOM_PREV))
				{
					/*if (0 <= m_iStageNum - 1)
					{
						--m_iStageNum;
						SetNextScene(SCENEID::SCENEID_STAGE, L"Map" + to_wstring(m_iStageNum));
					}*/

					//SetPlayerStartPos({ SCENE_LOAD_POS, SCENE_LOAD_POS, 0.f });
					SetNextScene(m_ePreScene, m_wstrPrevMap);
				}

				// 다음 씬
				if (m_pKeyMgr->KeyDown(KEY_ROOM_NEXT))
				{
					if (m_iMaxStageNum > m_iStageNum + 1)
					{
						++m_iStageNum;
						SetNextScene(SCENEID::SCENEID_STAGE, L"Map1");
					}
				}

				// 이벤트 씬으로
				if (m_pKeyMgr->KeyDown(KEY_ROOM_EVENT) && SCENEID::SCENEID_EVENT != m_eCurScene)
				{
					SetNextScene(SCENEID::SCENEID_EVENT, L"Event0");
				}

				// 플레이어 데이터 저장
				if (!m_pWordMgr->GetWord()) {
					/*if (m_pKeyMgr->KeyDown(KEY_WORD_EVENT))
					{
						//m_pWordMgr->AddWord(new WORD_DATA(L"Kyoko", L"Hello", -1));
						//m_pWordMgr->AddWord(new WORD_DATA(L"Kyoko", L"Word", -1));
						//m_pWordMgr->StartWord(true, L"Word0");
						dynamic_cast<CPlayer*>(CObjMgr::GetInstance()->GetPlayer())->SaveGame(L"PlayerData");
					}*/
				}

				// 충돌 렉트 렌더
				if (m_pKeyMgr->KeyDown(KEY_RECT_RENDER))
				{
					SetEvent(EVENT_RECT_RENDER, !GetEvent(EVENT_RECT_RENDER));
				}

				// 타일 렌더
				if (m_pKeyMgr->KeyDown(KEY_TILE_RENDER))
				{
					SetEvent(EVENT_TILE_RENDER, !GetEvent(EVENT_TILE_RENDER));
				}
			}
		}
		break;
	}

}

void CSceneMgr::LateUpdate()
{
	m_pScene->LateUpdate();
	m_pKeyMgr->Update();
}

void CSceneMgr::Render() {
	m_pDeviceMgr->Render_Begin();
	m_pViewMgr->Render();
	m_pScene->Render();

	m_pWordMgr->Render();

	if (SCENEID::SCENEID_LOGO != m_eCurScene) {
		m_pMouseMgr->Render();
	}

	// FadeOut
	if (SCENE_STATE::START_SCENE == m_eSceneState || SCENE_STATE::NEXT_SCENE == m_eSceneState)
	{
		DrawFadeBox();
	}
	
	//CFrameMgr::GetInstance()->RenderFPS();
	m_pDeviceMgr->Render_End();
}

void CSceneMgr::Release() {
	SafeDelete<CScene*>(m_pScene);

	m_pTileMgr->DestroyInstance();
	m_pViewMgr->DestroyInstance();
	m_pMouseMgr->DestroyInstance();
	CUIMgr::GetInstance()->DestroyInstance();
	m_pKeyMgr->DestroyInstance();
}

// 상태 패턴 (State Pattern)
void CSceneMgr::ChangeScene(SCENEID eID) {
	if (SCENEID::SCENEID_EVENT != m_eCurScene)
		m_ePreScene = m_eCurScene;

	m_eCurScene = eID;

	SafeDelete<CScene*>(m_pScene);

	switch (eID) {
	case SCENEID::SCENEID_LOGO:		
		m_pScene = new CLogo;
		break;

	case SCENEID::SCENEID_TITLE:
		m_pScene = new CTitle;
		break;

	case SCENEID::SCENEID_STAGE:
		m_pScene = new CStage;			
		break;

	case SCENEID::SCENEID_EVENT:
		m_pScene = new CEventScene;
		break;
	}

	m_pScene->Initialize();
}

void CSceneMgr::SetEvent(int iEvent, bool bIsOnOff)
{
	m_iEvent = (bIsOnOff) ? (m_iEvent | iEvent) : (m_iEvent & (~iEvent));
}

void CSceneMgr::SetQuest(int iQuest, bool bIsOnOff)
{
	m_iQuest = (bIsOnOff) ? (m_iQuest | iQuest) : (m_iQuest & (~iQuest));
}

void CSceneMgr::SetIsEventScene(int iEvent, bool bIsOnOff)
{
	m_iEventScene = (bIsOnOff) ? (m_iEventScene | iEvent) : (m_iEventScene & (~iEvent));
}

void CSceneMgr::SetNextScene(SCENEID eID, wstring wstrMapName)
{
	switch (eID)
	{
	case SCENEID::SCENEID_EVENT:
		m_wstrEventName = wstrMapName;
		break;

	case SCENEID::SCENEID_STAGE:
		if (lstrcmp(m_wstrCurMap.c_str(), m_wstrPrevMap.c_str()))
			m_wstrPrevMap = m_wstrCurMap;
		m_wstrCurMap = wstrMapName;
	}

	m_eNextScene = eID;
	m_eSceneState = SCENE_STATE::NEXT_SCENE;
}

void CSceneMgr::DrawFadeBox()
{
	D3DXMATRIX matScale, matTrans, matWorld;

	if (nullptr == m_pFadeTex)
	{
		m_pTextureMgr->LoadTexture(CTextureMgr::MULTI_TEXTURE, L"../Texture/BackGround/BlackBack/BlackBack.png", L"Background", L"BlackBack", 0.f, 0.f, 1, 1);
		m_pFadeTex = m_pTextureMgr->GetTexInfo(L"Background", L"BlackBack");
	}

	if (nullptr != m_pFadeTex)
	{
		D3DXMatrixScaling(&matScale, float(CLIENTCX) / m_pFadeTex->tImgInfo.Width, float(CLIENTCY) / m_pFadeTex->tImgInfo.Height, 0.f);
		D3DXMatrixTranslation(&matTrans, 0.f, 0.f, 0.f);
		matWorld = matScale * matTrans;

		m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
		m_pDeviceMgr->GetSprite()->Draw(m_pFadeTex->pTexture, nullptr,
			&m_pFadeTex->vOffset, nullptr, D3DCOLOR_ARGB(int(m_fFadeAlpha), 255, 255, 255));
	}
}
