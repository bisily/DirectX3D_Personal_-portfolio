#include "stdafx.h"
#include "Player.h"

#include "Export_Function.h"

CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)	
{

}

CPlayer::~CPlayer(void)
{
	
}

HRESULT CPlayer::Ready_Object(void)
{
	if(FAILED(Add_Component()))
		return E_FAIL;

	m_pMeshCom->Set_AnimationSet(57);

	m_pTransCom->Set_Scale(0.01f, 0.01f, 0.01f);
	
	return S_OK;
}

Engine::_int CPlayer::Update_Object(const Engine::_float& fTimeDelta)
{
	Engine::_uint iExit = Engine::CGameObject::Update_Object(fTimeDelta);
		
	SetUp_OnTerrain();
	
	if (!bPlayAnimation)
	{
		Key_Check(fTimeDelta);
	}

	if (!bPlayAnimation)
	{
		if (m_pMeshCom->Is_AnimationSetEnd() != false)
		{
			m_pMeshCom->Play_AnimationSet(fTimeDelta);
		}
	}

	else if(bPlayAnimation)
	{
		if (m_pMeshCom->Is_AnimationSetEnd())
		{
			const Engine::D3DXFRAME_DERIVED* pFrame = m_pMeshCom->Get_FrameByName("BODY1");

			Engine::_matrix matMovePos = pFrame->TransformationMatrix;
			Engine::_matrix matTest = matMovePos * m_pTransCom->m_matWorld;
		}

		if (m_fPlayrSpeed == 0.f)
		{
			m_pMeshCom->Play_AnimationSet(fTimeDelta);
		}
		else
		{
			m_pMeshCom->Play_AnimationSet(m_fPlayrSpeed);
		}
	}

	const Engine::D3DXFRAME_DERIVED* pFrame = m_pMeshCom->Get_FrameByName("BODY1");

	m_pRendererCom->Add_RenderGroup(Engine::CRenderer::RENDER_NONALPHA, this);
	
	return iExit;
}

void CPlayer::Render_Object(void)
{
	m_pTransCom->SetTransform(m_pGraphicDev);

	m_pMeshCom->Render_Mesh();
}

void CPlayer::SetUp_Animation(Engine::_int iIndex)
{
	m_pMeshCom->Set_AnimationSet(iIndex);
}

HRESULT CPlayer::Add_Component(void)
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

	//// For.Collider
	//pComponent = m_pColliderCom = Engine::CCollider::Create(m_pGraphicDev,
	//	m_pMeshCom->Get_VtxPos(),
	//	m_pMeshCom->Get_Numvertices(),
	//	m_pMeshCom->Get_Stride());

	//NULL_CHECK_RETURN(pComponent, E_FAIL);
	//m_mapComponent[Engine::CComponent::ID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Collider", pComponent));

	return S_OK;
}

void CPlayer::Key_Check(const Engine::_float& fTimeDelta)
{
	
	if(Engine::Get_DIKeyState(DIK_UP) & 0x80)
	{
		Engine::_vec3 vPos, vDir, vDest;

		m_pTransCom->Get_Infomation(Engine::CTransform::INFO_POS, &vPos);
		m_pTransCom->Get_Infomation(Engine::CTransform::INFO_LOOK, &vDir);
		D3DXVec3Normalize(&vDir, &vDir);

		vDest = m_pNaviMeshCom->Move_OnNaviMesh(&vPos, &((vDir * -1.f) * fTimeDelta * 3.f));

		m_pTransCom->m_vInfo[Engine::CTransform::INFO_POS] = vDest;
		m_pMeshCom->Set_AnimationSet(54);
	}
	else
		m_pMeshCom->Set_AnimationSet(57);

	if(Engine::Get_DIKeyState(DIK_DOWN) & 0x80)
	{
		m_pTransCom->Move_Forward(-5.f, fTimeDelta);
		m_pMeshCom->Set_AnimationSet(35);
	}
	else
		m_pMeshCom->Set_AnimationSet(57);

	if(Engine::Get_DIKeyState(DIK_LEFT) & 0x80)
	{
		m_pTransCom->Rotation(Engine::CTransform::ROT_Y, D3DXToRadian(90.f * fTimeDelta));
	}

	if(Engine::Get_DIKeyState(DIK_RIGHT) & 0x80)
	{
		m_pTransCom->Rotation(Engine::CTransform::ROT_Y, D3DXToRadian(-90.f * fTimeDelta));
	}

	//if(Engine::Get_DIMouseState(Engine::CDXInput::DIM_LB) & 0x80)
	//{
	//	Engine::_vec3		vPickPos = PickUp_OnTerrain();

	//	m_pTransCom->Move_TargetPos(&vPickPos, 5.f, fTimeDelta);
	//}
}

void CPlayer::SetUp_OnTerrain(void)
{
	Engine::_vec3		vPosition;

	m_pTransCom->Get_Infomation(Engine::CTransform::INFO_POS, &vPosition);

	Engine::CTerrainTex*	pTerrainBufferCom = dynamic_cast<Engine::CTerrainTex*>
												(Engine::Get_Component(L"Layer_GameLogic", 
												L"Terrain",
												L"Com_Buffer", 
												Engine::CComponent::ID_STATIC));
		

	NULL_CHECK(pTerrainBufferCom);
	
	Engine::_float		fHeight = m_pCalculatorCom->Compute_HeightOnTerrain(&vPosition, pTerrainBufferCom->Get_VtxPos(), pTerrainBufferCom->Get_VtxCntX(), pTerrainBufferCom->Get_VtxCntZ());

	m_pTransCom->Set_Position(vPosition.x, fHeight, vPosition.z);


}

Engine::_vec3 CPlayer::PickUp_OnTerrain(void)
{
	Engine::CTerrainTex*		pTerrainBufferCom = dynamic_cast<Engine::CTerrainTex*>
		(Engine::Get_Component(L"Layer_GameLogic", L"Terrain", L"Com_Buffer", Engine::CComponent::ID_STATIC));
	NULL_CHECK_RETURN(pTerrainBufferCom, Engine::_vec3(0.f, 0.f, 0.f));

	Engine::CTransform*		pTerrainTransCom = dynamic_cast<Engine::CTransform*>
		(Engine::Get_Component(L"Layer_GameLogic", L"Terrain", L"Com_Transform", Engine::CComponent::ID_DYNAMIC));
	NULL_CHECK_RETURN(pTerrainTransCom, Engine::_vec3(0.f, 0.f, 0.f));

	return m_pCalculatorCom->Picking_OnTerrain(g_hWnd, pTerrainBufferCom, pTerrainTransCom);

}

CPlayer* CPlayer::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPlayer*		pInstance = new CPlayer(pGraphicDev);

	if(FAILED(pInstance->Ready_Object()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CPlayer::Free(void)
{
	Engine::CGameObject::Free();
}

