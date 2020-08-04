#ifndef Loading_h__
#define Loading_h__

#include "Defines.h"
#include "Base.h"
#include "Engine_Defines.h"

class CLoading : public CBase 
{
public:
	enum LOADINGID { LOADING_STAGE, LOADING_BOSS, LOADING_END };

private:
	explicit						CLoading(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual							~CLoading();

public: // Getter
	LOADINGID						Get_LoadingID() const	{ return m_eLoadingID; }
	CRITICAL_SECTION*				Get_CriticalSection()	{ return &m_Crt; }
	Engine::_bool					Get_Finish() const		{ return m_bFinish;  }
	const _tchar*					Get_String() const		{ return m_szLoadingString;	}

public:
	static unsigned int CALLBACK	Thread_Main(void* pArg);

	HRESULT							Ready_Loading(LOADINGID eLoadingID);
	_uint							Loading_ForTexture();
	_uint							Loading_ForStage();
	_uint							Load_From_TextFile();

private:
	LOADINGID						m_eLoadingID;
	CRITICAL_SECTION				m_Crt;
	HANDLE							m_hThread		= nullptr;
	LPDIRECT3DDEVICE9				m_pGraphicDev	= nullptr;
	Engine::_bool					m_bFinish		= false;
	_tchar							m_szLoadingString[128];

public:
	static	CLoading*				Create(LPDIRECT3DDEVICE9 pGraphicDev, LOADINGID eLoadingID);
private:
	virtual void					Free();
};


#endif // Loading_h__
