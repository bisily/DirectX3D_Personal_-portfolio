#include "Renderer.h"
#include "RenderTargetMgr.h"

#include "Export_Utility.h"
#include "Export_System.h"

USING(Engine)
IMPLEMENT_SINGLETON(CRenderer)

Engine::CRenderer::CRenderer(void)
{
	
}

Engine::CRenderer::~CRenderer(void)
{
	Free();
}

void Engine::CRenderer::Add_RenderGroup(RENDERID eGroup, CGameObject* pGameObject)
{
	if(RENDER_END <= eGroup)
		return;

	m_RenderGroup[eGroup].push_back(pGameObject);

	pGameObject->AddRef();
}

void Engine::CRenderer::Render_GameObject(LPDIRECT3DDEVICE9 pGraphicDev)
{
	Render_Priority(pGraphicDev);

	Render_Defferd(pGraphicDev);
	Render_LightAcc(pGraphicDev);

	Render_AlphaTest(pGraphicDev);
	Render_Blend(pGraphicDev);
	
	Render_AlphaBlend(pGraphicDev);
	Render_Ui(pGraphicDev);


#ifdef _DEBUG
	if (Engine::Get_DIKeyState(DIK_5) & 0x80)
	{
		Engine::Render_DebugBuffer(L"MRT_Deffered");
		Engine::Render_DebugBuffer(L"MRT_LightAcc");
	}

#endif

	Clear_RenderGroup();
}

HRESULT CRenderer::Ready_Renderer(LPDIRECT3DDEVICE9 pGraphicDev)
{
	HRESULT	hr = NULL;

	hr = pGraphicDev->CreateVertexBuffer(sizeof(VTXSCREEN) * 4,
		0,
		VTXFVF_SCREEN,
		D3DPOOL_MANAGED,
		&m_pVB,
		nullptr);
	FAILED_CHECK_MSG(hr, L"렌더타겟 버퍼 생성 실패");

	hr = pGraphicDev->CreateIndexBuffer(sizeof(INDEX16) * 2,
		0,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIB,
		nullptr);
	FAILED_CHECK_MSG(hr, L"렌더타겟 인덱스 버퍼 생성 실패");

	D3DVIEWPORT9		ViewPort;
	pGraphicDev->GetViewport(&ViewPort);


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

void CRenderer::Render_ToolObject(LPDIRECT3DDEVICE9 pGraphicDev)
{
	for(_uint i = 0; i < RENDER_END; ++i)
	{
		for(auto& iter : m_RenderGroup[i])
		{
			iter->Render_Object();
			Engine::Safe_Release(iter); // Add_RenderGroup 함수가 호출되면 지속적으로 레퍼런스 카운트가 증가하게 된다.
										// 그에 따른 레퍼런스 카운트 감소를 위해 이와 같은 함수를 호출하게 되었다.
		}
		m_RenderGroup[i].clear();
	}
}

void Engine::CRenderer::Clear_RenderGroup(void)
{
	for(_uint i = 0; i < RENDER_END; ++i)
	{
		for_each(m_RenderGroup[i].begin(), m_RenderGroup[i].end(), CDeleteObj());
		m_RenderGroup[i].clear();
	}
}

void CRenderer::Render_Priority(LPDIRECT3DDEVICE9 pGraphicDev)
{
	pGraphicDev->SetRenderState(D3DRS_ZENABLE, FALSE);
	pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	for (auto& iter : m_RenderGroup[RENDER_PRIORITY])
		iter->Render_Object();

	pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);
	pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

void CRenderer::Render_NoneAlpha(LPDIRECT3DDEVICE9 pGraphicDev)
{
	for (auto& iter : m_RenderGroup[RENDER_NONALPHA])
	{
		iter->Render_Object();
	}
}

_bool Compare_Z(CGameObject* pDest, CGameObject* pSrc)
{
	return pDest->Get_ViewZ() > pSrc->Get_ViewZ();
}

void CRenderer::Render_AlphaTest(LPDIRECT3DDEVICE9 pGraphicDev)
{
	pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0x00000088);
	pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	/*m_RenderGroup[RENDER_ALPHATEST].sort(Compare_Z);*/

	for (auto& iter : m_RenderGroup[RENDER_ALPHATEST])
	{
		iter->Render_Object();
	}

	pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

void CRenderer::Render_AlphaBlend(LPDIRECT3DDEVICE9 pGraphicDev)
{
	pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_RenderGroup[RENDER_ALPHABLEND].sort(Compare_Z);

	for (auto& iter : m_RenderGroup[RENDER_ALPHABLEND])
	{
		iter->Render_Object();
	}

	pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

void CRenderer::Render_Ui(LPDIRECT3DDEVICE9 pGraphicDev)
{
	pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	pGraphicDev->SetRenderState(D3DRS_ZENABLE, FALSE);
	pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	for (auto& iter : m_RenderGroup[RENDER_UI])
		iter->Render_Object();

	pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);
	pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

void CRenderer::Render_Defferd(LPDIRECT3DDEVICE9 pGraphicDev)
{
	Engine::Get_RenderTarget()->Begin_MRT(L"MRT_Deffered");

	Render_NoneAlpha(pGraphicDev);

	Engine::Get_RenderTarget()->End_MRT(L"MRT_Deffered");
}

void CRenderer::Render_LightAcc(LPDIRECT3DDEVICE9 pGraphicDev)
{
	Engine::Get_RenderTarget()->Begin_MRT(L"MRT_LightAcc");

	CShader*	pShader = (Engine::CShader*)Engine::Clone_Prototype(L"Shader_Shade");
	NULL_CHECK(pShader);

	LPD3DXEFFECT pEffect = pShader->Get_EffectHandle();
	pEffect->AddRef();

	Engine::SetUp_OnShader(pEffect, L"Target_Normal", "g_NormalTexture");
	Engine::SetUp_OnShader(pEffect, L"Target_Depth", "g_DepthTexture");

	pEffect->Begin(nullptr, 0);

	Engine::Render_Light(pEffect);

	pEffect->End();

	Engine::Get_RenderTarget()->End_MRT(L"MRT_LightAcc");

	Engine::Safe_Release(pShader);
}

void CRenderer::Render_Blend(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CShader*	pShader = (Engine::CShader*)Engine::Clone_Prototype(L"Shader_Blend");
	NULL_CHECK(pShader);

	LPD3DXEFFECT pEffect = pShader->Get_EffectHandle();
	pEffect->AddRef();

	Engine::SetUp_OnShader(pEffect, L"Target_Albedo", "g_AlbedoTexture");
	Engine::SetUp_OnShader(pEffect, L"Target_Shade", "g_ShadeTexture");
	Engine::SetUp_OnShader(pEffect, L"Target_Specular", "g_SpecularTexture");

	pEffect->Begin(NULL, 0);
	pEffect->BeginPass(0);

	pGraphicDev->SetStreamSource(0, m_pVB, 0, sizeof(VTXSCREEN));
	pGraphicDev->SetFVF(VTXFVF_SCREEN);
	pGraphicDev->SetIndices(m_pIB);
	pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

	pEffect->EndPass();
	pEffect->End();

	Engine::Safe_Release(pShader);
}

void Engine::CRenderer::Free(void)
{
	Clear_RenderGroup();
}

