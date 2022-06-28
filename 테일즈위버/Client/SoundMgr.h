#pragma once
class CSoundMgr
{
	DECLARE_SINGLETON(CSoundMgr)

public:
	CSoundMgr();
	~CSoundMgr();

public:
	void	Initialize();
	void	UpdateSound();

	void	PlaySound(const wstring& wstrSoundKey, CHANNEL_ID eID);
	void	PlayBGM(const wstring& wstrSoundKey);

	void	StopSound(CHANNEL_ID eID);
	void	StopAll();

	bool	isPlaying(CHANNEL_ID eID);
	
public:
	float&	GetVolume() { return m_fVolume; }

public:
	void	SetVolume(float fVolume);

private:
	void	Release();
	void	LoadSoundFile();

private:
	map<wstring, FMOD_SOUND*>	m_mapSound;
	FMOD_CHANNEL*	m_pChannelArr[CHANNEL_END];

	// FMOD_SOUND¿Í FMOD_CHANNELÀ» ÃÑ°ý °ü¸®
	FMOD_SYSTEM*	m_pSystem;

	float			m_fVolume = 0.7f;
};

