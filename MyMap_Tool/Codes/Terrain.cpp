#include "stdafx.h"
#include "Terrain.h"

#include "Export_Function.h"


CTerrain::CTerrain(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{

}

CTerrain::~CTerrain(void)
{

}

HRESULT CTerrain::Ready_Object(void)
{
	if(FAILED(Add_Component()))
		return E_FAIL;

	if (FAILED(SetUp_Material()))
		return E_FAIL;

	return S_OK;
}

Engine::_int CTerrain::Update_Object(const Engine::_float& fTimeDelta)
{
	Engine::_int iExit = Engine::CGameObject::Update_Object(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(Engine::CRenderer::RENDER_NONALPHA, this);

	return 0;
}

void CTerrain::Render_Object(void)
{
	if (bWireframe == true)
	{
		m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	}
	else
	{
		m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	}

	m_pTransCom->SetTransform(m_pGraphicDev);

	m_pTextureCom->Set_Texture(0);
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}

HRESULT CTerrain::Add_Component(void)
{
	Engine::CComponent*		pComponent = nullptr;

	// For.Transform
	pComponent = m_pTransCom = Engine::CTransform::Create();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::CComponent::ID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));

	// For.Buffer
	pComponent = m_pBufferCom = dynamic_cast<Engine::CTerrainTex*>(Engine::Clone_Resource(Engine::RESOURCE_STATIC, L"Buffer_Terrain"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::CComponent::ID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Buffer", pComponent));

	// For.Texture
	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone_Resource(RESOURCE_STAGE, L"Texture_Terrain"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::CComponent::ID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture", pComponent));

	// For.Renderer
	pComponent = m_pRendererCom = Engine::Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->AddRef();
	m_mapComponent[Engine::CComponent::ID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Renderer", pComponent));

	return S_OK;
}

HRESULT CTerrain::SetUp_Material()
{
	D3DMATERIAL9			MtrlInfo;
	ZeroMemory(&MtrlInfo, sizeof(D3DMATERIAL9));

	MtrlInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	MtrlInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	MtrlInfo.Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 0.2f);
	MtrlInfo.Emissive = D3DXCOLOR(0.f, 0.f, 0.f, 1.f);

	MtrlInfo.Power = 0.f;

	m_pGraphicDev->SetMaterial(&MtrlInfo);

	return S_OK;
}

CTerrain* CTerrain::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTerrain*		pInstance = new CTerrain(pGraphicDev);

	if(FAILED(pInstance->Ready_Object()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CTerrain::Free(void)
{
	Engine::CGameObject::Free();
}

