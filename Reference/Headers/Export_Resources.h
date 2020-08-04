#ifndef Export_Resources_h__
#define Export_Resources_h__

#include "ResourcesMgr.h"

BEGIN(Engine)

//ResourcesMgr Instance
inline CResourcesMgr*	Get_ResourcesMgr();

//ResourcesMgr Instance
//Getter
inline	CResources*	Clone_Resource(const _ushort& wContainerIdx, wstring pResourceTag);
//General
inline HRESULT	 Reserve_ContainerSize(const _ushort& wSize);

inline HRESULT	 Ready_Buffers(LPDIRECT3DDEVICE9 pGraphicDev,
								const _ushort& wContainerIdx,
								wstring pBufferTag,
								Engine::CVIBuffer::BUFFERID eID,
								const _int iFlag,
								const _ulong& dwCntX = 0,
								const _ulong& dwCntZ = 0,
								const _ulong& dwVtxItv = 1,
								const Engine::_ulong& dwVtxDetail = 20);

inline HRESULT	Ready_Textures(LPDIRECT3DDEVICE9 pGraphicDev,
								const _ushort& wContainerIdx,
								wstring pTextureTag,
								Engine::CTexture::TEXTURETYPE eType,
								wstring pPath,
								const _uint& iCnt = 1);

inline HRESULT	Ready_Mesh(LPDIRECT3DDEVICE9 pGraphicDev,
								const _ushort& wContainerIdx,
								wstring pMeshTag,
								Engine::CMesh::MESHTYPE eType,
								wstring pFilePath,
								wstring pFileName,
								int iFlage = 0);

inline void		Render_Buffer(const _ushort& wContainerIdx, wstring pBufferTag);

inline void		Release_Resources(void);

#include "Export_Resources.inl"

END
#endif // Export_Resources_h__
