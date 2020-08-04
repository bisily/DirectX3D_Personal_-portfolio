#ifndef Optimization_h__
#define Optimization_h__

#include "Component.h"
#include "Frustum.h"
#include "QuadTree.h"

BEGIN(Engine)

class ENGINE_DLL COptimization : public CComponent
{
private:
	explicit COptimization(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit COptimization(const COptimization& rhs);

	virtual ~COptimization();

public:
	HRESULT Ready_Optimization(const _ulong& dwCntX, const _ulong& dwCntZ);

	_bool	Is_Frustum_ForObject(const _vec3* pPositionInWorld, 
		const _float fRadius);

	void	Is_Frustum_ForTerrain(const _vec3* pVertexPos,
								const _ulong& dwCntX,
								const _ulong& dwCntZ,
								Engine::INDEX32* pIndices,
								_ulong* pTriCnt);

private:
	LPDIRECT3DDEVICE9			m_pGraphicDev = nullptr;
	_bool						m_bIsClone;
	CFrustum*					m_pFrustum;
	CQuadTree*					m_pQuadTree;

public:
	static COptimization*	Create(LPDIRECT3DDEVICE9 pGraphicDev, 
		const _ulong& dwCntX, 
		const _ulong& dwCntZ);

	CComponent*			Clone();

	virtual void  Free();
};

END

#endif // Optimization_h__