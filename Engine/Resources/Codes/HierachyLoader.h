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
	STDMETHOD(CreateFrame)(THIS_ LPCSTR Name, LPD3DXFRAME* ppNewFrame);	// ���븦 ����� �ִ� �Լ�

	STDMETHOD(CreateMeshContainer)(THIS_ LPCSTR Name,
		CONST D3DXMESHDATA* pMeshData,
		CONST D3DXMATERIAL* pMaterials,
		CONST D3DXEFFECTINSTANCE* pEffectInstances,
		DWORD NumMaterials,
		CONST DWORD* pAdjacency,
		LPD3DXSKININFO pSkinInfo,
		LPD3DXMESHCONTAINER* ppNewContainer); // ���� ������ִ� �Լ�(�޽��� �����ϴ� �����̳�)



	STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME pFrameToFree);			// ���븦 �����ϴ� �Լ�
	STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER pMeshContainerToFree); // ���� �����ϴ� �Լ�

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
