#include "InfoSubject.h"

USING(Engine)
IMPLEMENT_SINGLETON(CInfoSubject)

Engine::CInfoSubject::CInfoSubject(void)
{

}

Engine::CInfoSubject::~CInfoSubject(void)
{
	Release();
}

Engine::DATALIST* Engine::CInfoSubject::GetDataList(int iMessage)
{
	auto		iter = m_MapDataList.find(iMessage);

	if(m_MapDataList.end() == iter)
		return nullptr;

	return &iter->second;
}

void Engine::CInfoSubject::AddData(int iMessage, void* pData)
{
	if(pData)
	{
		auto	iter = m_MapDataList.find(iMessage);
		
		if(m_MapDataList.end() == iter)
		{
			m_MapDataList[iMessage] = DATALIST();
		}
		
		m_MapDataList[iMessage].push_back(pData);
		Notify(iMessage);
	}
}

void Engine::CInfoSubject::RemoveData(int iMessage, void* pData)
{
	auto		mapiter = m_MapDataList.find(iMessage);

	if(mapiter != m_MapDataList.end())
	{
		auto		iterList = mapiter->second.begin();
		auto		iterList_end = mapiter->second.end();

		for(; iterList != iterList_end; ++iterList)
		{
			if((*iterList) == pData)
			{
				mapiter->second.erase(iterList);
				return;
			}
		}
	}
}

void Engine::CInfoSubject::Release(void)
{
	for(auto& iter = m_MapDataList.begin();
		iter != m_MapDataList.end(); ++iter)
	{
		iter->second.clear();
	}
	m_MapDataList.clear();
}

