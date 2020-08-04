#include "stdafx.h"
#include "Box.h"

#include "Export_Function.h"

CBox::CBox(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{

}

CBox::~CBox(void)
{

}

HRESULT CBox::Ready_Object(void)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_pTransCom->Set_Position(5.f, 2.f, 5.f);
	m_pTransCom->Set_Scale(0.01f, 0.01f, 0.01f);

	return S_OK;
}

Engine::_int CBox::Update_Object(const Engine::_float& fTimeDelta)
{
	Engine::_uint iExit = Engine::CGameObject::Update_Object(fTimeDelta);

	//SetUp_OnTerrain();

	m_pRendererCom->Add_RenderGroup(Engine::CRenderer::RENDER_NONALPHA, this);

	return iExit;
}

void CBox::Render_Object(void)
{
	m_pTransCom->SetTransform(m_pGraphicDev);

	m_pMeshCom->Render_Mesh();
}

HRESULT CBox::Add_Component(void)
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
	pComponent = m_pMeshCom = dynamic_cast<Engine::CStaticMesh*>(Engine::Clone_Resource(RESOURCE_STAGE, L"Mesh_Box"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::CComponent::ID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Mesh", pComponent));

	return S_OK;
}

void CBox::SetUp_OnTerrain(void)
{
	Engine::_vec3		vPosition;

	m_pTransCom->Get_Infomation(Engine::CTransform::INFO_POS, &vPosition);

	Engine::CTerrainTex*	pTerrainBufferCom = dynamic_cast<Engine::CTerrainTex*>
		(Engine::Get_Component(L"Layer_GameLogic",
			L"Terrain",
			L"Com_Buffer",
			Engine::CComponent::ID_STATIC));

	NULL_CHECK(pTerrainBufferCom);

	Engine::_float		fHeight = m_pCalculatorCom->Compute_HeightOnTerrain(&vPosition, pTerrainBufferCom->Get_VtxPos(), 129, 129);

	m_pTransCom->Set_Position(vPosition.x, fHeight, vPosition.z);


}


CBox* CBox::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBox*		pInstance = new CBox(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CBox::Free(void)
{
	Engine::CGameObject::Free();
}

