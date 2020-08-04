#ifndef MainApp_h__
#define MainApp_h__

#include "Defines.h"
#include "Export_Function.h"
#include "../SuperClass/Base.h"

namespace Engine
{
	class CGraphicDev;
	class CResourcesMgr;
	class CManagement;
}

class CMainApp : public CBase
{
	// 생성자
private:
	explicit				CMainApp();
public:
	// 소멸자
	virtual					~CMainApp();

	// Getter
	// Setter

	// public 함수
public:
	HRESULT					Ready_MainApp();
	_int					Update_MainApp(const _float& fTimeDelta);
	void					Render_MainApp();
	// public 변수

	// protected 함수
	// protected 변수

	// private 함수
private:
	HRESULT					SetUp_DefaultSetting(LPDIRECT3DDEVICE9* ppGraphicDev);
	HRESULT					Ready_ResourceForStatic(LPDIRECT3DDEVICE9 ppGraphicDev, RESOURCEID eMax);
	HRESULT					Ready_OpeningScene(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CManagement** ppManagementInst);
	


	// private 변수
private:
	Engine::CGraphicDev*	m_pGraphicInstance	= nullptr;
	Engine::CManagement*	m_pManagement		= nullptr;
	LPDIRECT3DDEVICE9		m_pGraphicDev		= nullptr;
	_ulong					m_dwRenderCnt		= 0;
	_tchar					m_szFPS[128]		= L"";
	_float					m_fTime				= 0.f;

	// 생성 함수
public:
	static CMainApp*		Create();

	// 해제 함수
private:
	virtual void			Free();
};


#endif // !MainApp_h__
