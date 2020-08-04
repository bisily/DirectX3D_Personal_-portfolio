#include "stdafx.h"
#include "Tool_Boss_WeaPon.h"

#include "Export_Function.h"

CTool_Boss_WeaPon::CTool_Boss_WeaPon(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{

}

CTool_Boss_WeaPon::~CTool_Boss_WeaPon()
{

}

HRESULT CTool_Boss_WeaPon::Ready_Object()
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_pTransCom->Rotation(Engine::CTransform::ROT_X, D3DXToRadian(90.f));

	return S_OK;
}

Engine::_int CTool_Boss_WeaPon::Update_Object(const Engine::_float& fTimeDelta)
{
	if (nullptr == m_pParentBoneMatrix)
	{
		Engine::CDynamicMesh*		pPlayerMeshCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Get_Component(L"Layer_GameLogic",
																						L"Boss",
																						L"Com_Mesh",
																						Engine::CComponent::ID_STATIC));
		if (nullptr == pPlayerMeshCom)
			return 0;

		const Engine::D3DXFRAME_DERIVED*		pFrame = pPlayerMeshCom->Get_FrameByName("R_Hand");

		m_pParentBoneMatrix = &pFrame->CombinedTransformationMatrix;

		Engine::CTransform*		pPlayerTransCom = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(
																					L"Layer_GameLogic",
																					L"Boss",
																					L"Com_Transform",
																					Engine::CComponent::ID_DYNAMIC));

		if (nullptr == pPlayerTransCom)
			return 0;

		m_pParentWorldMatrix = pPlayerTransCom->Get_WorldMatrixPointer();

	}

	Engine::_uint iExit = Engine::CGameObject::Update_Object(fTimeDelta);

	m_pTransCom->Set_ParentMatrix(&((*m_pParentBoneMatrix) * (*m_pParentWorldMatrix)));

	m_pRendererCom->Add_RenderGroup(Engine::CRenderer::RENDER_NONALPHA, this);

	return iExit;
}

void CTool_Boss_WeaPon::Render_Object(void)
{
	m_pTransCom->SetTransform(m_pGraphicDev);

	m_pMeshCom->Render_Mesh();
}

HRESULT CTool_Boss_WeaPon::Add_Component()
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
	pComponent = m_pMeshCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Clone_Resource(RESOURCE_STAGE, L"Mesh_Boss_WeaPon"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::CComponent::ID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Mesh", pComponent));


	//// For.Collider
	//pComponent = m_pColliderCom = Engine::CCollider::Create(m_pGraphicDev,
	//	m_pMeshCom->Get_VtxPos(),
	//	m_pMeshCom->Get_Numvertices(),
	//	m_pMeshCom->Get_Stride());

	//NULL_CHECK_RETURN(pComponent, E_FAIL);
	//m_mapComponent[Engine::CComponent::ID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Collider", pComponent));

	return S_OK;
}

CTool_Boss_WeaPon* CTool_Boss_WeaPon::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTool_Boss_WeaPon*		pInstance = new CTool_Boss_WeaPon(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CTool_Boss_WeaPon::Free()
{
	Engine::CGameObject::Free();
}

