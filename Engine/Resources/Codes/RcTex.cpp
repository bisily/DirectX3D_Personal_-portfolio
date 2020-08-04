#include "RcTex.h"

USING(Engine)

Engine::CRcTex::CRcTex(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev)
{

}

Engine::CRcTex::CRcTex(const CRcTex& rhs)
	: CVIBuffer(rhs)
{

}

Engine::CRcTex::~CRcTex(void)
{

}

HRESULT Engine::CRcTex::Ready_Buffer(void)
{
	m_dwTriCnt = 2;
	m_dwVtxCnt = 4;

	m_dwVtxFVF = VTXFVF_TEX;
	m_dwVtxSize = sizeof(VTXTEX);

	m_dwIdxSize = sizeof(INDEX32);
	m_IdxFmt = D3DFMT_INDEX32;

	if(FAILED(CVIBuffer::CreateBuffer()))
		return E_FAIL;

	VTXTEX*		pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vPosition = _vec3(-0.5f, 0.5f, 0.f);
	pVertex[0].vTex = _vec2(0.f, 0.f);

	pVertex[1].vPosition = _vec3(0.5f, 0.5f, 0.f);
	pVertex[1].vTex = _vec2(1.f, 0.f);

	pVertex[2].vPosition = _vec3(0.5f, -0.5f, 0.f);
	pVertex[2].vTex = _vec2(1.f, 1.f);

	pVertex[3].vPosition = _vec3(-0.5f, -0.5f, 0.f);
	pVertex[3].vTex = _vec2(0.f, 1.f);

	m_pVB->Unlock();

	INDEX32*		pIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	pIndex[0]._1 = 0;
	pIndex[0]._2 = 1;
	pIndex[0]._3 = 2;

	pIndex[1]._1 = 0;
	pIndex[1]._2 = 2;
	pIndex[1]._3 = 3;

	m_pIB->Unlock();


	return S_OK;
}

HRESULT CRcTex::Ready_Buffer(_int iFlag)
{
	m_dwTriCnt = 2;
	m_dwVtxCnt = 4;

	m_dwVtxFVF = VTXFVF_TEX;
	m_dwVtxSize = sizeof(VTXTEX);

	m_dwIdxSize = sizeof(INDEX32);
	m_IdxFmt = D3DFMT_INDEX32;

	if (FAILED(CVIBuffer::CreateBuffer()))
		return E_FAIL;

	VTXTEX*		pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vPosition = _vec3(0.f, 1.f, 0.f);
	pVertex[0].vTex = _vec2(0.f, 0.f);

	pVertex[1].vPosition = _vec3(1.f, 1.f, 0.f);
	pVertex[1].vTex = _vec2(1.f, 0.f);

	pVertex[2].vPosition = _vec3(1.f, 0.f, 0.f);
	pVertex[2].vTex = _vec2(1.f, 1.f);

	pVertex[3].vPosition = _vec3(0.f, 0.f, 0.f);
	pVertex[3].vTex = _vec2(0.f, 1.f);

	m_pVB->Unlock();

	INDEX32*		pIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	pIndex[0]._1 = 0;
	pIndex[0]._2 = 1;
	pIndex[0]._3 = 2;

	pIndex[1]._1 = 0;
	pIndex[1]._2 = 2;
	pIndex[1]._3 = 3;

	m_pIB->Unlock();

	return S_OK;
}

void Engine::CRcTex::Render_Buffer(void)
{
	m_pGraphicDev->SetStreamSource(0, m_pVB, 0, m_dwVtxSize);
	m_pGraphicDev->SetFVF(m_dwVtxFVF);
	m_pGraphicDev->SetIndices(m_pIB);
	m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_dwVtxCnt, 0, m_dwTriCnt);

}

void CRcTex::SetUp_VtxPos(_vec3 vecPos)
{
	m_dwTriCnt = 2;
	m_dwVtxCnt = 4;

	m_dwVtxFVF = VTXFVF_TEX;
	m_dwVtxSize = sizeof(VTXTEX);

	m_dwIdxSize = sizeof(INDEX32);
	m_IdxFmt = D3DFMT_INDEX32;

	CVIBuffer::CreateBuffer();	

	VTXTEX*		pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vPosition = _vec3(-vecPos.x - 0.25f, 0.5f, 0.f);
	pVertex[0].vTex = _vec2(0.f, 0.f);

	pVertex[1].vPosition = _vec3(vecPos.x - 0.25f, 0.5f, 0.f);
	pVertex[1].vTex = _vec2(1.f, 0.f);

	pVertex[2].vPosition = _vec3(vecPos.x - 0.25f, 0.5f, 0.f);
	pVertex[2].vTex = _vec2(1.f, 1.f);

	pVertex[3].vPosition = _vec3(-vecPos.x - 0.25f, 0.5f, 0.f);
	pVertex[3].vTex = _vec2(0.f, 1.f);

	m_pVB->Unlock();

	m_pVB->Unlock();

	INDEX32*		pIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	pIndex[0]._1 = 0;
	pIndex[0]._2 = 1;
	pIndex[0]._3 = 2;

	pIndex[1]._1 = 0;
	pIndex[1]._2 = 2;
	pIndex[1]._3 = 3;

	m_pIB->Unlock();
}

Engine::CRcTex* Engine::CRcTex::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CRcTex* pInstance = new CRcTex(pGraphicDev);

	if(FAILED(pInstance->Ready_Buffer()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

CRcTex * CRcTex::Create(LPDIRECT3DDEVICE9 pGraphicDev, _int iFlag)
{
	CRcTex* pInstance = new CRcTex(pGraphicDev);

	if (FAILED(pInstance->Ready_Buffer(iFlag)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void Engine::CRcTex::Free(void)
{
	Engine::CVIBuffer::Free();
}

Engine::CResources* Engine::CRcTex::Clone(void)
{
	return new CRcTex(*this);
}

