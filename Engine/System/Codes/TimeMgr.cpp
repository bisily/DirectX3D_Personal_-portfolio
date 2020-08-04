
#include "TimeMgr.h"
#include "Timer.h"

USING(Engine)
IMPLEMENT_SINGLETON(CTimerMgr)

Engine::CTimerMgr::CTimerMgr()
{

}

Engine::CTimerMgr::~CTimerMgr()
{
	Free();
}

HRESULT Engine::CTimerMgr::Ready_Timer(const _tchar* pTimerTag)
{
	CTimer*		pTimer = Find_Timer(pTimerTag);

	if(nullptr != pTimer)
		return E_FAIL;

	pTimer = CTimer::Create();
	NULL_CHECK_RETURN(pTimer, E_FAIL);

	m_mapTimers.insert(MAPTIMER::value_type(pTimerTag, pTimer));

	return S_OK;
}

void Engine::CTimerMgr::Free()
{
	for_each(m_mapTimers.begin(), m_mapTimers.end(), CDeleteMap());
	m_mapTimers.clear();
}

Engine::CTimer* Engine::CTimerMgr::Find_Timer(const _tchar* pTimerTag)
{
	auto	iter = find_if(m_mapTimers.begin(), m_mapTimers.end(), CTag_Finder(pTimerTag));

	if(iter == m_mapTimers.end())
		return nullptr;

	return iter->second;
}

_float Engine::CTimerMgr::Get_TimeDelta(const _tchar* pTimerTag) 
{
	CTimer*			pTimer = Find_Timer(pTimerTag);

	if(nullptr == pTimer)
		return 0.f;

	return pTimer->Get_TimeDelta();
}

void Engine::CTimerMgr::Set_TimeDelta(const _tchar* pTimerTag)
{
	CTimer*			pTimer = Find_Timer(pTimerTag);

	if(nullptr == pTimer)
		return;

	pTimer->Set_TimeDelta();
}

