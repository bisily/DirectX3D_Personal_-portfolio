#ifndef HierachyLoader_h__
#define HierachyLoader_h__

#include "Engine_Defines.h"

BEGIN(Engine)

class CHierachyLoader : public ID3DXAllocateHierarchy
{
private:
	explicit CHierachyLoader(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pPath);
	virtual ~CHierachyLoader(void);

public:
	STDMETHOD(Ready_Loader)(void);

public:
	STDMETHOD(CreateFrame)(THIS_ LPCSTR Name, LPD3DXFRAME* ppNewFrame);	// 뼈대를 만들어 주는 함수

	STDMETHOD(CreateMeshContainer)(THIS_ LPCSTR Name,
		CONST D3DXMESHDATA* pMeshData,
		CONST D3DXMATERIAL* pMaterials,
		CONST D3DXEFFECTINSTANCE* pEffectInstances,
		DWORD NumMaterials,
		CONST DWORD* pAdjacency,
		LPD3DXSKININFO pSkinInfo,
		LPD3DXMESHCONTAINER* ppNewContainer); // 살은 만들어주는 함수(메쉬를 저장하는 컨테이너)



	STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME pFrameToFree);			// 뼈대를 삭제하는 함수
	STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER pMeshContainerToFree); // 살을 삭제하는 함수

private:
	void	Allocate_Name(char** ppName, const char* pFrameName);

private:
	LPDIRECT3DDEVICE9			m_pGraphicDev = nullptr;
	const	_tchar*				m_pPath = nullptr;

public:
	static	CHierachyLoader*		Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pPath);
	_ulong	Release(void);

};

END
#endif // HierachyLoader_h__
