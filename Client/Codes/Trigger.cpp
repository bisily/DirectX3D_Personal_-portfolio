#include "stdafx.h"
#include "Trigger.h"

#include "Export_Function.h"

#include "BossLifeBar.h"
#include "Monster00.h"
#include "Monster01.h"

CTrigger::CTrigger(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{

}

CTrigger::~CTrigger()
{

}

HRESULT CTrigger::Ready_Object()
{
	if (FAILED(Add_Component()))
		return E_FAIL;
	
	bClearCondition[0] = false;
	bClearCondition[1] = false;
	bClearCondition[2] = false;

	vecTriggerPos[0] = _vec3(56.f, 2.f, 53.f);
	vecTriggerPos[1] = _vec3(179.f, 21.f, -19.f);
	vecTriggerPos[2] = _vec3(220.f, 21.f, -36.f);

	m_tagSphere.fRadius = 2.f;
	m_tagSphere.fScaling = 1.f;
	
	D3DXCreateSphere(m_pGraphicDev, m_tagSphere.fRadius, 10, 10, &m_tagSphere.pMesh, nullptr);

	_ulong dwFvF = m_tagSphere.pMesh->GetFVF();

	m_tagSphere.pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&m_tagSphere.pVertices);

	_vec3 vPos;

	D3DXComputeBoundingSphere(m_tagSphere.pVertices, m_tagSphere.pMesh->GetNumVertices(), D3DXGetFVFVertexSize(dwFvF), &vPos, &m_tagSphere.fRadius);

	m_tagSphere.pMesh->UnlockVertexBuffer();

	return S_OK;
}

Engine::_int CTrigger::Update_Object(const Engine::_float& fTimeDelta)
{
	Engine::_uint iExit = Engine::CGameObject::Update_Object(fTimeDelta);
	
	Get_Object();

	if (bClearCondition[4] == true)
	{
		return 0;
	}


	int iCheckCnt = Engine::Get_Management()->Get_CurrentScene()->Get_Layer(L"Layer_GameLogic")->Get_ObjectMultiMap().count(L"Monster00");

	// 몬스터 6마리 처치
	if (bClearCondition[0] == false &&
		m_vecMoveComponent.size() < 1)
	{
		_vec3 vecLength = (*m_pTargetPos) - vecTriggerPos[0];
		float fLength = D3DXVec3Length(&vecLength);

		if (fLength <= m_tagSphere.fRadius * 2.f)
		{
			int iClearCnt = Engine::Get_Management()->Get_CurrentScene()->Get_Layer(L"Layer_GameLogic")->Get_ObjectMultiMap().count(L"Monster00");

			if (iClearCnt <= 12)
			{
				bClearCondition[0] = true;

				Engine::CComponent* pDoor1 = Engine::Get_ComponentIndex(L"Layer_Ground", L"Mesh_Door", L"Com_Transform", Engine::CComponent::ID_DYNAMIC, 84);

				Engine::CComponent* pDoor2 = Engine::Get_ComponentIndex(L"Layer_Ground", L"Mesh_Door", L"Com_Transform", Engine::CComponent::ID_DYNAMIC, 85);

				m_vecMoveComponent.push_back(pDoor1);
				m_vecMoveComponent.push_back(pDoor2);
			}
		}
	}

	// 몬스터 27마리 처치
	else if (iCheckCnt == 0 || bClearCondition[1] == false &&
		m_vecMoveComponent.size() < 3)
	{
		_vec3 vecLength = (*m_pTargetPos) - vecTriggerPos[1];
		float fLength = D3DXVec3Length(&vecLength);

		if (fLength <= m_tagSphere.fRadius * 2.f)
		{
			int iClearCnt = Engine::Get_Management()->Get_CurrentScene()->Get_Layer(L"Layer_GameLogic")->Get_ObjectMultiMap().count(L"Monster00");
			iClearCnt += Engine::Get_Management()->Get_CurrentScene()->Get_Layer(L"Layer_GameLogic")->Get_ObjectMultiMap().count(L"Monster01");

			if (iClearCnt == 0)
			{
				bClearCondition[1] = true;

				Engine::CComponent* pDoor1 = Engine::Get_ComponentIndex(L"Layer_Ground", L"Mesh_Castle_Door", L"Com_Transform", Engine::CComponent::ID_DYNAMIC, 80);

				Engine::CComponent* pDoor2 = Engine::Get_ComponentIndex(L"Layer_Ground", L"Mesh_Castle_Door", L"Com_Transform", Engine::CComponent::ID_DYNAMIC, 81);

				m_vecMoveComponent.push_back(pDoor1);
				m_vecMoveComponent.push_back(pDoor2);
			}
		}
	}

	// 보스 처치
	else if (bClearCondition[2] == false &&
		m_vecMoveComponent.size() < 5)
	{
		_vec3 vecLength = (*m_pTargetPos) - vecTriggerPos[2];
		float fLength = D3DXVec3Length(&vecLength);

		if (fLength <= m_tagSphere.fRadius * 2.f)
		{
			Engine::CGameObject* pCheckObject = Engine::Get_Management()->Get_Object(L"Layer_GameLogic", L"Boss");

			if (pCheckObject == nullptr)
			{
				bClearCondition[2] = true;

				Engine::CComponent* pDoor1 = Engine::Get_ComponentIndex(L"Layer_Ground", L"Mesh_Door", L"Com_Transform", Engine::CComponent::ID_DYNAMIC, 86);

				Engine::CComponent* pDoor2 = Engine::Get_ComponentIndex(L"Layer_Ground", L"Mesh_Door", L"Com_Transform", Engine::CComponent::ID_DYNAMIC, 87);

				m_vecMoveComponent.push_back(pDoor1);
				m_vecMoveComponent.push_back(pDoor2);
			}
		}
	}
	
	if (bClearCondition[0] == true)
	{
		if (m_fAngle * 50.f < 0.8f)
		{
			m_fAngle += 0.005f * fTimeDelta;
			dynamic_cast<Engine::CTransform*>(m_vecMoveComponent[0])->Rotation(Engine::CTransform::ROT_Y, m_fAngle);
			dynamic_cast<Engine::CTransform*>(m_vecMoveComponent[1])->Rotation(Engine::CTransform::ROT_Y, -m_fAngle);
		}
		else
		{
			bClearCondition[0] = false;
			m_fAngle = 0.f;
		}
	}

	else if (bClearCondition[1] == true)
	{
		if (m_fAngle * 50.f < 0.8f)
		{
			m_fAngle += 0.005f * fTimeDelta;
			dynamic_cast<Engine::CTransform*>(m_vecMoveComponent[2])->Rotation(Engine::CTransform::ROT_Y, m_fAngle);
			dynamic_cast<Engine::CTransform*>(m_vecMoveComponent[3])->Rotation(Engine::CTransform::ROT_Y, -m_fAngle);
		}
		else
		{
			Engine::CGameObject* pGameObject = Engine::Get_Management()->Get_Object(L"Layer_UI", L"Boss_LifeBar");
			NULL_CHECK_RETURN(pGameObject, -1);
			dynamic_cast<CBossLifeBar*>(pGameObject)->SetUp_Trigger(TRUE);

			bClearCondition[1] = false;
			m_fAngle = 0.f;
		}
	}
	else if (bClearCondition[2] == true)
	{
		if (m_fAngle * 50.f < 0.8f)
		{
			Engine::CGameObject* pGameObject = Engine::Get_Management()->Get_Object(L"Layer_UI", L"Boss_LifeBar");
			
			if (pGameObject == nullptr)
			{
				m_fAngle += 0.005f * fTimeDelta;
				dynamic_cast<Engine::CTransform*>(m_vecMoveComponent[4])->Rotation(Engine::CTransform::ROT_Y, m_fAngle);
				dynamic_cast<Engine::CTransform*>(m_vecMoveComponent[5])->Rotation(Engine::CTransform::ROT_Y, -m_fAngle);
			}

		}
		else
		{
			bClearCondition[4] = true;
			bClearCondition[2] = false;
			m_fAngle = 0.f;
		}
	}
	return 0;
}

void CTrigger::Render_Object()
{
}


HRESULT CTrigger::Add_Component()
{
	Engine::CComponent*		pComponent = nullptr;

	// For.Texture
	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone_Resource(RESOURCE_STAGE, L"Texture_Terrain"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::CComponent::ID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture", pComponent));

	return S_OK;
}

void CTrigger::Get_Object()
{
	if (m_pTargetPos == nullptr)
	{
		Engine::CTransform* pPos = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Com_Transform", Engine::CComponent::ID_DYNAMIC));

		m_pTargetPos = pPos->Get_PosPointer();
	}
}


CTrigger* CTrigger::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTrigger*		pInstance = new CTrigger(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CTrigger::Free()
{
	Engine::CGameObject::Free();
}

