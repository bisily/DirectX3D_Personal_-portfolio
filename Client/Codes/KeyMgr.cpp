#include "stdafx.h"
#include "KeyMgr.h"

IMPLEMENT_SINGLETON(CKeyMgr)

CKeyMgr::CKeyMgr()
{
	// 모든 키 상태를 false 초기화.
	memset(m_bKey, 0, sizeof(m_bKey));
}

CKeyMgr::~CKeyMgr()
{

}

bool CKeyMgr::GetKeyPressing(int iKey)
{
	if (GetAsyncKeyState(iKey) & 0x8000)
		return true;

	return false;
}

bool CKeyMgr::GetKeyDown(int iKey)
{
	// 전에 누른적 없고 현 시점에서 눌렸을 때
	if (!m_bKey[iKey] && GetAsyncKeyState(iKey) & 0x8000)
	{
		m_bKey[iKey] = !m_bKey[iKey];
		return true;
	}

	return false;
}

bool CKeyMgr::GetKeyUp(int iKey)
{
	// 전에 누른 적이 있고 현 시점에서 눌리지 않았을 때
	if (m_bKey[iKey] && !(GetAsyncKeyState(iKey) & 0x8000))
	{
		m_bKey[iKey] = !m_bKey[iKey];
		return true;
	}

	return false;
}

// Update함수는 GetKeyPressing, GetKeyDown, GetKeyUp 등을 호출한 후에 호출해주는 함수.
void CKeyMgr::Update()
{
	for (int i = 0; i < MAX_VIRTUAL_KEY; ++i)
	{
		if (m_bKey[i] && !(GetAsyncKeyState(i) & 0x8000))
			m_bKey[i] = !m_bKey[i];

		if (!m_bKey[i] && GetAsyncKeyState(i) & 0x8000)
			m_bKey[i] = !m_bKey[i];
	}
}