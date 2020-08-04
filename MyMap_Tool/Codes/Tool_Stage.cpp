#include "stdafx.h"
#include "Tool_Stage.h"

#include "Export_Function.h"

CTool_Stage::CTool_Stage(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CScene(pGraphicDev)
{
}

CTool_Stage::~CTool_Stage()
{
}

HRESULT CTool_Stage::Ready_Scene(void)
{
	if (FAILED(Engine::CScene::Ready_Scene()))
		return E_FAIL;

	if (FAILED(SetUp_DefaultSetting()))
		return E_FAIL;

	if (FAILED(Ready_Layer_GameLogic(L"Layer_GameLogic")))
		return E_FAIL;

	if (FAILED(Ready_LightInfo()))
		return E_FAIL;

	return S_OK;
}

int CTool_Stage::Update_Scene(const float & fTimeDelta)
{
	return  Engine::CScene::Update_Scene(fTimeDelta);
}

void CTool_Stage::Render_Scene(void)
{
	if (m_bWireFrame == true)
	{
		m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	}
	else
	{
		m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	}

}

void CTool_Stage::Add_GameObject(wstring _pLayerTag
									, wstring _tagName
									, Engine::CComponent::COMPONENTID _eID
									, int _iIndex
									, Engine::_vec3 _vPos
									, Engine::_vec3 _vScale)
{
	

	Engine::CLayer* pLayer = Engine::Get_Management()->Get_CurrentScene()->Get_Layer(_pLayerTag.c_str());
	NULL_CHECK(pLayer);

	Engine::CGameObject* pGameObject = nullptr;

	switch (_eID)
	{
	case Engine::CComponent::ID_DYNAMIC:

		if (!_tagName.compare(L"Mesh_Monster00"))
		{
			pGameObject = CTool_Monster00::Create(m_pGraphicDev, _tagName, _iIndex, _vPos, _vScale);
		}

		if (!_tagName.compare(L"Mesh_Monster01"))
		{
			pGameObject = CTool_Monster01::Create(m_pGraphicDev, _tagName, _iIndex, _vPos, _vScale);
		}

		if (!_tagName.compare(L"Mesh_Boss"))
		{
			pGameObject = CTool_Boss::Create(m_pGraphicDev, _tagName, _iIndex, _vPos, _vScale);
		}
		break;

	case Engine::CComponent::ID_STATIC:
		pGameObject = CTool_StaticMesh::Create(m_pGraphicDev, _tagName.c_str(), _iIndex, _vPos, _vScale);
		break;
	}	
	
	NULL_CHECK(pGameObject);

	pLayer->Add_GameObject(_tagName.c_str(), pGameObject);

	m_mapLayer.insert(MAPLAYER::value_type(_pLayerTag.c_str(), pLayer));
}

void CTool_Stage::Load_GameObject(wstring _pLayerTag,
									wstring _tagName,
									Engine::CComponent::COMPONENTID _eID,
									Engine::_vec3 * _temp_Right, 
									Engine::_vec3 * _temp_Up, 
									Engine::_vec3 * _temp_Look, 
									Engine::_vec3 * _temp_Pos, 
									Engine::_vec3 * _temp_Scale, 
									Engine::_vec3 * _temp_Angle, 
									int _iObjIndex)
{
	Engine::CLayer* pLayer = Engine::Get_Management()->Get_CurrentScene()->Get_Layer(_pLayerTag.c_str());
	NULL_CHECK(pLayer);

	Engine::CGameObject* pGameObject = nullptr;

	switch (_eID)
	{
	case Engine::CComponent::ID_DYNAMIC:

		if (!_tagName.compare(L"Mesh_Monster00"))
		{
			pGameObject = CTool_Monster00::CreateLoad(m_pGraphicDev,
													_tagName.c_str(),
													_temp_Right,
													_temp_Up,
													_temp_Look,
													_temp_Pos,
													_temp_Scale,
													_temp_Angle,
													_iObjIndex);
		}

		if (!_tagName.compare(L"Mesh_Monster01"))
		{
			pGameObject = CTool_Monster01::CreateLoad(m_pGraphicDev,
													_tagName.c_str(),
													_temp_Right,
													_temp_Up,
													_temp_Look,
													_temp_Pos,
													_temp_Scale,
													_temp_Angle,
													_iObjIndex);
		}

		if (!_tagName.compare(L"Mesh_Boss"))
		{
			pGameObject = CTool_Boss::CreateLoad(m_pGraphicDev,
												_tagName.c_str(),
												_temp_Right,
												_temp_Up,
												_temp_Look,
												_temp_Pos,
												_temp_Scale,
												_temp_Angle,
												_iObjIndex);
		}
		break;

	case Engine::CComponent::ID_STATIC:
		pGameObject = CTool_StaticMesh::CreateLoad(m_pGraphicDev, 
													_tagName.c_str(),
													_temp_Right,
													_temp_Up,
													_temp_Look,
													_temp_Pos,
													_temp_Scale,
													_temp_Angle,
													_iObjIndex);
		break;
	}

	NULL_CHECK(pGameObject);

	pLayer->Add_GameObject(_tagName.c_str(), pGameObject);

	m_mapLayer.insert(MAPLAYER::value_type(_pLayerTag.c_str(), pLayer));

}

HRESULT CTool_Stage::Ready_Layer_GameLogic(const Engine::_tchar * pLayerTag)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject*		pGameObject = nullptr;

	// For.Terrain Instance
	pGameObject = CTerrain::Create(m_pGraphicDev);

	if (nullptr == pGameObject)
		goto		except;

	if (FAILED(pLayer->Add_GameObject(L"Terrain", pGameObject)))
		goto		except;

	// For.DynamicCamera Instance
	pGameObject = CDynamicCamera::Create(m_pGraphicDev,
		&Engine::_vec3(0.f, 10.f, -5.f),
		&Engine::_vec3(0.f, 0.f, 0.f),
		&Engine::_vec3(0.f, 1.f, 0.f));

	if (nullptr == pGameObject)
		goto		except;

	if (FAILED(pLayer->Add_GameObject(L"DynamicCamera", pGameObject)))
		goto		except;

	//// For.SkyBox Instance
	//pGameObject = CSkyBox::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//pLayer->Add_GameObject(L"SkyBox", pGameObject);

	// For.Box Instance
	pGameObject = CBox::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->Add_GameObject(L"Box_Mesh", pGameObject);

	//// For.Player Instance
	//pGameObject = CPlayer::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//pLayer->Add_GameObject(L"Player", pGameObject);

	//// For.Monster00 Instance
	//pGameObject = CTool_Monster00::Create(m_pGraphicDev, L"Mesh_Monster00");
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//pLayer->Add_GameObject(L"Monster00", pGameObject);

	//// For.Monster01 Instance
	//pGameObject = CTool_Monster01::Create(m_pGraphicDev, L"Mesh_Monster0119", 1, _vec3(2.f, 0.f, 2.f));
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//pLayer->Add_GameObject(L"Monster01", pGameObject);

	// For.Boss Instance
	pGameObject = CTool_Boss::Create(m_pGraphicDev, L"Mesh_Boss", 0);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->Add_GameObject(L"Boss", pGameObject);

	//// For.Tool_Boss_WeaPon
	//pGameObject = CTool_Boss_WeaPon::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//pLayer->Add_GameObject(L"Boss_WeaPon", pGameObject);
	
	// For.Navi Instance
	pGameObject = CNavi::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->Add_GameObject(L"Navi_Mesh", pGameObject);
		
	//// For.Sword Instance
	//pGameObject = CPlayer_WeaPon::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//pLayer->Add_GameObject(L"Player_WeaPon", pGameObject);

	//// For.Arrow Instance
	//pGameObject = CMonster01Arrow::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//pLayer->Add_GameObject(L"Arrow", pGameObject);

	//// For.Monster00Sword Instance
	//pGameObject = CMonster00Sword::Create(m_pGraphicDev, L"Monster00", 0);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//pLayer->Add_GameObject(L"Monster00Sword", pGameObject);

	//// For.Tool_Boss_Wave Instance
	//pGameObject = CTool_Boss_Wave::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//pLayer->Add_GameObject(L"Wave", pGameObject);

	m_mapLayer.insert(MAPLAYER::value_type(pLayerTag, pLayer));

	return S_OK;

except:
	Engine::Safe_Release(pLayer);
	return S_OK;
}

HRESULT CTool_Stage::SetUp_DefaultSetting(void)
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);

	m_pGraphicDev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pGraphicDev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

	return S_OK;
}

HRESULT CTool_Stage::Ready_LightInfo(void)
{
	D3DLIGHT9		LightInfo;
	ZeroMemory(&LightInfo, sizeof(D3DLIGHT9));

	LightInfo.Type = D3DLIGHT_DIRECTIONAL;
	// ³­¹Ý»ç±¤ ¢Ø ¡è ¢Ö
	LightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	// ¹Ý»ç±¤ ¢Ù ¢Ö
	LightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	// È¯°æ±¤ 
	LightInfo.Ambient = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.f);
	// ¹æÇâ º¤ÅÍ
	LightInfo.Direction = Engine::_vec3(1.f, -1.f, 1.f);

	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, 0)))
		return E_FAIL;

	return S_OK;
}

CTool_Stage * CTool_Stage::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTool_Stage*	pInstance = new CTool_Stage(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
	{
		Engine::Safe_Release(pInstance);
	}

	return pInstance;
}

void CTool_Stage::Free(void)
{
	Engine::CScene::Free();
}
