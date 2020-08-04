#include "NaviMesh.h"

#include "Export_System.h"

USING(Engine)

Engine::CNaviMesh::CNaviMesh(LPDIRECT3DDEVICE9 pGraphicDev)
	:CMesh(pGraphicDev)
{

}

Engine::CNaviMesh::CNaviMesh(const CNaviMesh& rhs)
	: CMesh(rhs)
	, m_dwIndex(rhs.m_dwIndex)
	, m_vecCell(rhs.m_vecCell)
{
	for(auto& i : m_vecCell)
		i->AddRef();
}

Engine::CNaviMesh::~CNaviMesh(void)
{
	
}

HRESULT CNaviMesh::Add_NeviMesh(const _vec3 * PositionA, const _vec3 * PositionB, const _vec3 * PositionC)
{
	m_vecCell.reserve(m_vecCell.size() + 1);

	CCell*	pCell = CCell::Create(m_pGraphicDev, m_vecCell.size(), PositionA, PositionB, PositionC);
	NULL_CHECK_RETURN(pCell, E_FAIL);
	m_vecCell.push_back(pCell);

	if (FAILED(Link_Cell()))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT Engine::CNaviMesh::Ready_NaviMesh(int iFlage)
{
	CCell*		pCell = nullptr;
	
	// For.Tool
	if (0 == iFlage)
	{
		m_vecCell.reserve(4);

		// 0
		pCell = CCell::Create(m_pGraphicDev, m_vecCell.size(), &_vec3(0.f, 0.f, 2.f), &_vec3(2.f, 0.f, 0.f), &_vec3(0.f, 0.f, 0.f));
		NULL_CHECK_RETURN(pCell, E_FAIL);
		m_vecCell.push_back(pCell);
	
		// 1
		pCell = CCell::Create(m_pGraphicDev, m_vecCell.size(), &_vec3(0.f, 0.f, 2.f), &_vec3(2.f, 0.f, 2.f), &_vec3(2.f, 0.f, 0.f));
		NULL_CHECK_RETURN(pCell, E_FAIL);
		m_vecCell.push_back(pCell);

		// 2
		pCell = CCell::Create(m_pGraphicDev, m_vecCell.size(), &_vec3(0.f, 0.f, 4.f), &_vec3(2.f, 0.f, 2.f), &_vec3(0.f, 0.f, 2.f));
		NULL_CHECK_RETURN(pCell, E_FAIL);
		m_vecCell.push_back(pCell);

		// 3
		pCell = CCell::Create(m_pGraphicDev, m_vecCell.size(), &_vec3(2.f, 0.f, 2.f), &_vec3(4.f, 0.f, 0.f), &_vec3(2.f, 0.f, 0.f));
		NULL_CHECK_RETURN(pCell, E_FAIL);
		m_vecCell.push_back(pCell);
	}
	else if (1 == iFlage)
	{
		HANDLE	hFile = CreateFile(L"../../Data/Navi.dat", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

		DWORD	dwByte = 99;
		int		iNavi_index = 0;
		D3DXVECTOR3 vecPosA, vecPosB, vecPosC;

		while (true)
		{

			ReadFile(hFile, &vecPosA, sizeof(D3DXVECTOR3), &dwByte, NULL);

			if (dwByte == 0)
			{
				break;
			}

			ReadFile(hFile, &vecPosB, sizeof(D3DXVECTOR3), &dwByte, NULL);
			ReadFile(hFile, &vecPosC, sizeof(D3DXVECTOR3), &dwByte, NULL);

			pCell = CCell::Create(m_pGraphicDev, m_vecCell.size(), &vecPosA, &vecPosB, &vecPosC);

			NULL_CHECK_RETURN(pCell, E_FAIL);
			m_vecCell.push_back(pCell);
		}

	}

	if(FAILED(Link_Cell()))
		return E_FAIL;

	return S_OK;
}

void CNaviMesh::Render_NaviCell(void)
{
	if (Engine::Get_DIKeyState(DIK_8) & 0x80)
	{
		for(auto& i : m_vecCell)
			i->Render_Cell();
	}
}

_vec3 CNaviMesh::Move_OnNaviMesh(const _vec3 * pTargetPos, const _vec3 * pTargetDir)
{
	_vec3		vEndPos = *pTargetPos + *pTargetDir;

	if (CCell::COMPARE_MOVE == m_vecCell[m_dwIndex]->Compare(&vEndPos, &m_dwIndex))
	{
		Get_Height(&vEndPos, m_dwIndex);

		return vEndPos;
	}

	else if (CCell::COMPARE_STAY == m_vecCell[m_dwIndex]->Compare(&vEndPos, &m_dwIndex))
	{
		Get_Height(&vEndPos, m_dwIndex);

		return *pTargetPos;
	}

	return *pTargetPos;
}

void CNaviMesh::Get_Height(_vec3* vecEnPos, _ulong dwIndex)
{
	D3DXPLANE		Plane;

	D3DXPlaneFromPoints(&Plane,
		m_vecCell[dwIndex]->Get_Point(CCell::POINT_A),
		m_vecCell[dwIndex]->Get_Point(CCell::POINT_B),
		m_vecCell[dwIndex]->Get_Point(CCell::POINT_C));

	/*ax + by + cz + d = 0;
	by = -ax - cz - d;
	y = (-ax - cz - d) / b*/

	vecEnPos->y = (-Plane.a * vecEnPos->x - Plane.c * vecEnPos->z - Plane.d) / Plane.b;

	//vecEnPos->y += 1.f; // 이것은 그냥 텍스쳐를 조금 더 자연스럽게 보이게 하기 위한 의미없는 코드이다.
}

void CNaviMesh::ReSet_Index()
{
	int iIndex = 0;

	for (auto iter : m_vecCell)
	{
		iter->ReSet_Index(iIndex);

		++iIndex;
	}

}

void CNaviMesh::SetUp_PosText(_bool bRender)
{
	for (auto& iter : m_vecCell)
	{
		iter->SetUp_PosText(bRender);
	}
}

void CNaviMesh::SetUp_Index(_bool bRender)
{
	/*for (auto& iter : m_vecCell)
	{
		iter->SetUp_Index(bRender);
	}*/
}

HRESULT Engine::CNaviMesh::Link_Cell(void)
{
	for(_ulong i = 0; i < m_vecCell.size(); ++i)
	{
		for(_ulong j = 0; j < m_vecCell.size(); ++j)
		{
			if(i == j)
				continue;
			// AB
			if(nullptr == m_vecCell[i]->Get_Neighbor(CCell::NEIGHBOR_AB)
				&& true == m_vecCell[j]->Compare_Point(
				m_vecCell[i]->Get_Point(CCell::POINT_A), 
				m_vecCell[i]->Get_Point(CCell::POINT_B), 
				m_vecCell[i]))
			{
				m_vecCell[i]->Set_Neighbor(CCell::NEIGHBOR_AB, m_vecCell[j]);
				continue;
			}
			
			// BC
			if(nullptr == m_vecCell[i]->Get_Neighbor(CCell::NEIGHBOR_BC)
				&& true == m_vecCell[j]->Compare_Point(
				m_vecCell[i]->Get_Point(CCell::POINT_B),
				m_vecCell[i]->Get_Point(CCell::POINT_C),
				m_vecCell[i]))
			{
				m_vecCell[i]->Set_Neighbor(CCell::NEIGHBOR_BC, m_vecCell[j]);
				continue;
			}
			
			// CA
			if(nullptr == m_vecCell[i]->Get_Neighbor(CCell::NEIGHBOR_CA)
				&& true == m_vecCell[j]->Compare_Point(
				m_vecCell[i]->Get_Point(CCell::POINT_C),
				m_vecCell[i]->Get_Point(CCell::POINT_A),
				m_vecCell[i]))
			{
				m_vecCell[i]->Set_Neighbor(CCell::NEIGHBOR_CA, m_vecCell[j]);
				continue;
			}
		}
	}
	
	return S_OK;
}

Engine::CNaviMesh* Engine::CNaviMesh::Create(LPDIRECT3DDEVICE9 pGraphicDev, int iFlage)
{
	CNaviMesh*	pInstance = new CNaviMesh(pGraphicDev);

	if(FAILED(pInstance->Ready_NaviMesh(iFlage)))
	{
		ERR_BOX(L"NaviMesh Created Failed");
		Engine::Safe_Release(pInstance);
	}

	return pInstance;
}

Engine::CResources* Engine::CNaviMesh::Clone(void)
{
	return new CNaviMesh(*this);
}

void Engine::CNaviMesh::Free(void)
{
	Engine::CMesh::Free();

	for(auto& i : m_vecCell)
		Engine::Safe_Release(i);

	m_vecCell.clear();
	
}


