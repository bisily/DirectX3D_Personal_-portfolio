#include "RcCol.h"

USING(Engine)

Engine::CRcCol::CRcCol(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev)
{

}

Engine::CRcCol::CRcCol(const CRcCol& rhs)
	: CVIBuffer(rhs)
{

}

Engine::CRcCol::~CRcCol(void)
{

}

HRESULT Engine::CRcCol::Ready_Buffer(void)
{
	m_dwTriCnt = 2;
	m_dwVtxCnt = 4;

	m_dwVtxFVF = VTXFVF_COL;
	m_dwVtxSize = sizeof(VTXCOL);

	m_dwIdxSize = sizeof(INDEX32);
	m_IdxFmt = D3DFMT_INDEX32;

	if(FAILED(CVIBuffer::CreateBuffer()))
		return E_FAIL;

	VTXCOL*		pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vPosition = _vec3(-0.5f, 0.5f, 0.f);
	pVertex[0].dwColor = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);

	pVertex[1].vPosition = _vec3(0.5f, 0.5f, 0.f);
	pVertex[1].dwColor = D3DXCOLOR(1.f, 1.f, 0.f, 1.f);

	pVertex[2].vPosition = _vec3(0.5f, -0.5f, 0.f);
	pVertex[2].dwColor = D3DXCOLOR(1.f, 0.f, 1.f, 1.f);

	pVertex[3].vPosition = _vec3(-0.5f, -0.5f, 0.f);
	pVertex[3].dwColor = D3DXCOLOR(0.f, 1.f, 1.f, 1.f);

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

void Engine::CRcCol::Render_Buffer(void)
{
	m_pGraphicDev->SetStreamSource(0, m_pVB, 0, sizeof(VTXCOL));
	m_pGraphicDev->SetFVF(VTXFVF_COL);
	m_pGraphicDev->SetIndices(m_pIB);
	m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

	//m_pGraphicDev->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);
}

Engine::CRcCol* Engine::CRcCol::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CRcCol* pInstance = new CRcCol(pGraphicDev);

	if(FAILED(pInstance->Ready_Buffer()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void Engine::CRcCol::Free(void)
{
	Engine::CVIBuffer::Free();
}

Engine::CResources* Engine::CRcCol::Clone(void)
{
	return new CRcCol(*this);
}

