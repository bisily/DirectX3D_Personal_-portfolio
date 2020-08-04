#include "stdafx.h"
#include "MeshStepFog.h"

#include "Export_Function.h"

CMeshStepFog::CMeshStepFog(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CMeshStepFog::~CMeshStepFog()
{
}

HRESULT CMeshStepFog::Ready_Object(_matrix* pmatPos, _vec3* pDir)
{
	if (FAILED(Add_Component()))
	{
		ERR_BOX(L"Tool_StaticMesh, Load Add_Component Failed");
		return E_FAIL;
	}

	memcpy(&m_vPos, &pmatPos->m[3][0], sizeof(_vec3));
	m_vPos.y += 0.2f;

	m_vLook = *pDir;
	D3DXVec3Normalize(&m_vLook, &m_vLook);

	m_pTransCom->Set_Scale(0.0008f, 0.0008f, 0.0008f);

	m_pTransCom->m_vInfo[Engine::CTransform::INFO_POS] = m_vPos;

	return S_OK;
}

Engine::_int CMeshStepFog::Update_Object(const Engine::_float & fTimeDelta)
{
	if (m_pTargetTransCom == nullptr)
	{
		m_pTargetTransCom = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Com_Transform", Engine::CComponent::ID_DYNAMIC));
		NULL_CHECK_RETURN(m_pTargetTransCom, -1);
	}

	Engine::_uint iExit = Engine::CGameObject::Update_Object(fTimeDelta);

	m_vPos += m_vLook * fTimeDelta;

	m_pTransCom->m_vInfo[Engine::CTransform::INFO_POS] = m_vPos;

	m_fLifeTime += fTimeDelta;

	if (1.f < m_fLifeTime)
	{
		return 0;
	}
	else
	{
		m_pRendererCom->Add_RenderGroup(Engine::CRenderer::RENDER_NONALPHA, this);
	}


	return 0;
}

void CMeshStepFog::Render_Object()
{

	//m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	//m_pTransCom->SetTransform(m_pGraphicDev);

	//m_pMeshCom->Render_Mesh();

	//m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);

	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);

	pEffect->AddRef();

	// 재질정보 넘겨주기
	SetUp_ConstantTable(pEffect);

	_uint iPassCnt = 0;

	pEffect->Begin(&iPassCnt, 0);
	pEffect->BeginPass(0);

	m_pMeshCom->Render_Mesh(pEffect);

	pEffect->EndPass();
	pEffect->End();

	Engine::Safe_Release(pEffect);
}

HRESULT CMeshStepFog::Add_Component()
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

	// For.Mesh
	pComponent = m_pMeshCom = dynamic_cast<Engine::CStaticMesh*>(Engine::Clone_Resource(RESOURCE_STAGE, L"Mesh_StepFog"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::CComponent::ID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Mesh", pComponent));

	// For.Optimization
	pComponent = m_pOptimizationCom = dynamic_cast<Engine::COptimization*>(Engine::Clone_Prototype(L"Proto_Optimization"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::CComponent::ID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Optimization", pComponent));

	// For.Shader
	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_DefferdMesh"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::CComponent::ID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Shader", pComponent));

	return S_OK;
}

HRESULT CMeshStepFog::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	pEffect->AddRef();

	_matrix		matWorld, matView, matProj;

	m_pTransCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	const D3DLIGHT9*		pLightInfo = Engine::Get_LightInfo();

	pEffect->SetVector("g_vLightDir", &_vec4(pLightInfo->Direction, 0.f));
	pEffect->SetVector("g_vLightDiffuse", (_vec4*)&pLightInfo->Diffuse);
	pEffect->SetVector("g_vLightSpecular", (_vec4*)&pLightInfo->Specular);
	pEffect->SetVector("g_vLightAmbient", (_vec4*)&pLightInfo->Ambient);
	
	D3DMATERIAL9			MtrlInfo;
	ZeroMemory(&MtrlInfo, sizeof(D3DMATERIAL9));

	MtrlInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	MtrlInfo.Specular = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.f);
	MtrlInfo.Ambient = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.f);
	MtrlInfo.Emissive = D3DXCOLOR(0.f, 0.f, 0.f, 1.f);

	MtrlInfo.Power = 100.f;

	pEffect->SetVector("g_vMtrlDiffuse", (_vec4*)&MtrlInfo.Diffuse);
	pEffect->SetVector("g_vMtrlSpecular", (_vec4*)&MtrlInfo.Specular);
	pEffect->SetVector("g_vMtrlAmbient", (_vec4*)&MtrlInfo.Ambient);
	pEffect->SetFloat("g_fPower", MtrlInfo.Power);

	D3DXMatrixInverse(&matView, nullptr, &matView);
	pEffect->SetVector("g_vCamPosition", (_vec4*)&matView.m[3][0]);

	Engine::Safe_Release(pEffect);

	return S_OK;
}

CMeshStepFog * CMeshStepFog::Create(LPDIRECT3DDEVICE9 pGraphicDev, _matrix* pmatPos, _vec3* pDir)
{
	CMeshStepFog* pInstance = new CMeshStepFog(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pmatPos, pDir)))
	{
		Engine::Safe_Release(pInstance);
		ERR_BOX(L"MeshStepFog, Create Failed");
	}

	return pInstance;
}

void CMeshStepFog::Free()
{
	Engine::CGameObject::Free();
}
