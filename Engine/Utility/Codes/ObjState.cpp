#include "ObjState.h"

USING(Engine)

Engine::CObjState::CObjState()
{
}

Engine::CObjState::~CObjState()
{

}

HRESULT Engine::CObjState::Ready_ObjState(_float fHp, _float fDamge)
{
	m_fHp = fHp;
	m_fMaxHp = fHp;
	m_fDamge = fDamge;

	return S_OK;
}

Engine::CObjState* Engine::CObjState::Create(_float fHp, _float fDamge)
{
	CObjState*	pInstance = new CObjState();

	if (FAILED(pInstance->Ready_ObjState(fHp, fDamge)))
	{
		ERR_BOX(L"ObjState Create Failed");
		Engine::Safe_Release(pInstance);
	}

	return pInstance;
}

void Engine::CObjState::Free()
{
}

