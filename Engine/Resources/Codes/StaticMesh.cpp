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

	if (FAILED(D3DXLoadMeshFromX(szFullPath,			// �ε���  x����
		D3DXMESH_MANAGED,	// �޽� ���� �ɼ�
		m_pGraphicDev,		// ����� ��ġ
		&m_pAdjacency,		// �ִϸ��̼� �޽� ���� �� ��Ű���� ���� ���� ���۸� �����صδ� output������(��Ű���� �ϱ� ���ؼ� ����� ���۸� �˾ƾ� �Ѵ�)
		&m_pSubsetBuffer,   // ����¿� ���� ������ �����ϴ� ������, �޽��� ���������� ������ �δ� ���� ������
		NULL,				// �޽� ��ü�� ���ϰ� �ִ� ����Ʈ�� ���� ����
		&m_dwSubsetCnt,		// ������ ����ϴ� ������� ������ ��� ����
		&m_pOriMesh)))			// �ε�� �޽��� ��ǥ�ϴ� ��ü
		return E_FAIL;

	// �޽ÿ� �븻 ���� ���� ��� �븻�� ����������.
	_ulong		dwFVF = m_pOriMesh->GetFVF();	// �޽��� ���ϰ� �ִ� ������ fvf ���� �������ִ� �Լ�

	if (!(dwFVF & D3DFVF_NORMAL))		// �븻�� ���� �����, �� ���� ���� �ְ� �Ųٷ� ������
	{
		m_pOriMesh->CloneMeshFVF(m_pOriMesh->GetOptions(), dwFVF | D3DFVF_NORMAL, m_pGraphicDev, &m_pMesh);
		// ������ load�� �޽��� ������ �������ִ� �Լ�

		D3DXComputeNormals(m_pMesh, (_ulong*)(m_pAdjacency->GetBufferPointer()));
		// ������ normal���� ����ؼ� �־��ִ� �Լ�
	}
	else
	{
		m_pOriMesh->CloneMeshFVF(m_pOriMesh->GetOptions(), dwFVF, m_pGraphicDev, &m_pMesh);
	}

	// �޽� ������ �´� �ٿ�� �ڽ��� �����Ѵ�.

	void*			pVertex = nullptr;

	m_dwNumvertices = m_pMesh->GetNumVertices();
	m_pVtxPos = new _vec3[m_dwNumvertices];

	m_pMesh->LockVertexBuffer(0, &pVertex);

	D3DVERTEXELEMENT9		Decl[MAX_FVF_DECL_SIZE];		// ���� �ȿ� ���ִ� FVF ���� �ϳ��� �����صα� ���� ����ü, ���� �ϳ��� �� �� �ִ� �ִ��� FVF ����
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

	// ������ fvf�� �˰� ���� ��, ������ ũ�⸦ ���� �� �ְ� ���ִ� �Լ�
	m_dwStride = D3DXGetFVFVertexSize(dwFVF);

	for (_ulong i = 0; i < m_dwNumvertices; ++i)
	{
		m_pVtxPos[i] = *((_vec3*)((_ubyte*)pVertex + (m_dwStride * i + byOffset)));
	}


	m_pMesh->UnlockVertexBuffer();

	// d3dxbuffer �� ��� �ִ� ������ ���� �� �ּҸ� �����Ѵ�.
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
	// ������ ������ ī��Ʈ +1
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