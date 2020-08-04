#include "stdafx.h"
#include "Tool_Monster00.h"

#include "Export_Function.h"

CTool_Monster00::CTool_Monster00(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{

}

CTool_Monster00::~CTool_Monster00(void)
{

}

HRESULT CTool_Monster00::Ready_Object(wstring _tagMeshName
										, int _iObjIndex
										, Engine::_vec3 _vPosition
										, Engine::_vec3 _vScale)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	//m_pMeshCom->Set_AnimationSet(0);

	m_pTransCom->Set_Position(_vPosition);
	m_pTransCom->Set_Scale(_vScale.x, _vScale.y, _vScale.z);

	m_bWireFrame = FALSE;
	m_iObjIndex = _iObjIndex;
	m_strMeshName = _tagMeshName;

	return S_OK;
}

HRESULT CTool_Monster00::Ready_Load_Object(wstring _tagMeshName
											, Engine::_vec3 * _temp_Right
											, Engine::_vec3 * _temp_Up
											, Engine::_vec3 * _temp_Look
											, Engine::_vec3 * _temp_Pos
											, Engine::_vec3 * _temp_Scale
											, Engine::_vec3 * _temp_Angle
											, int _iObjIndex)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_pTransCom->m_vInfo[0] = *_temp_Right;
	m_pTransCom->m_vInfo[1] = *_temp_Up;
	m_pTransCom->m_vInfo[2] = *_temp_Look;
	m_pTransCom->m_vInfo[3] = *_temp_Pos;

	m_pTransCom->Set_Scale(_temp_Scale->x, _temp_Scale->y, _temp_Scale->z);
	m_pTransCom->m_vAngle = *_temp_Angle;

	m_bWireFrame = true;
	m_iObjIndex = _iObjIndex;
	m_strMeshName = _tagMeshName;

	return S_OK;
}

Engine::_int CTool_Monster00::Update_Object(const Engine::_float& fTimeDelta)
{
	Engine::_uint iExit = Engine::CGameObject::Update_Object(fTimeDelta);
	
	if (!bPlayAnimation)
	{
		if (m_pMeshCom->Is_AnimationSetEnd() != false)
		{
			m_pMeshCom->Play_AnimationSet(fTimeDelta);
		}
	}

	else if (bPlayAnimation)
	{
		m_pMeshCom->Play_AnimationSet(0.f);
	}

	m_pRendererCom->Add_RenderGroup(Engine::CRenderer::RENDER_NONALPHA, this);

	return iExit;
}

void CTool_Monster00::Render_Object(void)
{
	if (m_bWireFrame == true)
	{
		m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	}
	else
	{
		m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	}

	m_pTransCom->SetTransform(m_pGraphicDev);

	m_pMeshCom->Render_Mesh();

	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}

void CTool_Monster00::SetUp_Animation(Engine::_int iIndex)
{
	m_pMeshCom->Set_AnimationSet(iIndex);
}

HRESULT CTool_Monster00::Add_Component(void)
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
	pComponent = m_pMeshCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Clone_Resource(RESOURCE_STAGE, L"Mesh_Monster001"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::CComponent::ID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Mesh", pComponent));

	//// For.Collider
	//pComponent = m_pColliderCom = Engine::CCollider::Create(m_pGraphicDev,
	//	m_pMeshCom->Get_VtxPos(),
	//	m_pMeshCom->Get_Numvertices(),
	//	m_pMeshCom->Get_Stride());

	//NULL_CHECK_RETURN(pComponent, E_FAIL);
	//m_mapComponent[Engine::CComponent::ID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Collider", pComponent));

	return S_OK;
}

void CTool_Monster00::SetUp_OnTerrain(void)
{
	Engine::_vec3		vPosition;

	m_pTransCom->Get_Infomation(Engine::CTransform::INFO_POS, &vPosition);

	Engine::CTerrainTex*	pTerrainBufferCom = dynamic_cast<Engine::CTerrainTex*>
		(Engine::Get_Component(L"Layer_GameLogic",
			L"Terrain",
			L"Com_Buffer",
			Engine::CComponent::ID_STATIC));


	NULL_CHECK(pTerrainBufferCom);

	Engine::_float		fHeight = m_pCalculatorCom->Compute_HeightOnTerrain(&vPosition, pTerrainBufferCom->Get_VtxPos(), 129, 129);

	m_pTransCom->Set_Position(vPosition.x, fHeight, vPosition.z);


}

Engine::_vec3 CTool_Monster00::PickUp_OnTerrain(void)
{
	Engine::CTerrainTex*		pTerrainBufferCom = dynamic_cast<Engine::CTerrainTex*>
		(Engine::Get_Component(L"Layer_GameLogic", L"Terrain", L"Com_Buffer", Engine::CComponent::ID_STATIC));
	NULL_CHECK_RETURN(pTerrainBufferCom, Engine::_vec3(0.f, 0.f, 0.f));

	Engine::CTransform*		pTerrainTransCom = dynamic_cast<Engine::CTransform*>
		(Engine::Get_Component(L"Layer_GameLogic", L"Terrain", L"Com_Transform", Engine::CComponent::ID_DYNAMIC));
	NULL_CHECK_RETURN(pTerrainTransCom, Engine::_vec3(0.f, 0.f, 0.f));

	return m_pCalculatorCom->Picking_OnTerrain(g_hWnd, pTerrainBufferCom, pTerrainTransCom);

}

CTool_Monster00* CTool_Monster00::Create(LPDIRECT3DDEVICE9 pGraphicDev
											, wstring _tagMeshName
											, int _iObjIndex
											, Engine::_vec3 vPosition
											, Engine::_vec3 vScale)
{
	CTool_Monster00*		pInstance = new CTool_Monster00(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(_tagMeshName, _iObjIndex, vPosition, vScale)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

CTool_Monster00 * CTool_Monster00::CreateLoad(LPDIRECT3DDEVICE9 _pGraphicDev, wstring _tagMeshName, Engine::_vec3 * _temp_Right, Engine::_vec3 * _temp_Up, Engine::_vec3 * _temp_Look, Engine::_vec3 * _temp_Pos, Engine::_vec3 * _temp_Scale, Engine::_vec3 * _temp_Angle, int _iObjIndex)
{
	CTool_Monster00*		pInstance = new CTool_Monster00(_pGraphicDev);

	if (FAILED(pInstance->Ready_Load_Object(_tagMeshName, _temp_Right, _temp_Up, _temp_Look, _temp_Pos, _temp_Scale, _temp_Angle, _iObjIndex)))
	{
		Engine::Safe_Release(pInstance);
		ERR_BOX(L"Tool_DynamicMesh, Create Load Failed");
	}

	return pInstance;
}

void CTool_Monster00::Free(void)
{
	Engine::CGameObject::Free();
}

