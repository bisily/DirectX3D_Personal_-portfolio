#ifndef SoundMgr_h__
#define SoundMgr_h__

#include "Defines.h"

class CSoundMgr
{
public:
	enum CHANNEL_ID { CHANNEL_BGM, CHANNEL_PLAYER, CHANNEL_PLAYER_HIT, CHANNEL_MONSTER, CHANNEL_EFFECT, CHANNEL_HIT, CHANNEL_BOSS_SWING, CHANNEL_BOSS_WAVE, CHANNEL_END };

private:
	CSoundMgr();
	~CSoundMgr();

public:
	void Initialize();
	void Update();
	void Release();

public:
	void LoadSoundFile();
	void PlaySound(const TCHAR* pSoundKey, CHANNEL_ID eID);
	void PlayBGM(const TCHAR* pSoundKey);
	void StopSound(CHANNEL_ID eID);
	void StopSoundAll();
	
public:
	// 싱글톤
	static CSoundMgr* GetInstance()
	{
		if (nullptr == m_pInstance)
			m_pInstance = new CSoundMgr;

		return m_pInstance;
	}

	void DestroyInstance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

private:
	static CSoundMgr* m_pInstance;

	FMOD_SYSTEM*	m_pSystem;	// 사운드 시스템 총괄 관리자
	FMOD_CHANNEL*	m_pChannelArr[CHANNEL_END]; // 사운드를 재생하기 위한 채널 배열

	// FMOD_SOUND:사운드 리소스 정보를 갖는 사운드 타입
	map<const TCHAR*, FMOD_SOUND*>	m_MapSound; 
};

#endif