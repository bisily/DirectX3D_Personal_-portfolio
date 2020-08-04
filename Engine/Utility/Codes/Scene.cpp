#include "Scene.h"
#include "Layer.h"

USING(Engine)

Engine::CScene::CScene(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
{
	m_pGraphicDev->AddRef();
}

Engine::CScene::~CScene()
{
	
}

HRESULT Engine::CScene::Ready_Scene()
{
	return S_OK;
}

_int Engine::CScene::Update_Scene(const _float& fTimeDelta)
{
	_int		isEnd = 0;

	for(auto& i : m_mapLayer)
	{
		isEnd = i.second->Update_Layer(fTimeDelta);

		if(isEnd & 0x80000000)
			return -1;
	}

	return 0;
}

void Engine::CScene::Free()
{
	Engine::Safe_Release(m_pGraphicDev);

	for_each(m_mapLayer.begin(), m_mapLayer.end(), CDeleteMap());
	m_mapLayer.clear();
}

Engine::CComponent* Engine::CScene::Get_Component(const wstring pLayerTag, 
													const wstring pObjTag, 
													const wstring pComponentTag, 
													Engine::CComponent::COMPONENTID eID)
{
	auto		iter = find_if(m_mapLayer.begin(), m_mapLayer.end(), CSTD_Tag_Finder(pLayerTag));

	if(iter == m_mapLayer.end())
		return nullptr;
	
	return iter->second->Get_Component(pObjTag, pComponentTag, eID);
}

CComponent * CScene::Get_Component(const wstring pLayerTag, const wstring pObjTag, const wstring pComponentTag, Engine::CComponent::COMPONENTID eID, _int iObjectIndex)
{

	auto		iter = find_if(m_mapLayer.begin(), m_mapLayer.end(), CSTD_Tag_Finder(pLayerTag));

	if (iter == m_mapLayer.end())
		return nullptr;

	return iter->second->Get_Component(pObjTag, pComponentTag, eID, iObjectIndex);
}

CGameObject * CScene::Get_Object(const wstring pLayerTag, const wstring pObjTag, _long iObjIndex)
{
	auto iter = m_mapLayer.find(pLayerTag);

	if (m_mapLayer.end() == iter)
		return nullptr;

	return iter->second->Get_Object(pObjTag, iObjIndex);
}

CLayer * CScene::Get_Layer(const wstring pLayerTag)
{
	auto iter = m_mapLayer.find(pLayerTag);

	if (m_mapLayer.end() == iter)
	{
		return nullptr;
	}

	return iter->second;
}