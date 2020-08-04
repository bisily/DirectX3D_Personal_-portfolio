#include "TrailTex.h"

USING(Engine)

Engine::CTrailTex::CTrailTex(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev)
{

}

Engine::CTrailTex::CTrailTex(const CTrailTex& rhs)
	: CVIBuffer(rhs)
{

}

Engine::CTrailTex::~CTrailTex(void)
{

}

HRESULT Engine::CTrailTex::Ready_Buffer(void)
{
	m_dwVtxSize = sizeof(VTXTRAIL);
	
	m_dwVtxFVF = VTXFVF_TRAIL;

	m_IdxFmt = D3DFMT_INDEX16;

	return S_OK;
}

void Engine::CTrailTex::Render_Buffer()
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pMatWorld);
	m_pGraphicDev->SetFVF(m_dwVtxFVF);
	m_pGraphicDev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, m_dwTriCnt, m_pVertex, m_dwVtxSize);

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	Safe_Delete_Array(m_pVertex);

}

void CTrailTex::SetVertexTrail(const _vec3 * pPos, const _vec3 * pDir, const _float & fLength, const DWORD & dwCnt, _matrix* pmatWorld)
{
	if (m_pMatWorld == nullptr)
	{
		m_pMatWorld = pmatWorld;
	}

	m_pVertex = new VTXTRAIL[dwCnt];
	ZeroMemory(m_pVertex, sizeof(VTXTRAIL) * dwCnt);

	for (DWORD i = 0; i < dwCnt / 2; ++i)
	{
		m_pVertex[i * 2 + 0].vPos = pPos[i] - pDir[i] * fLength;
		m_pVertex[i * 2 + 0].vTexUV = D3DXVECTOR2(1.f, (i * 2 + 0) / _float(dwCnt - 2.f));

		m_pVertex[i * 2 + 1].vPos = pPos[i] + pDir[i] * fLength;
		m_pVertex[i * 2 + 1].vTexUV = D3DXVECTOR2(0.f, (i * 2 + 1) / _float(dwCnt - 1.f));
	}
	m_dwTriCnt = dwCnt - 2;
}

Engine::CTrailTex* Engine::CTrailTex::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTrailTex* pInstance = new CTrailTex(pGraphicDev);

	if (FAILED(pInstance->Ready_Buffer()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void Engine::CTrailTex::Free(void)
{
	Engine::CVIBuffer::Free();
}

Engine::CResources* Engine::CTrailTex::Clone(void)
{
	return new CTrailTex(*this);
}

