#include "stdafx.h"
#include "Monster00Sword.h"

#include "Export_Function.h"
#include "Player.h"

CMonster00Sword::CMonster00Sword(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{

}

CMonster00Sword::~CMonster00Sword()
{

}

HRESULT CMonster00Sword::Ready_Object(wstring strName, _int iIndex)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_iIndex = iIndex;
	m_strName = strName;

	m_pTransCom->Rotation(Engine::CTransform::ROT_X, D3DXToRadian(90.f));

	return S_OK;
}

_int CMonster00Sword::Update_Object(const _float& fTimeDelta)
{
	Get_ParentBoneMatrix();
	
	_vec3		vPosition;
	m_pTransCom->Get_Infomation(Engine::CTransform::INFO_POS, &vPosition);
	m_bIsDraw = m_pOptimizationCom->Is_Frustum_ForObject(&vPosition, 15.f);

	if (m_pStateParentCom != nullptr &&
		m_pStateParentCom->Get_Hp() < 0)
	{
		return -1;
	}

	_uint iExit = Engine::CGameObject::Update_Object(fTimeDelta);


	Engine::_matrix matWorld = ((*m_pParentBoneMatrix) * (*m_pParentWorldMatrix));

	Fixed_Pos(&matWorld);

	m_pTransCom->Set_ParentMatrix(&matWorld);

	Check_Target();

	m_pRendererCom->Add_RenderGroup(Engine::CRenderer::RENDER_NONALPHA, this);

	return 0;
}

void CMonster00Sword::Render_Object()
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

	_matrix matTrans = ((*m_pParentBoneMatrix) * (*m_pParentWorldMatrix));
	
	Fixed_Pos(&matTrans);
}

HRESULT CMonster00Sword::Add_Component()
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
	pComponent = m_pMeshCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Clone_Resource(RESOURCE_STAGE, L"Mesh_Monster_Sword"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::CComponent::ID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Mesh", pComponent));

	// For.BoreName
	map<wstring, _float> mapBore;
	Make_BoreNames(&mapBore);

	// Form.CollisionSquare
	Make_CollisionSquare();

	// For.Shader
	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_Mesh"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::CComponent::ID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Shader", pComponent));

	// For.ObjState
	pComponent = m_pStateCom = Engine::CObjState::Create(1.f, 2.f);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::CComponent::ID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Com_State", pComponent));

	// For.Optimization
	pComponent = m_pOptimizationCom = dynamic_cast<Engine::COptimization*>(Engine::Clone_Prototype(L"Proto_Optimization"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::CComponent::ID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Optimization", pComponent));

	return S_OK;
}

HRESULT CMonster00Sword::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
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

HRESULT CMonster00Sword::Make_BoreNames(map<wstring, _float>* pmapBore)
{
	TCHAR szBoreName[64] = L"", szBoreSize[64] = L"";
	DWORD dwByte = 99;

	wifstream	LoadFile;

	LoadFile.open(L"../../Data/Collision_Monster00Sword.txt", ios::in);

	while (!LoadFile.eof())
	{
		LoadFile.getline(szBoreName, 64, '|');
		LoadFile.getline(szBoreSize, 64);

		if (!lstrcmp(szBoreName, L""))
		{
			break;
		}

		pmapBore->insert(make_pair(szBoreName, (float)_ttof(szBoreSize)));
	}

	LoadFile.close();

	return S_OK;
}

void CMonster00Sword::Get_ParentBoneMatrix()
{
	if (nullptr == m_pParentBoneMatrix)
	{
		Engine::CDynamicMesh*		pMonster00SwordMeshCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Get_ComponentIndex(L"Layer_GameLogic",
			L"Monster00",
			L"Com_Mesh",
			Engine::CComponent::ID_STATIC, 
			m_iIndex));

		if (nullptr == pMonster00SwordMeshCom)
			return;

		const Engine::D3DXFRAME_DERIVED*		pFrame = pMonster00SwordMeshCom->Get_FrameByName("R_Hand");

		m_pParentBoneMatrix = &pFrame->CombinedTransformationMatrix;

		Engine::CTransform*		pMonster00TransCom = dynamic_cast<Engine::CTransform*>(Engine::Get_ComponentIndex(
			L"Layer_GameLogic",
			L"Monster00",
			L"Com_Transform",
			Engine::CComponent::ID_DYNAMIC, 
			m_iIndex));

		if (nullptr == pMonster00TransCom)
			return;

		m_pParentWorldMatrix = pMonster00TransCom->Get_WorldMatrixPointer();

		Engine::CObjState* pMonster00StateCom = dynamic_cast<Engine::CObjState*>(Engine::Get_ComponentIndex(
			L"Layer_GameLogic",
			L"Monster00",
			L"Com_State",
			Engine::CComponent::ID_DYNAMIC,
			m_iIndex));

		NULL_CHECK(pMonster00StateCom);

		m_pStateParentCom = pMonster00StateCom;

		m_pParent = dynamic_cast<CMonster00*>(Engine::Get_Management()->Get_Object(L"Layer_GameLogic", L"Monster00", m_iIndex));
	}
}

void CMonster00Sword::Check_Target()
{
	Engine::CObjState* pParentState = dynamic_cast<Engine::CObjState*>(Engine::Get_ComponentIndex(L"Layer_GameLogic", L"Monster00", L"Com_State", Engine::CComponent::ID_DYNAMIC, m_iIndex));

	if (pParentState->Get_State() != Engine::CObjState::NOW_ATTACK)
	{
		return;
	}

	Engine::CSphereCollider* pSphereCollider = dynamic_cast<Engine::CSphereCollider*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Com_SphereCol", Engine::CComponent::ID_STATIC));

	Engine::CTransform* pTransform = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Com_Transform", Engine::CComponent::ID_DYNAMIC));

	Engine::CGameObject* pTarget = dynamic_cast<CPlayer*>(Engine::Get_Management()->Get_Object(L"Layer_GameLogic", L"Player"));

	if (pTarget->Get_IsHit() == true)
	{
		return;
	}

	if (m_pCalculatorCom->Collision_AABB_Sphere(m_pColliderCom->Get_Min(), m_pColliderCom->Get_Max(), &((*m_pParentBoneMatrix) * (*m_pParentWorldMatrix)), pSphereCollider->Get_SphereList(), &pTransform->m_matWorld))
	{
		pTarget->Set_Hit(TRUE);

		Engine::CObjState* pPlayerState = dynamic_cast<Engine::CObjState*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Com_State", Engine::CComponent::ID_DYNAMIC));

		pPlayerState->Set_Damge(m_pStateCom->Get_Damage());
	}
}

void CMonster00Sword::Fixed_Pos(_matrix * pmatPos)
{
	if (m_pStateParentCom->Get_State() == Engine::CObjState::NOW_IDLE)
	{
		pmatPos->m[3][2] -= 0.07f;
	}
	else if (m_pParent->Get_AniIndex() == 1)
	{
		pmatPos->m[3][0] -= 0.f;
		pmatPos->m[3][2] -= 0.f;
	}
	else if (m_pStateParentCom->Get_State() == Engine::CObjState::NOW_ATTACK)
	{
		pmatPos->m[3][0] -= 0.0f;
		pmatPos->m[3][1] += 0.1f;
		pmatPos->m[3][2] -= 0.02f;
	}
}

HRESULT CMonster00Sword::Make_CollisionSquare()
{
	Engine::CComponent* pComponent = nullptr;

	wfstream	pFile;
	_vec3		vMin, vMax, vPont;
	_tchar		szBoreName[64], szMaxX[64], szMaxY[64], szMaxZ[64], szMinX[64], szMinY[64], szMinZ[64];
	wstring		strBore;
	_matrix		matPos;

	pFile.open(L"../../Data/Collision_Monster00Sword.txt", ios::in);

	while (!pFile.eof())
	{
		pFile.getline(szBoreName, 64, '|');
		pFile.getline(szMaxX, 64, '|');
		pFile.getline(szMaxY, 64, '|');
		pFile.getline(szMaxZ, 64, '|');
		pFile.getline(szMinX, 64, '|');
		pFile.getline(szMinY, 64, '|');
		pFile.getline(szMinZ, 64);

		if (!lstrcmp(szMinZ, L""))
		{
			break;
		}

		vMax = _vec3((float)_ttof(szMaxX), (float)_ttof(szMaxY), (float)_ttof(szMaxZ));
		vMin = _vec3((float)_ttof(szMinX), (float)_ttof(szMinY), (float)_ttof(szMinZ));

		// For.Collider
		pComponent = m_pColliderCom = Engine::CCollider::Create(m_pGraphicDev, 8, sizeof(_vec3), &vMax, &vMin);

		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[Engine::CComponent::ID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Com_Collider", pComponent));
	}

	return S_OK;
}

CMonster00Sword* CMonster00Sword::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring m_strName, _int iIndex)
{
	CMonster00Sword*		pInstance = new CMonster00Sword(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(m_strName, iIndex)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CMonster00Sword::Free()
{
	Engine::CGameObject::Free();
}

