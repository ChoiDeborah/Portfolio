#include "stdafx.h"
#include "WordMgr.h"

#include <locale>
#include <codecvt>

IMPLEMENT_SINGLETON(CWordMgr)

CWordMgr::CWordMgr()
	: m_pDeviceMgr(CDeviceMgr::GetInstance()),
	m_pTextureMgr(CTextureMgr::GetInstance()),
	m_pKeyMgr(CKeyMgr::GetInstance()),
	m_pSoundMgr(CSoundMgr::GetInstance())
{
}

CWordMgr::~CWordMgr()
{
	Release();
}

void CWordMgr::Release()
{
	ClearWord();
}

HRESULT CWordMgr::Initialize()
{
	m_pWordBoxTex = m_pTextureMgr->GetTexInfo(L"UI", L"WordBox");
	NULL_CHECK_MSG_RETURN(m_pWordBoxTex, L"WordBox Load Failed", E_FAIL);

	m_pWordNextTex = m_pTextureMgr->GetTexInfo(L"UI", L"WordNext");
	NULL_CHECK_MSG_RETURN(m_pWordNextTex, L"WordNext Load Failed", E_FAIL);

	if (nullptr == m_pSceneMgr)
		m_pSceneMgr = CSceneMgr::GetInstance();

	m_tWordAlarm.SetAlarm(0.07f, true);

	return S_OK;
}

void CWordMgr::Update()
{
	if (!m_bIsWord)
		return;

	if (nullptr == m_pSceneMgr)
		m_pSceneMgr = CSceneMgr::GetInstance();

	if (m_bIsWord) {
		switch (m_iRectState)
		{
		case 0:
			m_fRectY += 5.f;
			if (CLIENTCY * 0.15f <= m_fRectY)
			{
				m_fRectY = CLIENTCY * 0.15f;
				m_iRectState = 1;
			}
			break;

		case 1:
			if (m_pKeyMgr->KeyDown(KEY_COMUNICATION))
			{
				NextWord();
			}

			if (m_tWordAlarm.CheckAlarm())
			{
				if (int(m_tCurWord.wstrWord.length()) > m_iCurSize)
				{
					if (m_iCurSize % 3 == 0)
						m_pSoundMgr->PlaySound(L"Typing_Word.wav", CHANNEL_ID::CHANNEL_WINDOW);
					m_wstrCurWord += m_tCurWord.wstrWord[m_iCurSize];
					++m_iCurSize;
				}
			}

			if (m_bIsTexMove)
			{
				// ����
				if (0 == m_iTalkCount % 2)
				{
					m_vLeftCharPos.x += 10.f;
					if (0 <= m_vLeftCharPos.x)
					{
						m_vLeftCharPos.x = 0.f;
						m_bIsTexMove = false;
					}
				}

				// ������
				else
				{
					m_vRightCharPos.x -= 10.f;
					if (float(CLIENTCX) >= m_vRightCharPos.x)
					{
						m_vRightCharPos.x = float(CLIENTCX);
						m_bIsTexMove = false;
					}
				}
			}
			break;

		case 2:
			m_fRectY -= 5.f;
			if (0.f >= m_fRectY)
			{
				m_iRectState = 3;
				m_fRectY = 0.f;

				m_bIsWord = false;
				ClearWord();
				m_pSceneMgr->SetEvent(EVENT_ALL_STOP, false);
				return;
			}
			break;
		}
	}
}

void CWordMgr::Render()
{
	if (!m_bIsWord)
		return;

	m_pDeviceMgr->GetSprite()->End();
	m_pDeviceMgr->GetDevice()->EndScene();

	m_pDeviceMgr->GetDevice()->BeginScene();
	m_pDeviceMgr->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);

	D3DXMATRIX matScale, matTrans, matWorld;

	if (SCENEID::SCENEID_EVENT != m_pSceneMgr->GetCurScene())
	{
		// �� ��Ʈ
		const TEX_INFO*	pTexInfo = m_pTextureMgr->GetTexInfo(L"Background", L"BlackBack");
		if (nullptr != pTexInfo)
		{
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
		}
	}
	
	if (1 != m_iRectState)
		return;

	// ���� �̹���
	const TEX_INFO* pPrevTex = m_pTextureMgr->GetTexInfo(m_tRightWord.wstrObjKey, m_tRightWord.wstrStateKey);
	if (nullptr != pPrevTex)
	{
		D3DXMatrixScaling(&matScale, -1.f, 1.f, 0.f);
		D3DXMatrixTranslation(&matTrans, m_vRightCharPos.x, m_vRightCharPos.y, 0.f);
		matWorld = matScale * matTrans;
		m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
		m_pDeviceMgr->GetSprite()->Draw(pPrevTex->pTexture, nullptr, &pPrevTex->vOffset, nullptr, (0 == m_iTalkCount % 2) ? D3DCOLOR_ARGB(255, 128, 128, 128) : D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	// ĳ���� �̹���, 0���� ���� �� �׸��� ����
	const TEX_INFO*	pCharacterTex = m_pTextureMgr->GetTexInfo(m_tLeftWord.wstrObjKey, m_tLeftWord.wstrStateKey);
	if (nullptr != pCharacterTex)
	{
		D3DXMatrixScaling(&matScale,1.f, 1.f, 0.f);
		D3DXMatrixTranslation(&matTrans, m_vLeftCharPos.x, m_vLeftCharPos.y, 0.f);
		matWorld = matScale * matTrans;
		m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
		m_pDeviceMgr->GetSprite()->Draw(pCharacterTex->pTexture, nullptr, &pCharacterTex->vOffset, nullptr, (1 == m_iTalkCount % 2) ? D3DCOLOR_ARGB(255, 128, 128, 128) : D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	// ���� �ڽ�
	NULL_CHECK(m_pWordBoxTex);
	D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
	D3DXMatrixTranslation(&matTrans, CLIENTCX * 0.5f, CLIENTCY, 0.f);
	matWorld = matScale * matTrans;
	m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
	m_pDeviceMgr->GetSprite()->Draw(m_pWordBoxTex->pTexture, nullptr,
		&m_pWordBoxTex->vOffset, nullptr, m_pWordBoxTex->dwBlend);

	// �ؽ�Ʈ ����
	if (m_bIsTexMove && nullptr != m_pWordNextTex)
	{
		NULL_CHECK(m_pWordNextTex);
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
		D3DXMatrixTranslation(&matTrans, CLIENTCX * 0.5f + m_pWordBoxTex->tImgInfo.Width * 0.5f - 30.f, CLIENTCY - 30.f, 0.f);
		matWorld = matScale * matTrans;
		m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
		m_pDeviceMgr->GetSprite()->Draw(m_pWordNextTex->pTexture, nullptr,
			&m_pWordNextTex->vOffset, nullptr, m_pWordNextTex->dwBlend);
	}

	// �̸�
	D3DXMatrixScaling(&matScale, 2.f, 2.f, 0.f);
	D3DXMatrixTranslation(&matTrans, CLIENTCX * 0.5f - m_pWordBoxTex->tImgInfo.Width * 0.5f + 35.f, CLIENTCY - 220.f, 0.f);
	m_pDeviceMgr->GetSprite()->SetTransform(&(matScale * matTrans));
	m_pDeviceMgr->DrawFont(m_tCurWord.wstrName, nullptr, DT_LEFT | DT_TOP | DT_NOCLIP | DT_WORDBREAK);

	// �ؽ�Ʈ
	D3DXMatrixScaling(&matScale, 2.f, 2.f, 0.f);
	D3DXMatrixTranslation(&matTrans, CLIENTCX * 0.5f - m_pWordBoxTex->tImgInfo.Width * 0.5f + 35.f, CLIENTCY - 170.f, 0.f);
	m_pDeviceMgr->GetSprite()->SetTransform(&(matScale * matTrans));
	m_pDeviceMgr->DrawFont(m_wstrCurWord, nullptr, DT_LEFT | DT_TOP | DT_NOCLIP | DT_WORDBREAK);

	m_pDeviceMgr->GetSprite()->End();
	m_pDeviceMgr->GetDevice()->EndScene();

	m_pDeviceMgr->GetDevice()->BeginScene();
	m_pDeviceMgr->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);
}



void CWordMgr::StartWord(bool bIsLoadFile, const wstring& wstrFileName)
{
	if (bIsLoadFile)
	{
		LoadWordData(wstrFileName);
	}
	m_pSceneMgr->SetEvent(EVENT_ALL_STOP, true);
	m_bIsWord = true;
	m_iRectState = 0;
	NextWord();
}

HRESULT CWordMgr::LoadWordData(const wstring & wstrFileName)
{
	wstring wstrFilePath = L"../Data/WordData/";
	wstrFilePath += wstrFileName;
	wstrFilePath += L".txt";

	wifstream LoadFile;
	LoadFile.open(wstrFilePath, ios::in);
	LoadFile.imbue(std::locale("kor"));

	if (!LoadFile.good()) {
		FAILED_CHECK_MSG_RETURN(E_FAIL, L"Load Failed", E_FAIL);
	}

	// ����
	ClearWord();

	// ��ȭ ����
	TCHAR szWordNum[MAX_STR] = L"";
	LoadFile.getline(szWordNum, MAX_STR);
	int iWordNum = int(_wtoi(szWordNum));
	
	
	TCHAR szName[MAX_STR]		= L"";	// �̸�
	TCHAR szObjKey[MAX_STR]		= L"";	// ������Ʈ Ű
	TCHAR szStateKey[MAX_STR]	= L"";	// ���� Ű
	TCHAR szWord[MAX_STR] = L"";	// ��ȭ

	// ������Ʈ
	for (int i = 0; i < iWordNum; ++i)
	{
		LoadFile.getline(szName, MAX_STR, '|');
		LoadFile.getline(szObjKey, MAX_STR, '|');
		LoadFile.getline(szStateKey, MAX_STR, '|');
		LoadFile.getline(szWord, MAX_STR);

		AddWord(new WORD_DATA(szName, szWord, szObjKey, szStateKey));
	}

	LoadFile.close();
	return S_OK;
}

void CWordMgr::NextWord()
{
	if (m_lstWord.empty()) {
		m_iRectState = 2;
		return;
	}

	WORD_DATA* pWord = m_lstWord.front();
	if (nullptr != pWord)
	{
		if (lstrcmp(m_tCurWord.wstrStateKey.c_str(), pWord->wstrStateKey.c_str()))
		{
			++m_iTalkCount;

			// ����
			if (0 == m_iTalkCount % 2)
			{
				if (lstrcmp(m_tLeftWord.wstrStateKey.c_str(), pWord->wstrStateKey.c_str()))
				{
					m_tLeftWord = *pWord;
					m_vLeftCharPos = { -200.f, float(CLIENTCY), 0.f };
				}
			}
			// ������
			else
			{
				if (lstrcmp(m_tRightWord.wstrStateKey.c_str(), pWord->wstrStateKey.c_str()))
				{
					m_tRightWord = *pWord;
					m_vRightCharPos = { CLIENTCX + 200.f, float(CLIENTCY), 0.f };
				}
			}

			m_bIsTexMove = true;
		}

		m_tCurWord = *pWord;
		m_iCurSize = 0;
		m_wstrCurWord = L"";
		SafeDelete(pWord);
		m_lstWord.pop_front();
	}
	else
	{
		SafeDelete(pWord);
		m_lstWord.pop_front();
		NextWord();
	}
}

void CWordMgr::AddWord(WORD_DATA * pWordData)
{
	NULL_CHECK(pWordData);
	m_lstWord.push_back(pWordData);
}

void CWordMgr::ClearWord()
{
	m_tCurWord.wstrName = L"";
	m_tCurWord.wstrObjKey = L"";
	m_tCurWord.wstrStateKey = L"";
	m_tCurWord.wstrWord = L"";

	m_iTalkCount = 0;
	m_tLeftWord = m_tRightWord = m_tCurWord;
	m_vRightCharPos = { float(CLIENTCX + 200.f), float(CLIENTCY), 0.f };	// ���� �̹��� ��ġ
	m_vLeftCharPos = { -200.f, float(CLIENTCY), 0.f };	// ĳ���� �̹��� ��ġ

	for (auto& Word : m_lstWord)
		SafeDelete(Word);
	m_lstWord.clear();
}
