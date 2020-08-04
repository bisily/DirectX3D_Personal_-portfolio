#include "RenderTarget.h"

USING(Engine)

Engine::CRenderTarget::CRenderTarget(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
{
	m_pGraphicDev->AddRef();
}

Engine::CRenderTarget::~CRenderTarget(void)
{

}

HRESULT Engine::CRenderTarget::Ready_RenderTarget(const _uint& iWidth,
													const _uint& iHeight,
													D3DFORMAT Format,
													D3DXCOLOR Color)
{
	if (FAILED(D3DXCreateTexture(m_pGraphicDev,
		iWidth,
		iHeight,
		1,		// �ӷ��� ����
		D3DUSAGE_RENDERTARGET,
		Format,
		D3DPOOL_DEFAULT,			// ���� Ÿ�� ���� �ÿ��� managed�� �����ϴ� ��� �������� �ʰ� Ŭ���̾�Ʈ�� ���߰� ��
		&m_pTargetTexture)))
		return E_FAIL;

	// �ؽ��ķκ��� ����Ÿ���� ����
	m_pTargetTexture->GetSurfaceLevel(0, &m_pTargetSurface);
	// 1. �� ������ ���� �� ������� ���. �� ��°�� ����� ���ΰ��� �����ϴ� ���ڰ�

	Engine::Safe_Release(m_pTargetSurface);

	m_ClearColor = Color;

	return S_OK;
}

void Engine::CRenderTarget::SetUp_OnGraphicDev(const _uint& iIndex)
{
	m_pGraphicDev->GetRenderTarget(iIndex, &m_pOldTargetSurface);

	// ���� �ڵ尡 ���ٸ�?
	// ������ �ִ� ���� Ÿ���� �ڵ� �Ҹ��, �׷��� ���� ���� ������ �ִ� ���� Ÿ���� ������ �ӽ������� ���� ��, ���ο� ���ǽ��� ����
	m_pGraphicDev->SetRenderTarget(iIndex, m_pTargetSurface);
}

void Engine::CRenderTarget::SetUp_OnShader(LPD3DXEFFECT pEffect, const char* pContantName)
{
	pEffect->SetTexture(pContantName, m_pTargetTexture);
}

void CRenderTarget::Clear_RenderTarget(void)
{
	m_pGraphicDev->GetRenderTarget(0, &m_pOldTargetSurface); // ����� ������ �õ弭�ǽ��� ����

	m_pGraphicDev->SetRenderTarget(0, m_pTargetSurface); // �˺��� Ÿ���� ��ũ

	m_pGraphicDev->Clear(0, nullptr, D3DCLEAR_TARGET, m_ClearColor, 1.f, 0); // �˺��� Ÿ���� ���ϴ� �������� ���

	m_pGraphicDev->SetRenderTarget(0, m_pOldTargetSurface);	// ������ ���·� �������´�

	Engine::Safe_Release(m_pOldTargetSurface);	// �ʿ䰡 ������ ���ǽ��� �����Ѵ�.

}

void CRenderTarget::Release_OnGraphicDev(const _uint & iIndex)
{
	m_pGraphicDev->SetRenderTarget(iIndex, m_pOldTargetSurface);	 // ������ �����ϴ� ���� Ÿ�� ���۵��� Ÿ�� ���ǽ��� �����Ǿ� �ִ� �ݸ�, 
																	 // ����۴� �����ϰ� �ִ� ������ ���� ������ ��ó�� ������ ������ ��Ų��.
	Engine::Safe_Release(m_pOldTargetSurface);

}

Engine::CRenderTarget* Engine::CRenderTarget::Create(LPDIRECT3DDEVICE9 pGraphicDev,
													const _uint& iWidth,
													const _uint& iHeight,
													D3DFORMAT Format,
													D3DXCOLOR Color)
{
	CRenderTarget*		pInstance = new CRenderTarget(pGraphicDev);

	if (FAILED(pInstance->Ready_RenderTarget(iWidth, iHeight, Format, Color)))
	{
		ERR_BOX(L"CRenderdasfsdfTagret Create Failed");
		Engine::Safe_Release(pInstance);
	}

	return pInstance;
}

void Engine::CRenderTarget::Free(void)
{
	Engine::Safe_Release(m_pVB);
	Engine::Safe_Release(m_pIB);

	Engine::Safe_Release(m_pGraphicDev);
	Engine::Safe_Release(m_pTargetTexture);
	Engine::Safe_Release(m_pOldTargetSurface);
}


HRESULT Engine::CRenderTarget::Ready_DebugBuffer(const _float& fX,
												const _float& fY,
												const _float& fSizeX,
												const _float& fSizeY)
{
	HRESULT	hr = NULL;

	hr = m_pGraphicDev->CreateVertexBuffer(sizeof(VTXSCREEN) * 4,
		0,
		VTXFVF_SCREEN,
		D3DPOOL_MANAGED,
		&m_pVB,
		nullptr);
	FAILED_CHECK_MSG(hr, L"����Ÿ�� ���� ���� ����");

	hr = m_pGraphicDev->CreateIndexBuffer(sizeof(INDEX16) * 2,
		0,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIB,
		nullptr);
	FAILED_CHECK_MSG(hr, L"����Ÿ�� �ε��� ���� ���� ����");


	VTXSCREEN*		pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vPosition = _vec4(fX, fY, 0.f, 1.f);
	pVertex[0].vTexUV = _vec2(0.f, 0.f);

	pVertex[1].vPosition = _vec4(fX + fSizeX, fY, 0.f, 1.f);
	pVertex[1].vTexUV = _vec2(1.f, 0.f);

	pVertex[2].vPosition = _vec4(fX + fSizeX, fY + fSizeY, 0.f, 1.f);
	pVertex[2].vTexUV = _vec2(1.f, 1.f);

	pVertex[3].vPosition = _vec4(fX, fY + fSizeY, 0.f, 1.f);
	pVertex[3].vTexUV = _vec2(0.f, 1.f);

	m_pVB->Unlock();

	INDEX16*	pIndex = nullptr;

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

void Engine::CRenderTarget::Render_DebugBuffer(void)
{
	m_pGraphicDev->SetTexture(0, m_pTargetTexture);
	m_pGraphicDev->SetStreamSource(0, m_pVB, 0, sizeof(VTXSCREEN));
	m_pGraphicDev->SetFVF(VTXFVF_SCREEN);
	m_pGraphicDev->SetIndices(m_pIB);
	m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);
}
