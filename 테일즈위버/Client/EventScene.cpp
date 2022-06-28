#include "stdafx.h"
#include "EventScene.h"

#include "EventObj.h"

CEventScene::CEventScene()
{
}

CEventScene::~CEventScene()
{
	Release();
}

HRESULT CEventScene::Initialize()
{
	HRESULT hr = LoadEvent(m_pSceneMgr->GetEventName());
	FAILED_CHECK_MSG_RETURN(hr, L"LoadMap Failed", E_FAIL);
	m_pWordMgr->Initialize();

	m_pTexInfo = m_pTextureMgr->GetTexInfo(L"Background", L"Map", m_tMapData.iMapImageNum);
	
	if (nullptr != m_pTexInfo)
		m_pViewMgr->SetRoomSize(D3DXVECTOR3{ float(m_pTexInfo->tImgInfo.Width), float(m_pTexInfo->tImgInfo.Height), 0.f });

	if (!lstrcmp(L"Event0", m_pSceneMgr->GetEventName().c_str()))
	{
		m_pSoundMgr->StopSound(CHANNEL_ID::CHANNEL_BGM);
		m_pSoundMgr->PlayBGM(L"BGM_Boss.mp3");
	}
	else if (!lstrcmp(L"Event1", m_pSceneMgr->GetEventName().c_str()))
	{
		m_pSoundMgr->StopSound(CHANNEL_ID::CHANNEL_BGM);
		m_pSoundMgr->PlayBGM(L"OtherVillageBgm.wav");
	}
	else
	{
		m_pSoundMgr->StopSound(CHANNEL_ID::CHANNEL_BGM);
		m_pSoundMgr->PlayBGM(L"OtherVillageBgm.wav");
	}

	return S_OK;
}

void CEventScene::Update()
{
	m_pViewMgr->Update();

	if (m_pKeyMgr->KeyDown(VK_RETURN))
		m_pSceneMgr->SetNextScene(m_pSceneMgr->GetPreScene(), m_pSceneMgr->GetMapName());

	if (0 == m_iRectState)
	{
		m_fRectY += 5.f;
		if (CLIENTCY * 0.15f <= m_fRectY)
		{
			m_fRectY = CLIENTCY * 0.15f;
			m_iRectState = 1;
		}
	}
	else if (2 == m_iRectState)
	{
		m_fRectY -= 5.f;
		if (0.f >= m_fRectY)
		{
			m_iRectState = 3;
			m_fRectY = 0.f;

			if (!lstrcmp(L"Event0", m_pSceneMgr->GetEventName().c_str()))
				m_pSceneMgr->SetNextScene(SCENEID::SCENEID_EVENT, L"Event1");
			else if (!lstrcmp(L"Event1", m_pSceneMgr->GetEventName().c_str()))
				m_pSceneMgr->SetNextScene(SCENEID::SCENEID_STAGE, L"Map0");
			else
				m_pSceneMgr->SetNextScene(m_pSceneMgr->GetPreScene(), m_pSceneMgr->GetMapName());

			return;
		}
	}
}

void CEventScene::LateUpdate()
{
	// 프레임 이동
	if (!m_pWordMgr->GetWord())
	{
		m_tCurTime.fFrameTime += CTimeMgr::GetInstance()->GetDeltaTime();
		if (m_tCurTime.fFrameDelay < m_tCurTime.fFrameTime) {
			m_tCurTime.fFrameTime = m_tCurTime.fFrameDelay;
			++m_tCurTime.iFrameCur;

			if (m_tCurTime.iFrameCur >= m_tCurTime.iFrameEnd) {
				m_iRectState = 2;
			}
		}
	}

	// 워드
	vector<WORD_EVENT*>::iterator iter_word = m_vecEventWord.begin();
	for (; iter_word != m_vecEventWord.end(); ++iter_word)
	{
		if ((*iter_word)->fEventTime <= m_tCurTime.fFrameTime)
		{
			m_pWordMgr->StartWord(true, (*iter_word)->wstrFileName);
			SafeDelete(*iter_word);
			m_vecEventWord.erase(iter_word);
			break;
		}
	}

	// 이벤트 오브젝트
	for (auto& Notify : m_vecEventObj)
		Notify->UpdateNotify(m_tCurTime.fFrameTime);
}

void CEventScene::Render()
{
	// Background
	NULL_CHECK(m_pTexInfo);
	
	D3DXMATRIX matScale, matTrans, matWorld;

	D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
	D3DXMatrixTranslation(&matTrans, 0.f, 0.f, 0.f);
	matWorld = matScale * matTrans;
	m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
	m_pDeviceMgr->GetSprite()->Draw(m_pTexInfo->pTexture, nullptr,
		&m_pTexInfo->vOffset, nullptr, m_pTexInfo->dwBlend);

	const TEX_INFO* pTexInfo = nullptr;

	// 오브젝트
	for (size_t i = 0; i < m_vecEventObj.size(); ++i)
	{
		CEventObj* pEventObj = m_vecEventObj[i];

		if (nullptr != pEventObj)
		{
			EVENT_NOTIFY* pNotify = pEventObj->GetCurState();
			if (nullptr == pNotify)
				continue;

			// Obj Render
			if (pNotify->bIsActive)
			{
				pTexInfo = m_pTextureMgr->GetTexInfo(pNotify->wstrObjKey, pNotify->wstrStateKey, pEventObj->GetCurFrame());
				if (nullptr == pTexInfo)
					continue;

				D3DXMatrixScaling(&matScale, 1.f * pTexInfo->m_iMirror, 1.f, 0.f);
				D3DXMatrixTranslation(&matTrans, pNotify->vPos.x, pNotify->vPos.y, 0.f);
				matWorld = matScale * matTrans;
				m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
				m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
					&pTexInfo->vOffset, nullptr, pTexInfo->dwBlend);
			}
		}
	}


	// 블랙 렉트
	pTexInfo = m_pTextureMgr->GetTexInfo(L"Background", L"BlackBack");
	if (nullptr != pTexInfo)
	{
		m_pDeviceMgr->GetSprite()->End();
		m_pDeviceMgr->GetDevice()->EndScene();

		m_pDeviceMgr->GetDevice()->BeginScene();
		m_pDeviceMgr->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);
		//////////////////////////////////////////////////////////////////////////////////

		D3DXMatrixScaling(&matScale, float(CLIENTCX) / pTexInfo->tImgInfo.Width, float(CLIENTCY * 0.15f) / pTexInfo->tImgInfo.Height, 0.f);
		D3DXMatrixTranslation(&matTrans, 0.f, m_fRectY - float(CLIENTCY * 0.15f), 0.f);
		matWorld = matScale * matTrans;

		m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
		m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
			&pTexInfo->vOffset, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

		D3DXMatrixTranslation(&matTrans, 0.f, float(CLIENTCY * 1.f) - m_fRectY, 0.f);
		matWorld = matScale * matTrans;

		m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
		m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
			&pTexInfo->vOffset, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

		//////////////////////////////////////////////////////////////////////////////////

		m_pDeviceMgr->GetSprite()->End();
		m_pDeviceMgr->GetDevice()->EndScene();

		m_pDeviceMgr->GetDevice()->BeginScene();
		m_pDeviceMgr->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);

		m_pViewMgr->Render();
	}
}

void CEventScene::Release()
{
	// 삭제
	// 오브젝트
	for (auto& EventObj : m_vecEventObj) {
		SafeDelete(EventObj);
	}
	m_vecEventObj.clear();
	m_vecEventObj.shrink_to_fit();

	// 워드
	for (auto& EventWord : m_vecEventWord) {
		SafeDelete(EventWord);
	}
	m_vecEventWord.clear();
	m_vecEventWord.shrink_to_fit();
}

HRESULT CEventScene::LoadEvent(const wstring & wstrFileName)
{
	wstring wstrFilePath = L"../Data/EventData/";
	wstrFilePath += wstrFileName;
	wstrFilePath += L".txt";

	wifstream LoadFile;
	LoadFile.open(wstrFilePath, ios::in);

	if (!LoadFile.good()) {
		FAILED_CHECK_MSG_RETURN(E_FAIL, L"Load Failed", E_FAIL);
	}
	
	// 삭제
	for (auto& EventObj : m_vecEventObj) {
		SafeDelete(EventObj);
	}
	m_vecEventObj.clear();
	m_vecEventObj.shrink_to_fit();
	
	m_tMapData.wstrMapName = wstrFileName;

	// 맵 이미지 번호
	TCHAR szMapImageNum[MAX_STR] = L"";
	LoadFile.getline(szMapImageNum, MAX_STR);
	m_tMapData.iMapImageNum = int(_wtoi(szMapImageNum));


	// 씬 시간
	TCHAR szSceneMaxTime[MAX_STR] = L"";
	LoadFile.getline(szSceneMaxTime, MAX_STR);
	m_tCurTime.fFrameDelay = float(_wtof(szSceneMaxTime));


	// 오브젝트 개수
	TCHAR szObjNum[MAX_STR] = L"";
	LoadFile.getline(szObjNum, MAX_STR);
	int iObjNum = int(_wtoi(szObjNum));

	m_vecEventObj.reserve(iObjNum);

	// 각 오브젝트 wstrObjKey, wstrStateKey, vPos.x, vPos.y, vPos.z, vScale.x, vScale.y, vScale.z

	TCHAR szNotifyNum[MAX_STR] = L"";

	TCHAR szObjName[MAX_STR] = L"";
	TCHAR szIsViewObj[MAX_STR] = L"";

	TCHAR szTime[MAX_STR] = L"";
	TCHAR szX[MAX_STR] = L"";
	TCHAR szY[MAX_STR] = L"";
	TCHAR szObjKey[MAX_STR] = L"";
	TCHAR szStateKey[MAX_STR] = L"";
	TCHAR szMoving[MAX_STR] = L"";
	TCHAR szActive[MAX_STR] = L"";

	// 오브젝트
	for (int i = 0; i < iObjNum; ++i)
	{
		CEventObj* pEventObj = nullptr;

		// 오브젝트 이름
		LoadFile.getline(szObjName, MAX_STR, '|');
		LoadFile.getline(szIsViewObj, MAX_STR, '|');

		// 노티파이 개수
		LoadFile.getline(szNotifyNum, MAX_STR);
		int iNotifyNum = int(_ttoi(szNotifyNum));
		for (int j = 0; j < iNotifyNum; ++j)
		{
			LoadFile.getline(szTime, MAX_STR, '|');
			LoadFile.getline(szX, MAX_STR, '|');
			LoadFile.getline(szY, MAX_STR, '|');
			LoadFile.getline(szObjKey, MAX_STR, '|');
			LoadFile.getline(szStateKey, MAX_STR, '|');
			LoadFile.getline(szMoving, MAX_STR, '|');
			LoadFile.getline(szActive, MAX_STR);

			bool bIsMove = (1 == _ttoi(szMoving)) ? true : false;
			bool bIsActive = (1 == _ttoi(szActive)) ? true : false;

			// 노티파이 저장 : 시간 위치X, Y, 오브젝트키, 상태키, 무빙, 액티브
			if (0 == j)
			{
				pEventObj = new CEventObj(new EVENT_NOTIFY({ float(_ttof(szX)), float(_ttof(szY)), 0.f }, szObjKey, szStateKey, float(_ttof(szTime)), bIsMove, bIsActive));

				pEventObj->SetName(szObjName);
				pEventObj->SetViewObj(int(_wtoi(szIsViewObj)));

				m_vecEventObj.push_back(pEventObj);
			}
			else
			{
				if (nullptr == pEventObj)
					break;

				pEventObj->InsertNotify(new EVENT_NOTIFY({ float(_ttof(szX)), float(_ttof(szY)), 0.f }, szObjKey, szStateKey, float(_ttof(szTime)), bIsMove, bIsActive));
			}
		}
	}


	// 워드 이벤트 개수
	TCHAR szWordNum[MAX_STR] = L"";
	LoadFile.getline(szWordNum, MAX_STR);
	int iWordNum = int(_wtoi(szWordNum));

	TCHAR szFileName[MAX_STR] = L"";

	for (int i = 0; i < iWordNum; ++i)
	{
		LoadFile.getline(szTime, MAX_STR, '|');
		LoadFile.getline(szFileName, MAX_STR);

		m_vecEventWord.push_back(new WORD_EVENT(float(_wtof(szTime)), szFileName));
	}


	LoadFile.close();
	return S_OK;
}
