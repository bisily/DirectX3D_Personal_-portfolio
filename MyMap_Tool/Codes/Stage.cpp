#include "stdafx.h"
#include "Stage.h"

#include "Export_Function.h"
#include "Field.h"

CStage::CStage(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CScene(pGraphicDev)
{

}

CStage::~CStage(void)
{
	Free();
}

HRESULT CStage::Ready_Scene(void)
{
	if (FAILED(Engine::CScene::Ready_Scene()))
		return E_FAIL;

	if (FAILED(Ready_Resources()))
		return E_FAIL;

	if (FAILED(Ready_Layer_GameLogic(L"Layer_GameLogic")))
		return E_FAIL;

	return S_OK;
}

int CStage::Update_Scene(const float& fTimeDelta)
{
	return Engine::CScene::Update_Scene(fTimeDelta);
}

void CStage::Render_Scene(void)
{

}

HRESULT CStage::Ready_Layer_GameLogic(const TCHAR* pLayerTag)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject*		pGameObject = nullptr;

	pGameObject = CField::Create(m_pGraphicDev);

	NULL_CHECK_RETURN(pLayer->Add_GameObject(L"Field", pGameObject), E_FAIL);

	m_mapLayer.insert(MAPLAYER::value_type(pLayerTag, pLayer));

	return S_OK;
}

HRESULT CStage::Ready_Resources(void)
{

	return S_OK;
}

CStage* CStage::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CStage*	pInstance = new CStage(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CStage::Free(void)
{
	Engine::CScene::Free();
}

