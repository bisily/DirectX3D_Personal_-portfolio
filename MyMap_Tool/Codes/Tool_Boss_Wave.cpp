#include "stdafx.h"
#include "Tool_Boss_Wave.h"

#include "Export_Function.h"

CTool_Boss_Wave::CTool_Boss_Wave(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{

}

CTool_Boss_Wave::~CTool_Boss_Wave()
{

}

HRESULT CTool_Boss_Wave::Ready_Object()
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_pTransCom->m_vInfo[Engine::CTransform::INFO_POS] = Engine::_vec3(20.f, 1.f, 20.f);
	m_pTransCom->Set_Scale(0.02f, 0.02f, 0.02f);

	return S_OK;
}

Engine::_int CTool_Boss_Wave::Update_Object(const Engine::_float& fTimeDelta)
{
	if (bIs_Dead == true)
	{
		return -1;
	}

	Engine::_uint iExit = Engine::CGameObject::Update_Object(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(Engine::CRenderer::RENDER_NONALPHA, this);

	//m_pTransCom->m_vInfo[Engine::CTransform::INFO_POS].x += 0.1f;

	return iExit;
}

void CTool_Boss_Wave::Render_Object()
{

	/*m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0x00000088);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);*/


	m_pTransCom->SetTransform(m_pGraphicDev);

	m_pMeshCom->Render_Mesh();
	
	//m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	//m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	m_pColliderCom->Render_Collider(Engine::CCollider::COLL_TRUE, &m_pTransCom->m_matWorld);
}

HRESULT CTool_Boss_Wave::Add_Component()
{
	Engine::CComponent*		pComponent = nullptr;

	// For.Transform
	pComponent = m_pTransCom = Engine::CTransform::Create();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::CComponent::ID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));

	// For.Renderer
	pComponent = m_pRendererCom = Engine::Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->AddRef();
	m_mapComponent[Engine::CComponent::ID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Renderer", pComponent));

	// For.Calculator
	pComponent = m_pCalculatorCom = Engine::CCalculator::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::CComponent::ID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Calculator", pComponent));

	// For.Mesh
	pComponent = m_pMeshCom = dynamic_cast<Engine::CStaticMesh*>(Engine::Clone_Resource(RESOURCE_STAGE, L"Mesh_Wave"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::CComponent::ID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Mesh", pComponent));

	// For.ObjState
	pComponent = m_pStateCom = Engine::CObjState::Create(1.f, 5.f);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::CComponent::ID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Com_State", pComponent));

	// For.Collider
	pComponent = m_pColliderCom = Engine::CCollider::Create(m_pGraphicDev,
		m_pMeshCom->Get_VtxPos(),
		m_pMeshCom->Get_Numvertices(),
		m_pMeshCom->Get_Stride());

	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::CComponent::ID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Collider", pComponent));

	return S_OK;
}

CTool_Boss_Wave* CTool_Boss_Wave::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTool_Boss_Wave*		pInstance = new CTool_Boss_Wave(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CTool_Boss_Wave::Free()
{
	Engine::CGameObject::Free();
}

