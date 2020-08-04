#include "stdafx.h"
#include "Square.h"

#include "Export_Function.h"

#include "../Default/MainFrm.h"
#include "../Default/MyMap_ToolView.h"

CSquare::CSquare(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CSquare::~CSquare(void)
{
	Free();
}

HRESULT CSquare::Ready_Object(D3DXVECTOR3* pVecPos, wstring strParentName, wstring strBoreName, Engine::_vec3 vMin, Engine::_vec3 vMax, int iIndex)
{
	if (FAILED(Add_Component()))
	{
		ERR_BOX(L"Navi, Add_Component Failed");
		return E_FAIL;
	}

	m_strBoreName = strBoreName;
	m_strParentName = strParentName;
	m_vMax = vMax;
	m_vMin = vMin;

	FAILED_CHECK_RETURN(Engine::GetDevice()->CreateVertexBuffer(sizeof(Engine::VTXCUBE) * 8,
		0,
		D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0),
		D3DPOOL_MANAGED,
		&m_pVB,
		NULL), E_FAIL);

	FAILED_CHECK_RETURN(Engine::GetDevice()->CreateIndexBuffer(sizeof(Engine::INDEX16) * 12,
		0,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIB,
		nullptr), E_FAIL);


	Engine::VTXCUBE*		pVtxCube = NULL;

	m_pVB->Lock(0, 0, (void**)&pVtxCube, 0);

	// 전면부
	pVtxCube[0].vPosition = D3DXVECTOR3(m_vMin.x, m_vMax.y, m_vMin.z);
	pVtxCube[0].vTex = pVtxCube[0].vPosition;

	pVtxCube[1].vPosition = D3DXVECTOR3(m_vMax.x, m_vMax.y, m_vMin.z);
	pVtxCube[1].vTex = pVtxCube[1].vPosition;

	pVtxCube[2].vPosition = D3DXVECTOR3(m_vMax.x, m_vMin.y, m_vMin.z);
	pVtxCube[2].vTex = pVtxCube[2].vPosition;

	pVtxCube[3].vPosition = D3DXVECTOR3(m_vMin.x, m_vMin.y, m_vMin.z);
	pVtxCube[3].vTex = pVtxCube[3].vPosition;

	// 후면부
	pVtxCube[4].vPosition = D3DXVECTOR3(m_vMin.x, m_vMax.y, m_vMax.z);
	pVtxCube[4].vTex = pVtxCube[4].vPosition;

	pVtxCube[5].vPosition = D3DXVECTOR3(m_vMax.x, m_vMax.y, m_vMax.z);
	pVtxCube[5].vTex = pVtxCube[5].vPosition;

	pVtxCube[6].vPosition = D3DXVECTOR3(m_vMax.x, m_vMin.y, m_vMax.z);
	pVtxCube[6].vTex = pVtxCube[6].vPosition;

	pVtxCube[7].vPosition = D3DXVECTOR3(m_vMin.x, m_vMin.y, m_vMax.z);
	pVtxCube[7].vTex = pVtxCube[7].vPosition;

	m_pVB->Unlock();

	Engine::INDEX16*		pIndex = NULL;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	// +x
	pIndex[0]._1 = 1;
	pIndex[0]._2 = 5;
	pIndex[0]._3 = 6;

	pIndex[1]._1 = 1;
	pIndex[1]._2 = 6;
	pIndex[1]._3 = 2;

	// -x
	pIndex[2]._1 = 4;
	pIndex[2]._2 = 0;
	pIndex[2]._3 = 3;

	pIndex[3]._1 = 4;
	pIndex[3]._2 = 3;
	pIndex[3]._3 = 7;

	// +y
	pIndex[4]._1 = 4;
	pIndex[4]._2 = 5;
	pIndex[4]._3 = 1;

	pIndex[5]._1 = 4;
	pIndex[5]._2 = 1;
	pIndex[5]._3 = 0;

	// -y
	pIndex[6]._1 = 3;
	pIndex[6]._2 = 2;
	pIndex[6]._3 = 6;

	pIndex[7]._1 = 3;
	pIndex[7]._2 = 6;
	pIndex[7]._3 = 7;

	// +z
	pIndex[8]._1 = 7;
	pIndex[8]._2 = 6;
	pIndex[8]._3 = 5;

	pIndex[9]._1 = 7;
	pIndex[9]._2 = 5;
	pIndex[9]._3 = 4;

	// -z
	pIndex[10]._1 = 0;
	pIndex[10]._2 = 1;
	pIndex[10]._3 = 2;

	pIndex[11]._1 = 0;
	pIndex[11]._2 = 2;
	pIndex[11]._3 = 3;

	m_pIB->Unlock();

	return S_OK;
}

Engine::_int CSquare::Update_Object(const Engine::_float & fTimeDelta)
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

	matWorld = ((*m_pParentBoneMatrix) * (*m_pParentWorldMatrix));

	D3DXVec3TransformCoord(&m_vPos, &m_vPos, &((*m_pParentBoneMatrix) * (*m_pParentWorldMatrix)));

	Engine::_int iExit = Engine::CGameObject::Update_Object(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(Engine::CRenderer::RENDER_NONALPHA, this);

	return iExit;
}

void CSquare::Render_Object(void)
{

	Engine::GetDevice()->SetRenderState(D3DRS_LIGHTING, FALSE);
	Engine::GetDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	Engine::GetDevice()->SetTransform(D3DTS_WORLD, &matWorld);

	Engine::GetDevice()->SetRenderState(D3DRS_LIGHTING, FALSE);

	Engine::GetDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	Engine::GetDevice()->SetTexture(0, m_pTexture);

	Engine::GetDevice()->SetStreamSource(0, m_pVB, 0, sizeof(Engine::VTXCUBE));
	Engine::GetDevice()->SetFVF(D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0));
	Engine::GetDevice()->SetIndices(m_pIB);
	Engine::GetDevice()->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 8, 0, 12);

	Engine::GetDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

}

void CSquare::SetSquare(Engine::_vec3 vMin, Engine::_vec3 vMax)
{
	Engine::Safe_Release(m_pVB);
	Engine::Safe_Release(m_pIB);

	m_vMax = vMax;
	m_vMin = vMin;

	Engine::GetDevice()->CreateVertexBuffer(sizeof(Engine::VTXCUBE) * 8,
		0,
		D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0),
		D3DPOOL_MANAGED,
		&m_pVB,
		NULL);

	Engine::GetDevice()->CreateIndexBuffer(sizeof(Engine::INDEX16) * 12,
		0,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIB,
		nullptr);

	Engine::VTXCUBE*		pVtxCube = NULL;

	m_pVB->Lock(0, 0, (void**)&pVtxCube, 0);

	// 전면부
	pVtxCube[0].vPosition = D3DXVECTOR3(m_vMin.x, m_vMax.y, m_vMin.z);
	pVtxCube[0].vTex = pVtxCube[0].vPosition;

	pVtxCube[1].vPosition = D3DXVECTOR3(m_vMax.x, m_vMax.y, m_vMin.z);
	pVtxCube[1].vTex = pVtxCube[1].vPosition;

	pVtxCube[2].vPosition = D3DXVECTOR3(m_vMax.x, m_vMin.y, m_vMin.z);
	pVtxCube[2].vTex = pVtxCube[2].vPosition;

	pVtxCube[3].vPosition = D3DXVECTOR3(m_vMin.x, m_vMin.y, m_vMin.z);
	pVtxCube[3].vTex = pVtxCube[3].vPosition;

	// 후면부
	pVtxCube[4].vPosition = D3DXVECTOR3(m_vMin.x, m_vMax.y, m_vMax.z);
	pVtxCube[4].vTex = pVtxCube[4].vPosition;

	pVtxCube[5].vPosition = D3DXVECTOR3(m_vMax.x, m_vMax.y, m_vMax.z);
	pVtxCube[5].vTex = pVtxCube[5].vPosition;

	pVtxCube[6].vPosition = D3DXVECTOR3(m_vMax.x, m_vMin.y, m_vMax.z);
	pVtxCube[6].vTex = pVtxCube[6].vPosition;

	pVtxCube[7].vPosition = D3DXVECTOR3(m_vMin.x, m_vMin.y, m_vMax.z);
	pVtxCube[7].vTex = pVtxCube[7].vPosition;

	m_pVB->Unlock();

	Engine::INDEX16*		pIndex = NULL;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	// +x
	pIndex[0]._1 = 1;
	pIndex[0]._2 = 5;
	pIndex[0]._3 = 6;

	pIndex[1]._1 = 1;
	pIndex[1]._2 = 6;
	pIndex[1]._3 = 2;

	// -x
	pIndex[2]._1 = 4;
	pIndex[2]._2 = 0;
	pIndex[2]._3 = 3;

	pIndex[3]._1 = 4;
	pIndex[3]._2 = 3;
	pIndex[3]._3 = 7;

	// +y
	pIndex[4]._1 = 4;
	pIndex[4]._2 = 5;
	pIndex[4]._3 = 1;

	pIndex[5]._1 = 4;
	pIndex[5]._2 = 1;
	pIndex[5]._3 = 0;

	// -y
	pIndex[6]._1 = 3;
	pIndex[6]._2 = 2;
	pIndex[6]._3 = 6;

	pIndex[7]._1 = 3;
	pIndex[7]._2 = 6;
	pIndex[7]._3 = 7;

	// +z
	pIndex[8]._1 = 7;
	pIndex[8]._2 = 6;
	pIndex[8]._3 = 5;

	pIndex[9]._1 = 7;
	pIndex[9]._2 = 5;
	pIndex[9]._3 = 4;

	// -z
	pIndex[10]._1 = 0;
	pIndex[10]._2 = 1;
	pIndex[10]._3 = 2;

	pIndex[11]._1 = 0;
	pIndex[11]._2 = 2;
	pIndex[11]._3 = 3;

	m_pIB->Unlock();
}

HRESULT CSquare::Add_Component(void)
{
	Engine::CComponent*		pComponent = nullptr;

	// For.Renderer
	pComponent = m_pRendererCom = Engine::Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->AddRef();
	m_mapComponent[Engine::CComponent::ID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Renderer", pComponent));

	return S_OK;
}

CSquare * CSquare::Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3* pVecPos, wstring strParentName, wstring strBoreName, _vec3 vMin, _vec3 vMax, int iIndex)
{
	CSquare* pInstance = new CSquare(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pVecPos, strParentName, strBoreName, vMin, vMax, iIndex)))
	{
		Engine::Safe_Release(pInstance);
		ERR_BOX(L"Square, Create Failed");
	}

	return pInstance;
}

void CSquare::Free()
{
	Engine::CGameObject::Free();
}
