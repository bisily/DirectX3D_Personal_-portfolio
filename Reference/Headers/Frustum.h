#ifndef Frusum_h__
#define Frusum_h__

#include "QuadTree.h"

BEGIN(Engine)

class ENGINE_DLL CFrustum : public CBase
{

private:
	explicit CFrustum(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CFrustum();

public:
	LPDIRECT3DDEVICE9		Get_GraphicDev(void) { return m_pGraphicDev; }

public:
	HRESULT		Ready_Frustum();
	
	_bool		Is_Frustum_ForObject(const _vec3* pPositionInWorld, 
									const _float fRadius = 0.f);

	void		Is_Frustum_ForTerrain(const _vec3* pVertexPos, 
										const _ulong& dwCntX, 
										const _ulong& dwCntZ, 
										Engine::INDEX32* pIndices, 
										_ulong* pTriCnt, 
										CQuadTree* pQuadTree);

	_bool		Is_InFrustum(const _vec3* pPosition, 
								const _float fRadius = 0.f);

private:
	LPDIRECT3DDEVICE9	m_pGraphicDev = nullptr;
	_vec3				m_vPoint[8];
	D3DXPLANE			m_Plane[6];

public:
	static CFrustum*	Create(LPDIRECT3DDEVICE9 pGraphicDev);

	virtual void Free();
};

END

#endif // !Frusum_h__
