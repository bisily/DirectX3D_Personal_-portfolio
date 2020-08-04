#include "stdafx.h"
#include "CollisionSphere.h"

#include "Export_Function.h"

#include "../Default/MainFrm.h"
#include "../Default/MyMap_ToolView.h"

CCollisionSphere::CCollisionSphere(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CCollisionSphere::~CCollisionSphere(void)
{
	Free();
}

HRESULT CCollisionSphere::Ready_Object(D3DXVECTOR3* pVecPos, wstring strParentName, wstring strBoreName, int iIndex)
{
	if (FAILED(Add_Component()))
	{
		ERR_BOX(L"Navi, Add_Component Failed");
		return E_FAIL;
	}

	m_pGraphicDev->CreateTexture(1, 1, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &m_pTexture, NULL);

	D3DLOCKED_RECT			LockRect;
	ZeroMemory(&LockRect, sizeof(D3DLOCKED_RECT));

	m_pTexture->LockRect(0, &LockRect, NULL, 0);

	*((Engine::_ulong*)LockRect.pBits) = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);
	m_pTexture->UnlockRect(0);	
	
	// 구 생성 버퍼
	D3DXCreateSphere(m_pGraphicDev, 0.2f, 10, 10, &m_pMesh, NULL);

	Engine::_ulong dwFvF = m_pMesh->GetFVF();
	m_pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&m_pVtxPos);

	D3DXComputeBoundingSphere(m_pVtxPos, m_pMesh->GetNumVertices(), D3DXGetFVFVertexSize(dwFvF), &m_vPos, &m_fRadius);
	m_pMesh->UnlockVertexBuffer();

	m_vPos = *pVecPos;

	m_strParentName = strParentName;

	m_strBoreName = strBoreName;
	
	m_iIndex = iIndex;
	m_fRadius = 0.2f;

	return S_OK;
}

Engine::_int CCollisionSphere::Update_Object(const Engine::_float & fTimeDelta)
{	
	if (nullptr == m_pParentBoneMatrix)
	{
		Engine::CDynamicMesh*		pPlayerMeshCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Get_Component(L"Layer_GameLogic",
																				m_strParentName.c_str(),
																				L"Com_Mesh",
																				Engine::CComponent::ID_STATIC));
		if (nullptr == pPlayerMeshCom)
			return 0;

		CStringA strConver;
		strConver = m_strBoreName.c_str();

		const Engine::D3DXFRAME_DERIVED*		pFrame = pPlayerMeshCom->Get_FrameByName(strConver);

		m_pParentBoneMatrix = &pFrame->CombinedTransformationMatrix;

		Engine::CTransform*		pPlayerTransCom = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(L"Layer_GameLogic",
																					m_strParentName.c_str(),
																					L"Com_Transform",
																					Engine::CComponent::ID_DYNAMIC));

		if (nullptr == pPlayerTransCom)
			return 0;

		m_pParentWorldMatrix = &pPlayerTransCom->m_matWorld;

	}

	D3DXVec3TransformCoord(&m_vPos, &m_vPos, &((*m_pParentBoneMatrix) * (*m_pParentWorldMatrix)));

	Engine::_int iExit = Engine::CGameObject::Update_Object(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(Engine::CRenderer::RENDER_NONALPHA, this);

	Engine::_matrix matMovePos = ((*m_pParentBoneMatrix) * (*m_pParentWorldMatrix));

	Engine::_vec3 vecPos;
	D3DXVec3TransformCoord(&vecPos, &vecPos, &matMovePos);
	
	return iExit;
}

void CCollisionSphere::Render_Object(void)
{
	Engine::_matrix matWorld;
	Engine::GetDevice()->SetRenderState(D3DRS_LIGHTING, FALSE);
	Engine::GetDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixTranslation(&matWorld, m_vPos.x, m_vPos.y, m_vPos.z);

	Engine::GetDevice()->SetTransform(D3DTS_WORLD, &matWorld);
	Engine::GetDevice()->SetTexture(0, m_pTexture);

	m_pMesh->DrawSubset(0);

	Engine::GetDevice()->SetRenderState(D3DRS_LIGHTING, TRUE);
	Engine::GetDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}

void CCollisionSphere::SetRadius(float fRadius)
{
	Engine::Safe_Release(m_pMesh);

	D3DXCreateSphere(m_pGraphicDev, fRadius, 10, 10, &m_pMesh, NULL);

	m_fRadius = fRadius;

	Engine::_ulong dwFvF = m_pMesh->GetFVF();
	m_pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&m_pVtxPos);

	D3DXComputeBoundingSphere(m_pVtxPos, m_pMesh->GetNumVertices(), D3DXGetFVFVertexSize(dwFvF), &m_vPos, &m_fRadius);
	m_pMesh->UnlockVertexBuffer();
}

HRESULT CCollisionSphere::Add_Component(void)
{
	Engine::CComponent*		pComponent = nullptr;

	// For.Renderer
	pComponent = m_pRendererCom = Engine::Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->AddRef();
	m_mapComponent[Engine::CComponent::ID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Renderer", pComponent));

	return S_OK;
}

CCollisionSphere * CCollisionSphere::Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3* pVecPos, wstring strParentName, wstring strBoreName, int iIndex)
{
	CCollisionSphere* pInstance = new CCollisionSphere(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pVecPos, strParentName, strBoreName, iIndex)))
	{
		Engine::Safe_Release(pInstance);
		ERR_BOX(L"CollisionSphere, Create Failed");
	}

	return pInstance;
}

void CCollisionSphere::Free(void)
{
	Engine::CGameObject::Free();
}
