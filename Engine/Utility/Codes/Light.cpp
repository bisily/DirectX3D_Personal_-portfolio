#include "Light.h"

USING(Engine)

Engine::CLight::CLight(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
{
	ZeroMemory(&m_tLightInfo, sizeof(D3DLIGHT9));
	m_pGraphicDev->AddRef();
}

Engine::CLight::~CLight(void)
{

}

HRESULT Engine::CLight::Ready_Light(const D3DLIGHT9* pLightInfo, const _uint& iIndex)
{
	memcpy(&m_tLightInfo, pLightInfo, sizeof(D3DLIGHT9));

	m_iIndex = iIndex;
	
	m_pGraphicDev->SetLight(m_iIndex, &m_tLightInfo);
	m_pGraphicDev->LightEnable(m_iIndex, TRUE);

	HRESULT	hr = NULL;

	hr = m_pGraphicDev->CreateVertexBuffer(sizeof(VTXSCREEN) * 4,
		0,
		VTXFVF_SCREEN,
		D3DPOOL_MANAGED,
		&m_pVB,
		nullptr);
	FAILED_CHECK_MSG(hr, L"·»´õÅ¸°Ù ¹öÆÛ »ý¼º ½ÇÆÐ");

	hr = m_pGraphicDev->CreateIndexBuffer(sizeof(INDEX16) * 2,
		0,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIB,
		nullptr);
	FAILED_CHECK_MSG(hr, L"·»´õÅ¸°Ù ÀÎµ¦½º ¹öÆÛ »ý¼º ½ÇÆÐ");

	D3DVIEWPORT9		ViewPort;
	m_pGraphicDev->GetViewport(&ViewPort);


	VTXSCREEN*		pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vPosition = _vec4(0.f, 0.f, 0.f, 1.f);
	pVertex[0].vTexUV = _vec2(0.f, 0.f);

	pVertex[1].vPosition = _vec4(_float(ViewPort.Width), 0.f, 0.f, 1.f);
	pVertex[1].vTexUV = _vec2(1.f, 0.f);

	pVertex[2].vPosition = _vec4(_float(ViewPort.Width), _float(ViewPort.Height), 0.f, 1.f);
	pVertex[2].vTexUV = _vec2(1.f, 1.f);

	pVertex[3].vPosition = _vec4(0.f, _float(ViewPort.Height), 0.f, 1.f);
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

void CLight::Render(LPD3DXEFFECT pEffect)
{
	_uint iPassIdx = 0;

	if (D3DLIGHT_DIRECTIONAL == m_tLightInfo.Type)
	{
		iPassIdx = 0;
		pEffect->SetVector("g_vLightDir", &_vec4(m_tLightInfo.Direction, 0.f));
	}

	else if (D3DLIGHT_POINT == m_tLightInfo.Type)
	{
		iPassIdx = 1;
		pEffect->SetVector("g_vLightPos", &_vec4(m_tLightInfo.Position, 1.f));
		pEffect->SetFloat("g_fRange", m_tLightInfo.Range);
	}

	pEffect->SetVector("g_vLightDiffuse", (_vec4*)&m_tLightInfo.Diffuse);
	pEffect->SetVector("g_vLightAmbient", (_vec4*)&m_tLightInfo.Ambient);

	_matrix		matView, matProj;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, nullptr, &matView);

	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matProj, nullptr, &matProj);

	pEffect->SetVector("g_vCamPosition", (_vec4*)&matView._41);
	pEffect->SetMatrix("g_matViewInv", &matView);
	pEffect->SetMatrix("g_matProjInv", &matProj);

	pEffect->CommitChanges();

	pEffect->BeginPass(iPassIdx);

	m_pGraphicDev->SetStreamSource(0, m_pVB, 0, sizeof(VTXSCREEN));
	m_pGraphicDev->SetFVF(VTXFVF_SCREEN);
	m_pGraphicDev->SetIndices(m_pIB);
	m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

	pEffect->EndPass();
}

Engine::CLight* Engine::CLight::Create(LPDIRECT3DDEVICE9 pGraphicDev,
									const D3DLIGHT9* pLightInfo,
									const _uint& iIndex)
{
	CLight*		pInstance = new CLight(pGraphicDev);

	if(FAILED(pInstance->Ready_Light(pLightInfo, iIndex)))
	{
		ERR_BOX(L"Light Created Failed");
		Engine::Safe_Release(pInstance);
	}

	return pInstance;
}

void Engine::CLight::Free(void)
{
	m_pGraphicDev->LightEnable(m_iIndex, FALSE);

	Engine::Safe_Release(m_pVB);
	Engine::Safe_Release(m_pIB);

	Engine::Safe_Release(m_pGraphicDev);
}

