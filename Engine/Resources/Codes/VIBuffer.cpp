#include "VIBuffer.h"

USING(Engine)

Engine::CVIBuffer::CVIBuffer(LPDIRECT3DDEVICE9 pGraphicDev)
	: CResources(pGraphicDev)
{
	
}

Engine::CVIBuffer::CVIBuffer(const CVIBuffer& rhs)
	: CResources(rhs)
	, m_pVB(rhs.m_pVB)
	, m_pIB(rhs.m_pIB)
	, m_dwIdxSize(rhs.m_dwIdxSize)
	, m_dwVtxSize(rhs.m_dwVtxSize)
	, m_dwVtxCnt(rhs.m_dwVtxCnt)
	, m_dwVtxFVF(rhs.m_dwVtxFVF)
	, m_dwTriCnt(rhs.m_dwTriCnt)
	, m_IdxFmt(rhs.m_IdxFmt)
{
	if (m_pVB != nullptr &&
		m_pIB != nullptr)
	{
		m_pVB->AddRef();
		m_pIB->AddRef();
	}	
}

Engine::CVIBuffer::~CVIBuffer(void)
{

}

void CVIBuffer::SetVtxInfo(void * pVertex)
{
	void*		pOriginVertex = NULL;

	m_pVB->Lock(0, 0, &pOriginVertex, 0);

	memcpy(pOriginVertex, pVertex, m_dwVtxCnt * m_dwVtxSize);

	m_pVB->Unlock();

}

void Engine::CVIBuffer::Free(void)
{
	Engine::CResources::Free();

	Engine::Safe_Release(m_pIB);
	Engine::Safe_Release(m_pVB);
}


HRESULT Engine::CVIBuffer::CreateBuffer(void)
{
	HRESULT	hr = NULL;

	hr = m_pGraphicDev->CreateVertexBuffer(
		m_dwVtxSize * m_dwVtxCnt,	
		0,							
		m_dwVtxFVF,					
		D3DPOOL_MANAGED,			
		&m_pVB,						
		nullptr);					

	FAILED_CHECK_MSG(hr, L"버텍스 버퍼 생성 실패");

	hr = m_pGraphicDev->CreateIndexBuffer(
		m_dwIdxSize * m_dwTriCnt,	
		0,
		m_IdxFmt,
		D3DPOOL_MANAGED,
		&m_pIB,
		nullptr);

	FAILED_CHECK_MSG(hr, L"인덱스 버퍼 생성 실패");

	return S_OK;
}

