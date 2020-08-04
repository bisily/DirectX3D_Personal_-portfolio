#include "ResourcesMgr.h"

USING(Engine)
IMPLEMENT_SINGLETON(CResourcesMgr)


Engine::CResourcesMgr::CResourcesMgr(void)
{

}

Engine::CResourcesMgr::~CResourcesMgr(void)
{
	Free();
}

HRESULT Engine::CResourcesMgr::Reserve_ContainerSize(const _ushort& wSize)
{
	if(nullptr != m_pmapResources)
		return E_FAIL;

	m_pmapResources = new MAPRESOURCE[wSize];

	m_wMaxContainerSize = wSize;

	return S_OK;
}

HRESULT Engine::CResourcesMgr::Ready_Buffers(LPDIRECT3DDEVICE9 pGraphicDev,
												const _ushort& wContainerIdx,
												wstring pBufferTag,
												Engine::CVIBuffer::BUFFERID eID,
												const _int iFlag,
												const _ulong& dwCntX,
												const _ulong& dwCntZ,
												const _ulong& dwVtxItv,
												const Engine::_ulong& dwVtxDetail)
{
	if(nullptr == m_pmapResources)
	{
		ERR_BOX(L"Resource Reserve Failed");
		return E_FAIL;
	}

	CResources*		pResources = Find_Resources(wContainerIdx, pBufferTag);

	if(nullptr != pResources)
		return E_FAIL;

	switch(eID)
	{
	case Engine::CVIBuffer::BUFFER_RCCOL:
		pResources = CRcCol::Create(pGraphicDev);
		break;

	case Engine::CVIBuffer::BUFFER_RCTEX:

		if (iFlag != 0)
		{
			pResources = CRcTex::Create(pGraphicDev, iFlag);
		}
		else
			pResources = CRcTex::Create(pGraphicDev);

		break;

	case Engine::CVIBuffer::BUFFER_TERRAINTEX:
		pResources = CTerrainTex::Create(pGraphicDev, dwCntX, dwCntZ, dwVtxItv, dwVtxDetail);
		break;

	case Engine::CVIBuffer::BUFFER_CUBETEX:
		pResources = CCubeTex::Create(pGraphicDev);
		break;

	case Engine::CVIBuffer::BUFFER_SWORDTRAIL:
		pResources = CTrailTex::Create(pGraphicDev);
		break;
	}
	NULL_CHECK_RETURN(pResources, E_FAIL);

	m_pmapResources[wContainerIdx].insert(MAPRESOURCE::value_type(pBufferTag, pResources));

	return S_OK;
}

Engine::CResources* Engine::CResourcesMgr::Find_Resources(const _ushort& wContainerIdx, 
														wstring pResourceTag)
{
	auto		iter = find_if(m_pmapResources[wContainerIdx].begin(), 
								m_pmapResources[wContainerIdx].end(), 
								CSTD_Tag_Finder(pResourceTag));
	
	if(iter == m_pmapResources[wContainerIdx].end())
		return nullptr;

	return iter->second;
	
}
void Engine::CResourcesMgr::Render_Buffer(const _ushort& wContainerIdx, wstring pBufferTag)
{
	CResources*	pResources = Find_Resources(wContainerIdx, pBufferTag);

	if(nullptr == pResources)
		return;

	dynamic_cast<CVIBuffer*>(pResources)->Render_Buffer();
}

void Engine::CResourcesMgr::Free(void)
{
	for(_uint i = 0; i < m_wMaxContainerSize; ++i)
	{
		for_each(m_pmapResources[i].begin(), m_pmapResources[i].end(), CDeleteMap());
		m_pmapResources[i].clear();
	}
	
	Engine::Safe_Delete_Array(m_pmapResources);
}

Engine::CResources* Engine::CResourcesMgr::Clone_Resource(const _ushort& wContainerIdx, 
															wstring pResourceTag)
{
	NULL_CHECK_RETURN(m_pmapResources, nullptr);

	CResources*		pPrototype = Find_Resources(wContainerIdx, pResourceTag);

	return pPrototype->Clone();
}

void CResourcesMgr::SetVtxInfo(RESOURCETYPE eResourceType, wstring pResourceKey, void * pVertex)
{
	auto	iter = m_pmapResources[eResourceType].find(pResourceKey);

	if (m_pmapResources[eResourceType].end() == iter)
	{
		ERR_BOX(L"버텍스를 찾을 수 없음");
		return;
	}

	((CVIBuffer*)(iter->second))->SetVtxInfo(pVertex);
}

HRESULT Engine::CResourcesMgr::Ready_Textures(LPDIRECT3DDEVICE9 pGraphicDev, 
												const _ushort& wContainerIdx, 
												wstring pTextureTag, 
												Engine::CTexture::TEXTURETYPE eType, 
												wstring pPath, 
												const _uint& iCnt /*= 1*/)
{
	if(nullptr == m_pmapResources)
	{
		ERR_BOX(L"Resource Reserve Failed");
		return E_FAIL;
	}

	CResources*		pResources = Find_Resources(wContainerIdx, pTextureTag);

	if(nullptr != pResources)
		return E_FAIL;

	pResources = CTexture::Create(pGraphicDev, pPath, eType, iCnt);
	NULL_CHECK_RETURN(pResources, E_FAIL);

	m_pmapResources[wContainerIdx].insert(MAPRESOURCE::value_type(pTextureTag, pResources));

	return S_OK;
}

HRESULT CResourcesMgr::Ready_Mesh(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort & wContainerIdx, wstring pMeshTag, Engine::CMesh::MESHTYPE eType, wstring pFilePath, wstring pFileName, int iFlage)
{
	NULL_CHECK_RETURN_MSG(m_pmapResources, E_FAIL, L"Mesh Container reserve failed");

	CResources*		pResources = Find_Resources(wContainerIdx, pMeshTag);

	if (nullptr != pResources)
		return E_FAIL;

	switch (eType)
	{
	case CMesh::TYPE_STATIC:
		pResources = CStaticMesh::Create(pGraphicDev, pFilePath, pFileName);
		break;

	case CMesh::TYPE_DYNAMIC:
		pResources = CDynamicMesh::Create(pGraphicDev, pFilePath, pFileName);
		break;

	case CMesh::TYPE_NAVI:
		pResources = CNaviMesh::Create(pGraphicDev, iFlage);
		break;
	}

	NULL_CHECK_RETURN(pResources, E_FAIL);

	m_pmapResources[wContainerIdx].insert(MAPRESOURCE::value_type(pMeshTag, pResources));

	return S_OK;
}
