#include "Collider.h"
#include "Export_Resources.h"

USING(Engine)

Engine::CCollider::CCollider(LPDIRECT3DDEVICE9 pGraphicDev)
{
	m_pGraphicDev = pGraphicDev;
	m_pGraphicDev->AddRef();
}

Engine::CCollider::~CCollider()
{

}

HRESULT Engine::CCollider::Ready_Collider(const _vec3* pPosition, 
											const _ulong& dwNumVertices, 
											const _ulong& dwStride)
{
	D3DXMatrixIdentity(&m_matWorld);

	// 바운딩 박스롤 생성하는 함수
	D3DXComputeBoundingBox(pPosition, dwNumVertices, sizeof(_vec3), &m_vMin, &m_vMax);
	// 1인자 : 전체 정점들 중에 첫번째 정점의 포지션 값
	// 4,5인자 : output 인자, 큐브의 3번 버텍스, 5번 버텍스에 해당

#ifdef _DEBUG

	FAILED_CHECK_RETURN(m_pGraphicDev->CreateVertexBuffer(sizeof(VTXCUBE) * 8,
		0,
		D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0),
		D3DPOOL_MANAGED,
		&m_pVB,
		NULL), E_FAIL);

	FAILED_CHECK_RETURN(m_pGraphicDev->CreateIndexBuffer(sizeof(INDEX16) * 12,
		0,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIB,
		nullptr), E_FAIL);


	VTXCUBE*		pVtxCube = NULL;

	m_pVB->Lock(0, 0, (void**)&pVtxCube, 0);

	// 전면부
	pVtxCube[0].vPosition = D3DXVECTOR3(m_vMin.x, m_vMax.y, m_vMin.z);
	pVtxCube[0].vTex = pVtxCube[0].vPosition;

	pVtxCube[1].vPosition = D3DXVECTOR3(m_vMax.x, m_vMax.y, m_vMin.z);
	pVtxCube[1].vTex = pVtxCube[1].vPosition;

	pVtxCube[2].vPosition = D3DXVECTOR3(m_vMax.x, m_vMin.y, m_vMin.z);
	pVtxCube[2].vTex = pVtxCube[2].vPosition;

	pVtxCube[3].vPosition = D3DXVECTOR3(m_vMin.x, m_vMin.y, m_vMin.z);
	pVtxCube[3].vTex = pVtxCube[3].vPosition;

	// 후면부
	pVtxCube[4].vPosition = D3DXVECTOR3(m_vMin.x, m_vMax.y, m_vMax.z);
	pVtxCube[4].vTex = pVtxCube[4].vPosition;

	pVtxCube[5].vPosition = D3DXVECTOR3(m_vMax.x, m_vMax.y, m_vMax.z);
	pVtxCube[5].vTex = pVtxCube[5].vPosition;

	pVtxCube[6].vPosition = D3DXVECTOR3(m_vMax.x, m_vMin.y, m_vMax.z);
	pVtxCube[6].vTex = pVtxCube[6].vPosition;

	pVtxCube[7].vPosition = D3DXVECTOR3(m_vMin.x, m_vMin.y, m_vMax.z);
	pVtxCube[7].vTex = pVtxCube[7].vPosition;

	m_pVB->Unlock();

	INDEX16*		pIndex = NULL;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	// +x
	pIndex[0]._1 = 1;
	pIndex[0]._2 = 5;
	pIndex[0]._3 = 6;

	pIndex[1]._1 = 1;
	pIndex[1]._2 = 6;
	pIndex[1]._3 = 2;

	// -x
	pIndex[2]._1 = 4;
	pIndex[2]._2 = 0;
	pIndex[2]._3 = 3;

	pIndex[3]._1 = 4;
	pIndex[3]._2 = 3;
	pIndex[3]._3 = 7;

	// +y
	pIndex[4]._1 = 4;
	pIndex[4]._2 = 5;
	pIndex[4]._3 = 1;

	pIndex[5]._1 = 4;
	pIndex[5]._2 = 1;
	pIndex[5]._3 = 0;

	// -y
	pIndex[6]._1 = 3;
	pIndex[6]._2 = 2;
	pIndex[6]._3 = 6;

	pIndex[7]._1 = 3;
	pIndex[7]._2 = 6;
	pIndex[7]._3 = 7;

	// +z
	pIndex[8]._1 = 7;
	pIndex[8]._2 = 6;
	pIndex[8]._3 = 5;

	pIndex[9]._1 = 7;
	pIndex[9]._2 = 5;
	pIndex[9]._3 = 4;

	// -z
	pIndex[10]._1 = 0;
	pIndex[10]._2 = 1;
	pIndex[10]._3 = 2;

	pIndex[11]._1 = 0;
	pIndex[11]._2 = 2;
	pIndex[11]._3 = 3;

	m_pIB->Unlock();

	for (_uint i = 0; i < COLL_END; ++i)
	{
		m_pGraphicDev->CreateTexture(1, 1, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &m_pTexture[i], NULL);

		D3DLOCKED_RECT			LockRect;
		ZeroMemory(&LockRect, sizeof(D3DLOCKED_RECT));

		m_pTexture[i]->LockRect(0, &LockRect, NULL, 0);

		*((_ulong*)LockRect.pBits) = D3DXCOLOR(1.f * i, 0.5f * (1.f - i), 0.f, 1.f);

		m_pTexture[i]->UnlockRect(0);
	}

#endif

	return S_OK;
}

HRESULT CCollider::Ready_Collider(const _ulong& dwNumVertices,
									const _ulong& dwStride,
									const _vec3* vMin,
									const _vec3* vMax)
{
	D3DXMatrixIdentity(&m_matWorld);

	m_vMax = *vMax;
	m_vMin = *vMin;

#ifdef _DEBUG

	FAILED_CHECK_RETURN(m_pGraphicDev->CreateVertexBuffer(sizeof(VTXCUBE) * 8,
		0,
		D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0),
		D3DPOOL_MANAGED,
		&m_pVB,
		NULL), E_FAIL);

	FAILED_CHECK_RETURN(m_pGraphicDev->CreateIndexBuffer(sizeof(INDEX16) * 12,
		0,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIB,
		nullptr), E_FAIL);

	VTXCUBE*		pVtxCube = NULL;

	m_pVB->Lock(0, 0, (void**)&pVtxCube, 0);

	// 전면부
	pVtxCube[0].vPosition = D3DXVECTOR3(vMin->x, vMax->y, vMin->z);
	pVtxCube[0].vTex = pVtxCube[0].vPosition;

	pVtxCube[1].vPosition = D3DXVECTOR3(vMax->x, vMax->y, vMin->z);
	pVtxCube[1].vTex = pVtxCube[1].vPosition;

	pVtxCube[2].vPosition = D3DXVECTOR3(vMax->x, vMin->y, vMin->z);
	pVtxCube[2].vTex = pVtxCube[2].vPosition;

	pVtxCube[3].vPosition = D3DXVECTOR3(vMin->x, vMin->y, vMin->z);
	pVtxCube[3].vTex = pVtxCube[3].vPosition;


	// 후면부
	pVtxCube[4].vPosition = D3DXVECTOR3(vMin->x, vMax->y, vMax->z);
	pVtxCube[4].vTex = pVtxCube[4].vPosition;

	pVtxCube[5].vPosition = D3DXVECTOR3(vMax->x, vMax->y, vMax->z);
	pVtxCube[5].vTex = pVtxCube[5].vPosition;

	pVtxCube[6].vPosition = D3DXVECTOR3(vMax->x, vMin->y, vMax->z);
	pVtxCube[6].vTex = pVtxCube[6].vPosition;

	pVtxCube[7].vPosition = D3DXVECTOR3(vMin->x, vMin->y, vMax->z);
	pVtxCube[7].vTex = pVtxCube[7].vPosition;

	m_pVB->Unlock();

	INDEX16*		pIndex = NULL;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	// +x
	pIndex[0]._1 = 1;
	pIndex[0]._2 = 5;
	pIndex[0]._3 = 6;

	pIndex[1]._1 = 1;
	pIndex[1]._2 = 6;
	pIndex[1]._3 = 2;

	// -x
	pIndex[2]._1 = 4;
	pIndex[2]._2 = 0;
	pIndex[2]._3 = 3;

	pIndex[3]._1 = 4;
	pIndex[3]._2 = 3;
	pIndex[3]._3 = 7;

	// +y
	pIndex[4]._1 = 4;
	pIndex[4]._2 = 5;
	pIndex[4]._3 = 1;

	pIndex[5]._1 = 4;
	pIndex[5]._2 = 1;
	pIndex[5]._3 = 0;

	// -y
	pIndex[6]._1 = 3;
	pIndex[6]._2 = 2;
	pIndex[6]._3 = 6;

	pIndex[7]._1 = 3;
	pIndex[7]._2 = 6;
	pIndex[7]._3 = 7;

	// +z
	pIndex[8]._1 = 7;
	pIndex[8]._2 = 6;
	pIndex[8]._3 = 5;

	pIndex[9]._1 = 7;
	pIndex[9]._2 = 5;
	pIndex[9]._3 = 4;

	// -z
	pIndex[10]._1 = 0;
	pIndex[10]._2 = 1;
	pIndex[10]._3 = 2;

	pIndex[11]._1 = 0;
	pIndex[11]._2 = 2;
	pIndex[11]._3 = 3;

	m_pIB->Unlock();

	for (_uint i = 0; i < COLL_END; ++i)
	{
		m_pGraphicDev->CreateTexture(1, 1, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &m_pTexture[i], NULL);

		D3DLOCKED_RECT			LockRect;
		ZeroMemory(&LockRect, sizeof(D3DLOCKED_RECT));

		m_pTexture[i]->LockRect(0, &LockRect, NULL, 0);

		*((_ulong*)LockRect.pBits) = D3DXCOLOR(1.f * i, 0.5f * (1.f - i), 0.f, 1.f);

		m_pTexture[i]->UnlockRect(0);

	}

#endif

	return S_OK;
}

HRESULT CCollider::Ready_Collider(const _vec3 * vMin, const _vec3 * vMax)
{
	D3DXMatrixIdentity(&m_matWorld);

	m_vMax = *vMax;
	m_vMin = *vMin;

#ifdef _DEBUG

	FAILED_CHECK_RETURN(m_pGraphicDev->CreateVertexBuffer(sizeof(VTXCUBE) * 8,
		0,
		D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0),
		D3DPOOL_MANAGED,
		&m_pVB,
		NULL), E_FAIL);

	FAILED_CHECK_RETURN(m_pGraphicDev->CreateIndexBuffer(sizeof(INDEX16) * 12,
		0,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIB,
		nullptr), E_FAIL);

	VTXCUBE*		pVtxCube = NULL;

	m_pVB->Lock(0, 0, (void**)&pVtxCube, 0);

	// 전면부
	pVtxCube[0].vPosition = D3DXVECTOR3(vMin->x, vMax->y, vMin->z);
	pVtxCube[0].vTex = pVtxCube[0].vPosition;

	pVtxCube[1].vPosition = D3DXVECTOR3(vMax->x, vMax->y, vMin->z);
	pVtxCube[1].vTex = pVtxCube[1].vPosition;

	pVtxCube[2].vPosition = D3DXVECTOR3(vMax->x, vMin->y, vMin->z);
	pVtxCube[2].vTex = pVtxCube[2].vPosition;

	pVtxCube[3].vPosition = D3DXVECTOR3(vMin->x, vMin->y, vMin->z);
	pVtxCube[3].vTex = pVtxCube[3].vPosition;


	// 후면부
	pVtxCube[4].vPosition = D3DXVECTOR3(vMin->x, vMax->y, vMax->z);
	pVtxCube[4].vTex = pVtxCube[4].vPosition;

	pVtxCube[5].vPosition = D3DXVECTOR3(vMax->x, vMax->y, vMax->z);
	pVtxCube[5].vTex = pVtxCube[5].vPosition;

	pVtxCube[6].vPosition = D3DXVECTOR3(vMax->x, vMin->y, vMax->z);
	pVtxCube[6].vTex = pVtxCube[6].vPosition;

	pVtxCube[7].vPosition = D3DXVECTOR3(vMin->x, vMin->y, vMax->z);
	pVtxCube[7].vTex = pVtxCube[7].vPosition;

	m_pVB->Unlock();

	INDEX16*		pIndex = NULL;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	// +x
	pIndex[0]._1 = 1;
	pIndex[0]._2 = 5;
	pIndex[0]._3 = 6;

	pIndex[1]._1 = 1;
	pIndex[1]._2 = 6;
	pIndex[1]._3 = 2;

	// -x
	pIndex[2]._1 = 4;
	pIndex[2]._2 = 0;
	pIndex[2]._3 = 3;

	pIndex[3]._1 = 4;
	pIndex[3]._2 = 3;
	pIndex[3]._3 = 7;

	// +y
	pIndex[4]._1 = 4;
	pIndex[4]._2 = 5;
	pIndex[4]._3 = 1;

	pIndex[5]._1 = 4;
	pIndex[5]._2 = 1;
	pIndex[5]._3 = 0;

	// -y
	pIndex[6]._1 = 3;
	pIndex[6]._2 = 2;
	pIndex[6]._3 = 6;

	pIndex[7]._1 = 3;
	pIndex[7]._2 = 6;
	pIndex[7]._3 = 7;

	// +z
	pIndex[8]._1 = 7;
	pIndex[8]._2 = 6;
	pIndex[8]._3 = 5;

	pIndex[9]._1 = 7;
	pIndex[9]._2 = 5;
	pIndex[9]._3 = 4;

	// -z
	pIndex[10]._1 = 0;
	pIndex[10]._2 = 1;
	pIndex[10]._3 = 2;

	pIndex[11]._1 = 0;
	pIndex[11]._2 = 2;
	pIndex[11]._3 = 3;

	m_pIB->Unlock();

	for (_uint i = 0; i < COLL_END; ++i)
	{
		m_pGraphicDev->CreateTexture(1, 1, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &m_pTexture[i], NULL);

		D3DLOCKED_RECT			LockRect;
		ZeroMemory(&LockRect, sizeof(D3DLOCKED_RECT));

		m_pTexture[i]->LockRect(0, &LockRect, NULL, 0);

		*((_ulong*)LockRect.pBits) = D3DXCOLOR(1.f * i, 0.5f * (1.f - i), 0.f, 1.f);

		m_pTexture[i]->UnlockRect(0);
	}

#endif


	return S_OK;
}

void Engine::CCollider::Render_Collider(COLLTYPE eType, const _matrix* pColliderMatrix)
{
	m_matWorld = *pColliderMatrix;

	#ifdef _DEBUG

	m_pGraphicDev->SetTransform(D3DTS_WORLD, pColliderMatrix);

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	m_pGraphicDev->SetTexture(0, m_pTexture[eType]);

	m_pGraphicDev->SetStreamSource(0, m_pVB, 0, sizeof(VTXCUBE));
	m_pGraphicDev->SetFVF(D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0));
	m_pGraphicDev->SetIndices(m_pIB);
	m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 8, 0, 12);

	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	#endif // _DEBUG

}

Engine::CCollider* Engine::CCollider::Create(LPDIRECT3DDEVICE9 pGraphicDev, 
												const _vec3* pPosition,
												const _ulong& dwNumVertices, 
												const _ulong& dwStride)
{
	CCollider*	pInstance = new CCollider(pGraphicDev);

	if(FAILED(pInstance->Ready_Collider(pPosition, dwNumVertices, dwStride)))
	{
		ERR_BOX(L"Collider Create Failed");
		Engine::Safe_Release(pInstance);
	}

	return pInstance;
}

Engine::CCollider * Engine::CCollider::Create(LPDIRECT3DDEVICE9 pGraphicDev,
								const _ulong& dwNumVertices,
								const _ulong& dwStride,
								const _vec3* vMin,
								const _vec3* vMax)
{
	CCollider*	pInstance = new CCollider(pGraphicDev);

	if (FAILED(pInstance->Ready_Collider(dwNumVertices, dwStride, vMin, vMax)))
	{
		ERR_BOX(L"Collider Create Failed");
		Engine::Safe_Release(pInstance);
	}

	return pInstance;
}

CCollider * CCollider::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3 * vMin, const _vec3 * vMax)
{

	CCollider*	pInstance = new CCollider(pGraphicDev);

	if (FAILED(pInstance->Ready_Collider(vMin, vMax)))
	{
		ERR_BOX(L"Collider Create Failed");
		Engine::Safe_Release(pInstance);
	}

	return pInstance;
}

void Engine::CCollider::Free(void)
{
	Engine::Safe_Release(m_pGraphicDev);
	Engine::Safe_Release(m_pIB);
	Engine::Safe_Release(m_pVB);

	for(_uint i = 0; i < COLL_END; ++i)
	{
		Engine::Safe_Release(m_pTexture[i]);
	}
}

