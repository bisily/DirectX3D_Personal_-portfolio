#include "Optimization.h"

USING(Engine)

Engine::COptimization::COptimization(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
	, m_pFrustum(nullptr)
	, m_pQuadTree(nullptr)
	, m_bIsClone(false)
{
	m_pGraphicDev->AddRef();
}

Engine::COptimization::COptimization(const COptimization & rhs)
	: m_pGraphicDev(rhs.m_pGraphicDev)
	, m_pFrustum(rhs.m_pFrustum)
	, m_pQuadTree(rhs.m_pQuadTree)
	, m_bIsClone(true)
{
	m_pGraphicDev->AddRef();
}

Engine::COptimization::~COptimization()
{
}

HRESULT Engine::COptimization::Ready_Optimization(const _ulong & dwCntX, 
													const _ulong & dwCntZ)
{
	m_pFrustum = CFrustum::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(m_pFrustum, E_FAIL);

	m_pQuadTree = CQuadTree::Create(dwCntX, dwCntZ);
	NULL_CHECK_RETURN(m_pQuadTree, E_FAIL);

	return S_OK;
}

_bool COptimization::Is_Frustum_ForObject(const _vec3 * pPositionInWorld, 
											const _float fRadius)
{
	return m_pFrustum->Is_Frustum_ForObject(pPositionInWorld, fRadius);
}

void COptimization::Is_Frustum_ForTerrain(const _vec3 * pVertexPos, 
											const _ulong & dwCntX, 
											const _ulong & dwCntZ, 
											Engine::INDEX32 * pIndices, 
											_ulong * pTriCnt)
{
	m_pFrustum->Is_Frustum_ForTerrain(pVertexPos, dwCntX, dwCntZ, pIndices, pTriCnt, m_pQuadTree);
}

COptimization * Engine::COptimization::Create(LPDIRECT3DDEVICE9 pGraphicDev, 
												const _ulong & dwCntX, 
												const _ulong & dwCntZ)
{
	COptimization*	pInstance = new COptimization(pGraphicDev);

	if (FAILED(pInstance->Ready_Optimization(dwCntX, dwCntZ)))
	{
		ERR_BOX(L"Optimization Create Failed");
		Engine::Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * Engine::COptimization::Clone()
{
	return new COptimization(*this);
}

void Engine::COptimization::Free()
{
	Engine::Safe_Release(m_pGraphicDev);

	if (false == m_bIsClone)
	{
		Engine::Safe_Release(m_pFrustum);
		Engine::Safe_Release(m_pQuadTree);
	}
}
