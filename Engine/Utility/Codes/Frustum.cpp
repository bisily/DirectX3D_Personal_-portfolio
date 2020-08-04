#include "Frustum.h"

USING(Engine)

CFrustum::CFrustum(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
{
	m_pGraphicDev->AddRef();
	ZeroMemory(m_vPoint, sizeof(_vec3) * 8);
	ZeroMemory(m_Plane, sizeof(D3DXPLANE) * 6);
}

CFrustum::~CFrustum()
{
}

HRESULT CFrustum::Ready_Frustum()
{
	// Make Vertex

	m_vPoint[0] = _vec3(-1.f, 1.f, 0.f);
	m_vPoint[1] = _vec3(1.f, 1.f, 0.f);
	m_vPoint[2] = _vec3(1.f, -1.f, 0.f);
	m_vPoint[3] = _vec3(-1.f, 1.f, 0.f);

	m_vPoint[4] = _vec3(-1.f, 1.f, 1.f);
	m_vPoint[5] = _vec3(1.f, 1.f, 1.f);
	m_vPoint[6] = _vec3(1.f, -1.f, 1.f);
	m_vPoint[7] = _vec3(-1.f, -1.f, 1.f);

	return S_OK;
}

_bool CFrustum::Is_Frustum_ForObject(const _vec3 * pPositionInWorld, 
									const _float fRadius)
{
	Ready_Frustum();

	_matrix		matProj, matView;

	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matProj, nullptr, &matProj);

	for (_uint i = 0; i < 8; ++i)
		D3DXVec3TransformCoord(&m_vPoint[i], &m_vPoint[i], &matProj);


	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, nullptr, &matView);

	for (_uint i = 0; i < 8; ++i)
		D3DXVec3TransformCoord(&m_vPoint[i], &m_vPoint[i], &matView);

	// 월드 좌표 상에서 절두체의 평면을 구성해주자
	//	x+
	D3DXPlaneFromPoints(&m_Plane[0], &m_vPoint[1], &m_vPoint[5], &m_vPoint[6]);

	//	x-
	D3DXPlaneFromPoints(&m_Plane[1], &m_vPoint[4], &m_vPoint[0], &m_vPoint[3]);

	//	y+
	D3DXPlaneFromPoints(&m_Plane[2], &m_vPoint[4], &m_vPoint[5], &m_vPoint[1]);

	//	y-
	D3DXPlaneFromPoints(&m_Plane[3], &m_vPoint[3], &m_vPoint[2], &m_vPoint[6]);

	//	z+
	D3DXPlaneFromPoints(&m_Plane[4], &m_vPoint[7], &m_vPoint[6], &m_vPoint[5]);

	//	z-
	D3DXPlaneFromPoints(&m_Plane[5], &m_vPoint[0], &m_vPoint[1], &m_vPoint[2]);

	return Is_InFrustum(pPositionInWorld, fRadius);
}

void CFrustum::Is_Frustum_ForTerrain(const _vec3 * pVertexPos, 
									const _ulong & dwCntX, 
									const _ulong & dwCntZ, 
									Engine::INDEX32 * pIndices, 
									_ulong * pTriCnt, 
									CQuadTree * pQuadTree)
{
	Ready_Frustum();
	ZeroMemory(pIndices, sizeof(Engine::INDEX32) * (dwCntX - 1) * (dwCntZ - 1) * 2);

	_matrix		matProj, matView;

	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matProj, nullptr, &matProj);

	for (_uint i = 0; i < 8; ++i)
		D3DXVec3TransformCoord(&m_vPoint[i], &m_vPoint[i], &matProj);


	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, nullptr, &matView);

	for (_uint i = 0; i < 8; ++i)
		D3DXVec3TransformCoord(&m_vPoint[i], &m_vPoint[i], &matView);

	// 월드 좌표 상에서 절두체의 평면을 구성해주자
	//	x+
	D3DXPlaneFromPoints(&m_Plane[0], &m_vPoint[1], &m_vPoint[5], &m_vPoint[6]);

	//	x-
	D3DXPlaneFromPoints(&m_Plane[1], &m_vPoint[4], &m_vPoint[0], &m_vPoint[3]);

	//	y+
	D3DXPlaneFromPoints(&m_Plane[2], &m_vPoint[4], &m_vPoint[5], &m_vPoint[1]);

	//	y-
	D3DXPlaneFromPoints(&m_Plane[3], &m_vPoint[3], &m_vPoint[2], &m_vPoint[6]);

	//	z+
	D3DXPlaneFromPoints(&m_Plane[4], &m_vPoint[7], &m_vPoint[6], &m_vPoint[5]);

	//	z-
	D3DXPlaneFromPoints(&m_Plane[5], &m_vPoint[0], &m_vPoint[1], &m_vPoint[2]);

	*pTriCnt = 0;

	if (nullptr != pQuadTree)
		pQuadTree->CullingForTerrain(this, pVertexPos, pIndices, pTriCnt);
	else
	{
		_bool		bIsIn[3] = { false };
		_ulong		dwTriCnt = 0;

		for (_ulong i = 0; i < dwCntZ - 1; ++i)
		{
			for (_ulong j = 0; j < dwCntX - 1; ++j)
			{
				_ulong dwIndex = i * dwCntX + j;

				// 오른쪽 상단 폴리곤
				bIsIn[0] = Is_InFrustum(&pVertexPos[dwIndex + dwCntX]);
				bIsIn[1] = Is_InFrustum(&pVertexPos[dwIndex + dwCntX + 1]);
				bIsIn[2] = Is_InFrustum(&pVertexPos[dwIndex + 1]);

				if (true == bIsIn[0] || true == bIsIn[1] || true == bIsIn[2])
				{
					pIndices[dwTriCnt]._1 = dwIndex + dwCntX;
					pIndices[dwTriCnt]._2 = dwIndex + dwCntX + 1;
					pIndices[dwTriCnt]._3 = dwIndex + 1;
					++dwTriCnt;
				}

				// 왼쪽 하단 폴리곤
				bIsIn[0] = Is_InFrustum(&pVertexPos[dwIndex + dwCntX]);
				bIsIn[1] = Is_InFrustum(&pVertexPos[dwIndex + 1]);
				bIsIn[2] = Is_InFrustum(&pVertexPos[dwIndex]);

				if (true == bIsIn[0] || true == bIsIn[1] || true == bIsIn[2])
				{
					pIndices[dwTriCnt]._1 = dwIndex + dwCntX;
					pIndices[dwTriCnt]._2 = dwIndex + 1;
					pIndices[dwTriCnt]._3 = dwIndex;
					++dwTriCnt;
				}
			}
		}

		*pTriCnt = dwTriCnt;
	}
}

_bool CFrustum::Is_InFrustum(const _vec3 * pPosition, 
							const _float fRadius)
{
	_float	fDistance = 0.f;

	for (_uint i = 0; i < 6; ++i)
	{
		fDistance = D3DXPlaneDotCoord(&m_Plane[i], pPosition);

		if (fDistance > fRadius)
			return false;
	}

	return true;
}

CFrustum * CFrustum::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CFrustum*	pInstance = new CFrustum(pGraphicDev);

	if (FAILED(pInstance->Ready_Frustum()))
	{
		ERR_BOX(L"Frustum Create Failed");
		Engine::Safe_Release(pInstance);
	}

	return pInstance;
}

void CFrustum::Free()
{
	Engine::Safe_Release(m_pGraphicDev);
}
