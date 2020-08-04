#include "Timer.h"

USING(Engine)

CTimer::CTimer()
{

}

CTimer::~CTimer()
{

}

HRESULT CTimer::Ready_Timer()
{
	QueryPerformanceCounter(&m_FrameTime);
	QueryPerformanceCounter(&m_FixTime);
	QueryPerformanceCounter(&m_LastTime);
	QueryPerformanceFrequency(&m_CpuTick);

	return S_OK;
}

void Engine::CTimer::Set_TimeDelta()
{
	QueryPerformanceCounter(&m_FrameTime);

	if(m_FrameTime.QuadPart - m_FixTime.QuadPart >= m_CpuTick.QuadPart)
	{
		QueryPerformanceFrequency(&m_CpuTick);
		m_FixTime = m_FrameTime;
	}

	m_fTimeDelta = (_float(m_FrameTime.QuadPart) - _float(m_LastTime.QuadPart)) / _float(m_CpuTick.QuadPart);

	m_LastTime = m_FrameTime;
}

Engine::CTimer* CTimer::Create()
{
	CTimer*		pInstance = new CTimer;

	if(FAILED(pInstance->Ready_Timer()))
	{
		ERR_BOX(L"Timer Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTimer::Free()
{

}

