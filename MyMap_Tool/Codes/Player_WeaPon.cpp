#include "stdafx.h"
#include "Player_WeaPon.h"

#include "Export_Function.h"

CPlayer_WeaPon::CPlayer_WeaPon(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{

}

CPlayer_WeaPon::~CPlayer_WeaPon()
{

}

HRESULT CPlayer_WeaPon::Ready_Object()
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	//m_pTransCom->Set_Scale(0.01f, 0.01f, 0.01f);
	m_pTransCom->Rotation(Engine::CTransform::ROT_X, D3DXToRadian(90.f));

	return S_OK;
}

Engine::_int CPlayer_WeaPon::Update_Object(const Engine::_float& fTimeDelta)
{
	if (nullptr == m_pParentBoneMatrix)
	{
		Engine::CDynamicMesh*		pPlayerMeshCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Get_Component(L"Layer_GameLogic",
																						L"Player",
																						L"Com_Mesh",
																						Engine::CComponent::ID_STATIC));
		if (nullptr == pPlayerMeshCom)
			return 0;

		const Engine::D3DXFRAME_DERIVED*		pFrame = pPlayerMeshCom->Get_FrameByName("R_FINGER12");

		m_pParentBoneMatrix = &pFrame->CombinedTransformationMatrix;

		Engine::CTransform*		pPlayerTransCom = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(
																					L"Layer_GameLogic",
																					L"Player",
																					L"Com_Transform",
																					Engine::CComponent::ID_DYNAMIC));

		if (nullptr == pPlayerTransCom)
			return 0;

		m_pParentWorldMatrix = pPlayerTransCom->Get_WorldMatrixPointer();

	}

	Engine::_uint iExit = Engine::CGameObject::Update_Object(fTimeDelta);

	m_pTransCom->Set_ParentMatrix(&((*m_pParentBoneMatrix) * (*m_pParentWorldMatrix)));


	/*Engine::CSphereCollider* pSphereCollider = dynamic_cast<Engine::CSphereCollider*>(Engine::Get_Component(L"Layer_GameLogic", L"Boss", L"Com_SphereCol", Engine::CComponent::ID_STATIC));

	m_pCalculatorCom->Collision_AABB_Sphere(m_pColliderCom->Get_Min(), m_pColliderCom->Get_Max(), pSphereCollider->Get_SphereList(), &dynamic_cast<Engine::CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Boss", L"Com_Transform", Engine::CComponent::ID_DYNAMIC))->m_matWorld);*/

	m_pRendererCom->Add_RenderGroup(Engine::CRenderer::RENDER_NONALPHA, this);

	return iExit;
}

void CPlayer_WeaPon::Render_Object()
{
	m_pTransCom->SetTransform(m_pGraphicDev);

	m_pMeshCom->Render_Mesh();

	//LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	//NULL_CHECK(pEffect);

	//pEffect->AddRef();

	//SetUp_ConstantTable(pEffect);

	//Engine::_uint iPassCnt = 0;

	//pEffect->Begin(&iPassCnt, 0);
	//pEffect->BeginPass(0);

	//m_pMeshCom->Render_Mesh(pEffect);

	//pEffect->EndPass();
	//pEffect->End();

	//Engine::Safe_Release(pEffect);

	//m_pColliderCom->Render_Collider(Engine::CCollider::COLL_TRUE, &m_pTransCom->m_matWorld);
}

HRESULT CPlayer_WeaPon::Add_Component()
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
	pComponent = m_pMeshCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Clone_Resource(RESOURCE_STAGE, L"Mesh_Player_WeaPon"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::CComponent::ID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Mesh", pComponent));
	
	//// For.Collider
	//pComponent = m_pColliderCom = Engine::CCollider::Create(m_pGraphicDev,
	//	m_pMeshCom->Get_VtxPos(),
	//	m_pMeshCom->Get_Numvertices(),
	//	m_pMeshCom->Get_Stride());

	//NULL_CHECK_RETURN(pComponent, E_FAIL);
	//m_mapComponent[Engine::CComponent::ID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Collider", pComponent));

	//// For.Shader
	//pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_Mesh"));
	//NULL_CHECK_RETURN(pComponent, E_FAIL);
	//m_mapComponent[Engine::CComponent::ID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Shader", pComponent));

	// For.ObjState
	pComponent = m_pStateCom = Engine::CObjState::Create(1.f, 10.f);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::CComponent::ID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Com_State", pComponent));

	return S_OK;
}

HRESULT CPlayer_WeaPon::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	pEffect->AddRef();

	Engine::_matrix		matWorld, matView, matProj;

	m_pTransCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	const D3DLIGHT9*		pLightInfo = Engine::Get_LightInfo();

	pEffect->SetVector("g_vLightDir", &Engine::_vec4(pLightInfo->Direction, 0.f));
	pEffect->SetVector("g_vLightDiffuse", (Engine::_vec4*)&pLightInfo->Diffuse);
	pEffect->SetVector("g_vLightSpecular", (Engine::_vec4*)&pLightInfo->Specular);
	pEffect->SetVector("g_vLightAmbient", (Engine::_vec4*)&pLightInfo->Ambient);


	D3DMATERIAL9			MtrlInfo;
	ZeroMemory(&MtrlInfo, sizeof(D3DMATERIAL9));

	MtrlInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	MtrlInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	MtrlInfo.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	MtrlInfo.Emissive = D3DXCOLOR(0.f, 0.f, 0.f, 1.f);

	MtrlInfo.Power = 20.f;

	pEffect->SetVector("g_vMtrlDiffuse", (Engine::_vec4*)&MtrlInfo.Diffuse);
	pEffect->SetVector("g_vMtrlSpecular", (Engine::_vec4*)&MtrlInfo.Specular);
	pEffect->SetVector("g_vMtrlAmbient", (_vec4*)&MtrlInfo.Ambient);
	pEffect->SetFloat("g_fPower", MtrlInfo.Power);

	D3DXMatrixInverse(&matView, nullptr, &matView);
	pEffect->SetVector("g_vCamPosition", (_vec4*)&matView.m[3][0]);

	Engine::Safe_Release(pEffect);

	return S_OK;
}

CPlayer_WeaPon* CPlayer_WeaPon::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPlayer_WeaPon*		pInstance = new CPlayer_WeaPon(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CPlayer_WeaPon::Free()
{
	Engine::CGameObject::Free();
}

