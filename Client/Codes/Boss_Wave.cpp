#include "stdafx.h"
#include "Boss_Wave.h"

#include "Export_Function.h"
#include "Player.h"
#include "SwordTrail.h"

CBoss_Wave::CBoss_Wave(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{

}

CBoss_Wave::~CBoss_Wave()
{

}

HRESULT CBoss_Wave::Ready_Object(_int iIndex, _vec3 vPos)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_iIndex = iIndex;

	m_pTransCom->Set_Scale(0.02f, 0.02f, 0.02f);

	m_pTransCom->m_vInfo[Engine::CTransform::INFO_POS] = vPos;

	m_pTransCom->m_vInfo[Engine::CTransform::INFO_POS].y += 1.f;

	Engine::CTransform* pTransform = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Com_Transform", Engine::CComponent::ID_DYNAMIC));

	_vec3 vTem = pTransform->m_vInfo[Engine::CTransform::INFO_POS] - vPos;
	D3DXVec3Normalize(&vTem, &vTem);
	m_vDir = vTem;

	// È¸Àü
	_vec3 vLook = m_pTransCom->m_vInfo[Engine::CTransform::INFO_RIGHT]; 

	D3DXVec3Normalize(&vLook, &vLook);

	if (pTransform->m_vInfo[Engine::CTransform::INFO_POS].z <= m_pTransCom->m_vInfo[Engine::CTransform::INFO_POS].z)
	{
		_float fAngle = acosf(D3DXVec3Dot(&vLook, &vTem));
	
		_float fRotAngle = D3DXToDegree(fAngle);

		m_pTransCom->Rotation(Engine::CTransform::ROT_Y, D3DXToRadian(fRotAngle));
	}
	else if (pTransform->m_vInfo[Engine::CTransform::INFO_POS].z > m_pTransCom->m_vInfo[Engine::CTransform::INFO_POS].z)
	{
		_float fAngle = acosf(D3DXVec3Dot(&vLook, &vTem));

		_float fRotAngle = D3DXToDegree(-fAngle);

		m_pTransCom->Rotation(Engine::CTransform::ROT_Y, D3DXToRadian(fRotAngle));
	}


	return S_OK;
}

_int CBoss_Wave::Update_Object(const _float& fTimeDelta)
{
	if (bIs_Dead == true)
	{
		wstring strName1, strName2, strwTem;
		string strTem;

		strTem = to_string(m_iIndex);

		// Boss_Wave0_Up
		strName1 = L"Boss_Wave";
		strName2 = L"Boss_Wave";

		strwTem.assign(strTem.begin(), strTem.end());
		strName1 += strwTem + L"_Up";
		Engine::CGameObject* pGameObject = Engine::Get_Management()->Get_Object(L"Layer_GameLogic", strName1, m_iIndex);
		NULL_CHECK_RETURN(pGameObject, -1);

		dynamic_cast<CSwordTrail*>(pGameObject)->SetDead();
		
		strName2 += strwTem + L"_Down";
		pGameObject = Engine::Get_Management()->Get_Object(L"Layer_GameLogic", strName2, m_iIndex);
		NULL_CHECK_RETURN(pGameObject, -1);
		dynamic_cast<CSwordTrail*>(pGameObject)->SetDead();
		
		return -1;
	}

	_uint iExit = Engine::CGameObject::Update_Object(fTimeDelta);

	Check_Target();

	fDrawTime += 5.f * fTimeDelta;

	if (10.f < fDrawTime)
	{
		bIs_Dead = true;
	}

	_vec3 vTem;

	vTem = m_vDir;
	D3DXVec3Normalize(&vTem, &vTem);

	m_pTransCom->m_vInfo[Engine::CTransform::INFO_POS] += vTem * 0.2f;
	
	if (bIs_Dead == false)
	{
		m_pRendererCom->Add_RenderGroup(Engine::CRenderer::RENDER_ALPHABLEND, this);
	}

	return 0;
}

void CBoss_Wave::Render_Object()
{
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTransCom->SetTransform(m_pGraphicDev);

	m_pMeshCom->Render_Mesh();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	//m_pColliderCom->Render_Collider(Engine::CCollider::COLL_TRUE, &m_pTransCom->m_matWorld);
}

HRESULT CBoss_Wave::Add_Component()
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
	pComponent = m_pMeshCom = dynamic_cast<Engine::CStaticMesh*>(Engine::Clone_Resource(RESOURCE_STAGE, L"Mesh_Wave"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::CComponent::ID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Mesh", pComponent));

	// For.Collider
	pComponent = m_pColliderCom = Engine::CCollider::Create(m_pGraphicDev,
		m_pMeshCom->Get_VtxPos(),
		m_pMeshCom->Get_Numvertices(),
		m_pMeshCom->Get_Stride());

	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::CComponent::ID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Collider", pComponent));

	// For.ObjState
	pComponent = m_pStateCom = Engine::CObjState::Create(1.f, 5.f);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::CComponent::ID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Com_State", pComponent));

	return S_OK;
}

void CBoss_Wave::Check_Target()
{
	
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

		if (m_pCalculatorCom->Collision_AABB_Sphere(m_pColliderCom->Get_Min(), m_pColliderCom->Get_Max(), &m_pTransCom->m_matWorld, pSphereCollider->Get_SphereList(), &pTransform->m_matWorld))
		{
			pTarget->Set_Hit(TRUE);

			Engine::CObjState* pPlayerState = dynamic_cast<Engine::CObjState*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Com_State", Engine::CComponent::ID_DYNAMIC));
			NULL_CHECK(pPlayerState);

			pPlayerState->Set_Damge(m_pStateCom->Get_Damage());

			return;
		}

		i++;
	}
}

CBoss_Wave* CBoss_Wave::Create(LPDIRECT3DDEVICE9 pGraphicDev, _int iIndex, _vec3 vPos)
{
	CBoss_Wave*		pInstance = new CBoss_Wave(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(iIndex, vPos)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CBoss_Wave::Free()
{
	Engine::CGameObject::Free();
}

