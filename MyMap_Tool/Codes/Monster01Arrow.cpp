#include "stdafx.h"
#include "Monster01Arrow.h"

#include "Export_Function.h"

CMonster01Arrow::CMonster01Arrow(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{

}

CMonster01Arrow::~CMonster01Arrow()
{

}

HRESULT CMonster01Arrow::Ready_Object()
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_pTransCom->Set_Scale(0.01f, 0.01f, 0.01f);

	return S_OK;
}

Engine::_int CMonster01Arrow::Update_Object(const Engine::_float& fTimeDelta)
{
	Engine::_uint iExit = Engine::CGameObject::Update_Object(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(Engine::CRenderer::RENDER_NONALPHA, this);

	return iExit;
}

void CMonster01Arrow::Render_Object()
{
	m_pTransCom->SetTransform(m_pGraphicDev);

	m_pMeshCom->Render_Mesh();

	m_pColliderCom->Render_Collider(Engine::CCollider::COLL_FALSE, &m_pTransCom->m_matWorld);
}

HRESULT CMonster01Arrow::Add_Component()
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
	pComponent = m_pMeshCom = dynamic_cast<Engine::CStaticMesh*>(Engine::Clone_Resource(RESOURCE_STAGE, L"Mesh_Monster_Arrow"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::CComponent::ID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Mesh", pComponent));

	pComponent = m_pColliderCom = Engine::CCollider::Create(m_pGraphicDev, &Engine::_vec3(0.f, 0.f, 0.f), 8, 8);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::CComponent::ID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Mesh", pComponent));

	//// For.SphereCollider
	//pComponent = m_pSphereColliderCom = Engine::CSphereCollider::Create(m_pGraphicDev, m_pMeshCom->Get_RootBore(), &mapBore, &m_pTransCom->m_matWorld);
	//NULL_CHECK_RETURN(pComponent, E_FAIL);
	//m_mapComponent[Engine::CComponent::ID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_SphereCol", pComponent));


	return S_OK;
}

CMonster01Arrow* CMonster01Arrow::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMonster01Arrow*		pInstance = new CMonster01Arrow(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CMonster01Arrow::Free()
{
	Engine::CGameObject::Free();
}

