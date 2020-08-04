#include "stdafx.h"
#include "Player_WeaPon.h"

#include "Export_Function.h"
#include "MonsterLifeBar.h"

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

	m_pTransCom->Rotation(Engine::CTransform::ROT_X, D3DXToRadian(90.f));

	return S_OK;
}

_int CPlayer_WeaPon::Update_Object(const _float& fTimeDelta)
{
	Get_ParentBoneMatrix();
	
	_uint iExit = Engine::CGameObject::Update_Object(fTimeDelta);

	m_pTransCom->Set_ParentMatrix(&((*m_pParentBoneMatrix) * (*m_pParentWorldMatrix)));

	MonsterCollision();

	BossCollision();
		
	m_pRendererCom->Add_RenderGroup(Engine::CRenderer::RENDER_NONALPHA, this);

	return 0;
}

void CPlayer_WeaPon::Render_Object()
{
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
	
	// For.BoreName
	map<wstring, _float> mapBore;
	Make_BoreNames(&mapBore);

	// For.SphereCollider
	pComponent = m_pSphereColliderCom = Engine::CSphereCollider::Create(m_pGraphicDev, m_pMeshCom->Get_RootBore(), &mapBore, &m_pTransCom->m_matWorld);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::CComponent::ID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_SphereCol", pComponent));
	
	// Form.CollisionSquare
	Make_CollisionSquare();
	
	// For.Shader
	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_Mesh"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::CComponent::ID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Shader", pComponent));

	// For.ObjState
	pComponent = m_pStateCom = Engine::CObjState::Create(1.f, 10.f);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::CComponent::ID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Com_State", pComponent));

	return S_OK;
}

HRESULT CPlayer_WeaPon::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
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

HRESULT CPlayer_WeaPon::Make_BoreNames(map<wstring, _float>* pmapBore)
{
	TCHAR szBoreName[64] = L"", szBoreSize[64] = L"";
	DWORD dwByte = 99;

	wifstream	LoadFile;

	LoadFile.open(L"../../Data/Collision_Player_WeaPon_Swrod.txt", ios::in);

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

void CPlayer_WeaPon::Get_ParentBoneMatrix()
{
	if (nullptr == m_pParentBoneMatrix)
	{
		Engine::CDynamicMesh*		pPlayerMeshCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Get_Component(L"Layer_GameLogic",
			L"Player",
			L"Com_Mesh",
			Engine::CComponent::ID_STATIC));

		if (nullptr == pPlayerMeshCom)
			return;

		const Engine::D3DXFRAME_DERIVED*		pFrame = pPlayerMeshCom->Get_FrameByName("R_FINGER12");

		m_pParentBoneMatrix = &pFrame->CombinedTransformationMatrix;

		Engine::CTransform*		pPlayerTransCom = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(
			L"Layer_GameLogic",
			L"Player",
			L"Com_Transform",
			Engine::CComponent::ID_DYNAMIC));

		if (nullptr == pPlayerTransCom)
			return;

		m_pParentWorldMatrix = pPlayerTransCom->Get_WorldMatrixPointer();

		pFrame = m_pMeshCom->Get_FrameByName("M_CENTER");

		m_pMyBore = &pFrame->CombinedTransformationMatrix;

		Engine::CObjState* pPlayerStateCom = dynamic_cast<Engine::CObjState*>(Engine::Get_Component(
			L"Layer_GameLogic", 
			L"Player", 
			L"Com_State", 
			Engine::CComponent::ID_DYNAMIC));

		NULL_CHECK(pPlayerStateCom);

		m_pStateParentCom = pPlayerStateCom;
	}
}

void CPlayer_WeaPon::BossCollision()
{	
	if (m_pStateParentCom->Get_State() != Engine::CObjState::NOW_ATTACK)
	{
		return;
	}

	if (m_pParentWorldMatrix->m[3][0] < fTriggerX)
	{
		return;
	}

	Engine::CObjState* pBossState = dynamic_cast<Engine::CObjState*>(Engine::Get_Component(L"Layer_GameLogic", L"Boss", L"Com_State", Engine::CComponent::ID_DYNAMIC));
	
	if (pBossState == nullptr)
	{
		return;
	}


	if (pBossState->Get_Hp() < 0)
	{
		return;
	}

	Engine::CGameObject* pBoss = Engine::Get_Management()->Get_Object(L"Layer_GameLogic", L"Boss");

	if (pBoss->Get_IsHit() == true)
	{
		return;
	}

	if (pBossState != nullptr &&
		pBossState->Get_State() != Engine::CObjState::NOW_DEAD)
	{
		Engine::CSphereCollider* pSphereCollider = dynamic_cast<Engine::CSphereCollider*>(Engine::Get_Component(L"Layer_GameLogic", L"Boss", L"Com_SphereCol", Engine::CComponent::ID_STATIC));
		NULL_CHECK(pSphereCollider);

		Engine::CTransform* pTransform = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Boss", L"Com_Transform", Engine::CComponent::ID_DYNAMIC));
		NULL_CHECK(pTransform);

		if (m_pCalculatorCom->Collision_AABB_Sphere(m_pColliderCom->Get_Min(), m_pColliderCom->Get_Max(), &((*m_pParentBoneMatrix) * (*m_pParentWorldMatrix)), pSphereCollider->Get_SphereList(), &pTransform->m_matWorld))
		{
			CSoundMgr::GetInstance()->PlaySound(L"Hit_Sound.wav", CSoundMgr::CHANNEL_HIT);

			if (pBossState->Get_Hp() <= 0)
			{
				pBossState->Set_State(Engine::CObjState::NOW_DEAD);
			}

			pBossState->Set_Damge(m_pStateCom->Get_Damage());

			pBoss->Set_Hit(true);
		}
	}
}

void CPlayer_WeaPon::MonsterCollision()
{
	if (m_pStateParentCom->Get_State() != Engine::CObjState::NOW_ATTACK)
	{
		return;
	}
	
	int iSize = Engine::Get_Management()->Get_CurrentScene()->Get_Layer(L"Layer_GameLogic")->Get_ObjectMultiMap().count(L"Monster00");

	if (1 <= iSize)
	{
		multimap<const wstring, Engine::CGameObject*> multiMap = Engine::Get_Management()->Get_CurrentScene()->Get_Layer(L"Layer_GameLogic")->Get_ObjectMultiMap();

		auto iter = multiMap.find(L"Monster00");

		for (int i = 0; i < iSize; ++iter, ++i)
		{
			Engine::CSphereCollider* pSphereCollider = dynamic_cast<Engine::CSphereCollider*>((*iter).second->Get_Component(L"Com_SphereCol", Engine::CComponent::ID_STATIC));
			NULL_CHECK(pSphereCollider);

			Engine::CTransform* pTransform = dynamic_cast<Engine::CTransform*>((*iter).second->Get_Component(L"Com_Transform", Engine::CComponent::ID_DYNAMIC));
			NULL_CHECK(pTransform);

			Engine::CObjState* pMonster00 = dynamic_cast<Engine::CObjState*>((*iter).second->Get_Component(L"Com_State", Engine::CComponent::ID_DYNAMIC));
			NULL_CHECK(pMonster00);

			if (pMonster00->Get_Hp() < 0)
			{
				continue;
			}
			else if ((*iter).second->Get_IsHit() == true)
			{
				continue;
			}

			if (m_pCalculatorCom->Collision_AABB_Sphere(m_pColliderCom->Get_Min(), m_pColliderCom->Get_Max(), &((*m_pParentBoneMatrix) * (*m_pParentWorldMatrix)), pSphereCollider->Get_SphereList(), &pTransform->m_matWorld))
			{
				pMonster00->Set_Damge(m_pStateCom->Get_Damage());
				(*iter).second->Set_Hit(true);
				
				CSoundMgr::GetInstance()->PlaySound(L"Hit_Sound.wav", CSoundMgr::CHANNEL_HIT);

				if (pMonster00->Get_Hp() <= 0)
				{
					pMonster00->Set_State(Engine::CObjState::NOW_DEAD);
					continue;
				}
				
				Engine::CGameObject* pGameObject = Engine::Get_Management()->Get_Object(L"Layer_UI", L"MonsterLifeBar", (*iter).second->GetIndex());
				NULL_CHECK(pGameObject);
				
				dynamic_cast<CMonsterLifeBar*>(pGameObject)->SetUp_Trigger(true);
			}
		}
	}

	if (13 <= iSize)
	{
		return;
	}

	iSize = Engine::Get_Management()->Get_CurrentScene()->Get_Layer(L"Layer_GameLogic")->Get_ObjectMultiMap().count(L"Monster01");

	if (1 <= iSize)
	{
		multimap<const wstring, Engine::CGameObject*> multiMap = Engine::Get_Management()->Get_CurrentScene()->Get_Layer(L"Layer_GameLogic")->Get_ObjectMultiMap();

		auto iter = multiMap.find(L"Monster01");

		for (int i = 0; i < iSize; ++iter, ++i)
		{
			Engine::CSphereCollider* pSphereCollider = dynamic_cast<Engine::CSphereCollider*>((*iter).second->Get_Component(L"Com_SphereCol", Engine::CComponent::ID_STATIC));
			NULL_CHECK(pSphereCollider);
		
			Engine::CTransform* pTransform = dynamic_cast<Engine::CTransform*>((*iter).second->Get_Component(L"Com_Transform", Engine::CComponent::ID_DYNAMIC));
			NULL_CHECK(pTransform);

			Engine::CObjState* pMonster01 = dynamic_cast<Engine::CObjState*>((*iter).second->Get_Component(L"Com_State", Engine::CComponent::ID_DYNAMIC));
			NULL_CHECK(pMonster01);

			if (pMonster01->Get_Hp() < 0)
			{
				continue;
			}
			else if ((*iter).second->Get_IsHit() == true)
			{
				continue;
			}

			if (m_pCalculatorCom->Collision_AABB_Sphere(m_pColliderCom->Get_Min(), m_pColliderCom->Get_Max(), &((*m_pParentBoneMatrix) * (*m_pParentWorldMatrix)), pSphereCollider->Get_SphereList(), &pTransform->m_matWorld))
			{
				pMonster01->Set_Damge(m_pStateCom->Get_Damage());
				(*iter).second->Set_Hit(true);

				CSoundMgr::GetInstance()->PlaySound(L"Hit_Sound.wav", CSoundMgr::CHANNEL_HIT);

				if (pMonster01->Get_Hp() <= 0)
				{
					pMonster01->Set_State(Engine::CObjState::NOW_DEAD);

					continue;
				}

				Engine::CGameObject* pGameObject = Engine::Get_Management()->Get_Object(L"Layer_UI", L"MonsterLifeBar", (*iter).second->GetIndex());
				NULL_CHECK(pGameObject);

				dynamic_cast<CMonsterLifeBar*>(pGameObject)->SetUp_Trigger(true);
			}
		}
	}
}

HRESULT CPlayer_WeaPon::Make_CollisionSquare()
{
	Engine::CComponent* pComponent = nullptr;

	wfstream	pFile;
	_vec3		vMin, vMax, vPont;
	_tchar		szBoreName[64], szMaxX[64], szMaxY[64], szMaxZ[64], szMinX[64], szMinY[64], szMinZ[64];
	wstring		strBore;
	_matrix		matPos;

	pFile.open(L"../../Data/Collision_Player_WeaPon_Swrod.txt", ios::in);

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

CPlayer_WeaPon* CPlayer_WeaPon::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPlayer_WeaPon*		pInstance = new CPlayer_WeaPon(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CPlayer_WeaPon::Free()
{
	for_each(m_vecCollider.begin(), m_vecCollider.end(), Engine::CDeleteObj());
	Engine::CGameObject::Free();
}

