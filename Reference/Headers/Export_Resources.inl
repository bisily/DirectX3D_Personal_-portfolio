Engine::CResourcesMgr*	Get_ResourcesMgr()
{
	return Engine::CResourcesMgr::GetInstance();
}

Engine::CResources*	Engine::Clone_Resource(const _ushort& wContainerIdx, wstring pResourceTag)
{
	return Engine::CResourcesMgr::GetInstance()->Clone_Resource(wContainerIdx, pResourceTag);
}


HRESULT	 Engine::Reserve_ContainerSize(const Engine::_ushort& wSize)
{
	return Engine::CResourcesMgr::GetInstance()->Reserve_ContainerSize(wSize);
}

HRESULT	 Engine::Ready_Buffers(LPDIRECT3DDEVICE9 pGraphicDev,
	const Engine::_ushort& wContainerIdx,
	wstring pBufferTag,
	Engine::CVIBuffer::BUFFERID eID,
	const _int iFlag,
	const Engine::_ulong& dwCntX,
	const Engine::_ulong& dwCntZ,
	const Engine::_ulong& dwVtxItv,
	const Engine::_ulong& dwVtxDetail)
{
	return Engine::CResourcesMgr::GetInstance()->Ready_Buffers(pGraphicDev, wContainerIdx, pBufferTag, eID, iFlag, dwCntX, dwCntZ, dwVtxItv, dwVtxDetail);
}

HRESULT	Engine::Ready_Textures(LPDIRECT3DDEVICE9 pGraphicDev,
	const Engine::_ushort& wContainerIdx,
	wstring pTextureTag,
	Engine::CTexture::TEXTURETYPE eType,
	wstring pPath,
	const Engine::_uint& iCnt)
{
	return Engine::CResourcesMgr::GetInstance()->Ready_Textures(pGraphicDev, wContainerIdx, pTextureTag, eType, pPath, iCnt);
}

HRESULT Engine::Ready_Mesh(LPDIRECT3DDEVICE9 pGraphicDev,
	const Engine::_ushort& wContainerIdx,
	wstring pMeshTag,
	Engine::CMesh::MESHTYPE eType,
	wstring pFilePath,
	wstring pFileName,
	int iFlage)
{
	return Engine::CResourcesMgr::GetInstance()->Ready_Mesh(pGraphicDev, wContainerIdx, pMeshTag, eType, pFilePath, pFileName, iFlage);
}

void		Engine::Render_Buffer(const Engine::_ushort& wContainerIdx, wstring pBufferTag)
{
	Engine::CResourcesMgr::GetInstance()->Render_Buffer(wContainerIdx, pBufferTag);
}

void		Engine::Release_Resources(void)
{
	Engine::CResourcesMgr::GetInstance()->DestroyInstance();
}