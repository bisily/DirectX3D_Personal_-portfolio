#include "Layer.h"

USING(Engine)

Engine::CLayer::CLayer()
{

}

Engine::CLayer::~CLayer()
{
	
}

HRESULT Engine::CLayer::Ready_Layer()
{
	return S_OK;
}

_int Engine::CLayer::Update_Layer(const _float& fTimeDelta)
{
	_int	isEnd = 0;

	auto& iter = m_mapObject.begin();
	auto& iterEnd = m_mapObject.end();
	for( ; iter != iterEnd; ++iter)
	{
		isEnd = iter->second->Update_Object(fTimeDelta);

		if (isEnd & 0x80000000)
		{
			Engine::Safe_Release(iter->second);
			m_mapObject.erase(iter++);
		}
	}

	return isEnd;
}

void Engine::CLayer::Render_Layer()
{
	for(auto& i : m_mapObject)
		i.second->Render_Object();
}

Engine::CLayer* Engine::CLayer::Create()
{
	CLayer*		pInstance = new CLayer;

	if(FAILED(pInstance->Ready_Layer()))
	{
		ERR_BOX(L"CLayer Created Failed");
		Engine::Safe_Release(pInstance);
	}

	return pInstance;
}

void Engine::CLayer::Free()
{
	for_each(m_mapObject.begin(), m_mapObject.end(), CDeleteMap());
	m_mapObject.clear();
}

HRESULT Engine::CLayer::Add_GameObject(const wstring pObjTag, CGameObject* pGameObject)
{
	NULL_CHECK_RETURN(pGameObject, E_FAIL);

	m_mapObject.insert(MAPOBJECT::value_type(pObjTag, pGameObject));

	return S_OK;
}

Engine::CComponent* Engine::CLayer::Get_Component(const wstring pObjTag, 
												const wstring pComponentTag, 
												Engine::CComponent::COMPONENTID eID)
{
	auto			iter = find_if(m_mapObject.begin(), m_mapObject.end(), CSTD_Tag_Finder(pObjTag));

	if(iter == m_mapObject.end())
		return nullptr;

	return iter->second->Get_Component(pComponentTag, eID);
}

CGameObject * CLayer::Get_Object(const wstring pObjTag)
{
	auto iter = find_if(m_mapObject.begin(), m_mapObject.end(), CSTD_Tag_Finder(pObjTag));

	if (m_mapObject.end() == iter)
	{
		return nullptr;
	}
	
	return iter->second;
}

CGameObject * CLayer::Get_Object(const wstring pObjTag, _long iObjIndex)
{
	int iCount = (int)m_mapObject.count(pObjTag);

	auto			iter = m_mapObject.find(pObjTag);

	if (1 != iCount)
	{
		for (int i = 0; i < iCount; ++i, ++iter)
		{
			if (iObjIndex != -1 && iObjIndex == iter->second->GetIndex())
			{
				return iter->second;
			}

			if (iter == m_mapObject.end())
			{
				return nullptr;
			}
		}
	}
	else
	{
		return iter->second;
	}


	return nullptr;
}

CComponent * CLayer::Get_Component(const wstring pObjTag, const wstring pComponentTag, Engine::CComponent::COMPONENTID eID, int iObjectIndex)
{
	int iCount = (int)m_mapObject.count(pObjTag);

	auto			iter = m_mapObject.find(pObjTag);
	
	for (int i = 0; i < iCount; ++i, ++iter)
	{
		if (iObjectIndex == iter->second->GetIndex())
		{
			return iter->second->Get_Component(pComponentTag, eID);
		}
	}

	return nullptr;
}

void CLayer::DeleteObj(int iIndex, wstring strObjKey)
{
	int iCount = (int)m_mapObject.count(strObjKey);

	auto iter = find_if(m_mapObject.begin(), m_mapObject.end(), CSTD_Tag_Finder(strObjKey));

	for (int i = 0; i < iCount; ++iter)
	{
		if (iIndex == iter->second->GetIndex())
		{
			iter->second->Free();
			m_mapObject.erase(iter);
			break;
		}
	}

}
