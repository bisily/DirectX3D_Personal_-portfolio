#ifndef ResourcesMgr_h__
#define ResourcesMgr_h__

#include "Engine_Defines.h"
#include "Base.h"

// Buffer
#include "RcCol.h"
#include "RcTex.h"
#include "TerrainTex.h"
#include "CubeTex.h"
#include "TrailTex.h"

// Texture
#include "Texture.h"

// Mesh
#include "StaticMesh.h"
#include "DynamicMesh.h"
#include "NaviMesh.h"

BEGIN(Engine)

class CResources;
class ENGINE_DLL CResourcesMgr : public CBase
{
	DECLARE_SINGLETON(CResourcesMgr)

private:
	explicit CResourcesMgr(void);
	virtual ~CResourcesMgr(void);

public:
	HRESULT	 Reserve_ContainerSize(const _ushort& wSize);
	
	HRESULT	 Ready_Buffers(LPDIRECT3DDEVICE9 pGraphicDev, 
							const _ushort& wContainerIdx, 
							wstring pBufferTag, 
							Engine::CVIBuffer::BUFFERID eID,
							const _int iFlag,
							const _ulong& dwCntX = 0, 
							const _ulong& dwCntZ = 0,
							const _ulong& dwVtxItv = 1,
							const Engine::_ulong& dwVtxDetail = 20);
	
	HRESULT	Ready_Textures(LPDIRECT3DDEVICE9 pGraphicDev, 
							const _ushort& wContainerIdx, 
							wstring pTextureTag, 
							Engine::CTexture::TEXTURETYPE eType, 
							wstring pPath, 
							const _uint& iCnt = 1);

	HRESULT	Ready_Mesh(LPDIRECT3DDEVICE9 pGraphicDev,
						const _ushort& wContainerIdx,
						wstring pMeshTag,
						Engine::CMesh::MESHTYPE eType,
						wstring pFilePath,
						wstring pFileName,
						int iFlage = 0);


	void	Render_Buffer(const _ushort& wContainerIdx, wstring pBufferTag);

	CResources*	Clone_Resource(const _ushort& wContainerIdx, wstring pResourceTag);

	void	SetVtxInfo(RESOURCETYPE eResourceType, wstring pResourceKey, void* pVertex);

private:
	CResources*		Find_Resources(const _ushort& wContainerIdx, wstring pResourceTag);

private:
	map<wstring, CResources*>*			m_pmapResources = nullptr;
	typedef map<wstring, CResources*>  MAPRESOURCE;

	_uint										m_wMaxContainerSize = 0;

public:
	virtual void Free(void);

};

END
#endif // ResourcesMgr_h__
