#include "CubeTex.h"

USING(Engine)

Engine::CCubeTex::CCubeTex(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev)
{

}

Engine::CCubeTex::CCubeTex(const CCubeTex& rhs)
	: CVIBuffer(rhs)
{

}

Engine::CCubeTex::~CCubeTex(void)
{

}

HRESULT Engine::CCubeTex::Ready_Buffer(void)
{
	m_dwTriCnt = 12;
	m_dwVtxCnt = 8;

	m_dwVtxFVF = VTXFVF_CUBE;
	m_dwVtxSize = sizeof(VTXCUBE);

	m_dwIdxSize = sizeof(INDEX16);
	m_IdxFmt = D3DFMT_INDEX16;

	if(FAILED(CVIBuffer::CreateBuffer()))
		return E_FAIL;

	VTXCUBE*		pVtxCube = NULL;

	m_pVB->Lock(0, 0, (void**)&pVtxCube, 0);

	// 전면부
	pVtxCube[0].vPosition = D3DXVECTOR3(-1.f, 1.f, -1.f);
	pVtxCube[0].vTex = pVtxCube[0].vPosition;

	pVtxCube[1].vPosition = D3DXVECTOR3(1.f, 1.f, -1.f);
	pVtxCube[1].vTex = pVtxCube[1].vPosition;

	pVtxCube[2].vPosition = D3DXVECTOR3(1.f, -1.f, -1.f);
	pVtxCube[2].vTex = pVtxCube[2].vPosition;

	pVtxCube[3].vPosition = D3DXVECTOR3(-1.f, -1.f, -1.f);
	pVtxCube[3].vTex = pVtxCube[3].vPosition;

	// 후면부
	pVtxCube[4].vPosition = D3DXVECTOR3(-1.f, 1.f, 1.f);
	pVtxCube[4].vTex = pVtxCube[4].vPosition;

	pVtxCube[5].vPosition = D3DXVECTOR3(1.f, 1.f, 1.f);
	pVtxCube[5].vTex = pVtxCube[5].vPosition;

	pVtxCube[6].vPosition = D3DXVECTOR3(1.f, -1.f, 1.f);
	pVtxCube[6].vTex = pVtxCube[6].vPosition;

	pVtxCube[7].vPosition = D3DXVECTOR3(-1.f, -1.f, 1.f);
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

	return S_OK;
}

void Engine::CCubeTex::Render_Buffer(void)
{
	m_pGraphicDev->SetStreamSource(0, m_pVB, 0, m_dwVtxSize);
	m_pGraphicDev->SetFVF(m_dwVtxFVF);
	m_pGraphicDev->SetIndices(m_pIB);
	m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_dwVtxCnt, 0, m_dwTriCnt);

}

Engine::CCubeTex* Engine::CCubeTex::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCubeTex* pInstance = new CCubeTex(pGraphicDev);

	if(FAILED(pInstance->Ready_Buffer()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void Engine::CCubeTex::Free(void)
{
	Engine::CVIBuffer::Free();
}

Engine::CResources* Engine::CCubeTex::Clone(void)
{
	return new CCubeTex(*this);
}

