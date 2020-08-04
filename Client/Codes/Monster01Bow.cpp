#include "stdafx.h"
#include "Monster01Bow.h"

#include "Export_Function.h"

CMonster01Bow::CMonster01Bow(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{

}

CMonster01Bow::~CMonster01Bow()
{

}

HRESULT CMonster01Bow::Ready_Object(wstring strName, _int iIndex)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_iIndex = iIndex;
	m_strName = strName;

	m_pTransCom->Rotation(Engine::CTransform::ROT_X, D3DXToRadian(90.f));

	return S_OK;
}

_int CMonster01Bow::Update_Object(const _float& fTimeDelta)
{
	Get_ParentBoneMatrix();

	_matrix matPos = ((*m_pParentBoneMatrix) * (*m_pParentWorldMatrix));

	_uint iExit = Engine::CGameObject::Update_Object(fTimeDelta);

	_vec3		vPosition;
	memcpy(&vPosition, &matPos.m[3][0], sizeof(_vec3));

	m_bIsDraw = m_pOptimizationCom->Is_Frustum_ForObject(&vPosition, 30.f);
	
	if (m_pStateParentCom != nullptr &&
		m_pStateParentCom->Get_Hp() < 0)
	{
		return -1;
	}
	
	matPos.m[3][0] += 0.1f;
	matPos.m[3][1];
	matPos.m[3][2];

	m_pTransCom->Set_ParentMatrix(&matPos);

	m_pRendererCom->Add_RenderGroup(Engine::CRenderer::RENDER_NONALPHA, this);

	return 0;
}

void CMonster01Bow::Render_Object()
{
	if (false == m_bIsDraw)
		return;

	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);

	pEffect->AddRef();

	SetUp_ConstantTable(pEffect);

	_uint iPassCnt = 0;

	pEffect->Begin(&iPassCnt, 0);
	pEffect->BeginPass(0);

	m_pMeshCom->Render_Mesh(pEffect);

	pEffect->EndPass();
	pEffect->End();

	Engine::Safe_Release(pEffect);

}

HRESULT CMonster01Bow::Add_Component()
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
	pComponent = m_pMeshCom = dynamic_cast<Engine::CStaticMesh*>(Engine::Clone_Resource(RESOURCE_STAGE, L"Mesh_Monster_Bow"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::CComponent::ID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Mesh", pComponent));

	// For.Shader
	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_Mesh"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::CComponent::ID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Shader", pComponent));

	// For.Optimization
	pComponent = m_pOptimizationCom = dynamic_cast<Engine::COptimization*>(Engine::Clone_Prototype(L"Proto_Optimization"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::CComponent::ID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Optimization", pComponent));

	return S_OK;
}

HRESULT CMonster01Bow::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
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
	MtrlInfo.Specular = D3DXCOLOR(0.1f, 0.1f, 0.1f, 0.1f);
	MtrlInfo.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	MtrlInfo.Emissive = D3DXCOLOR(0.f, 0.f, 0.f, 1.f);

	MtrlInfo.Power = 20.f;

	pEffect->SetVector("g_vMtrlDiffuse", (_vec4*)&MtrlInfo.Diffuse);
	pEffect->SetVector("g_vMtrlSpecular", (_vec4*)&MtrlInfo.Specular);
	pEffect->SetVector("g_vMtrlAmbient", (_vec4*)&MtrlInfo.Ambient);
	pEffect->SetFloat("g_fPower", MtrlInfo.Power);

	D3DXMatrixInverse(&matView, nullptr, &matView);
	pEffect->SetVector("g_vCamPosition", (_vec4*)&matView.m[3][0]);

	Engine::Safe_Release(pEffect);

	return S_OK;
}

void CMonster01Bow::Get_ParentBoneMatrix()
{
	if (nullptr == m_pParentBoneMatrix)
	{
		Engine::CDynamicMesh*		pMonster01BowMeshCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Get_ComponentIndex(L"Layer_GameLogic",
			L"Monster01",
			L"Com_Mesh",
			Engine::CComponent::ID_STATIC,
			m_iIndex));

		if (nullptr == pMonster01BowMeshCom)
			return;

		const Engine::D3DXFRAME_DERIVED*		pFrame = pMonster01BowMeshCom->Get_FrameByName("L_Hand");

		m_pParentBoneMatrix = &pFrame->CombinedTransformationMatrix;

		Engine::CTransform*		pMonster01TransCom = dynamic_cast<Engine::CTransform*>(Engine::Get_ComponentIndex(
			L"Layer_GameLogic",
			L"Monster01",
			L"Com_Transform",
			Engine::CComponent::ID_DYNAMIC,
			m_iIndex));

		if (nullptr == pMonster01TransCom)
			return;

		m_pParentWorldMatrix = pMonster01TransCom->Get_WorldMatrixPointer();

		Engine::CObjState* pMonster01StateCom = dynamic_cast<Engine::CObjState*>(Engine::Get_ComponentIndex(
			L"Layer_GameLogic",
			L"Monster01",
			L"Com_State",
			Engine::CComponent::ID_DYNAMIC,
			m_iIndex));

		NULL_CHECK(pMonster01StateCom);

		m_pStateParentCom = pMonster01StateCom;
	}
}

CMonster01Bow* CMonster01Bow::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring m_strName, _int iIndex)
{
	CMonster01Bow*		pInstance = new CMonster01Bow(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(m_strName, iIndex)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CMonster01Bow::Free()
{
	Engine::CGameObject::Free();
}

