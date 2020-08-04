#include "StaticMesh.h"

USING(Engine)

Engine::CStaticMesh::CStaticMesh(LPDIRECT3DDEVICE9 pGraphicDev) 
	: Engine::CMesh(pGraphicDev)
{
}

Engine::CStaticMesh::CStaticMesh(const CStaticMesh & rhs)
	: Engine::CMesh(rhs)
	, m_pAdjacency(rhs.m_pAdjacency)
	, m_pSubsets(rhs.m_pSubsets)
	, m_pSubsetBuffer(rhs.m_pSubsetBuffer)
	, m_dwSubsetCnt(rhs.m_dwSubsetCnt)
	, m_pMesh(rhs.m_pMesh)
	, m_pOriMesh(rhs.m_pOriMesh)
	, m_dwNumvertices(rhs.m_dwNumvertices)
	, m_dwStride(rhs.m_dwStride)
	, m_pVtxPos(rhs.m_pVtxPos)
{
	m_ppTextures = new LPDIRECT3DTEXTURE9[rhs.m_dwSubsetCnt];

	for (_ulong i = 0; i < rhs.m_dwSubsetCnt; ++i)
	{
		m_ppTextures[i] = rhs.m_ppTextures[i];
		m_ppTextures[i]->AddRef();
	}
	m_pAdjacency->AddRef();
	m_pSubsetBuffer->AddRef();
	m_pMesh->AddRef();
	m_pOriMesh->AddRef();
}

Engine::CStaticMesh::~CStaticMesh()
{
}

HRESULT Engine::CStaticMesh::Ready_Meshs(wstring pFilePath, wstring pFileName)
{
	TCHAR			szFullPath[128] = L"";

	lstrcpy(szFullPath, pFilePath.c_str());
	lstrcat(szFullPath, pFileName.c_str());

	if (FAILED(D3DXLoadMeshFromX(szFullPath,			// 로드할  x파일
		D3DXMESH_MANAGED,	// 메시 생성 옵션
		m_pGraphicDev,		// 사용할 장치
		&m_pAdjacency,		// 애니메이션 메쉬 구동 시 스키닝을 위한 인접 버퍼를 저장해두는 output포인터(스키닝을 하기 위해선 가까운 버퍼를 알아야 한다)
		&m_pSubsetBuffer,   // 서브셋에 대한 정보를 저장하는 포인터, 메쉬의 재질정보를 저장해 두는 버퍼 포인터
		NULL,				// 메쉬 자체가 지니고 있는 이펙트에 대한 정보
		&m_dwSubsetCnt,		// 재질을 담당하는 서브셋의 갯수를 담는 변수
		&m_pOriMesh)))			// 로드된 메쉬를 대표하는 객체
		return E_FAIL;

	// 메시에 노말 값이 없을 경우 노말을 생성해주자.
	_ulong		dwFVF = m_pOriMesh->GetFVF();	// 메쉬가 지니고 있는 정점의 fvf 값을 추출해주는 함수

	if (!(dwFVF & D3DFVF_NORMAL))		// 노말이 없는 경우임, 꼭 성지 같은 애가 거꾸로 이해함
	{
		m_pOriMesh->CloneMeshFVF(m_pOriMesh->GetOptions(), dwFVF | D3DFVF_NORMAL, m_pGraphicDev, &m_pMesh);
		// 기존의 load한 메쉬의 정보를 복사해주는 함수

		D3DXComputeNormals(m_pMesh, (_ulong*)(m_pAdjacency->GetBufferPointer()));
		// 정점의 normal값을 계산해서 넣어주는 함수
	}
	else
	{
		m_pOriMesh->CloneMeshFVF(m_pOriMesh->GetOptions(), dwFVF, m_pGraphicDev, &m_pMesh);
	}

	// 메시 사이즈 맞는 바운딩 박스를 생성한다.

	void*			pVertex = nullptr;

	m_dwNumvertices = m_pMesh->GetNumVertices();
	m_pVtxPos = new _vec3[m_dwNumvertices];

	m_pMesh->LockVertexBuffer(0, &pVertex);

	D3DVERTEXELEMENT9		Decl[MAX_FVF_DECL_SIZE];		// 정점 안에 들어가있는 FVF 정보 하나를 저장해두기 위한 구조체, 정점 하나에 들어갈 수 있는 최대의 FVF 개수
	ZeroMemory(Decl, sizeof(D3DVERTEXELEMENT9) * MAX_FVF_DECL_SIZE);

	m_pMesh->GetDeclaration(Decl);	

	_ubyte		byOffset = 0;

	for (_ulong i = 0; i < MAX_FVF_DECL_SIZE; ++i)
	{
		if (Decl[i].Usage == D3DDECLUSAGE_POSITION)
		{
			byOffset = (_ubyte)Decl[i].Offset;
			break;
		}
	}

	// 정점의 fvf를 알고 있을 때, 정점의 크기를 얻어올 수 있게 해주는 함수
	m_dwStride = D3DXGetFVFVertexSize(dwFVF);

	for (_ulong i = 0; i < m_dwNumvertices; ++i)
	{
		m_pVtxPos[i] = *((_vec3*)((_ubyte*)pVertex + (m_dwStride * i + byOffset)));
	}


	m_pMesh->UnlockVertexBuffer();

	// d3dxbuffer 가 담고 있는 정보의 가장 앞 주소를 리턴한다.
	m_pSubsets = (D3DXMATERIAL*)m_pSubsetBuffer->GetBufferPointer();

	m_ppTextures = new LPDIRECT3DTEXTURE9[m_dwSubsetCnt];

	for (_ulong i = 0; i < m_dwSubsetCnt; ++i)
	{
		_tchar		szFileName[128] = L"";
		lstrcpy(szFullPath, pFilePath.c_str());

		MultiByteToWideChar(CP_ACP,
			0,
			m_pSubsets[i].pTextureFilename,
			strlen(m_pSubsets[i].pTextureFilename),
			szFileName, 128);

		lstrcat(szFullPath, szFileName);

		if (FAILED(D3DXCreateTextureFromFile(m_pGraphicDev, szFullPath, &m_ppTextures[i])))
			return E_FAIL;

	}

	return S_OK;
}

void CStaticMesh::Render_Mesh()
{
	for (_ulong i = 0; i < m_dwSubsetCnt; ++i)
	{
		m_pGraphicDev->SetTexture(0, m_ppTextures[i]);
		m_pMesh->DrawSubset(i);
	}
}

void CStaticMesh::Render_Mesh(LPD3DXEFFECT pEffect)
{
	// 복사해 왔으니 카운트 +1
	pEffect->AddRef();

	for (_ulong i = 0; i < m_dwSubsetCnt; ++i)
	{
		_bool bIsAlpha = false;
		_uint iPassCnt = 0;

		//if (bIsAlpha = Find_Alpha(m_pSubsets[i].pTextureFilename))
		//{
		//	iPassCnt = 1;
		//}

		pEffect->SetTexture("g_BaseTexture", m_ppTextures[i]);
		pEffect->CommitChanges();

		pEffect->BeginPass(iPassCnt);

		m_pMesh->DrawSubset(i);

		pEffect->EndPass();
	}

	Engine::Safe_Release(pEffect);
}

CStaticMesh * Engine::CStaticMesh::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring pFilePath, wstring pFileName)
{
	CStaticMesh*	pInstance = new CStaticMesh(pGraphicDev);

	if (FAILED(pInstance->Ready_Meshs(pFilePath, pFileName)))
	{
		ERR_BOX(L"Static Mesh Create Failed");
		Engine::Safe_Release(pInstance);
	}

	return pInstance;
}

CResources * Engine::CStaticMesh::Clone()
{
	return new CStaticMesh(*this);
}

void Engine::CStaticMesh::Free()
{
	Engine::CMesh::Free();

	for (_ulong i = 0; i < m_dwSubsetCnt; ++i)
	{
		Engine::Safe_Release(m_ppTextures[i]);
	}

	Engine::Safe_Release(m_pMesh);
	Engine::Safe_Release(m_pOriMesh);

	if (false == m_bClone)
		Engine::Safe_Delete_Array(m_pVtxPos);

	Engine::Safe_Release(m_pAdjacency);
	Engine::Safe_Release(m_pSubsetBuffer);

	Engine::Safe_Delete_Array(m_ppTextures);
}

_bool Engine::CStaticMesh::Find_Alpha(const char* pFileName)
{
	_uint	iLength = strlen(pFileName);

	for (_uint i = 0; i < iLength + 1; ++i)
	{
		if (pFileName[i] == '.')
		{
			if (pFileName[i - 1] == 'A')
				return true;
		}
	}
	return false;
}