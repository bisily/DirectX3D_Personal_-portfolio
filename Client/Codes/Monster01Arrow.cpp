#include "stdafx.h"
#include "Monster01Arrow.h"

#include "Export_Function.h"
#include "Monster01.h"
#include "Player.h"

CMonster01Arrow::CMonster01Arrow(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CMonster01Arrow::~CMonster01Arrow()
{
}

HRESULT CMonster01Arrow::Ready_Object(_matrix* pmatPos, int iIndex)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	_vec3 vPos;

	m_pParentWorldMatrix = pmatPos;

	m_pTransCom->Set_Scale(0.01f, 0.01f, 0.01f);

	m_iIndex = iIndex;

	return S_OK;
}

_int CMonster01Arrow::Update_Object(const _float & fTimeDelta)
{
	Get_ParentBoneMatrix();

	_uint iExit = Engine::CGameObject::Update_Object(fTimeDelta);

	_int iParentAni = m_pParent->Get_AniIndex();
	
	if (m_pStateCom->Get_State() == Engine::CObjState::NOW_DEAD)
	{
		return -1;
	}

	if (Is_bCollision == true)
	{
		return -1;
	}

	if (iParentAni == 1 &&
		76 < m_pParetMesh->Get_Percent())
	{
		m_bFire = true;
	}

	if (iParentAni == 1 &&
		m_bFire == false &&
		75 > m_pParetMesh->Get_Percent())
	{
		_matrix matPos, matScalie, matRot;
		D3DXMatrixIdentity(&matScalie);
		D3DXMatrixScaling(&matScalie, 100.f, 100.f, 100.f);
		D3DXMatrixRotationY(&matRot, D3DXToRadian(90.f));

		matPos = matScalie * matRot * ((*m_pParentBoneMatrix) * (*m_pParentWorldMatrix));
	
		m_pTransCom->Set_ParentMatrix(&matPos);

		memcpy(&vTargetLook, &m_pParentWorldMatrix->m[2][0], sizeof(_vec3));
		vTargetLook *= -1.f;
		memcpy(&vSelfPos, &m_pParentWorldMatrix->m[3][0], sizeof(_vec3));
		fHeight = matPos.m[3][1];
	}
	else if(m_bFire == true &&
		m_bPosSet == false)
	{
		m_bFire = false;

		if (m_bPosSet == false)
		{
			m_bPosSet = true;

			vSelfPos.y = fHeight;

			m_pTransCom->m_vInfo[Engine::CTransform::INFO_POS] = vSelfPos;

			// 스케일
			m_pTransCom->Set_Scale(0.01f, 0.01f, 0.01f);
			
			Angle_Chekc();
		}
		
	}

	if (m_bFire == true &&
		m_bPosSet == true)
	{
		D3DXVec3Normalize(&vTargetLook, &vTargetLook);
		// 이동
		m_pTransCom->m_vInfo[Engine::CTransform::INFO_POS] += (vTargetLook * 9.5f * fTimeDelta);
		
		_float fLength = D3DXVec3Length(&(m_pTransCom->m_vInfo[Engine::CTransform::INFO_POS] - vSelfPos));

		if (50.f <= fLength)
		{
			return -1;
		}
	
	}


	Collision_Player();
	
	m_pRendererCom->Add_RenderGroup(Engine::CRenderer::RENDER_NONALPHA, this);

	return 0;
}

void CMonster01Arrow::Render_Object()
{
	m_pTransCom->SetTransform(m_pGraphicDev);

	m_pMeshCom->Render_Mesh();

	//m_pColliderCom->Render_Collider(Engine::CCollider::COLL_TRUE,&m_pTransCom->m_matWorld);
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

	// For.Collider
	pComponent = m_pColliderCom = Engine::CCollider::Create(m_pGraphicDev,
		m_pMeshCom->Get_VtxPos(),
		m_pMeshCom->Get_Numvertices(),
		m_pMeshCom->Get_Stride());

	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::CComponent::ID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Collider", pComponent));

	// For.Shader
	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_Mesh"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::CComponent::ID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Shader", pComponent));

	// For.Optimization
	pComponent = m_pOptimizationCom = dynamic_cast<Engine::COptimization*>(Engine::Clone_Prototype(L"Proto_Optimization"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::CComponent::ID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Optimization", pComponent));

	// For.ObjState
	pComponent = m_pStateCom = Engine::CObjState::Create(1.f, 2.f);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::CComponent::ID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Com_State", pComponent));

	return S_OK;
}

void CMonster01Arrow::Get_ParentBoneMatrix()
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

		const Engine::D3DXFRAME_DERIVED*		pFrame = pMonster01BowMeshCom->Get_FrameByName("R_Hand");

		m_pParentBoneMatrix = &pFrame->CombinedTransformationMatrix;

		m_pParent = dynamic_cast<CMonster01*>(Engine::Get_Management()->Get_Object(L"Layer_GameLogic", L"Monster01", m_iIndex));
		NULL_CHECK(m_pParent);

		m_pParetMesh = dynamic_cast<Engine::CDynamicMesh*>(m_pParent->Get_Component(L"Com_Mesh", Engine::CComponent::ID_STATIC));
		NULL_CHECK(m_pParetMesh);
	}
}

void CMonster01Arrow::Collision_Player()
{
	Engine::CTransform* pTargetPos = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Com_Transform", Engine::CComponent::ID_DYNAMIC));
	_float fDist = D3DXVec3Length(&(pTargetPos->m_vInfo[Engine::CTransform::INFO_POS] - m_pTransCom->m_vInfo[Engine::CTransform::INFO_POS]));
	

	Engine::CSphereCollider* pSphereCollider = dynamic_cast<Engine::CSphereCollider*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Com_SphereCol", Engine::CComponent::ID_STATIC));
	NULL_CHECK(pSphereCollider);

	Engine::CTransform* pTransform = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Com_Transform", Engine::CComponent::ID_DYNAMIC));
	NULL_CHECK(pTransform);

	Engine::CGameObject* pTarget = dynamic_cast<CPlayer*>(Engine::Get_Management()->Get_Object(L"Layer_GameLogic", L"Player"));

	_vec3 vMin = *m_pColliderCom->Get_Min();
	_vec3 vMax = *m_pColliderCom->Get_Max();

	if (m_pCalculatorCom->Collision_AABB_Sphere(&vMin, &vMax, &m_pTransCom->m_matWorld, pSphereCollider->Get_SphereList(), &pTransform->m_matWorld))
	{
		pTarget->Set_Hit(TRUE);

		Engine::CObjState* pPlayerState = dynamic_cast<Engine::CObjState*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Com_State", Engine::CComponent::ID_DYNAMIC));
		NULL_CHECK(pPlayerState);

		pPlayerState->Set_Damge(m_pStateCom->Get_Damage());

		Is_bCollision = true;

		return;
	}
}

void CMonster01Arrow::Angle_Chekc()
{
	// 회전
	Engine::CTransform* pTransform = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Com_Transform", Engine::CComponent::ID_DYNAMIC));
	NULL_CHECK(pTransform);

	_vec3 vLook, vAngle, vParentPos;
	_float fRotateAngle;

	memcpy(&vParentPos, &m_pParentWorldMatrix->m[3][0], sizeof(_vec3));

	vAngle = pTransform->m_vInfo[Engine::CTransform::INFO_POS] - vParentPos;
	D3DXVec3Normalize(&vAngle, &vAngle);

	vLook = _vec3(0.f, 0.f, -1.f);
	D3DXVec3Normalize(&vLook, &vLook);

	fLookAngle = D3DXVec3Dot(&vAngle, &vLook);
	
	fRotateAngle = D3DXToDegree(acosf(fLookAngle));

	//// 플레이어 위치를 바라보는 벡터와 -x 방향 벡터와 각도 검사
	_vec3 vMinusXLook(-1.f, 0.f, 0.f);
	D3DXVec3Normalize(&vMinusXLook, &vMinusXLook);

	_float fCheck = D3DXVec3Dot(&vMinusXLook, &vAngle);
	_float fCheck_Value = D3DXToDegree(acosf(fCheck));

	if (pTransform->m_vInfo[Engine::CTransform::INFO_POS].x < vParentPos.x)
	{
		fRotateAngle = acosf(fLookAngle);
		m_pTransCom->m_vAngle[Engine::CTransform::ROT_Y] = fRotateAngle;
	}
	else
	{
		if (90.f < fCheck_Value)
		{
			fRotateAngle = acosf(-fLookAngle);
			m_pTransCom->m_vAngle[Engine::CTransform::ROT_Y] = fRotateAngle;
		}
		else
		{
			fRotateAngle = cosf(fLookAngle);
			m_pTransCom->m_vAngle[Engine::CTransform::ROT_Y] = fRotateAngle;
		}
	}
}

CMonster01Arrow * CMonster01Arrow::Create(LPDIRECT3DDEVICE9 pGraphicDev, _matrix* pmatPos, int iIndex)
{
	CMonster01Arrow*		pInstance = new CMonster01Arrow(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pmatPos, iIndex)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CMonster01Arrow::Free()
{
	Engine::CGameObject::Free();
}
