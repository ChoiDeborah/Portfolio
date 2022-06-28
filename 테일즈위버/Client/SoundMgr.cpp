#include "stdafx.h"
#include "SoundMgr.h"

IMPLEMENT_SINGLETON(CSoundMgr)

CSoundMgr::CSoundMgr()
{
}


CSoundMgr::~CSoundMgr()
{
	Release();
}

void CSoundMgr::Initialize()
{
	FMOD_System_Create(&m_pSystem); // �ý��� ����
	FMOD_System_Init(m_pSystem, CHANNEL_END, FMOD_INIT_NORMAL, nullptr);	// INIT_NORMAL : �ʱ�ȭ Ÿ��, ��� �÷��� Ÿ����
	LoadSoundFile();
}

void CSoundMgr::UpdateSound()
{
	FMOD_System_Update(m_pSystem);
}

void CSoundMgr::PlaySound(const wstring & wstrSoundKey, CHANNEL_ID eID)
{
	auto iter_find = m_mapSound.find(wstrSoundKey);

	if (m_mapSound.end() == iter_find)
		return;

	FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_FREE, iter_find->second,
		FALSE, &m_pChannelArr[eID]);

	SetVolume(m_fVolume);
	//UpdateSound();
}

void CSoundMgr::PlayBGM(const wstring & wstrSoundKey)
{
	auto iter_find = m_mapSound.find(wstrSoundKey);

	if (m_mapSound.end() == iter_find)
		return;

	FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_FREE, iter_find->second,
		FALSE, &m_pChannelArr[CHANNEL_BGM]);

	FMOD_Channel_SetMode(m_pChannelArr[CHANNEL_BGM], FMOD_LOOP_NORMAL);

	SetVolume(m_fVolume);
	//UpdateSound();
}

bool CSoundMgr::isPlaying(CHANNEL_ID eID)
{
	FMOD_BOOL bIsPlaying = false;
	
	FMOD_Channel_IsPlaying(m_pChannelArr[eID], &bIsPlaying);


	bool bIsResult = (0 == bIsPlaying) ? false : true;

	return bIsResult;
}

void CSoundMgr::StopSound(CHANNEL_ID eID)
{
	FMOD_Channel_Stop(m_pChannelArr[eID]);
	UpdateSound();
}

void CSoundMgr::StopAll()
{
	for (int i = 0; i < CHANNEL_END; ++i)
		FMOD_Channel_Stop(m_pChannelArr[i]);
	UpdateSound();
}

void CSoundMgr::SetVolume(float fVolume)
{
	m_fVolume = fVolume;
	for (int i = 0; i < CHANNEL_ID::CHANNEL_END; ++i) {
		FMOD_Channel_SetVolume(m_pChannelArr[i], fVolume);
	}
	UpdateSound();
}

void CSoundMgr::Release()
{
	for (auto Sound : m_mapSound) {
		FMOD_Sound_Release(Sound.second);
	}

	m_mapSound.clear();

	FMOD_System_Release(m_pSystem);
	FMOD_System_Close(m_pSystem);
}

void CSoundMgr::LoadSoundFile()
{
	_finddata_t fd;	// ����ü

	// ���� ������ ��� ���ϰ� ��� Ȯ���ڵ��� �˾Ƽ� �ҷ���
	int handle = int(_findfirst("../Sound/*.*", &fd));

	// �ҷ����� ����
	if (0 == handle)
		return;

	char szFullPath[128] = "";
	char szRelativePath[128] = "../Sound/";

	int iResult = 0;

	while (-1 != iResult) {
		strcpy_s(szFullPath, szRelativePath);

		strcat_s(szFullPath, fd.name);

		TCHAR* pSoundKey = new TCHAR[strlen(fd.name) + 1];

		// ��Ƽ->���̵� ��ȯ
		MultiByteToWideChar(CP_ACP, 0, fd.name, int(strlen(fd.name) + 1), pSoundKey, int(strlen(fd.name) + 1));

		FMOD_SOUND* pSound = nullptr;
		FMOD_RESULT eResult = FMOD_System_CreateSound(m_pSystem, szFullPath, FMOD_HARDWARE, nullptr, &pSound);

		if (FMOD_OK == eResult) {
			auto& iter_find = m_mapSound.find(pSoundKey);
			if (m_mapSound.end() == iter_find)
			{
				m_mapSound.insert({pSoundKey, pSound});
				if (!lstrcmp(pSoundKey, L"BGM2.wav"))
					PlayBGM(pSoundKey);
			}
			else {
				delete[] pSoundKey;
				FMOD_Sound_Release(pSound);
			}
		}
		else {
			delete[] pSoundKey;
		}

		iResult = _findnext(handle, &fd);
	}

	FMOD_System_Update(m_pSystem);
}