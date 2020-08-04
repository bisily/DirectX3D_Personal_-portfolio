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
		1,		// 밉레벨 개수
		D3DUSAGE_RENDERTARGET,
		Format,
		D3DPOOL_DEFAULT,			// 렌더 타겟 생성 시에는 managed로 설정하는 경우 생성되지 않고 클라이언트가 멈추게 됨
		&m_pTargetTexture)))
		return E_FAIL;

	// 텍스쳐로부터 렌더타겟을 추출
	m_pTargetTexture->GetSurfaceLevel(0, &m_pTargetSurface);
	// 1. 밉 레벨을 여러 개 만들었을 경우. 몇 번째를 사용할 것인가를 결정하는 인자값

	Engine::Safe_Release(m_pTargetSurface);

	m_ClearColor = Color;

	return S_OK;
}

void Engine::CRenderTarget::SetUp_OnGraphicDev(const _uint& iIndex)
{
	m_pGraphicDev->GetRenderTarget(iIndex, &m_pOldTargetSurface);

	// 위의 코드가 없다면?
	// 기존에 있던 렌더 타겟이 자동 소멸됨, 그래서 위와 같이 기존에 있던 렌더 타겟의 정보를 임시적으로 저장 후, 새로운 서피스로 세팅
	m_pGraphicDev->SetRenderTarget(iIndex, m_pTargetSurface);
}

void Engine::CRenderTarget::SetUp_OnShader(LPD3DXEFFECT pEffect, const char* pContantName)
{
	pEffect->SetTexture(pContantName, m_pTargetTexture);
}

void CRenderTarget::Clear_RenderTarget(void)
{
	m_pGraphicDev->GetRenderTarget(0, &m_pOldTargetSurface); // 백버퍼 정보를 올드서피스에 저장

	m_pGraphicDev->SetRenderTarget(0, m_pTargetSurface); // 알베도 타겟을 링크

	m_pGraphicDev->Clear(0, nullptr, D3DCLEAR_TARGET, m_ClearColor, 1.f, 0); // 알베도 타겟을 원하는 색상으로 출력

	m_pGraphicDev->SetRenderTarget(0, m_pOldTargetSurface);	// 원래의 상태로 돌려놓는다

	Engine::Safe_Release(m_pOldTargetSurface);	// 필요가 없어진 서피스를 해제한다.

}

void CRenderTarget::Release_OnGraphicDev(const _uint & iIndex)
{
	m_pGraphicDev->SetRenderTarget(iIndex, m_pOldTargetSurface);	 // 기존에 존재하던 렌더 타겟 버퍼들은 타겟 서피스에 보관되어 있는 반면, 
																	 // 백버퍼는 보관하고 있는 변수가 없기 때문에 이처럼 가져와 보존을 시킨다.
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
	FAILED_CHECK_MSG(hr, L"렌더타겟 버퍼 생성 실패");

	hr = m_pGraphicDev->CreateIndexBuffer(sizeof(INDEX16) * 2,
		0,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIB,
		nullptr);
	FAILED_CHECK_MSG(hr, L"렌더타겟 인덱스 버퍼 생성 실패");


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
