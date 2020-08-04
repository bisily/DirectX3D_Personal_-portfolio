#include "stdafx.h"
#include "Navi.h"

#include "Export_Function.h"

#include "../Default/MainFrm.h"
#include "../Default/MyMap_ToolView.h"
#include "KeyMgr.h"

CNavi::CNavi(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CNavi::~CNavi(void)
{
	Free();
}

HRESULT CNavi::Ready_Object(void)
{
	if (FAILED(Add_Component()))
	{
		ERR_BOX(L"Navi, Add_Component Failed");
		return E_FAIL;
	}

	// 텍스쳐네...
	for (Engine::_uint i = 0; i < COLL_END; ++i)
	{
		m_pGraphicDev->CreateTexture(1, 1, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &m_pTexture[i], NULL);

		D3DLOCKED_RECT			LockRect;
		ZeroMemory(&LockRect, sizeof(D3DLOCKED_RECT));

		m_pTexture[i]->LockRect(0, &LockRect, NULL, 0);

		*((Engine::_ulong*)LockRect.pBits) = D3DXCOLOR(1.f * i, 0.5f * (1.f - i), 0.f, 1.f);
		m_pTexture[i]->UnlockRect(0);
	}

	Engine::_vec3 vPos;


	// 구 생성 버퍼
	float Radius;
	D3DXCreateSphere(m_pGraphicDev, 0.5f, 10, 10, &m_pMesh, NULL);

	Engine::_ulong dwFvF = m_pMesh->GetFVF();

	m_pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&m_pVtxPos);

	D3DXComputeBoundingSphere(m_pVtxPos, m_pMesh->GetNumVertices(), D3DXGetFVFVertexSize(dwFvF), &vPos, &Radius);

	m_pMesh->UnlockVertexBuffer();

	/*
	유니코드
	CString -> char*
	*/
	//CString test_1 = L"Hellow";
	//
	//int iLength = WideCharToMultiByte(CP_ACP, 0, test_1, -1, NULL, 0, NULL, NULL);
	//char* Dst = new char[iLength];
	//memset(Dst, 0, sizeof(test_1.GetLength()));
	//
	//WideCharToMultiByte(CP_ACP, 0, test_1, -1, Dst, iLength, NULL, NULL);
	//delete Dst;

	return S_OK;
}

Engine::_int CNavi::Update_Object(const Engine::_float & fTimeDelta)
{
	KeyCheck();

	m_DrawCellList = *m_pNaviMeshCom->Get_CellList();

	if (3 == m_vecPos.size())
	{
		const Engine::_vec3 vecPos1 = m_vecPos[0];
		const Engine::_vec3 vecPos2 = m_vecPos[1];
		const Engine::_vec3 vecPos3 = m_vecPos[2];

		m_vecPos.clear();

		m_pNaviMeshCom->Add_NeviMesh(&vecPos1, &vecPos2, &vecPos3);

		CMainFrame* MainFrame = (CMainFrame*)AfxGetMainWnd();
		MainFrame->m_pMyForm->m_pDlgMap->CheckNaviList();
		
		MainFrame->m_pMyForm->m_pDlgMap->m_Pos_1.SetWindowTextW(L"");
		MainFrame->m_pMyForm->m_pDlgMap->m_Pos_2.SetWindowTextW(L"");
	}

	Engine::_int iExit = Engine::CGameObject::Update_Object(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(Engine::CRenderer::RENDER_NONALPHA, this);

	return iExit;
}

void CNavi::Render_Object(void)
{
	m_pNaviMeshCom->Render_NaviCell();

	Engine::_matrix matWorld;
	Engine::GetDevice()->SetRenderState(D3DRS_LIGHTING, FALSE);
	Engine::GetDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	for (auto& iter : m_vecPos)
	{
		if (!m_vecPos.empty())
		{
			D3DXMatrixIdentity(&matWorld);

			D3DXMatrixTranslation(&matWorld, iter.x, iter.y, iter.z);

			// Picking Pos
			Engine::GetDevice()->SetTransform(D3DTS_WORLD, &matWorld);
			Engine::GetDevice()->SetTexture(0, m_pTexture[COLL_CLICK]);
			m_pMesh->DrawSubset(0);
		}
	}

	for (auto& iter : m_DrawCellList)
	{
		D3DXVECTOR3 vDrawA = *iter->Get_Point(Engine::CCell::POINT_A);
		D3DXVECTOR3 vDrawB = *iter->Get_Point(Engine::CCell::POINT_B);
		D3DXVECTOR3 vDrawC = *iter->Get_Point(Engine::CCell::POINT_C);

		D3DXMatrixIdentity(&matWorld);

		D3DXMatrixTranslation(&matWorld, vDrawA.x, vDrawA.y, vDrawA.z);

		// Point A
		Engine::GetDevice()->SetTransform(D3DTS_WORLD, &matWorld);
		Engine::GetDevice()->SetTexture(0, m_pTexture[COLL_FALSE]);
		m_pMesh->DrawSubset(0);

		D3DXMatrixIdentity(&matWorld);

		D3DXMatrixTranslation(&matWorld, vDrawB.x, vDrawB.y, vDrawB.z);

		// Point B
		Engine::GetDevice()->SetTransform(D3DTS_WORLD, &matWorld);
		Engine::GetDevice()->SetTexture(0, m_pTexture[COLL_FALSE]);
		m_pMesh->DrawSubset(0);

		D3DXMatrixIdentity(&matWorld);

		D3DXMatrixTranslation(&matWorld, vDrawC.x, vDrawC.y, vDrawC.z);

		// Point C
		Engine::GetDevice()->SetTransform(D3DTS_WORLD, &matWorld);
		Engine::GetDevice()->SetTexture(0, m_pTexture[COLL_FALSE]);
		m_pMesh->DrawSubset(0);
	}
	
	Engine::GetDevice()->SetRenderState(D3DRS_LIGHTING, TRUE);
	Engine::GetDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}

void CNavi::Set_PopBack()
{
	if (!m_vecPos.empty())
	{
		m_vecPos.pop_back(); 

		CMainFrame* MainFrame = (CMainFrame*)AfxGetMainWnd();
		
		if (m_vecPos.size() == 0)
		{
			MainFrame->m_pMyForm->m_pDlgMap->m_Pos_1.SetWindowTextW(L"");
		}
		else
		{
			MainFrame->m_pMyForm->m_pDlgMap->m_Pos_2.SetWindowTextW(L"");
		}

	}
}

HRESULT CNavi::Add_Component(void)
{
	Engine::CComponent*		pComponent = nullptr;

	// For.Renderer
	pComponent = m_pRendererCom = Engine::Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->AddRef();
	m_mapComponent[Engine::CComponent::ID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Renderer", pComponent));

	// For.Calculator
	pComponent = m_pCalculatorCom = Engine::CCalculator::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::CComponent::ID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Calculator", pComponent));

	// For.Navi
	pComponent = m_pNaviMeshCom = dynamic_cast<Engine::CNaviMesh*>(Engine::Clone_Resource(RESOURCE_STAGE, L"Mesh_Navi"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::CComponent::ID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_NaviMesh", pComponent));

	return S_OK;
}

void CNavi::KeyCheck()
{
	if (CKeyMgr::GetInstance()->GetKeyDown('6'))
	{
		m_pNaviMeshCom->SetUp_Index(TRUE);
	}

	if (CKeyMgr::GetInstance()->GetKeyDown('7'))
	{
		m_pNaviMeshCom->SetUp_Index(FALSE);
	}

	if (CKeyMgr::GetInstance()->GetKeyDown('8'))
	{
		m_pNaviMeshCom->SetUp_PosText(FALSE);
	}

	if (CKeyMgr::GetInstance()->GetKeyDown('9'))
	{
		m_pNaviMeshCom->SetUp_PosText(TRUE);
	}

	if (m_bNeviMode == true && CKeyMgr::GetInstance()->GetKeyDown(VK_LBUTTON))
	{
		CMainFrame* MainFrame = (CMainFrame*)AfxGetMainWnd();
		CString strTemp;

		Engine::CTerrainTex*		pTerrainBufferCom = dynamic_cast<Engine::CTerrainTex*>
			(Engine::Get_Component(L"Layer_GameLogic", L"Terrain", L"Com_Buffer", Engine::CComponent::ID_STATIC));
		NULL_CHECK(pTerrainBufferCom);

		Engine::CTransform*		pTerrainTransCom = dynamic_cast<Engine::CTransform*>
			(Engine::Get_Component(L"Layer_GameLogic", L"Terrain", L"Com_Transform", Engine::CComponent::ID_DYNAMIC));
		NULL_CHECK(pTerrainTransCom);

		vTemp = m_pCalculatorCom->Picking_OnTerrain(g_hWnd, pTerrainBufferCom, pTerrainTransCom);

		if (vTemp == D3DXVECTOR3(0.f, 0.f, 0.f))
		{
			return;
		}

		D3DXMATRIX matWorld;

		D3DXMatrixIdentity(&matWorld);
		
		m_pGraphicDev->GetTransform(D3DTS_WORLD, &matWorld);

		D3DXVec3TransformCoord(&vTemp, &vTemp, &matWorld);

		for (auto& iter : m_DrawCellList)
		{
			Engine::_vec3 vDirA, vDirB, vDirC;
			vDirA = vTemp - *iter->Get_Point(Engine::CCell::POINT_A);
			vDirB = vTemp - *iter->Get_Point(Engine::CCell::POINT_B);
			vDirC = vTemp - *iter->Get_Point(Engine::CCell::POINT_C);

			Engine::_float fDistA = D3DXVec3Length(&vDirA);
			Engine::_float fDistB = D3DXVec3Length(&vDirB);
			Engine::_float fDistC = D3DXVec3Length(&vDirC);

			if (fDistA <= 0.5f)
			{
				vTemp = *iter->Get_Point(Engine::CCell::POINT_A);
			}
			else if (fDistB <= 0.5f)
			{
				vTemp = *iter->Get_Point(Engine::CCell::POINT_B);
			}
			else if (fDistC <= 0.5f)
			{
				vTemp = *iter->Get_Point(Engine::CCell::POINT_C);
			}
		}

		if (0 < m_vecPos.size())
		{
			if (m_vecPos.back() != vTemp)
			{
				strTemp.Format(L"X:%f, Y:%f, Z:%f", vTemp.x, vTemp.y, vTemp.z);
				MainFrame->m_pMyForm->m_pDlgMap->m_Pos_2.SetWindowTextW(strTemp);
				m_vecPos.push_back(vTemp);
			}
		}
		else
		{
			strTemp.Format(L"X:%f, Y:%f, Z:%f", vTemp.x, vTemp.y, vTemp.z);
			MainFrame->m_pMyForm->m_pDlgMap->m_Pos_1.SetWindowTextW(strTemp);
			m_vecPos.push_back(vTemp);
		}
	}
	

}

CNavi * CNavi::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	CNavi* pInstance = new CNavi(_pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Engine::Safe_Release(pInstance);
		ERR_BOX(L"Navi, Create Failed");
	}

	return pInstance;
}

void CNavi::Free(void)
{
	Engine::Safe_Release(m_pMesh);

	for (int i = 0; i < COLL_END; ++i)
	{
		Engine::Safe_Release(m_pTexture[i]);
	}

	Engine::CGameObject::Free();
}
