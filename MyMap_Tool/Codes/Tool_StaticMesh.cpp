#include "stdafx.h"
#include "Tool_StaticMesh.h"

#include "Export_Function.h"

CTool_StaticMesh::CTool_StaticMesh(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CTool_StaticMesh::~CTool_StaticMesh(void)
{
}

HRESULT CTool_StaticMesh::Ready_Object(const Engine::_tchar* _tagMeshName
										, int _iIndex
										, Engine::_vec3 _vPosition
										, Engine::_vec3 _vScale)
{
	if (FAILED(Add_Component(_tagMeshName)))
	{
		ERR_BOX(L"Tool_StaticMesh, Add_Component Failed");
		return E_FAIL;
	}

	m_pTransCom->Set_Position(_vPosition);
	m_pTransCom->Set_Scale(_vScale.x, _vScale.y, _vScale.z);
	m_bWireFrame = true;
	m_iObjIndex = _iIndex;
	m_strMeshName = _tagMeshName;

	return S_OK;
}

HRESULT CTool_StaticMesh::Ready_Load_Object(wstring _tagMeshName
											, Engine::_vec3 * _temp_Right
											, Engine::_vec3 * _temp_Up
											, Engine::_vec3 * _temp_Look
											, Engine::_vec3 * _temp_Pos
											, Engine::_vec3 * _temp_Scale
											, Engine::_vec3 * _temp_Angle
											, int _iObjIndex)
{
	if (FAILED(Add_Component(_tagMeshName.c_str())))
	{
		ERR_BOX(L"Tool_StaticMesh, Load Add_Component Failed");
		return E_FAIL;
	}

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

Engine::_int CTool_StaticMesh::Update_Object(const Engine::_float & fTimeDelta)
{
	Engine::_int iExit = Engine::CGameObject::Update_Object(fTimeDelta);
	
	m_pRendererCom->Add_RenderGroup(Engine::CRenderer::RENDER_NONALPHA, this);

	return iExit;
}

void CTool_StaticMesh::Render_Object(void)
{
	if (m_bWireFrame == TRUE)
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

HRESULT CTool_StaticMesh::Add_Component(const Engine::_tchar* tagMeshName)
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
	pComponent = m_pMeshCom = dynamic_cast<Engine::CStaticMesh*>(Engine::Clone_Resource(RESOURCE_STAGE, tagMeshName));
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

CTool_StaticMesh * CTool_StaticMesh::Create(LPDIRECT3DDEVICE9 _pGraphicDev
											, const Engine::_tchar* _tagMeshName
											, int _iIndex
											, Engine::_vec3 _vPosition
											, Engine::_vec3 _vScale)
{
	CTool_StaticMesh* pInstance = new CTool_StaticMesh(_pGraphicDev);

	if (FAILED(pInstance->Ready_Object(_tagMeshName, _iIndex, _vPosition, _vScale)))
	{
		Engine::Safe_Release(pInstance);
		ERR_BOX(L"Tool_StaticMesh, Create Failed");
	}

	return pInstance;
}

CTool_StaticMesh * CTool_StaticMesh::CreateLoad(LPDIRECT3DDEVICE9 _pGraphicDev
												, wstring _tagMeshName
												, Engine::_vec3 * _temp_Right
												, Engine::_vec3 * _temp_Up
												, Engine::_vec3 * _temp_Look 
												, Engine::_vec3 * _temp_Pos
												, Engine::_vec3 * _temp_Scale 
												, Engine::_vec3 * _temp_Angle 
												, int _iObjIndex)
{
	CTool_StaticMesh* pInstance = new CTool_StaticMesh(_pGraphicDev);

	if (FAILED(pInstance->Ready_Load_Object(_tagMeshName, _temp_Right, _temp_Up, _temp_Look, _temp_Pos, _temp_Scale, _temp_Angle, _iObjIndex)))
	{
		Engine::Safe_Release(pInstance);
		ERR_BOX(L"Tool_StaticMesh, Create Load Failed");
	}

	return pInstance;
}

void CTool_StaticMesh::Free(void)
{
	Engine::CGameObject::Free();
}
