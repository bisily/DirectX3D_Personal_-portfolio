#include "PrototypeMgr.h"

USING(Engine)
IMPLEMENT_SINGLETON(CPrototypeMgr)

Engine::CPrototypeMgr::CPrototypeMgr()
{

}

Engine::CPrototypeMgr::~CPrototypeMgr()
{
	Free();
}

HRESULT Engine::CPrototypeMgr::Ready_Prototype(const wstring pProtoTag, CComponent* pInstance)
{
	CComponent*			pPrototype = Find_Prototype(pProtoTag);

	if (nullptr != pPrototype)
		return E_FAIL;

	m_mapPrototype.insert(MAPPROTOTYPE::value_type(pProtoTag, pInstance));

	return S_OK;
}

Engine::CComponent* Engine::CPrototypeMgr::Clone_Prototype(const wstring pProtoTag)
{
	CComponent*		pPrototype = Find_Prototype(pProtoTag);
	NULL_CHECK_RETURN(pPrototype, nullptr);

	return pPrototype->Clone();
}

Engine::CComponent* Engine::CPrototypeMgr::Find_Prototype(const wstring pProtoTag)
{
	auto		iter = find_if(m_mapPrototype.begin(), m_mapPrototype.end(), CSTD_Tag_Finder(pProtoTag));

	if (iter == m_mapPrototype.end())
		return nullptr;

	return iter->second;
}

void Engine::CPrototypeMgr::Free()
{
	for_each(m_mapPrototype.begin(), m_mapPrototype.end(), CDeleteMap());
	m_mapPrototype.clear();
}

