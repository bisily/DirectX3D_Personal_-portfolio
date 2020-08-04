#ifndef Collision_h__
#define Collision_h__

#include "Component.h"
#include "TerrainTex.h"
#include "Transform.h"

BEGIN(Engine)

class ENGINE_DLL CCalculator: public CComponent
{
private:
	typedef	struct tagOBB
	{
		// 위치 벡터
		_vec3		vPoint[8];
		_vec3		vCenter;

		// 방향 벡터
		_vec3		vProjAxis[3]; // 투영하고자 하는 벡터(세 개의 면으로 뻗어나가는 벡터)
		_vec3		vAxis[3];	// 임의의 축 : 객체와 평행한 임의의 축으로 사용될 벡터

	}OBB;

private:
	explicit CCalculator(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCalculator();

public:
	HRESULT		Ready_Calculator();

	_float		Compute_HeightOnTerrain(_vec3* pPosition,
										const _vec3* pTerrainVtxPos,
										const _ulong& dwCntX,
										const _ulong& dwCntZ);

	_vec3		Picking_OnTerrain(HWND hWnd,
									const CTerrainTex* pTerrainBufferCom,
									const CTransform* pTerrainTransCom);

	_bool		Collision_AABB(const _vec3* pDestMin, const _vec3* pDestMax, const _matrix* pDestWorld,
								const _vec3* pSourMin, const _vec3* pSourMax, const _matrix* pSourWorld);

	_bool		Collision_OBB(const _vec3* pDestMin, const _vec3* pDestMax, const _matrix* pDestWorld,
								const _vec3* pSourMin, const _vec3* pSourMax, const _matrix* pSourWorld,
								_vec3* pOut);

	_bool		Collision_Sphere(vector<SPHERE_COL> pDst, _matrix* pDstMatrix, 
								vector<SPHERE_COL> pSrc, _matrix* pSrcMatrix);

	_bool		Collision_AABB_Sphere(const _vec3* pDestMin, const _vec3* pDestMax, _matrix* pDestmatWorld, 
											vector<SPHERE_COL> pSrcList, _matrix* pSrcMatrix);
	
public:
	void		Set_Point(OBB* pObb, const _vec3* pMin, const _vec3* pMax);

	void		Set_Axis(OBB* pObb);
	
	_bool		Collision_Detail(vector<SPHERE_COL> pDst,
							_matrix* pDstMatrix,
							vector<SPHERE_COL> pSrc,
							_matrix* pSrcMatrix);

	_bool		Collision_AABB_Sphere_Detail(const _vec3* pDestMin, const _vec3* pDestMax, _matrix* pDestmatWorld,
											vector<SPHERE_COL> pSrcList, _matrix* pSrcMatrix);

private:
	LPDIRECT3DDEVICE9			m_pGraphicDev = nullptr;
public:
	static	CCalculator*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual	void	Free();
};

END

#endif // !Collision_h__
