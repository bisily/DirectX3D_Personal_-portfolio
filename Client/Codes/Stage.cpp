#include "stdafx.h"
#include "Stage.h"

#include "Export_Function.h"

CStage::CStage(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CScene(pGraphicDev)
{
}

CStage::~CStage()
{
}

HRESULT CStage::Ready_Scene()
{
	FAILED_CHECK_RETURN(Engine::CScene::Ready_Scene(), E_FAIL);

	FAILED_CHECK_RETURN(Ready_Layer_GameLogic(L"Layer_Ground"), E_FAIL);

	FAILED_CHECK_RETURN(Ready_Layer_GameLogic(L"Layer_GameLogic"), E_FAIL);

	FAILED_CHECK_RETURN(Ready_Layer_GameLogic(L"Layer_UI"), E_FAIL);

	FAILED_CHECK_RETURN(Ready_Layer_GameLogic(L"Layer_Particle"), E_FAIL);
	
	if (FAILED(Ready_LightInfo()))
		return E_FAIL;

	CSoundMgr::GetInstance()->StopSoundAll();

	CSoundMgr::GetInstance()->PlayBGM(L"BackGround.wav");

	return S_OK;
}

int CStage::Update_Scene(const float & fTimeDelta)
{
	return Engine::CScene::Update_Scene(fTimeDelta);
}

void CStage::Render_Scene()
{
}

HRESULT CStage::Ready_Layer_GameLogic(const Engine::_tchar * pLayerTag)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject*		pGameObject = nullptr;
	Engine::CGameObject*		pCamera = nullptr;

	if (!lstrcmp(pLayerTag, L"Layer_Ground"))
	{
		LoadTerrainData(pLayer, pGameObject);
		Load_StaticObject(pLayer, pGameObject);

		m_mapLayer.insert(MAPLAYER::value_type(pLayerTag, pLayer));
	}

	else if (!lstrcmp(pLayerTag, L"Layer_GameLogic"))
	{
		// For.StaticCamera Instance
		pGameObject = CStaticCamera::Create(m_pGraphicDev,
			&Engine::_vec3(0.f, 0.f, 0.f),
			&Engine::_vec3(0.f, 0.f, 00.f),
			&Engine::_vec3(0.f, 1.f, 0.f));
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		pLayer->Add_GameObject(L"StaticCamera", pGameObject);

		// For.DynamicCamera Instance
		/*pGameObject = CDynamicCamera::Create(m_pGraphicDev,
			&Engine::_vec3(0.f, 10.f, -5.f),
			&Engine::_vec3(0.f, 0.f, 0.f),
			&Engine::_vec3(0.f, 1.f, 0.f));
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		pLayer->Add_GameObject(L"DynamicCamera", pGameObject);*/

		// For.SkyBox Instance
		pGameObject = CSkyBox::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		pLayer->Add_GameObject(L"SkyBox", pGameObject);

		// For.Player Instance
		pGameObject = CPlayer::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		pLayer->Add_GameObject(L"Player", pGameObject);

		// For.Sword Instance
		pGameObject = CPlayer_WeaPon::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		pLayer->Add_GameObject(L"Sword", pGameObject);

		// For.Boss_Sword Instance
		pGameObject = CBoss_WeaPon::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		pLayer->Add_GameObject(L"Boss_Sword", pGameObject);

		// For.Trigger Instance
		pGameObject = CTrigger::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		pLayer->Add_GameObject(L"Trigger", pGameObject);

		Load_DynamicObject(pLayer, pGameObject);

		m_mapLayer.insert(MAPLAYER::value_type(pLayerTag, pLayer));
	}

	else if (!lstrcmp(pLayerTag, L"Layer_UI"))
	{
		// UI
		// For.LifeBar Instance
		pGameObject = CLifeBar::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		pLayer->Add_GameObject(L"LifeBar", pGameObject);

		// For.Boss_Life Instance
		pGameObject = CBossLifeBar::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		pLayer->Add_GameObject(L"Boss_LifeBar", pGameObject);

		// 근접 몬스터 18마리
		// 원거리 몬스터 9마리
		for (int i = 1; i < 28; ++i)
		//for (int i = 1; i < 2; ++i)
		{
			wstring strObjKey;

			if (i < 19)
			{
				strObjKey = L"Monster00";
			}
			else
			{
				strObjKey = L"Monster01";
			}

			// For.Monster_Life Instance
			pGameObject = CMonsterLifeBar::Create(m_pGraphicDev, strObjKey, i);
			NULL_CHECK_RETURN(pGameObject, E_FAIL);
			pLayer->Add_GameObject(L"MonsterLifeBar", pGameObject);
		}

		// For.SwordTrail Player Instance
		pGameObject = CSwordTrail::Create(m_pGraphicDev, L"Player", L"Sword", L"Texture_SwordTrail00");
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		pLayer->Add_GameObject(L"SwordTrail", pGameObject);

		// For.SwordTrail Boss Instance
		pGameObject = CSwordTrail::Create(m_pGraphicDev, L"Boss", L"Boss_Sword", L"Texture_SwordTrail07");
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		pLayer->Add_GameObject(L"SwordTrail", pGameObject);

		m_mapLayer.insert(MAPLAYER::value_type(pLayerTag, pLayer));
	}
	else if (!lstrcmp(pLayerTag, L"Layer_Particle"))
	{
		m_mapLayer.insert(MAPLAYER::value_type(pLayerTag, pLayer));
	}

	

	return S_OK;
}

HRESULT CStage::SetUp_DefaultSetting()
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);

	m_pGraphicDev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pGraphicDev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

	return S_OK;
}

HRESULT CStage::Ready_LightInfo()
{
	D3DLIGHT9		LightInfo;
	ZeroMemory(&LightInfo, sizeof(D3DLIGHT9));

	LightInfo.Type = D3DLIGHT_DIRECTIONAL;
	LightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	LightInfo.Specular = D3DXCOLOR(0.f, 0.f, 0.f, 1.f);
	LightInfo.Ambient = D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.f);
	LightInfo.Direction = _vec3(1.f, -1.f, 1.f);

	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, 0)))
		return E_FAIL;

	return S_OK;
}

HRESULT CStage::Load_StaticObject(Engine::CLayer * pLayer, Engine::CGameObject * pGameObject)
{
	HANDLE hFile = CreateFile(L"../../Data/StaticMesh.dat", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	Engine::_vec3 temp_Right, temp_Up, temp_Look, temp_Pos;
	Engine::_vec3 temp_Scale, temp_Angle;
	TCHAR MeshName[64] = L"", Index[64] = L"";

	DWORD dwByte = 99;

	while (true)
	{
		ReadFile(hFile, temp_Right, sizeof(D3DXVECTOR3), &dwByte, NULL);
		
		if (dwByte == 0)
		{
			break;
		}

		ReadFile(hFile, temp_Up, sizeof(D3DXVECTOR3), &dwByte, NULL);
		ReadFile(hFile, temp_Look, sizeof(D3DXVECTOR3), &dwByte, NULL);
		ReadFile(hFile, temp_Pos, sizeof(D3DXVECTOR3), &dwByte, NULL);

		ReadFile(hFile, temp_Scale, sizeof(D3DXVECTOR3), &dwByte, NULL);

		ReadFile(hFile, temp_Angle, sizeof(D3DXVECTOR3), &dwByte, NULL);

		ReadFile(hFile, MeshName, MIN_STR, &dwByte, NULL);

		ReadFile(hFile, Index, MIN_STR, &dwByte, NULL);

		pGameObject = CStaticObject::Create(m_pGraphicDev,
											MeshName,
											&temp_Right,
											&temp_Up,
											&temp_Look,
											&temp_Pos,
											&temp_Scale,
											&temp_Angle,
											_tstoi(Index));
	
		pLayer->Add_GameObject(MeshName, pGameObject);
	}

	CloseHandle(hFile);

	m_mapLayer.insert(MAPLAYER::value_type(L"Layer_Ground", pLayer));

	return S_OK;
}

HRESULT CStage::Load_DynamicObject(Engine::CLayer * pLayer, Engine::CGameObject * pGameObject)
{
	HANDLE	hFile = CreateFile(L"../../Data/DynamicMesh.dat", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	Engine::_vec3 temp_Right, temp_Up, temp_Look, temp_Pos;
	Engine::_vec3 temp_Scale, temp_Angle;
	TCHAR MeshName[MIN_STR] = L"", Index[MIN_STR] = L"";
	DWORD dwByte = 99;
	int i = 0;
	while (true)
	{
		ReadFile(hFile, temp_Right, sizeof(D3DXVECTOR3), &dwByte, NULL);
		i++;
		if (dwByte == 0)
		{
			break;
		}

		ReadFile(hFile, temp_Up, sizeof(D3DXVECTOR3), &dwByte, NULL);
		ReadFile(hFile, temp_Look, sizeof(D3DXVECTOR3), &dwByte, NULL);
		ReadFile(hFile, temp_Pos, sizeof(D3DXVECTOR3), &dwByte, NULL);

		ReadFile(hFile, temp_Scale, sizeof(D3DXVECTOR3), &dwByte, NULL);

		ReadFile(hFile, temp_Angle, sizeof(D3DXVECTOR3), &dwByte, NULL);

		ReadFile(hFile, MeshName, MIN_STR, &dwByte, NULL);

		ReadFile(hFile, Index, MIN_STR, &dwByte, NULL);

		if (!lstrcmp(MeshName, L"Mesh_Boss"))
		{
			lstrcat(MeshName, Index);

			// For.Boss Instance
			pGameObject = CBoss::Create(m_pGraphicDev, MeshName, &temp_Right, &temp_Up, &temp_Look, &temp_Pos, &temp_Scale, &temp_Angle, 0);
			NULL_CHECK_RETURN(pGameObject, E_FAIL);
			pLayer->Add_GameObject(L"Boss", pGameObject);
		}
		
		else if (!lstrcmp(MeshName, L"Mesh_Monster00"))
		{
			// For.Monster00 Instance
			pGameObject = CMonster00::Create(m_pGraphicDev, MeshName, &temp_Right, &temp_Up, &temp_Look, &temp_Pos, &temp_Scale, &temp_Angle, _ttoi(Index));
			NULL_CHECK_RETURN(pGameObject, E_FAIL);
			pLayer->Add_GameObject(L"Monster00", pGameObject);

			// For.Monster00Sword Instance
			pGameObject = CMonster00Sword::Create(m_pGraphicDev, L"Monster00", _ttoi(Index));
			NULL_CHECK_RETURN(pGameObject, E_FAIL);
			pLayer->Add_GameObject(L"Monster00Sword", pGameObject);
			
		}

		else if (!lstrcmp(MeshName, L"Mesh_Monster01"))
		{
			// For.Monster01 Instance
			pGameObject = CMonster01::Create(m_pGraphicDev, MeshName, &temp_Right, &temp_Up, &temp_Look, &temp_Pos, &temp_Scale, &temp_Angle, _ttoi(Index));
			NULL_CHECK_RETURN(pGameObject, E_FAIL);
			pLayer->Add_GameObject(L"Monster01", pGameObject);

			// For.Monster01Arrow Instance
			pGameObject = CMonster01Bow::Create(m_pGraphicDev, L"Monster01", _ttoi(Index));
			NULL_CHECK_RETURN(pGameObject, E_FAIL);
			pLayer->Add_GameObject(L"Monster01Bow", pGameObject);
		}
	}

	CloseHandle(hFile);

	return S_OK;
}

HRESULT CStage::LoadTerrainData(Engine::CLayer * pLayer, Engine::CGameObject * pGameObject)
{	
	HANDLE hFile = CreateFile(L"../../Data/Terrain.dat", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	int iTerrainX, iTerrainZ, iTerrainITV, iTerrainDetail;
	float fTerrainPosZ;

	DWORD dwByte = 0;

	ReadFile(hFile, &iTerrainX, sizeof(int), &dwByte, NULL);
	ReadFile(hFile, &iTerrainZ, sizeof(int), &dwByte, NULL);
	ReadFile(hFile, &iTerrainITV, sizeof(int), &dwByte, NULL);
	ReadFile(hFile, &iTerrainDetail, sizeof(int), &dwByte, NULL);
	ReadFile(hFile, &fTerrainPosZ, sizeof(float), &dwByte, NULL);
	
	CloseHandle(hFile);

	// Terrain 정점 로딩
	if (FAILED(Engine::Ready_Buffers(m_pGraphicDev,
										RESOURCE_STATIC,
										L"Buffer_Terrain",
										Engine::CVIBuffer::BUFFER_TERRAINTEX,
										0,
										iTerrainX,
										iTerrainZ,
										iTerrainITV,
										iTerrainDetail)))
	{
		ERR_BOX(L"Buffer_Terrain, Create Failed");
		return E_FAIL;
	}

	Engine::CComponent*		pInstance = nullptr;

	//Optimization
	pInstance = Engine::COptimization::Create(m_pGraphicDev, iTerrainX, iTerrainZ);
	NULL_CHECK_RETURN(pInstance, E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Prototype(L"Proto_Optimization", pInstance), E_FAIL);

	// For.Terrain Instance
	pGameObject = CTerrain::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->Add_GameObject(L"Terrain", pGameObject);

	dynamic_cast<Engine::CTransform*>(pLayer->Get_Component(L"Terrain", L"Com_Transform", Engine::CComponent::ID_DYNAMIC))->m_vInfo[Engine::CTransform::INFO_POS].z = fTerrainPosZ;


	return S_OK;
}

CStage * CStage::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CStage*	pInstance = new CStage(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
	{
		Engine::Safe_Release(pInstance);
	}

	return pInstance;
}

void CStage::Free()
{
	Engine::CScene::Free();
}
