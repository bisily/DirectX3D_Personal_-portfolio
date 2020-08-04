#include "stdafx.h"
#include "Logo.h"

#include "Export_Function.h"
#include "BackGround.h"
#include "Stage.h"

CLogo::CLogo(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CScene(pGraphicDev)
{

}

CLogo::~CLogo()
{

}

HRESULT CLogo::Ready_Scene()
{
	if (FAILED(Engine::CScene::Ready_Scene()))
		return E_FAIL;

	if (FAILED(Ready_Resources()))
		return E_FAIL;

	if (FAILED(Ready_Layer_GameLogic(L"Layer_GameLogic")))
		return E_FAIL;

	m_pLoading = CLoading::Create(m_pGraphicDev, CLoading::LOADING_STAGE);
	NULL_CHECK_RETURN(m_pLoading, E_FAIL);
	
	return S_OK;
}

_int CLogo::Update_Scene(const _float& fTimeDelta)
{
	_int iExit = Engine::CScene::Update_Scene(fTimeDelta);

	if (true == m_pLoading->Get_Finish())
	{
		if (GetAsyncKeyState(VK_SPACE) & 0x8000)
		{
			Engine::CScene*	pScene = NULL;

			pScene = CStage::Create(m_pGraphicDev);
			NULL_CHECK_RETURN(pScene, E_FAIL);

			if (FAILED(Engine::SetUp_CurrentScene(pScene)))
				return E_FAIL;
		}
	}

	return 0;
}

void CLogo::Render_Scene()
{	
	if (FAILED(SetUp_DefaultSetting()))
		return;

	Engine::Render_Font(L"Font_Default",
						m_pLoading->Get_String(),
						&_vec2(0.f, 0.f),
						D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
}

HRESULT CLogo::Ready_Layer_GameLogic(const _tchar* pLayerTag)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject*		pGameObject = nullptr;

	// For.BackGround Logo Instance
	pGameObject = CBackGround::Create(m_pGraphicDev);

	if (nullptr == pGameObject)
		goto		except;

	if (FAILED(pLayer->Add_GameObject(L"BackGround", pGameObject)))
		goto		except;
	
	m_mapLayer.insert(MAPLAYER::value_type(pLayerTag, pLayer));

	return S_OK;

except:
	Engine::Safe_Release(pLayer);
	return E_FAIL;
}

HRESULT CLogo::Ready_Resources()
{
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev,
		RESOURCE_LOGO,
		L"Texture_Logo",
		Engine::CTexture::TYPE_NORMAL,
		L"../../Resources/Texture/Logo/Logo_0%d.jpg",
		7)))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CLogo::SetUp_DefaultSetting()
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);

	return S_OK;
}

CLogo* CLogo::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CLogo*	pInstance = new CLogo(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CLogo::Free()
{
	Engine::Safe_Release(m_pLoading);

	Engine::CScene::Free();
}

