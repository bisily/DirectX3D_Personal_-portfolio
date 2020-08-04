#include "HierachyLoader.h"

USING(Engine)

Engine::CHierachyLoader::CHierachyLoader(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pPath)
	: m_pGraphicDev(pGraphicDev)
	, m_pPath(pPath)
{
	m_pGraphicDev->AddRef();
}

Engine::CHierachyLoader::~CHierachyLoader(void)
{

}

STDMETHODIMP Engine::CHierachyLoader::Ready_Loader(void)
{
	return S_OK;
}

STDMETHODIMP Engine::CHierachyLoader::CreateFrame(THIS_ LPCSTR Name, LPD3DXFRAME* ppNewFrame)
{
	D3DXFRAME_DERIVED*			pNewFrame = new D3DXFRAME_DERIVED;
	ZeroMemory(pNewFrame, sizeof(D3DXFRAME_DERIVED));

	Allocate_Name(&pNewFrame->Name, Name);

	D3DXMatrixIdentity(&pNewFrame->TransformationMatrix);

	pNewFrame->CombinedTransformationMatrix = pNewFrame->TransformationMatrix;

	*ppNewFrame = pNewFrame;

	return S_OK;
}

STDMETHODIMP Engine::CHierachyLoader::CreateMeshContainer(THIS_ LPCSTR Name,
	CONST D3DXMESHDATA* pMeshData,
	CONST D3DXMATERIAL* pMaterials,
	CONST D3DXEFFECTINSTANCE* pEffectInstances,
	DWORD NumMaterials,
	CONST DWORD* pAdjacency,
	LPD3DXSKININFO pSkinInfo,
	LPD3DXMESHCONTAINER* ppNewContainer)
{
	D3DXMESHCONTAINER_DERIVED*		pNewMeshContainer = new D3DXMESHCONTAINER_DERIVED;
	ZeroMemory(pNewMeshContainer, sizeof(D3DXMESHCONTAINER_DERIVED));

	Allocate_Name(&pNewMeshContainer->Name, Name);

	pNewMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;

	LPD3DXMESH		pMesh = pMeshData->pMesh;

	_ulong	dwNumFaces = pMesh->GetNumFaces();	// 폴리곤의 개수를 의미

	pNewMeshContainer->pAdjacency = new _ulong[dwNumFaces * 3];	// 이웃한 폴리곤의 정보를 저장하고 있기 때문에 폴리곤 개수까지 계산하여 할당
	memcpy(pNewMeshContainer->pAdjacency, pAdjacency, sizeof(_ulong) * dwNumFaces * 3);

	// pNewMeshContainer->MeshData.pMesh = pMeshData->pMesh;

	_ulong dwFVF = pMesh->GetFVF();

	if (!(dwFVF & D3DFVF_NORMAL))
	{
		pMesh->CloneMeshFVF(pMesh->GetOptions(), dwFVF | D3DFVF_NORMAL, m_pGraphicDev, &pNewMeshContainer->MeshData.pMesh);

		D3DXComputeNormals(pNewMeshContainer->MeshData.pMesh, pNewMeshContainer->pAdjacency);
	}
	else
	{
		pMesh->CloneMeshFVF(pMesh->GetOptions(), dwFVF, m_pGraphicDev, &pNewMeshContainer->MeshData.pMesh);
	}

	pNewMeshContainer->NumMaterials = (NumMaterials == 0 ? 1 : NumMaterials);

	pNewMeshContainer->pMaterials = new D3DXMATERIAL[pNewMeshContainer->NumMaterials];
	ZeroMemory(pNewMeshContainer->pMaterials, sizeof(D3DXMATERIAL) * pNewMeshContainer->NumMaterials);

	pNewMeshContainer->ppTexture = new LPDIRECT3DTEXTURE9[pNewMeshContainer->NumMaterials];
	ZeroMemory(pNewMeshContainer->ppTexture, sizeof(LPDIRECT3DTEXTURE9) * pNewMeshContainer->NumMaterials);

	if (0 != NumMaterials)
	{
		memcpy(pNewMeshContainer->pMaterials, pMaterials, sizeof(D3DXMATERIAL) * pNewMeshContainer->NumMaterials);

		for (_ulong i = 0; i < pNewMeshContainer->NumMaterials; ++i)
		{
			TCHAR		szFullPath[128] = L"";
			TCHAR		szFileName[128] = L"";

			MultiByteToWideChar(CP_ACP,
				0,
				pNewMeshContainer->pMaterials[i].pTextureFilename,
				strlen(pNewMeshContainer->pMaterials[i].pTextureFilename),
				szFileName,
				128);

			lstrcpy(szFullPath, m_pPath);
			lstrcat(szFullPath, szFileName);

			if (FAILED(D3DXCreateTextureFromFile(m_pGraphicDev, szFullPath, &pNewMeshContainer->ppTexture[i])))
			{
				ERR_BOX(L"Dynamic Mesh Texture Create Failed");
				return E_FAIL;
			}
		}

	}
	else
	{
		pNewMeshContainer->pMaterials[0].MatD3D.Diffuse = D3DXCOLOR(0.f, 0.f, 0.f, 0.f);
		pNewMeshContainer->pMaterials[0].MatD3D.Specular = D3DXCOLOR(0.f, 0.f, 0.f, 0.f);
		pNewMeshContainer->pMaterials[0].MatD3D.Ambient = D3DXCOLOR(0.f, 0.f, 0.f, 0.f);
		pNewMeshContainer->pMaterials[0].MatD3D.Emissive = D3DXCOLOR(0.f, 0.f, 0.f, 0.f);
		pNewMeshContainer->pMaterials[0].MatD3D.Power = 0.f;

		pNewMeshContainer->ppTexture[0] = nullptr;
	}

	NULL_CHECK_RETURN(pSkinInfo, S_OK);

	pNewMeshContainer->pSkinInfo = pSkinInfo;
	pNewMeshContainer->pSkinInfo->AddRef();



	pNewMeshContainer->MeshData.pMesh->CloneMeshFVF(pNewMeshContainer->MeshData.pMesh->GetOptions(),
		pNewMeshContainer->MeshData.pMesh->GetFVF(),
		m_pGraphicDev,
		&pNewMeshContainer->pOriMesh); // 렌더링 시 필요한 메쉬 복제 작업

									   // 메쉬의 영향을 미치는 뼈대의 개수를 얻어온다.
	pNewMeshContainer->dwNumBones = pNewMeshContainer->pSkinInfo->GetNumBones();

	pNewMeshContainer->pFrameOffsetMatrix = new _matrix[pNewMeshContainer->dwNumBones];		// 최초의 행렬 상태 값을 저장하기 위해 동적할당
	ZeroMemory(pNewMeshContainer->pFrameOffsetMatrix, sizeof(_matrix) * pNewMeshContainer->dwNumBones);

	pNewMeshContainer->ppFrameCombinedMatrix = new _matrix*[pNewMeshContainer->dwNumBones];	// 뼈대들이 갖고 있는 combined 어쩌구를 매번 가져와서 곱해주는 연산작업 수행을 위한 준비
	ZeroMemory(pNewMeshContainer->ppFrameCombinedMatrix, sizeof(_matrix*) * pNewMeshContainer->dwNumBones);

	pNewMeshContainer->pRenderingMatices = new _matrix[pNewMeshContainer->dwNumBones];
	ZeroMemory(pNewMeshContainer->pRenderingMatices, sizeof(_matrix) * pNewMeshContainer->dwNumBones);

	for (_ulong i = 0; i < pNewMeshContainer->dwNumBones; ++i)
		pNewMeshContainer->pFrameOffsetMatrix[i] = *pNewMeshContainer->pSkinInfo->GetBoneOffsetMatrix(i); // 메쉬를 그리기 위한 뼈대들에는 인덱스 값이 매겨져 있다. 
																										  // 이런 뼈대들을 순회하면서 행렬 정보를 차례대로 대입해주는 코드

	*ppNewContainer = pNewMeshContainer;

	return S_OK;
}

STDMETHODIMP Engine::CHierachyLoader::DestroyFrame(THIS_ LPD3DXFRAME pFrameToFree)
{
	Engine::Safe_Delete_Array(pFrameToFree->Name);

	if (nullptr != pFrameToFree->pMeshContainer)
		DestroyMeshContainer(pFrameToFree->pMeshContainer);

	if (nullptr != pFrameToFree->pFrameSibling)
		DestroyFrame(pFrameToFree->pFrameSibling);

	if (nullptr != pFrameToFree->pFrameFirstChild)
		DestroyFrame(pFrameToFree->pFrameFirstChild);

	Engine::Safe_Delete(pFrameToFree);

	return S_OK;
}

STDMETHODIMP Engine::CHierachyLoader::DestroyMeshContainer(THIS_ LPD3DXMESHCONTAINER pMeshContainerToFree)
{
	D3DXMESHCONTAINER_DERIVED*		pMeshContainer = (D3DXMESHCONTAINER_DERIVED*)pMeshContainerToFree;

	for (_ulong i = 0; i < pMeshContainer->NumMaterials; ++i)
		Engine::Safe_Release(pMeshContainer->ppTexture[i]);

	Engine::Safe_Delete_Array(pMeshContainer->ppTexture);

	Engine::Safe_Release(pMeshContainer->pOriMesh);

	Engine::Safe_Delete_Array(pMeshContainer->pFrameOffsetMatrix);
	Engine::Safe_Delete_Array(pMeshContainer->ppFrameCombinedMatrix);
	Engine::Safe_Delete_Array(pMeshContainer->pRenderingMatices);

	Engine::Safe_Delete_Array(pMeshContainer->Name);
	Engine::Safe_Release(pMeshContainer->MeshData.pMesh);
	Engine::Safe_Delete_Array(pMeshContainer->pMaterials);
	Engine::Safe_Delete_Array(pMeshContainer->pAdjacency);
	Engine::Safe_Release(pMeshContainer->pSkinInfo);

	Engine::Safe_Delete(pMeshContainer);

	return S_OK;
}

void Engine::CHierachyLoader::Allocate_Name(char** ppName, const char* pFrameName)
{
	if (nullptr == pFrameName)
		return;

	_uint		iLength = strlen(pFrameName);

	*ppName = new char[iLength + 1];

	strcpy_s(*ppName, iLength + 1, pFrameName);
}

Engine::CHierachyLoader* Engine::CHierachyLoader::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pPath)
{
	/*CHierachyLoader*		pInstance = new CHierachyLoader(pGraphicDev, pPath);

	if(FAILED(pInstance->Ready_Loader()))
	{
	ERR_BOX(L"HierachyLoader Created Failed");
	Engine::Safe_Release(pInstance);
	}*/

	return new CHierachyLoader(pGraphicDev, pPath);
}

_ulong Engine::CHierachyLoader::Release(void)
{
	Engine::Safe_Release(m_pGraphicDev);

	delete this;

	return 0;
}

