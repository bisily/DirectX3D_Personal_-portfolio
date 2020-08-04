#include "stdafx.h"
#include "Monster00Sword.h"

#include "Export_Function.h"

CMonster00Sword::CMonster00Sword(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{

}

CMonster00Sword::~CMonster00Sword()
{

}

HRESULT CMonster00Sword::Ready_Object(wstring strName, Engine::_int iIndex)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_iIndex = iIndex;
	m_strName = strName;

	m_pTransCom->Rotation(Engine::CTransform::ROT_X, D3DXToRadian(90.f));

	return S_OK;
}

Engine::_int CMonster00Sword::Update_Object(const Engine::_float& fTimeDelta)
{
	Get_ParentBoneMatrix();

	Engine::_uint iExit = Engine::CGameObject::Update_Object(fTimeDelta);

	Engine::_matrix matWorld = ((*m_pParentBoneMatrix) * (*m_pParentWorldMatrix));

	matWorld.m[3][0] += 0.05f;
	matWorld.m[3][1];
	matWorld.m[3][2] -= 0.1f;

	m_pTransCom->Set_ParentMatrix(&matWorld);

	m_pRendererCom->Add_RenderGroup(Engine::CRenderer::RENDER_NONALPHA, this);

	return iExit;
}

void CMonster00Sword::Render_Object()
{
	m_pTransCom->SetTransform(m_pGraphicDev);

	m_pMeshCom->Render_Mesh();

	/*LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);

	pEffect->AddRef();

	SetUp_ConstantTable(pEffect);

	Engine::_uint iPassCnt = 0;

	pEffect->Begin(&iPassCnt, 0);
	pEffect->BeginPass(0);

	m_pMeshCom->Render_Mesh(pEffect);

	pEffect->EndPass();
	pEffect->End();

	Engine::Safe_Release(pEffect);*/

	/*for (auto iter : m_vecCollider)
	{
		Engine::_matrix matTem;

		matTem = *m_pMyBore * *m_pTransCom->Get_WorldMatrixPointer();

		iter->Render_Collider(Engine::CCollider::COLL_TRUE, &matTem);
	}*/

	//m_pColliderCom->Render_Collider(Engine::CCollider::COLL_TRUE, &((*m_pParentBoneMatrix) * (*m_pParentWorldMatrix)));

	//m_pSphereColliderCom->Render_SphereCollider();
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
	map<wstring, Engine::_float> mapBore;
	Make_BoreNames(&mapBore);

	//// For.SphereCollider
	//pComponent = m_pSphereColliderCom = Engine::CSphereCollider::Create(m_pGraphicDev, m_pMeshCom->Get_RootBore(), &mapBore, &m_pTransCom->m_matWorld);
	//NULL_CHECK_RETURN(pComponent, E_FAIL);
	//m_mapComponent[Engine::CComponent::ID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_SphereCol", pComponent));

	// Form.CollisionSquare
	Make_CollisionSquare();

	// For.ObjState
	pComponent = m_pStateCom = Engine::CObjState::Create(1.f, 2.f);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::CComponent::ID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Com_State", pComponent));

	return S_OK;
}

HRESULT CMonster00Sword::Make_BoreNames(map<wstring, Engine::_float>* pmapBore)
{
	TCHAR szBoreName[64] = L"", szBoreSize[64] = L"";
	DWORD dwByte = 99;

	wifstream	LoadFile;

	LoadFile.open(L"../../Data/Collision_Monster00Sword_Swrod.txt", ios::in);

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

		/*pFrame = m_pMeshCom->Get_FrameByName("M_CENTER");*/

		//m_pMyBore = &pFrame->CombinedTransformationMatrix;

		Engine::CObjState* pMonster00SwordStateCom = dynamic_cast<Engine::CObjState*>(Engine::Get_Component(
			L"Layer_GameLogic",
			L"Monster00Sword",
			L"Com_State",
			Engine::CComponent::ID_DYNAMIC));

		NULL_CHECK(pMonster00SwordStateCom);

		m_pStateParentCom = pMonster00SwordStateCom;
	}
}

HRESULT CMonster00Sword::Make_CollisionSquare()
{
	Engine::CComponent* pComponent = nullptr;

	wfstream	pFile;
	Engine::_vec3		vMin, vMax, vPont;
	Engine::_tchar		szBoreName[64], szMaxX[64], szMaxY[64], szMaxZ[64], szMinX[64], szMinY[64], szMinZ[64];
	wstring		strBore;
	Engine::_matrix		matPos;

	pFile.open(L"../../Data/Collision_Monster00Sword_Swrod.txt", ios::in);

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

		vMax = Engine::_vec3((float)_ttof(szMaxX), (float)_ttof(szMaxY), (float)_ttof(szMaxZ));
		vMin = Engine::_vec3((float)_ttof(szMinX), (float)_ttof(szMinY), (float)_ttof(szMinZ));

		// For.Collider
		pComponent = m_pColliderCom = Engine::CCollider::Create(m_pGraphicDev, 8, sizeof(Engine::_vec3), &vMax, &vMin);

		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[Engine::CComponent::ID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Com_Collider", pComponent));
	}

	return S_OK;
}

CMonster00Sword* CMonster00Sword::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring m_strName, Engine::_int iIndex)
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

