#include "Management.h"
#include "Renderer.h"
#include "Export_Utility.h"

USING(Engine)
IMPLEMENT_SINGLETON(CManagement)

Engine::CManagement::CManagement()
{

}

Engine::CManagement::~CManagement()
{
	Free();
}

HRESULT Engine::CManagement::SetUp_CurrentScene(CScene* pCurrentScene)
{
	Engine::Safe_Release(m_pCurrentScene);

	CRenderer::GetInstance()->Clear_RenderGroup();

	m_pCurrentScene = pCurrentScene;

	return S_OK;
}

HRESULT CManagement::Ready_Management(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CShader*	pShader = nullptr;

	// hpp 파일을 읽어들임
	wifstream	LoadFile;

	LoadFile.open(L"../../Data/Shader.txt", ios::in);

	TCHAR szFilPath[64] = L"", szTagName[64] = L"";

	while (true)
	{
		LoadFile.getline(szFilPath, 64, '|');
		LoadFile.getline(szTagName, 64);

		if (!lstrcmp(szFilPath, L""))
		{
			LoadFile.eof();
			break;
		}

		pShader = CShader::Create(pGraphicDev, szFilPath);
		NULL_CHECK_RETURN(pShader, E_FAIL);
		
		FAILED_CHECK_RETURN(CPrototypeMgr::GetInstance()->Ready_Prototype(szTagName, pShader), E_FAIL);
	}

	LoadFile.close();

	D3DVIEWPORT9			ViewPort;
	pGraphicDev->GetViewport(&ViewPort);

	// RenderTarget
	
	// Albedo
	FAILED_CHECK_RETURN(Engine::Ready_RenderTarget(L"Target_Albedo",
		pGraphicDev,
		ViewPort.Width,
		ViewPort.Height,
		D3DFMT_A16B16G16R16F,		// 마지막 f는 실수 값을 저장했다는 것을 의미, 32로 된 옵션을 사용할 경우 프레임 드랍이 심함, 
		D3DXCOLOR(0.f, 0.f, 0.f, 0.f)), E_FAIL);
	
	FAILED_CHECK_RETURN(Engine::Ready_DebugBuffer(L"Target_Albedo", 0.f, 0.f, 200.f, 200.f), E_FAIL);

	// Normal
	FAILED_CHECK_RETURN(Engine::Ready_RenderTarget(L"Target_Normal",
		pGraphicDev,
		ViewPort.Width,
		ViewPort.Height,
		D3DFMT_A16B16G16R16F,
		D3DXCOLOR(0.f, 0.f, 0.f, 1.f)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_DebugBuffer(L"Target_Normal", 0.f, 200.f, 200.f, 200.f), E_FAIL);

	// Shade
	FAILED_CHECK_RETURN(Engine::Ready_RenderTarget(L"Target_Shade",
		pGraphicDev,
		ViewPort.Width,
		ViewPort.Height,
		D3DFMT_A16B16G16R16F,
		D3DXCOLOR(0.f, 0.f, 0.f, 1.f)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_DebugBuffer(L"Target_Shade", 200.f, 0.f, 200.f, 200.f), E_FAIL);

	// Specular
	FAILED_CHECK_RETURN(Engine::Ready_RenderTarget(L"Target_Specular",
		pGraphicDev,
		ViewPort.Width,
		ViewPort.Height,
		D3DFMT_A16B16G16R16F,
		D3DXCOLOR(0.f, 0.f, 0.f, 0.f)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_DebugBuffer(L"Target_Specular", 200.f, 200.f, 200.f, 200.f), E_FAIL);

	// Depth
	FAILED_CHECK_RETURN(Engine::Ready_RenderTarget(L"Target_Depth",
		pGraphicDev,
		ViewPort.Width,
		ViewPort.Height,
		D3DFMT_A32B32G32R32F,
		D3DXCOLOR(0.f, 0.f, 0.f, 1.f)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_DebugBuffer(L"Target_Depth", 0.f, 400.f, 200.f, 200.f), E_FAIL);

	// 멀티 렌더 타겟
	
	FAILED_CHECK_RETURN(Engine::Ready_MRT(L"MRT_Deffered", L"Target_Albedo"), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_MRT(L"MRT_Deffered", L"Target_Normal"), E_FAIL);
	
	FAILED_CHECK_RETURN(Engine::Ready_MRT(L"MRT_Deffered", L"Target_Depth"), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_MRT(L"MRT_LightAcc", L"Target_Shade"), E_FAIL);
	
	FAILED_CHECK_RETURN(Engine::Ready_MRT(L"MRT_LightAcc", L"Target_Specular"), E_FAIL);

	return S_OK;
}

_int Engine::CManagement::Update_Scene(const _float& fTimeDelta)
{
	if (nullptr == m_pCurrentScene)
		return -1;

	m_pCurrentScene->Update_Scene(fTimeDelta);

	return 0;
}

void Engine::CManagement::Render_Scene(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CRenderer::GetInstance()->Render_GameObject(pGraphicDev);

	if(nullptr == m_pCurrentScene)
		return;

	m_pCurrentScene->Render_Scene();
}

void CManagement::Render_ToolScene(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CRenderer::GetInstance()->Render_GameObject(pGraphicDev);

	if (nullptr == m_pCurrentScene)
		return;

	m_pCurrentScene->Render_Scene();
}

void Engine::CManagement::Free(void)
{
	Engine::Safe_Release(m_pCurrentScene);
}

Engine::CComponent* Engine::CManagement::Get_Component(const wstring pLayerTag, 
													const wstring pObjTag, 
													const wstring pComponentTag, 
													Engine::CComponent::COMPONENTID eID)
{
	if(nullptr == m_pCurrentScene)
		return nullptr;

	return m_pCurrentScene->Get_Component(pLayerTag, pObjTag, pComponentTag, eID);
}

Engine::CComponent * CManagement::Get_Component(const wstring pLayerTag
													, const wstring pObjTag
													, const wstring pComponentTag
													, Engine::CComponent::COMPONENTID eID
													, _int iObjectIndex)
{
	if (nullptr == m_pCurrentScene)
		return nullptr;

	return m_pCurrentScene->Get_Component(pLayerTag, pObjTag, pComponentTag, eID, iObjectIndex);
}

CGameObject * CManagement::Get_Object(const wstring pLayerTag, const wstring pObjTag, _long iObjIndex)
 {
	if (nullptr == m_pCurrentScene)
		return nullptr;

	return m_pCurrentScene->Get_Object(pLayerTag, pObjTag, iObjIndex);
}
