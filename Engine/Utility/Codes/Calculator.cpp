#include "Calculator.h"

USING(Engine)

Engine::CCalculator::CCalculator(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
{
	m_pGraphicDev->AddRef();
}

Engine::CCalculator::~CCalculator()
{
}

HRESULT CCalculator::Ready_Calculator()
{
	return S_OK;
}

_float CCalculator::Compute_HeightOnTerrain(_vec3 * pPosition, const _vec3 * pTerrainVtxPos, const _ulong & dwCntX, const _ulong & dwCntZ)
{
	_ulong	dwIndex = _ulong(pPosition->z / 1.f) * _ulong(dwCntX) + _ulong(pPosition->x / 1.f);

	_float	fRatioX = (pPosition->x - pTerrainVtxPos[dwIndex + dwCntX].x) / 1.f;
	_float	fRatioZ = (pTerrainVtxPos[dwIndex + dwCntX].z - pPosition->z) / 1.f;

	_float	fHeight[4] = {
		pTerrainVtxPos[dwIndex + dwCntX].y,
		pTerrainVtxPos[dwIndex + dwCntX + 1].y,
		pTerrainVtxPos[dwIndex + 1].y,
		pTerrainVtxPos[dwIndex].y
	};

	// 오른쪽 상단
	if (fRatioX > fRatioZ)
	{
		return fHeight[0] + (fHeight[1] - fHeight[0]) * fRatioX + (fHeight[2] - fHeight[1]) * fRatioZ;
	}

	// 왼쪽 하단
	else
	{
		return fHeight[0] + (fHeight[3] - fHeight[0]) * fRatioZ + (fHeight[2] - fHeight[3]) * fRatioX;
	}
}

_vec3 CCalculator::Picking_OnTerrain(HWND hWnd, const CTerrainTex * pTerrainBufferCom, const CTransform * pTerrainTransCom)
{
	POINT			ptMouse = { 0 };

	GetCursorPos(&ptMouse);
	ScreenToClient(hWnd, &ptMouse);		// 뷰 포트 영역(윈도우 좌표)

										// 투영 변환
	_vec3		vMousePos;

	D3DVIEWPORT9		ViewPort;
	ZeroMemory(&ViewPort, sizeof(D3DVIEWPORT9));

	m_pGraphicDev->GetViewport(&ViewPort);

	vMousePos.x = ptMouse.x / (ViewPort.Width * 0.5f) - 1.f;
	vMousePos.y = (ptMouse.y / -(ViewPort.Height * 0.5f)) + 1.f;
	vMousePos.z = 0.f;

	// 뷰 스페이스 변환
	_matrix			matProj;

	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matProj, NULL, &matProj);
	D3DXVec3TransformCoord(&vMousePos, &vMousePos, &matProj);

	_vec3	vRayPos, vRayDir;

	vRayPos = _vec3(0.f, 0.f, 0.f);
	vRayDir = vMousePos - vRayPos;

	// 월드 스페이스로 변환
	_matrix		matView;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, NULL, &matView);
	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matView);
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matView);

	// 로컬로 변환
	_matrix		matWorld;
	pTerrainTransCom->Get_WorldMatrix(&matWorld);
	D3DXMatrixInverse(&matWorld, NULL, &matWorld);

	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matWorld);
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matWorld);

	// picking 처리
	_ulong			dwVtxCntX = pTerrainBufferCom->Get_VtxCntX();
	_ulong			dwVtxCntZ = pTerrainBufferCom->Get_VtxCntZ();

	const _vec3*	pTerrainVtx = pTerrainBufferCom->Get_VtxPos();

	_ulong dwVtxIdx[3];
	_float fU, fV, fDist;

	for (_ulong z = 0; z < dwVtxCntZ - 1; ++z)
	{
		for (_ulong x = 0; x < dwVtxCntX - 1; ++x)
		{
			_ulong	dwIndex = z * dwVtxCntX + x;

			// 오른쪽 위
			dwVtxIdx[0] = dwIndex + dwVtxCntX;
			dwVtxIdx[1] = dwIndex + dwVtxCntX + 1;
			dwVtxIdx[2] = dwIndex + 1;

			if (D3DXIntersectTri(&pTerrainVtx[dwVtxIdx[1]],
				&pTerrainVtx[dwVtxIdx[0]],
				&pTerrainVtx[dwVtxIdx[2]],
				&vRayPos,
				&vRayDir,
				&fU, &fV, &fDist))
			{
				return _vec3(
					pTerrainVtx[dwVtxIdx[1]].x + (pTerrainVtx[dwVtxIdx[0]].x - pTerrainVtx[dwVtxIdx[1]].x) * fU,
					0.f,
					pTerrainVtx[dwVtxIdx[1]].z + (pTerrainVtx[dwVtxIdx[2]].z - pTerrainVtx[dwVtxIdx[1]].z) * fV);
			}

			// 왼쪽 아래
			dwVtxIdx[0] = dwIndex + dwVtxCntX;
			dwVtxIdx[1] = dwIndex + 1;
			dwVtxIdx[2] = dwIndex;

			if (D3DXIntersectTri(&pTerrainVtx[dwVtxIdx[2]],
				&pTerrainVtx[dwVtxIdx[1]],
				&pTerrainVtx[dwVtxIdx[0]],
				&vRayPos,
				&vRayDir,
				&fU, &fV, &fDist))
			{
				return _vec3(
					pTerrainVtx[dwVtxIdx[2]].x + (pTerrainVtx[dwVtxIdx[1]].x - pTerrainVtx[dwVtxIdx[2]].x) * fU,
					0.f,
					pTerrainVtx[dwVtxIdx[2]].z + (pTerrainVtx[dwVtxIdx[0]].z - pTerrainVtx[dwVtxIdx[2]].z) * fV);
			}
		}
	}

	return _vec3(0.f, 0.f, 0.f);
}

_bool CCalculator::Collision_AABB(const _vec3 * pDestMin, const _vec3 * pDestMax, const _matrix * pDestWorld, const _vec3 * pSourMin, const _vec3 * pSourMax, const _matrix * pSourWorld)
{
	_vec3		vDestMin, vDestMax, vSourMin, vSourMax;
	_float		fMin, fMax;

	D3DXVec3TransformCoord(&vDestMin, pDestMin, pDestWorld);
	D3DXVec3TransformCoord(&vDestMax, pDestMax, pDestWorld);

	D3DXVec3TransformCoord(&vSourMin, pSourMin, pSourWorld);
	D3DXVec3TransformCoord(&vSourMax, pSourMax, pSourWorld);

	fMin = max(vDestMin.x, vSourMin.x);
	fMax = min(vDestMax.x, vSourMax.x);

	if (fMax < fMin)
		return false;

	fMin = max(vDestMin.y, vSourMin.y);
	fMax = min(vDestMax.y, vSourMax.y);

	if (fMax < fMin)
		return false;

	fMin = max(vDestMin.z, vSourMin.z);
	fMax = min(vDestMax.z, vSourMax.z);

	if (fMax < fMin)
		return false;

	return true;
}

_bool CCalculator::Collision_OBB(const _vec3 * pDestMin, const _vec3 * pDestMax, const _matrix * pDestWorld, const _vec3 * pSourMin, const _vec3 * pSourMax, const _matrix * pSourWorld, _vec3* pOut)
{
	OBB			tObb[2];
	ZeroMemory(tObb, sizeof(OBB) * 2);

	Set_Point(&tObb[0], pDestMin, pDestMax);
	Set_Point(&tObb[1], pSourMin, pSourMax);

	for (_uint i = 0; i < 8; ++i)
	{
		D3DXVec3TransformCoord(&tObb[0].vPoint[i], &tObb[0].vPoint[i], pDestWorld);
		D3DXVec3TransformCoord(&tObb[1].vPoint[i], &tObb[1].vPoint[i], pSourWorld);
	}
	D3DXVec3TransformCoord(&tObb[0].vCenter, &tObb[0].vCenter, pDestWorld);
	D3DXVec3TransformCoord(&tObb[1].vCenter, &tObb[1].vCenter, pSourWorld);

	for (_uint i = 0; i < 2; ++i)
		Set_Axis(&tObb[i]);


	_float		fDistance[3] = {};	// 0.첫 번째 obb에서 벡터를 임의의 축으로 투영한 길이의 합		
									// 1.두 번째 obb에서 벡터를 임의의 축으로 투영한 길이의 합		
									// 2.각 obb의 중점을 이은 벡터를 임의의 축으로 투영한 길이

	for (_uint i = 0; i < 2; ++i)
	{
		for (_uint j = 0; j < 3; ++j)
		{
			fDistance[0] = fabs(D3DXVec3Dot(&tObb[0].vProjAxis[0], &tObb[i].vAxis[j]))
				+ fabs(D3DXVec3Dot(&tObb[0].vProjAxis[1], &tObb[i].vAxis[j]))
				+ fabs(D3DXVec3Dot(&tObb[0].vProjAxis[2], &tObb[i].vAxis[j]));

			fDistance[1] = fabs(D3DXVec3Dot(&tObb[1].vProjAxis[0], &tObb[i].vAxis[j]))
				+ fabs(D3DXVec3Dot(&tObb[1].vProjAxis[1], &tObb[i].vAxis[j]))
				+ fabs(D3DXVec3Dot(&tObb[1].vProjAxis[2], &tObb[i].vAxis[j]));

			fDistance[2] = fabs(D3DXVec3Dot(&(tObb[1].vCenter - tObb[0].vCenter), &tObb[i].vAxis[j]));

			if (fDistance[0] + fDistance[1] < fDistance[2])
				return false;
		}
	}

	_vec3 vTem = tObb[1].vCenter - tObb[0].vCenter;
	pOut->x = vTem.x;
	pOut->y = vTem.y;
	pOut->z = vTem.z;

	return true;
}

_bool CCalculator::Collision_Sphere(vector<SPHERE_COL> pDst, _matrix* pDstMatrix, vector<SPHERE_COL> pSrc, _matrix* pSrcMatrix)
{
	_matrix matDstTemp, matSrcTemp;
	_vec3 vDstCenter, vSrcCenter;
	_vec3 vLength;
	_float fDist = 0.f;

	vector<SPHERE_COL>::iterator Dstiter = pDst.begin();
	
	matDstTemp = *Dstiter->pmatPos * *pDstMatrix;

	memcpy(&vDstCenter, &matDstTemp.m[3][0], sizeof(_vec3));

	vector<SPHERE_COL>::iterator Srciter = pSrc.begin();
	
	matSrcTemp = *Srciter->pmatPos * *pSrcMatrix;

	memcpy(&vSrcCenter, &matSrcTemp.m[3][0], sizeof(_vec3));

	vLength = vDstCenter - vSrcCenter;
	fDist = D3DXVec3Length(&vLength);

	if (fDist <= Dstiter->fRadius + Srciter->fRadius)
	{
		return Collision_Detail(pDst, pDstMatrix, pSrc, pSrcMatrix);
	}
	

	return false;
}

_bool CCalculator::Collision_AABB_Sphere(const _vec3 * pDestMin, const _vec3 * pDestMax, _matrix* pDestmatWorld, vector<SPHERE_COL> pSrcList, _matrix * pSrcMatrix)
{
	_float fMax = 0.f, fMin = 0.f;
	_vec3 fSphereMax, fSphereMin;
	_vec3 pDestEditMin, pDestEditMax, temMax, temMin;
	
	D3DXVec3TransformCoord(&pDestEditMax, pDestMax, pDestmatWorld);
	D3DXVec3TransformCoord(&pDestEditMin, pDestMin, pDestmatWorld);
	
	temMax.x = max(pDestEditMax.x,pDestEditMin.x);
	temMax.y = max(pDestEditMax.y,pDestEditMin.y);
	temMax.z = max(pDestEditMax.z,pDestEditMin.z);

	temMin.x = min(pDestEditMax.x, pDestEditMin.x);
	temMin.y = min(pDestEditMax.y, pDestEditMin.y);
	temMin.z = min(pDestEditMax.z, pDestEditMin.z);
	
	pDestEditMax = temMax;
	pDestEditMin = temMin;
	
	auto& pSrc = *pSrcList.begin();

	_vec3 pSrcPoint;

	memcpy(&pSrcPoint, &pSrc.pmatPos->m[3][0], sizeof(_vec3));
	D3DXVec3TransformCoord(&pSrcPoint, &pSrcPoint, pSrcMatrix);

	fSphereMax.x = pSrcPoint.x + pSrc.fRadius;
	fSphereMax.y = pSrcPoint.y + pSrc.fRadius;
	fSphereMax.z = pSrcPoint.z + pSrc.fRadius;

	fSphereMin.x = pSrcPoint.x - pSrc.fRadius;
	fSphereMin.y = pSrcPoint.y - pSrc.fRadius;
	fSphereMin.z = pSrcPoint.z - pSrc.fRadius;
		
	if (pDestEditMin.x < fSphereMax.x)
	{
		if (fSphereMin.x < pDestEditMax.x)
		{
			if (pDestEditMin.y < fSphereMax.y)
			{
				if (fSphereMin.y < pDestEditMax.y)
				{
					if (pDestEditMin.z < fSphereMax.z)
					{
						if (fSphereMin.z < pDestEditMax.z)
						{
							return true;
						}
						else
						{
							return false;
						}
					}
					else
					{
						return false;
					}
				}
				else
				{
					return false;
				}
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}

	return false;
}

void CCalculator::Set_Point(OBB * pObb, const _vec3 * pMin, const _vec3 * pMax)
{
	pObb->vPoint[0] = _vec3(pMin->x, pMax->y, pMin->z);
	pObb->vPoint[1] = _vec3(pMax->x, pMax->y, pMin->z);
	pObb->vPoint[2] = _vec3(pMax->x, pMin->y, pMin->z);
	pObb->vPoint[3] = _vec3(pMin->x, pMin->y, pMin->z);


	pObb->vPoint[4] = _vec3(pMin->x, pMax->y, pMax->z);
	pObb->vPoint[5] = _vec3(pMax->x, pMax->y, pMax->z);
	pObb->vPoint[6] = _vec3(pMax->x, pMin->y, pMax->z);
	pObb->vPoint[7] = _vec3(pMin->x, pMin->y, pMax->z);

	pObb->vCenter = (*pMin + *pMax) * 0.5f;
}

void CCalculator::Set_Axis(OBB * pObb)
{
	pObb->vProjAxis[0] = ((pObb->vPoint[2] + pObb->vPoint[5]) * 0.5f) - pObb->vCenter; // X
	pObb->vProjAxis[1] = ((pObb->vPoint[0] + pObb->vPoint[5]) * 0.5f) - pObb->vCenter; // Y 
	pObb->vProjAxis[2] = ((pObb->vPoint[7] + pObb->vPoint[5]) * 0.5f) - pObb->vCenter; // Z


	pObb->vAxis[0] = pObb->vPoint[2] - pObb->vPoint[3]; // 평행한 가로축
	pObb->vAxis[1] = pObb->vPoint[0] - pObb->vPoint[3]; // 평행한 세로축
	pObb->vAxis[2] = pObb->vPoint[7] - pObb->vPoint[3]; // 평행한 깊이축

	for (_uint i = 0; i < 3; ++i)
		D3DXVec3Normalize(&pObb->vAxis[i], &pObb->vAxis[i]);
}

_bool CCalculator::Collision_Detail(vector<SPHERE_COL> pDst, _matrix * pDstMatrix, vector<SPHERE_COL> pSrc, _matrix * pSrcMatrix)
{
	_matrix matDstTemp, matSrcTemp;
	_vec3 vDstCenter, vSrcCenter;
	_vec3 vLength;
	_float fDist = 0.f;
	
	vector<SPHERE_COL>::iterator Dst = pDst.begin();
	vector<SPHERE_COL>::iterator Src = pSrc.begin();
	
	Src++;

	for (; Dst != pDst.end(); ++Dst)
	{
		matDstTemp = *Dst->pmatPos * *pDstMatrix;

		memcpy(&vDstCenter, &matDstTemp.m[3][0], sizeof(_vec3));

		for (; Src != pSrc.end(); ++Src)
		{
			matSrcTemp = *Src->pmatPos * *pSrcMatrix;

			memcpy(&vSrcCenter, &matSrcTemp.m[3][0], sizeof(_vec3));

			vLength = vDstCenter - vSrcCenter;
			fDist = D3DXVec3Length(&vLength);

			if (fDist <= Dst->fRadius + Src->fRadius)
			{
				return true;
			}
		}
	}

	return false;
}

_bool	CCalculator::Collision_AABB_Sphere_Detail(const _vec3* pDestMin, 
												const _vec3* pDestMax, 
												_matrix* pDestmatWorld,
												vector<SPHERE_COL> pSrcList, 
												_matrix* pSrcMatrix)
{
	_float fMax = 0.f, fMin = 0.f;
	_vec3 fSphereMax, fSphereMin;
	_vec3 pDestEditMin, pDestEditMax;

	D3DXVec3TransformCoord(&pDestEditMax, pDestMax, pDestmatWorld);
	D3DXVec3TransformCoord(&pDestEditMin, pDestMin, pDestmatWorld);

	auto& pSrc = pSrcList.begin();

	pSrc++;
	
	for (; pSrc != pSrcList.end(); ++pSrc)
	{
		_vec3 pSrcPoint;

		memcpy(&pSrcPoint, &pSrc->pmatPos->m[3][0], sizeof(_vec3));
		D3DXVec3TransformCoord(&pSrcPoint, &pSrcPoint, pSrcMatrix);

		fSphereMax.x = pSrcPoint.x + pSrc->fRadius;
		fSphereMax.y = pSrcPoint.y + pSrc->fRadius;
		fSphereMax.z = pSrcPoint.z + pSrc->fRadius;

		fSphereMin.x = pSrcPoint.x - pSrc->fRadius;
		fSphereMin.y = pSrcPoint.y - pSrc->fRadius;
		fSphereMin.z = pSrcPoint.z - pSrc->fRadius;

		if (pDestEditMin.x < fSphereMax.x)
		{
			if (fSphereMin.x < pDestEditMax.x)
			{
				if (pDestEditMin.y < fSphereMax.y)
				{
					if (fSphereMin.y < pDestEditMax.y)
					{
						if (pDestEditMin.z < fSphereMax.z)
						{
							if (fSphereMin.z < pDestEditMax.z)
							{
								return true;
							}
							else
							{
								continue;
							}
						}
						else
						{
							continue;
						}
					}
					else
					{
						continue;
					}
				}
				else
				{
					continue;
				}
			}
			else
			{
				continue;
			}
		}
		else
		{
			continue;
		}
	}

	return false;
}

CCalculator * CCalculator::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCalculator*  pInstance = new CCalculator(pGraphicDev);

	if (FAILED(pInstance->Ready_Calculator()))
	{
		ERR_BOX(L"Calculator Created Failed");
		Engine::Safe_Release(pInstance);
	}

	return pInstance;
}

void Engine::CCalculator::Free()
{
	Engine::Safe_Release(m_pGraphicDev);
}
