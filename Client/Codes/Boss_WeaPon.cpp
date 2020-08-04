#include "stdafx.h"
#include "Boss_WeaPon.h"

#include "Export_Function.h"
#include "Player.h"

CBoss_WeaPon::CBoss_WeaPon(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{

}

CBoss_WeaPon::~CBoss_WeaPon()
{

}

HRESULT CBoss_WeaPon::Ready_Object()
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_pTransCom->Rotation(Engine::CTransform::ROT_X, D3DXToRadian(90.f));

	return S_OK;
}

_int CBoss_WeaPon::Update_Object(const _float& fTimeDelta)
{
	Get_Object();

	if (bIs_Dead == true)
	{
		return -1;
	}

	_uint iExit = Engine::CGameObject::Update_Object(fTimeDelta);

	_matrix matPos = *m_pParentBoneMatrix * (*m_pParentWorldMatrix);
	Fix_Pos(&matPos);
	
	m_pTransCom->Set_ParentMatrix(&matPos);

	_vec3		vPosition;
	m_pTransCom->Get_Infomation(Engine::CTransform::INFO_POS, &vPosition);
	m_bIsDraw = m_pOptimizationCom->Is_Frustum_ForObject(&vPosition, 30.f);

	Check_Target();

	m_pRendererCom->Add_RenderGroup(Engine::CRenderer::RENDER_NONALPHA, this);
	
	return 0;
}

void CBoss_WeaPon::Render_Object()
{
	if (false == m_bIsDraw)
		return;

	m_pTransCom->SetTransform(m_pGraphicDev);

	m_pMeshCom->Render_Mesh();

	/*_matrix matTest;

	const Engine::D3DXFRAME_DERIVED*		pFrame = m_pMeshCom->Get_FrameByName("M_ROOT");

	matTest = pFrame->CombinedTransformationMatrix * m_pTransCom->m_matWorld;

	m_pColliderCom->Render_Collider(Engine::CCollider::COLL_TRUE, &matTest);*/
}

HRESULT CBoss_WeaPon::Add_Component()
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
	
	// For.Collider
	Make_CollisionSquare();

	// For.ObjState
	pComponent = m_pStateCom = Engine::CObjState::Create(1.f, 5.f);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::CComponent::ID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Com_State", pComponent));

	// For.Optimization
	pComponent = m_pOptimizationCom = dynamic_cast<Engine::COptimization*>(Engine::Clone_Prototype(L"Proto_Optimization"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::CComponent::ID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Optimization", pComponent));

	return S_OK;
}

HRESULT CBoss_WeaPon::Make_CollisionSquare()
{
	Engine::CComponent* pComponent = nullptr;

	wfstream	pFile;
	_vec3		vMin, vMax, vPont;
	_tchar		szBoreName[64], szMaxX[64], szMaxY[64], szMaxZ[64], szMinX[64], szMinY[64], szMinZ[64];
	wstring		strBore;
	_matrix		matPos;

	pFile.open(L"../../Data/Collision_Boss_Sword.txt", ios::in);

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
		pComponent = m_pColliderCom = Engine::CCollider::Create(m_pGraphicDev, 8, sizeof(_vec3), &vMin, &vMax);

		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[Engine::CComponent::ID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Com_Collider", pComponent));
	}

	return S_OK;
}

void CBoss_WeaPon::Fix_Pos(_matrix * pmatPos)
{
	if (m_pBoss->Get_AniIndex() == 0)
	{
		pmatPos->m[3][0] -= 0.02f;
		pmatPos->m[3][1] -= 0.1f;
	}
	else if (m_pBoss->Get_AniIndex() == 1)
	{
		pmatPos->m[3][0] -= 0.02f;
		pmatPos->m[3][1] -= 0.1f;
		pmatPos->m[3][2] += 0.02f;
	}
	else if (m_pBoss->Get_AniIndex() == 2)
	{
		pmatPos->m[3][0] -= 0.02f;
		pmatPos->m[3][1] -= 0.1f;
		pmatPos->m[3][2] += 0.02f;
	}
	else if (m_pBoss->Get_AniIndex() == 3)
	{
		pmatPos->m[3][0] -= 0.13f;
		pmatPos->m[3][1] -= 0.05f;
		pmatPos->m[3][2] -= 0.1f;
	}
	else if (m_pBoss->Get_AniIndex() == 4)
	{
		pmatPos->m[3][2] -= 0.1f;
	}
	else if (m_pBoss->Get_AniIndex() == 5)
	{
		pmatPos->m[3][0] += 0.08f;
		pmatPos->m[3][2] += 0.1f;
	}
}

void CBoss_WeaPon::Get_Object()
{
	if (nullptr == m_pParentBoneMatrix)
	{
		Engine::CDynamicMesh*		pBossMeshCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Get_Component(L"Layer_GameLogic",
			L"Boss",
			L"Com_Mesh",
			Engine::CComponent::ID_STATIC));
	
		NULL_CHECK(pBossMeshCom);

		const Engine::D3DXFRAME_DERIVED*		pFrame = pBossMeshCom->Get_FrameByName("R_Hand");

		m_pParentBoneMatrix = &pFrame->CombinedTransformationMatrix;

		Engine::CTransform*		pBossTransCom = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(
			L"Layer_GameLogic",
			L"Boss",
			L"Com_Transform",
			Engine::CComponent::ID_DYNAMIC));

		NULL_CHECK(pBossTransCom);

		m_pParentWorldMatrix = pBossTransCom->Get_WorldMatrixPointer();

		m_pBoss = dynamic_cast<CBoss*>(Engine::Get_Management()->Get_Object(L"Layer_GameLogic", L"Boss"));
		NULL_CHECK(m_pBoss);
	}
}

void CBoss_WeaPon::Check_Target()
{
	Engine::CObjState* pParentState = dynamic_cast<Engine::CObjState*>(Engine::Get_Component(L"Layer_GameLogic", L"Boss", L"Com_State", Engine::CComponent::ID_DYNAMIC));
	NULL_CHECK(pParentState);

	if (pParentState->Get_State() != Engine::CObjState::NOW_ATTACK)
	{
		return;
	}

	CPlayer* pTarget = dynamic_cast<CPlayer*>(Engine::Get_Management()->Get_Object(L"Layer_GameLogic", L"Player"));
	NULL_CHECK(pTarget);

	if (pTarget->Get_IsHit() == true)
	{
		return;
	}
	
	Engine::CSphereCollider* pSphereCollider = dynamic_cast<Engine::CSphereCollider*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Com_SphereCol", Engine::CComponent::ID_STATIC));
	NULL_CHECK(pSphereCollider);

	Engine::CTransform* pTransform = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Com_Transform", Engine::CComponent::ID_DYNAMIC));
	NULL_CHECK(pTransform);

	Engine::CDynamicMesh* pMesh = dynamic_cast<Engine::CDynamicMesh*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Com_Mesh", Engine::CComponent::ID_STATIC));
	NULL_CHECK(pMesh);

	vector<wstring> vecBore = *pTarget->Get_BoreName();
	vector<Engine::CCollider*>* pvecCollider = pTarget->Get_Collider();
	int i = 0;

	for (auto iter : *pvecCollider)
	{
		if (pTarget->Get_IsHit() == true)
		{
			continue;
		}

		_matrix matDstPos;

		char* pStr;
		int strSize = WideCharToMultiByte(CP_ACP, 0, vecBore[i].c_str(), -1, NULL, 0, NULL, NULL);
		pStr = new char[strSize];
		WideCharToMultiByte(CP_ACP, 0, vecBore[i].c_str(), -1, pStr, strSize, 0, 0);

		const Engine::D3DXFRAME_DERIVED*		pFrame = pMesh->Get_FrameByName(pStr);

		delete pStr;
		
		_matrix matSrcPos;

		matSrcPos = pFrame->CombinedTransformationMatrix * pTransform->m_matWorld;

		pFrame = nullptr;

		const Engine::D3DXFRAME_DERIVED*		pDestFrame = pMesh->Get_FrameByName("M_ROOT");

		matDstPos = pDestFrame->CombinedTransformationMatrix * m_pTransCom->m_matWorld;

		_vec3 vCollPos;

		if (m_pCalculatorCom->Collision_OBB(m_pColliderCom->Get_Min(), m_pColliderCom->Get_Max(), &matDstPos, iter->Get_Min(), iter->Get_Max(), &matSrcPos, &vCollPos))
		{
			pTarget->Set_Hit(TRUE);;

			Engine::CObjState* pPlayerState = dynamic_cast<Engine::CObjState*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Com_State", Engine::CComponent::ID_DYNAMIC));
			NULL_CHECK(pPlayerState);

			pPlayerState->Set_Damge(m_pStateCom->Get_Damage());
		}

		i++;
	}
}

CBoss_WeaPon* CBoss_WeaPon::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBoss_WeaPon*		pInstance = new CBoss_WeaPon(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CBoss_WeaPon::Free()
{
	Engine::CGameObject::Free();
}

