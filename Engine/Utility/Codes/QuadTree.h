#ifndef QuadTree_h__
#define QuadTree_h__

#include "Engine_Defines.h"
#include "Base.h"

BEGIN(Engine)

class CFrustum;
class ENGINE_DLL CQuadTree : public CBase
{
public:
	enum CHILD { CHILD_LT, CHILD_RT, CHILD_LB, CHILD_RB, CHILD_END };
	enum CORNER { CORNER_LT, CORNER_RT, CORNER_LB, CORNER_RB, CORNER_END };
	enum NEIGHBOR { NEIGHBOR_LEFT, NEIGHBOR_TOP, NEIGHBOR_RIGHT, NEIGHBOR_BOTTOM, NEIGHBOR_END };

private:
	explicit CQuadTree();
	virtual ~CQuadTree();

public:
	HRESULT		Ready_QuadTree(const _ulong& dwCntX, 
								const _ulong dwCntZ);

	HRESULT		Ready_Neighbor();

	void		CullingForTerrain(Engine::CFrustum* pFrustum,
									const _vec3* pVerticesPos,
									INDEX32* pIndex,
									_ulong* pTriCnt);

	_bool		LevelOfDetail(LPDIRECT3DDEVICE9 pGraphicDev, 
								const _vec3* pVertexPos);

	void		Make_ChildCorner(CHILD eType,
								const _ulong& dwPLT,
								const _ulong& dwPRT,
								const _ulong& dwPLB,
								const _ulong& dwPRB,
								const _ulong& dwPCenter,
								_ulong* pLT,
								_ulong* pRT,
								_ulong* pLB,
								_ulong* pRB);

	void		Make_ChildQuadTree();

	void		Ready_Child(const _ulong& dwLT,
							const _ulong& dwRT,
							const _ulong& dwLB,
							const _ulong& dwRB);
	
	void		Make_Neighbor();

private:
	CQuadTree*			m_pChild[CHILD_END]{ nullptr };
	CQuadTree*			m_pNeighbor[NEIGHBOR_END]{ nullptr };
	_ulong				m_dwCorner[CORNER_END]{ 0 };
	_ulong				m_dwCenter = 0;
	_float				m_fRadius = 0.f;

public:
	static CQuadTree*	Create(const _ulong& dwCntX,
								const _ulong& dwCntZ);

	CQuadTree*	Create(const _ulong& dwLT,
						const _ulong& dwRT,
						const _ulong& dwLB,
						const _ulong& dwRB);

	virtual void Free();

};

END

#endif // QuadTree_h__