#include "DynamicMesh.h"

USING(Engine)

Engine::CDynamicMesh::CDynamicMesh(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CMesh(pGraphicDev)
{
}

Engine::CDynamicMesh::CDynamicMesh(const CDynamicMesh & rhs)
	: Engine::CMesh(rhs)
	, m_pRootFrame(rhs.m_pRootFrame)
	, m_pLoader(rhs.m_pLoader)
	, m_MeshContainerList(rhs.m_MeshContainerList)
	, m_vecBoreNames(rhs.m_vecBoreNames)
{
	m_pAniCtrl = CAnimationCtrl::Create(*rhs.m_pAniCtrl);
}

Engine::CDynamicMesh::~CDynamicMesh()
{
}

_double CDynamicMesh::Get_Position()
{
	return m_pAniCtrl->Get_Position();
}

HRESULT Engine::CDynamicMesh::Ready_Meshes(const _tchar* pFilePath, const _tchar* pFileName)
{
	_tchar		szFullPath[128] = L"";

	lstrcpy(szFullPath, pFilePath);
	lstrcat(szFullPath, pFileName);

	m_pLoader = CHierachyLoader::Create(m_pGraphicDev, pFilePath);
	NULL_CHECK_RETURN(m_pLoader, E_FAIL);

	LPD3DXANIMATIONCONTROLLER		pAniCtrl = nullptr;

	if (FAILED(D3DXLoadMeshHierarchyFromX(szFullPath,
		D3DXMESH_MANAGED,
		m_pGraphicDev,
		m_pLoader,
		nullptr,
		&m_pRootFrame,
		&pAniCtrl)))
	{
		return E_FAIL;
	}

	m_pAniCtrl = CAnimationCtrl::Create(pAniCtrl);
	NULL_CHECK_RETURN(m_pAniCtrl, E_FAIL);

	Engine::Safe_Release(pAniCtrl);

	_matrix		matTemp;
	D3DXMatrixIdentity(&matTemp);

	Update_FrameMatrices((D3DXFRAME_DERIVED*)m_pRootFrame, &matTemp);

	SetUp_FrameMatrices((D3DXFRAME_DERIVED*)m_pRootFrame);

	return S_OK;
}

void Engine::CDynamicMesh::Render_Mesh()
{
	for(auto iter : m_MeshContainerList)
	{
		D3DXMESHCONTAINER_DERIVED*		pMeshContainer = (iter);

		for (_ulong i = 0; i < pMeshContainer->dwNumBones; ++i)
			pMeshContainer->pRenderingMatices[i] =
			pMeshContainer->pFrameOffsetMatrix[i] * (*pMeshContainer->ppFrameCombinedMatrix[i]);


		void*	pSrcVertices = nullptr;
		void*	pDestVertices = nullptr;

		pMeshContainer->pOriMesh->LockVertexBuffer(0, &pSrcVertices);
		pMeshContainer->MeshData.pMesh->LockVertexBuffer(0, &pDestVertices);

		// 소프트웨어 스키닝을 수행하는 함수(스키닝 뿐만 아니라 애니메이션 변경, 뼈대들과 정점 정보들의 정보 변경을 동시에 수행해주기도 한다)
		pMeshContainer->pSkinInfo->UpdateSkinnedMesh(pMeshContainer->pRenderingMatices, // 뼈의 변환 상태
			NULL,								// 원래로 돌리기 위한 상태 행렬(원래는 1인자의 역 행렬을 구해서 넣어줘야 함, 굳이 안 넣어줘도 됨)
			pSrcVertices,						// 변하지 않는 원본 메쉬의 정점 정보
			pDestVertices);						// 변환된 정보를 저장하고 있는 정점 정보

		for (_ulong i = 0; i < pMeshContainer->NumMaterials; ++i)
		{
			m_pGraphicDev->SetTexture(0, pMeshContainer->ppTexture[i]);
			pMeshContainer->MeshData.pMesh->DrawSubset(i);
		}

		pMeshContainer->pOriMesh->UnlockVertexBuffer();
		pMeshContainer->MeshData.pMesh->UnlockVertexBuffer();

	}
}

void CDynamicMesh::Render_Mesh(LPD3DXEFFECT pEffect)
{
	pEffect->AddRef();

	auto		iter = m_MeshContainerList.begin();
	auto		iter_end = m_MeshContainerList.end();

	for (; iter != iter_end; ++iter)
	{
		D3DXMESHCONTAINER_DERIVED*		pMeshContainer = (*iter);

		for (_ulong i = 0; i < pMeshContainer->dwNumBones; ++i)
			pMeshContainer->pRenderingMatices[i] =
			pMeshContainer->pFrameOffsetMatrix[i] * (*pMeshContainer->ppFrameCombinedMatrix[i]);


		void*	pSrcVertices = nullptr;
		void*	pDestVertices = nullptr;

		pMeshContainer->pOriMesh->LockVertexBuffer(0, &pSrcVertices);
		pMeshContainer->MeshData.pMesh->LockVertexBuffer(0, &pDestVertices);

		// 소프트웨어 스키닝을 수행하는 함수(스키닝 뿐만 아니라 애니메이션 변경, 뼈대들과 정점 정보들의 정보 변경을 동시에 수행해주기도 한다)
		pMeshContainer->pSkinInfo->UpdateSkinnedMesh(pMeshContainer->pRenderingMatices, // 뼈의 변환 상태
			NULL,								// 원래로 돌리기 위한 상태 행렬(원래는 1인자의 역 행렬을 구해서 넣어줘야 함, 굳이 안 넣어줘도 됨)
			pSrcVertices,						// 변하지 않는 원본 메쉬의 정점 정보
			pDestVertices);						// 변환된 정보를 저장하고 있는 정점 정보

		for (_ulong i = 0; i < pMeshContainer->NumMaterials; ++i)
		{
			pEffect->SetTexture("g_BaseTexture", pMeshContainer->ppTexture[i]);
			pEffect->CommitChanges();

			pMeshContainer->MeshData.pMesh->DrawSubset(i);
		}

		pMeshContainer->pOriMesh->UnlockVertexBuffer();
		pMeshContainer->MeshData.pMesh->UnlockVertexBuffer();

	}

	Engine::Safe_Release(pEffect);
}

void CDynamicMesh::Set_AnimationSet(const _uint & iIdx)
{
	m_pAniCtrl->Set_AnimationSet(iIdx);
}

void CDynamicMesh::Play_AnimationSet(const _float & fTimeDelta)
{
	m_pAniCtrl->Play_AnimationSet(fTimeDelta);

	_matrix matTemp;

	D3DXMatrixIdentity(&matTemp);
	Update_FrameMatrices((D3DXFRAME_DERIVED*)m_pRootFrame, &matTemp);
}

_double CDynamicMesh::Get_Period()
{
	return m_pAniCtrl->Get_Period();
}

_double CDynamicMesh::Get_AccTime()
{
	return m_pAniCtrl->Get_AccTime();
}

_double CDynamicMesh::Get_Percent()
{
	return m_pAniCtrl->Get_Percent();
}

void Engine::CDynamicMesh::Update_FrameMatrices(D3DXFRAME_DERIVED * pFrame, const _matrix * pParentMatrix)
{
	NULL_CHECK(pFrame);

	pFrame->CombinedTransformationMatrix = pFrame->TransformationMatrix * (*pParentMatrix);
	
	// 형제 뼈가 있다면
	if (nullptr != pFrame->pFrameSibling)
	{
		Update_FrameMatrices(((D3DXFRAME_DERIVED*)pFrame->pFrameSibling), pParentMatrix);
	}

	// 자식의 뼈가 있다면
	if (nullptr != pFrame->pFrameFirstChild)
	{
		Update_FrameMatrices(((D3DXFRAME_DERIVED*)pFrame->pFrameFirstChild), &pFrame->CombinedTransformationMatrix);
	}
}

void Engine::CDynamicMesh::SetUp_FrameMatrices(D3DXFRAME_DERIVED * pFrame)
{
	if (nullptr != pFrame->pMeshContainer)
	{
		D3DXMESHCONTAINER_DERIVED*	pMeshContainer = (D3DXMESHCONTAINER_DERIVED*)pFrame->pMeshContainer;

		for (_ulong i = 0; i < pMeshContainer->dwNumBones; ++i)
		{
			const char*		pBoneName = pMeshContainer->pSkinInfo->GetBoneName(i);
			
			D3DXFRAME_DERIVED*		pFrame = (D3DXFRAME_DERIVED*)D3DXFrameFind(m_pRootFrame, pBoneName);
			pMeshContainer->ppFrameCombinedMatrix[i] = &pFrame->CombinedTransformationMatrix;
			
			// const char* -> wstring
			wstring strTemp;

			wchar_t* pszTmp = NULL; // 와이드만자열로 변경해서 저장될 주소
			int iLen = strlen(pFrame->Name) + 1;
			pszTmp = new wchar_t[iLen];
			MultiByteToWideChar(CP_ACP, 0, pFrame->Name, -1, pszTmp, iLen);

			strTemp = pszTmp;

			delete[] pszTmp;

			m_vecBoreNames.push_back(strTemp);

		}

		m_MeshContainerList.push_back(pMeshContainer);
	}

	if (nullptr != pFrame->pFrameSibling)
		SetUp_FrameMatrices((D3DXFRAME_DERIVED*)pFrame->pFrameSibling);

	if (nullptr != pFrame->pFrameFirstChild)
		SetUp_FrameMatrices((D3DXFRAME_DERIVED*)pFrame->pFrameFirstChild);

}

CDynamicMesh * Engine::CDynamicMesh::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring pFilePath, wstring pFileName)
{
	CDynamicMesh*		pInstance = new CDynamicMesh(pGraphicDev);

	const _tchar* szFilePath = (LPCTSTR)pFilePath.c_str();
	const _tchar* szFileName = (LPCTSTR)pFileName.c_str();

	if (FAILED(pInstance->Ready_Meshes(szFilePath, szFileName)))
	{
		ERR_BOX(L"Dynamic Mesh Create Failed");
		Engine::Safe_Release(pInstance);
	}

	return pInstance;
}

CResources * Engine::CDynamicMesh::Clone()
{
	return new CDynamicMesh(*this);
}

void Engine::CDynamicMesh::Free()
{
	Engine::CMesh::Free();

	Engine::Safe_Release(m_pAniCtrl);

	if (false == m_bClone)
	{
		m_pLoader->DestroyFrame(m_pRootFrame);
		Engine::Safe_Release(m_pLoader);
	}
	m_MeshContainerList.clear();
}
