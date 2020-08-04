#include "stdafx.h"
#include "StaticObject.h"

#include "Export_Function.h"

CStaticObject::CStaticObject(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CStaticObject::~CStaticObject()
{
}

HRESULT CStaticObject::Ready_Object(wstring tagMeshName, Engine::_vec3 * temp_Right, Engine::_vec3 * temp_Up, Engine::_vec3 * temp_Look, Engine::_vec3 * temp_Pos, Engine::_vec3 * temp_Scale, Engine::_vec3 * temp_Angle, int _iObjIndex)
{
	if (FAILED(Add_Component(tagMeshName.c_str())))
	{
		ERR_BOX(L"Tool_StaticMesh, Load Add_Component Failed");
		return E_FAIL;
	}

	m_pTransCom->m_vInfo[0] = *temp_Right;
	m_pTransCom->m_vInfo[1] = *temp_Up;
	m_pTransCom->m_vInfo[2] = *temp_Look;
	m_pTransCom->m_vInfo[3] = *temp_Pos;

	m_pTransCom->Set_Scale(temp_Scale->x, temp_Scale->y, temp_Scale->z);
	m_pTransCom->m_vAngle = *temp_Angle;

	m_iObjIndex = _iObjIndex;
	m_strMeshName = tagMeshName;
	
	if (!m_strMeshName.compare(L"Mesh_Castle"))
	{
		m_bCastle = true;
		m_bIsDraw = true;
	}

	return S_OK;
}

Engine::_int CStaticObject::Update_Object(const Engine::_float & fTimeDelta)
{
	Engine::_int iExit = Engine::CGameObject::Update_Object(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(Engine::CRenderer::RENDER_NONALPHA, this);

	return 0;
}

void CStaticObject::Render_Object()
{
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

HRESULT CStaticObject::Add_Component(const Engine::_tchar * tagMeshName)
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
	pComponent = m_pMeshCom = dynamic_cast<Engine::CStaticMesh*>(Engine::Clone_Resource(RESOURCE_STAGE, tagMeshName));
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

HRESULT CStaticObject::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
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
	MtrlInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	MtrlInfo.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
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

CStaticObject * CStaticObject::Create(LPDIRECT3DDEVICE9 pGraphicDev
										, wstring tagMeshName
										, Engine::_vec3 * temp_Right
										, Engine::_vec3 * temp_Up
										, Engine::_vec3 * temp_Look
										, Engine::_vec3 * temp_Pos
										, Engine::_vec3 * temp_Scale
										, Engine::_vec3 * temp_Angle
										, int iObjIndex)
{
	CStaticObject* pInstance = new CStaticObject(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(tagMeshName
										, temp_Right
										, temp_Up
										, temp_Look
										, temp_Pos
										, temp_Scale
										, temp_Angle
										, iObjIndex)))
	{
		Engine::Safe_Release(pInstance);
		ERR_BOX(L"StaticObject, Create Failed");
	}

	return pInstance;
}

void CStaticObject::Free()
{
	Engine::CGameObject::Free();
}
