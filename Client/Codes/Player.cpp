#include "stdafx.h"
#include "Player.h"

#include "Export_Function.h"
#include "MonsterLifeBar.h"
#include "StepFog.h"
#include "MeshStepFog.h"
#include "PlayerHitBlood.h"

CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)	
{

}

CPlayer::~CPlayer()
{
	
}

HRESULT CPlayer::Ready_Object()
{
	if(FAILED(Add_Component()))
		return E_FAIL;

	m_pMeshCom->Set_AnimationSet(57);

	m_pTransCom->Set_Scale(0.01f, 0.01f, 0.01f);
	m_pTransCom->Set_Position(20.f, 0.f, 20.f);
	m_pTransCom->Rotation(Engine::CTransform::ROT_Y, D3DXToRadian(-180.f));

	return S_OK;
}

Engine::_int CPlayer::Update_Object(const Engine::_float& fTimeDelta)
{
	Engine::_uint iExit = Engine::CGameObject::Update_Object(fTimeDelta);
	
	Key_Check(fTimeDelta);

	Play_Animation(fTimeDelta);

	if (IsbHit == true)
	{
		m_dHitDelay += fTimeDelta;

		if (iIndex != 17)
		{
			m_pMeshCom->Set_AnimationSet(17);
			iIndex = 17;

			srand((unsigned int)time(nullptr));

			int iSound = rand() % 2;

			CSoundMgr::GetInstance()->StopSound(CSoundMgr::CHANNEL_PLAYER_HIT);

			wstring strTem;

			if (iSound % 2 == 0)
			{
				strTem = L"Player_Hit_Sound.wav";
			}
			else
			{
				strTem = L"Player_Hit_Sound2.wav";
			}

			_vec3 vPos(0.f, 0.f, 0.f);
			_vec3 vTem0, vTem1;
			_int iTex, iTex0, iTex1;

			srand((unsigned int)time(nullptr));

			for (int i = 0; i < 3; ++i)
			{
				vPos.x = float(rand() % 800);
				vPos.y = float(rand() % 600);

				iTex = rand() % 3;

				if (vPos.x < 100.f || 900.f < vPos.x)
				{
					vPos.x += 200.f;
				}

				if (vPos.y < 200.f || 600.f < vPos.y)
				{
					vPos.y += 200.f;
				}
				
				if (i == 0)
				{
					vTem0 = vPos;
					iTex0 = iTex;
				}
				if (i == 1)
				{
					vTem1 = vPos;
					iTex1 = iTex;
				}

				if (i == 1 &&
					vTem0.x == vPos.x)
				{
					vPos.x = float(rand() % 800);
				}

				if (i == 1 && vTem0.y == vPos.y)
				{
					vPos.y = float(rand() % 600);
				}

				if (i == 1 && iTex == iTex0)
				{
					iTex = rand() % 3;
				}

				if (i == 2 && vTem1.x == vPos.x)
				{
					vPos.x = float(rand() % 800);
				}

				if (i == 2 && vTem1.y == vPos.y)
				{
					vPos.y = float(rand() % 600);
				}

				if (i == 2 && iTex == iTex1)
				{
					iTex = rand() % 3;
				}

				// For.PlayerHitBlood Instance
				Engine::CGameObject* pGameObject = CPlayer_HitBlood::Create(m_pGraphicDev, vPos, iTex);
				NULL_CHECK_RETURN(pGameObject, E_FAIL);
				Engine::Get_Management()->Get_CurrentScene()->Get_Layer(L"Layer_GameLogic")->Add_GameObject(L"PlayerHitBlood", pGameObject);

			}

			CSoundMgr::GetInstance()->PlaySound(strTem.c_str(), CSoundMgr::CHANNEL_PLAYER_HIT);
		}

		if (1.5f < m_dHitDelay)
		{
			m_pStateCom->Set_State(Engine::CObjState::NOW_IDLE);
			m_pMeshCom->Set_AnimationSet(57);
			iIndex = 57;
			m_dHitDelay = 0.f;
			IsbHit = false;
		}
	}

	m_pRendererCom->Add_RenderGroup(Engine::CRenderer::RENDER_NONALPHA, this);
	
	return 0;
}

void CPlayer::Render_Object()
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

HRESULT CPlayer::Add_Component()
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
	pComponent = m_pMeshCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Clone_Resource(RESOURCE_STAGE, L"Mesh_Player"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::CComponent::ID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Mesh", pComponent));

	// For.NaviMesh
	pComponent = m_pNaviMeshCom = dynamic_cast<Engine::CNaviMesh*>(Engine::Clone_Resource(RESOURCE_STAGE, L"Mesh_Navi"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::CComponent::ID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_NaviMesh", pComponent));

	// For.BoreName
	map<wstring, _float> mapBore;
	Make_BoreNames(&mapBore);

	// For.SphereCollider
	pComponent = m_pSphereColliderCom = Engine::CSphereCollider::Create(m_pGraphicDev, m_pMeshCom->Get_RootBore(), &mapBore, &m_pTransCom->m_matWorld);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::CComponent::ID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_SphereCol", pComponent));

	// For.Collision_Square
	Make_SquareCollision();
	
	// For.Shader
	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_DefferdMesh"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::CComponent::ID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Shader", pComponent));

	// For.Shader
	//pComponent = m_pShadowCom = dynamic_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_Shade"));
	//pComponent = m_pShadowCom = dynamic_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_Mesh"));
	//NULL_CHECK_RETURN(pComponent, E_FAIL);
	//m_mapComponent[Engine::CComponent::ID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Shader", pComponent));

	// For.ObjState
	pComponent = m_pStateCom = Engine::CObjState::Create(100.f, 5.f);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::CComponent::ID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Com_State", pComponent));

	return S_OK;
}

void CPlayer::Key_Check(const Engine::_float& fTimeDelta)
{
	if (Engine::Get_DIKeyState(DIK_4) & 0x80)
	{
		multimap<const wstring, Engine::CGameObject*> multiMap = Engine::Get_Management()->Get_CurrentScene()->Get_Layer(L"Layer_GameLogic")->Get_ObjectMultiMap();

		auto iter00 = multiMap.find(L"Monster00");
		int	iSize00 = multiMap.count(L"Monster00");
		auto iter01 = multiMap.find(L"Monster01");
		int iSize01 = multiMap.count(L"Monster01");

		for (int i = 0; i < iSize00; ++i, ++iter00)
		{
			Engine::CComponent* pState = Engine::Get_ComponentIndex(L"Layer_GameLogic", L"Monster00", L"Com_State", Engine::CComponent::ID_DYNAMIC, (*iter00).second->GetIndex());

			NULL_CHECK(pState);

			dynamic_cast<Engine::CObjState*>(pState)->Set_Damge(110);
			dynamic_cast<Engine::CObjState*>(pState)->Set_State(Engine::CObjState::NOW_DEAD);
		}

		for (int i = 0; i < iSize01; ++i, ++iter01)
		{
			Engine::CComponent* pState = Engine::Get_ComponentIndex(L"Layer_GameLogic", L"Monster01", L"Com_State", Engine::CComponent::ID_DYNAMIC, (*iter01).second->GetIndex());

			NULL_CHECK(pState);

			dynamic_cast<Engine::CObjState*>(pState)->Set_Damge(110);
			dynamic_cast<Engine::CObjState*>(pState)->Set_State(Engine::CObjState::NOW_DEAD);
		}
	}

	if (IsbHit == true)
	{
		return;
	}

	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	{
		if (GetAsyncKeyState(VK_LBUTTON) & 0x8000 &&
			m_pStateCom->Get_State() == Engine::CObjState::NOW_IDLE)
		{
			CSoundMgr::GetInstance()->StopSound(CSoundMgr::CHANNEL_PLAYER);

			CSoundMgr::GetInstance()->PlaySound(L"Swing_Sound.wav", CSoundMgr::CHANNEL_EFFECT);

			m_pStateCom->Set_State(Engine::CObjState::NOW_ATTACK);

			m_pMeshCom->Set_AnimationSet(34);

			iIndex = 34;
		}

		if (m_pStateCom->Get_State() == Engine::CObjState::NOW_ATTACK)
		{
			if (iIndex == 34 && 
				50 < m_pMeshCom->Get_Percent())
			{
				CSoundMgr::GetInstance()->PlaySound(L"Swing_Sound.wav", CSoundMgr::CHANNEL_EFFECT);

				m_pMeshCom->Set_AnimationSet(32);
				iIndex = 32;
			}

			if (iIndex == 32 &&
				50 < m_pMeshCom->Get_Percent())
			{
				m_pMeshCom->Set_AnimationSet(30);
				iIndex = 30;
			}

			if (iIndex == 30 &&
				50 < m_pMeshCom->Get_Percent())
			{
				m_pMeshCom->Set_AnimationSet(28);
				iIndex = 28;
			}
		}
	}
	
	// 1타
	if (iIndex == 34 &&
		95 < m_pMeshCom->Get_Percent())
	{
		m_pStateCom->Set_State(Engine::CObjState::NOW_IDLE);

		m_pMeshCom->Set_AnimationSet(33);
		iIndex = 33;
	}

	// 2타
	if (iIndex == 32 &&
		95 < m_pMeshCom->Get_Percent())
	{
		m_pStateCom->Set_State(Engine::CObjState::NOW_IDLE);
		m_pMeshCom->Set_AnimationSet(57);
		iIndex = 57;
	}

	// 3타
	if (iIndex == 30 &&
		90 < m_pMeshCom->Get_Percent())
	{
		m_pStateCom->Set_State(Engine::CObjState::NOW_IDLE);

		m_pMeshCom->Set_AnimationSet(29);
		iIndex = 29;
	}

	// 4타
	if (iIndex == 28 &&
		80 < m_pMeshCom->Get_Percent())
	{
		m_pStateCom->Set_State(Engine::CObjState::NOW_IDLE);

		m_pMeshCom->Set_AnimationSet(27);
		iIndex = 27;
	}

	// 4타
	if (iIndex == 33 &&
		80 < m_pMeshCom->Get_Percent())
	{
		m_pStateCom->Set_State(Engine::CObjState::NOW_IDLE);
		m_pMeshCom->Set_AnimationSet(57);
		iIndex = 57;
	}

	if (iIndex == 30 &&
		15 < m_pMeshCom->Get_Percent() &&
		m_pMeshCom->Get_Percent() < 17)
	{
		CSoundMgr::GetInstance()->PlaySound(L"Swing_Sound.wav", CSoundMgr::CHANNEL_EFFECT);
	}

	if (iIndex == 28 &&
		30 < m_pMeshCom->Get_Percent() &&
		m_pMeshCom->Get_Percent() < 32)
	{
		CSoundMgr::GetInstance()->PlaySound(L"Swing_Sound.wav", CSoundMgr::CHANNEL_EFFECT);
	}


	if (m_pStateCom->Get_State() == Engine::CObjState::NOW_ATTACK)
	{
		return;
	}

	if(Engine::Get_DIKeyState(DIK_W) & 0x80)
	{
		Engine::_vec3 vPos, vDir, vDest;

		m_pTransCom->Get_Infomation(Engine::CTransform::INFO_POS, &vPos);
		m_pTransCom->Get_Infomation(Engine::CTransform::INFO_LOOK, &vDir);
		D3DXVec3Normalize(&vDir, &vDir);

		vDest = m_pNaviMeshCom->Move_OnNaviMesh(&vPos, &((vDir * -1.f) * fTimeDelta * 5.f));

		m_pTransCom->m_vInfo[Engine::CTransform::INFO_POS] = vDest;
		m_pMeshCom->Set_AnimationSet(54);

		fTimer += fTimeDelta;

		if (0.6f < fTimer)
		{
			fTimer = 0.f;
			
			// For.StepFog Instance

			srand((unsigned int)time(nullptr));

			_vec3 vTem;
			/*for (int i = 0; i < 3; ++i)
			{*/
				vTem.x = (float)rand();
				vTem.y = 0.2f;
				vTem.z = (float)rand();

				Engine::CGameObject* pGameObject = CStepFog::Create(m_pGraphicDev, &m_pTransCom->m_matWorld, &vTem);
				//Engine::CGameObject* pGameObject = CMeshStepFog::Create(m_pGraphicDev, &m_pTransCom->m_matWorld, &vTem);
				Engine::Get_Management()->Get_CurrentScene()->Get_Layer(L"Layer_Particle")->Add_GameObject(L"StepFog", pGameObject);
			//}
		}

		fRunSoundTime += fTimeDelta;

		if (0.65f < fRunSoundTime)
		{
			fRunSoundTime = 0.f;
			CSoundMgr::GetInstance()->PlaySound(L"Run_Sound.wav", CSoundMgr::CHANNEL_PLAYER);
		}

	}
	else if (90 < m_pMeshCom->Get_Percent())
	{
		m_pMeshCom->Set_AnimationSet(57);
	}

	if(Engine::Get_DIKeyState(DIK_A) & 0x80)
	{
		m_pTransCom->Rotation(Engine::CTransform::ROT_Y, D3DXToRadian(90.f * fTimeDelta * 2.f));
	}

	if(Engine::Get_DIKeyState(DIK_D) & 0x80)
	{
		m_pTransCom->Rotation(Engine::CTransform::ROT_Y, D3DXToRadian(-90.f * fTimeDelta * 2.f));
	}

	// 데미지를 주는 함수
	if (Engine::Get_DIKeyState(DIK_2) & 0x80)
	{
		m_pStateCom->Set_Damge(1.f);
	}
}

void CPlayer::Play_Animation(const Engine::_float& fTimeDelta)
{
	m_pMeshCom->Play_AnimationSet(fTimeDelta);
}

HRESULT CPlayer::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
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

	_vec4 vDir;

	memcpy(&vDir, &m_pTransCom->m_vInfo[Engine::CTransform::INFO_LOOK], sizeof(_vec3));

	vDir *= -1.f;
	vDir.w = 0.f;

	D3DXVec4Normalize(&vDir, &vDir);

	pEffect->SetVector("g_vLightDir", &vDir);
	pEffect->SetVector("g_vLightDiffuse", (_vec4*)&pLightInfo->Diffuse);
	//pEffect->SetVector("g_vLightSpecular", (_vec4*)&pLightInfo->Specular);
	pEffect->SetVector("g_vLightAmbient", (_vec4*)&pLightInfo->Ambient);


	D3DMATERIAL9			MtrlInfo;
	ZeroMemory(&MtrlInfo, sizeof(D3DMATERIAL9));

	MtrlInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	//MtrlInfo.Specular = D3DXCOLOR(0.f, 0.f, 0.f, 1.f);
	MtrlInfo.Ambient = D3DXCOLOR(0.f, 0.f, 0.f, 1.f);
	MtrlInfo.Emissive = D3DXCOLOR(0.f, 0.f, 0.f, 1.f);

	MtrlInfo.Power = 5.f;

	pEffect->SetVector("g_vMtrlDiffuse", (_vec4*)&MtrlInfo.Diffuse);
	//pEffect->SetVector("g_vMtrlSpecular", (_vec4*)&MtrlInfo.Specular);
	pEffect->SetVector("g_vMtrlAmbient", (_vec4*)&MtrlInfo.Ambient);
	pEffect->SetFloat("g_fPower", MtrlInfo.Power);

	D3DXMatrixInverse(&matView, nullptr, &matView);
	pEffect->SetVector("g_vCamPosition", (_vec4*)&matView.m[3][0]);

	Engine::SetUp_OnShader(pEffect, L"Target_Depth", "g_DepthTexture");

	Engine::Safe_Release(pEffect);

	return S_OK;
}

HRESULT CPlayer::Make_BoreNames(map<wstring, _float>* pmapBore)
{
	TCHAR szBoreName[64] = L"", szBoreSize[64] = L"";
	DWORD dwByte = 99;

	wifstream	LoadFile;

	LoadFile.open(L"../../Data/Collision_Player.txt", ios::in);

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

HRESULT CPlayer::Make_SquareCollision()
{
	Engine::CComponent* pComponent = nullptr;
	
	wfstream	pFile;
	_vec3		vPoint, vMax, vMin;
	_matrix		matPos;
	_tchar		szBore[64];
	_tchar		szSizeX[64], szSizeY[64], szSizeZ[64];
	wstring		strBore;

	pFile.open(L"../../Data/Collision_Player_Square.txt", ios::in);

	while (!pFile.eof())
	{
		pFile.getline(szBore, 64, '|');

		m_vecBoreName.push_back(szBore);
		
		if (!lstrcmp(szBore, L""))
		{
			break;
		}

		pFile.getline(szSizeX, 64, '|');
		pFile.getline(szSizeY, 64, '|');
		pFile.getline(szSizeZ, 64, '|');

		vMax = _vec3((float)_ttof(szSizeX), (float)_ttof(szSizeY), (float)_ttof(szSizeZ));

		pFile.getline(szSizeX, 64, '|');
		pFile.getline(szSizeY, 64, '|');
		pFile.getline(szSizeZ, 64);

		vMin = _vec3((float)_ttof(szSizeX), (float)_ttof(szSizeY), (float)_ttof(szSizeZ));

		char* pStr;
		int strSize = WideCharToMultiByte(CP_ACP, 0, szBore, -1, NULL, 0, NULL, NULL);
		pStr = new char[strSize];
		WideCharToMultiByte(CP_ACP, 0, szBore, -1, pStr, strSize, 0, 0);

		const Engine::D3DXFRAME_DERIVED*		pFrame = m_pMeshCom->Get_FrameByName(pStr);

		delete pStr;

		matPos = pFrame->CombinedTransformationMatrix * *m_pTransCom->Get_WorldMatrixPointer();

		memcpy(&vPoint, &matPos.m[3][0], sizeof(_vec3));

		// For.Collider
		pComponent = Engine::CCollider::Create(m_pGraphicDev, &vMin, &vMax);
		NULL_CHECK_RETURN(pComponent, E_FAIL);

		m_vecCollider.push_back(dynamic_cast<Engine::CCollider*>(pComponent));
	}

	return S_OK;
}

CPlayer* CPlayer::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPlayer*		pInstance = new CPlayer(pGraphicDev);

	if(FAILED(pInstance->Ready_Object()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CPlayer::Free()
{
	for_each(m_vecCollider.begin(), m_vecCollider.end(), Engine::CDeleteObj());

	Engine::CGameObject::Free();
}

