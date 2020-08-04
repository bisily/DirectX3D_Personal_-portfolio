#include "GameObject.h"

USING(Engine)

Engine::CGameObject::CGameObject(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
{
	m_pGraphicDev->AddRef();
}

Engine::CGameObject::~CGameObject()
{
	
}

CComponent * CGameObject::Get_Component(const wstring pComponentTag, 
										Engine::CComponent::COMPONENTID eID)
{
	Engine::CComponent*		pComponent = Find_Component(pComponentTag, eID);

	NULL_CHECK_RETURN(pComponentTag.c_str(), nullptr);

	return pComponent;
}

CComponent * CGameObject::Find_Component(const wstring pComponentTag, 
										Engine::CComponent::COMPONENTID eID)
{
	auto		iter = find_if(m_mapComponent[eID].begin(), m_mapComponent[eID].end(), CSTD_Tag_Finder(pComponentTag));

	if(iter == m_mapComponent[eID].end())
		return nullptr;

	return iter->second;
}

void CGameObject::Compute_ViewZ(const _vec3 * pPosition)
{
	_matrix			matView;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);

	D3DXMatrixInverse(&matView, nullptr, &matView);

	_vec3	vCampos;
	memcpy(&vCampos, &matView.m[3][0], sizeof(_vec3));

	_vec3	vDist = vCampos - (*pPosition);

	m_fViewZ = D3DXVec3Length(&vDist);
}

HRESULT Engine::CGameObject::Ready_Object()
{
	return S_OK;
}

_int Engine::CGameObject::Update_Object(const _float& fTimeDelta)
{
	for(auto& i : m_mapComponent[CComponent::ID_DYNAMIC])
		i.second->Update_Component(fTimeDelta);

	return 0;
}

void Engine::CGameObject::Render_Object()
{

}

void Engine::CGameObject::Free()
{
	for(_int i = 0; i < CComponent::ID_END; ++i)
	{
		for_each(m_mapComponent[i].begin(), m_mapComponent[i].end(), CDeleteMap());
		m_mapComponent[i].clear();
	}

	Engine::Safe_Release(m_pGraphicDev);

}

