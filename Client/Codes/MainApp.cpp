#include "stdafx.h"
#include "MainApp.h"

#include "Export_Function.h"
#include "Logo.h"
#include "Stage.h"

CMainApp::CMainApp()
{
}

CMainApp::~CMainApp()
{
}

HRESULT CMainApp::Ready_MainApp()
{
	FAILED_CHECK_RETURN(SetUp_DefaultSetting(&m_pGraphicDev), E_FAIL);

	FAILED_CHECK_RETURN(Ready_ResourceForStatic(m_pGraphicDev, RESOURCE_END), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Renderer(m_pGraphicDev), E_FAIL);

	FAILED_CHECK_RETURN(Ready_OpeningScene(m_pGraphicDev, &m_pManagement), E_FAIL);

	Engine::Safe_Release(m_pGraphicDev);

	return S_OK;
}

_int CMainApp::Update_MainApp(const _float & fTimeDelta)
{
	m_fTime += fTimeDelta;

	if (nullptr == m_pManagement)
		return -1;

	Engine::SetUp_InputState();
	
	CKeyMgr::GetInstance()->Update();
	CSoundMgr::GetInstance()->Update();

	m_pManagement->Update_Scene(fTimeDelta);

	return 0;
}

void CMainApp::Render_MainApp()
{
	++m_dwRenderCnt;

	if (m_fTime >= 1.f)
	{
		wsprintf(m_szFPS, L"FPS : %d", m_dwRenderCnt);
		m_fTime = 0.f;
		m_dwRenderCnt = 0;
	}

	m_pGraphicInstance->Render_Begin(D3DXCOLOR(0.f, 0.f, 0.f, 1.f));

	if (nullptr != m_pManagement)
		m_pManagement->Render_Scene(m_pGraphicDev);

	//Engine::Render_Font(L"Font_Default", m_szFPS, &_vec2(400.f, 0.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

	m_pGraphicInstance->Render_End();
}

HRESULT CMainApp::SetUp_DefaultSetting(_In_ LPDIRECT3DDEVICE9 * ppGraphicDev)
{
	if (FAILED(Engine::Ready_GraphicDev(g_hWnd,
		Engine::CGraphicDev::MODE_WIN,
		WINCX, WINCY, &m_pGraphicInstance)))
		return E_FAIL;

	m_pGraphicInstance->AddRef();
	
	(*ppGraphicDev) = Engine::GetDevice();
	(*ppGraphicDev)->AddRef();

	(*ppGraphicDev)->SetRenderState(D3DRS_LIGHTING, TRUE);
	(*ppGraphicDev)->SetRenderState(D3DRS_ZENABLE, TRUE);		// Z버퍼에 무조건 기록은 하되 SORTING은 수행할 지 말지 판단,  TRUE인 경우 정렬을 하겠다는 의미
	(*ppGraphicDev)->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);  // Z버퍼에 픽셀의 깊이 값을 저장하겠다는 의미

	if (FAILED(Engine::Ready_InputDevice(g_hInst, g_hWnd)))
		return E_FAIL;

	// FontMgr
	if (FAILED(Engine::Ready_Font(m_pGraphicDev, L"Font_Default", L"바탕", 10, 15, FW_HEAVY)))
		return E_FAIL;

	CSoundMgr::GetInstance()->PlayBGM(L"oping_song.wav");

	return S_OK;
}

HRESULT CMainApp::Ready_ResourceForStatic(LPDIRECT3DDEVICE9 ppGraphicDev, RESOURCEID eMax)
{
	FAILED_CHECK(Engine::Reserve_ContainerSize(eMax));

	if (FAILED(Engine::Ready_Buffers(m_pGraphicDev,
		RESOURCE_STATIC,
		L"Buffer_RcTex_Center",
		Engine::CVIBuffer::BUFFER_RCTEX,
		0)))
	{
		ERR_BOX(L"Buffer_RcTex, Create Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Buffers(m_pGraphicDev,
		RESOURCE_STATIC,
		L"Buffer_RcTex",
		Engine::CVIBuffer::BUFFER_RCTEX,
		1)))
	{
		ERR_BOX(L"Buffer_RcTex, Create Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Buffers(m_pGraphicDev,
		RESOURCE_STATIC,
		L"Buffer_TrailTex",
		Engine::CVIBuffer::BUFFER_SWORDTRAIL,
		1)))
	{
		ERR_BOX(L"Buffer_SwordTrail, Create Failed");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CMainApp::Ready_OpeningScene(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CManagement ** ppManagementInst)
{
	if(FAILED(Engine::Create_Management(pGraphicDev, ppManagementInst)))
		return E_FAIL;

	(*ppManagementInst)->AddRef();

	Engine::CScene*			pOpeningScene = nullptr;

	pOpeningScene = CLogo::Create(pGraphicDev);
	NULL_CHECK_RETURN(pOpeningScene, E_FAIL);

	if(FAILED((*ppManagementInst)->SetUp_CurrentScene(pOpeningScene)))
		return E_FAIL;

	return S_OK;
}

CMainApp * CMainApp::Create()
{
	CMainApp*	pInstance = new CMainApp;

	if (FAILED(pInstance->Ready_MainApp()))
	{
		MSG_BOX("CMainApp Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainApp::Free()
{
	CSoundMgr::GetInstance()->DestroyInstance();
	CKeyMgr::GetInstance()->DestroyInstance();
	Engine::Safe_Release(m_pManagement);
	Engine::Safe_Release(m_pGraphicInstance);

	Engine::Release_Utility();
	Engine::Release_Resources();
	Engine::Release_System();
}
