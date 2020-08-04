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
	// �̱���
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

	FMOD_SYSTEM*	m_pSystem;	// ���� �ý��� �Ѱ� ������
	FMOD_CHANNEL*	m_pChannelArr[CHANNEL_END]; // ���带 ����ϱ� ���� ä�� �迭

	// FMOD_SOUND:���� ���ҽ� ������ ���� ���� Ÿ��
	map<const TCHAR*, FMOD_SOUND*>	m_MapSound; 
};

#endif