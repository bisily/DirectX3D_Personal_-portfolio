#include "stdafx.h"
#include "SoundMgr.h"

CSoundMgr* CSoundMgr::m_pInstance = nullptr;

CSoundMgr::CSoundMgr()
{
	Initialize();
}

CSoundMgr::~CSoundMgr()
{
	Release();
}

void CSoundMgr::Initialize()
{
	FMOD_System_Create(&m_pSystem);
	FMOD_System_Init(m_pSystem, CHANNEL_END, FMOD_INIT_NORMAL, nullptr);

	LoadSoundFile();
}

void CSoundMgr::Update()
{
	FMOD_System_Update(m_pSystem);
}

void CSoundMgr::Release()
{
	for (auto iter : m_MapSound)
	{
		delete[] iter.first;
		FMOD_Sound_Release(iter.second);
	}

	m_MapSound.clear();

	FMOD_System_Close(m_pSystem);
	FMOD_System_Release(m_pSystem);
}

void CSoundMgr::LoadSoundFile()
{
	//_finddata_t: <io.h>에서 제공하며 파일 정보를 저장하는 구조체이다.
	_finddata_t _fd = {};

	long handle = 0;

	// _findfirst: <io.h>에서 제공하며 사용자가 설정한 경로 내에서 가장 첫번째 파일을 찾는 함수
	handle = _findfirst("../../Sound/*.*", &_fd);

	if (-1 == handle)
	{
		MessageBox(g_hWnd, L"Not found file!!", L"Falied!!", MB_OK);
		return;
	}

	int iRes = 0;

	while (-1 != iRes)
	{
		char szFullPath[128] = "../../Sound/";
		strcat_s(szFullPath, _fd.name);		

		FMOD_SOUND* pSound = nullptr;
		FMOD_RESULT FRes = FMOD_System_CreateSound(m_pSystem, szFullPath, FMOD_HARDWARE, nullptr, &pSound);

		if (FMOD_OK == FRes)
		{
			TCHAR* pSoundKey = new TCHAR[strlen(_fd.name) + 1];

			MultiByteToWideChar(CP_ACP, 0, _fd.name, strlen(_fd.name) + 1
				, pSoundKey, strlen(_fd.name) + 1);

			m_MapSound.insert(make_pair(pSoundKey, pSound));
		}

		// _findnext: <io.h>에서 제공하며 다음 위치의 파일을 찾는다. 더이상 없다면 -1을 반환한다.
		iRes = _findnext(handle, &_fd);
	}
}

void CSoundMgr::PlaySound(const TCHAR * pSoundKey, CHANNEL_ID eID)
{
	auto iter_find = find_if(m_MapSound.begin(), m_MapSound.end(),
		[&pSoundKey](auto Mypair)->bool
	{
		return !lstrcmp(pSoundKey, Mypair.first);
	}
	);

	if (m_MapSound.end() == iter_find)
	{
		MessageBox(g_hWnd, pSoundKey, L"Play Failed!!", MB_OK);
		return;
	}

	FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_FREE, iter_find->second,
		FALSE, &(m_pChannelArr[eID]));
}

void CSoundMgr::PlayBGM(const TCHAR * pSoundKey)
{
	auto iter_find = find_if(m_MapSound.begin(), m_MapSound.end(),
		[&pSoundKey](auto Mypair)->bool
	{
		return !lstrcmp(pSoundKey, Mypair.first);
	}
	);

	if (m_MapSound.end() == iter_find)
	{
		MessageBox(g_hWnd, pSoundKey, L"Play Failed!!", MB_OK);
		return;
	}

	FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_FREE, iter_find->second,
		FALSE, &(m_pChannelArr[CHANNEL_BGM]));

	FMOD_Channel_SetMode(m_pChannelArr[CHANNEL_BGM], FMOD_LOOP_NORMAL);	
}

void CSoundMgr::StopSound(CHANNEL_ID eID)
{
	FMOD_Channel_Stop(m_pChannelArr[eID]);
}

void CSoundMgr::StopSoundAll()
{
	for(auto i : m_pChannelArr)
		FMOD_Channel_Stop(i);
}
