#ifndef StaticMesh_h__
#define StaticMesh_h__

#include "Mesh.h"

BEGIN(Engine)

class ENGINE_DLL CStaticMesh : public CMesh
{
private:
	explicit CStaticMesh(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CStaticMesh(const CStaticMesh& rhs);
	virtual ~CStaticMesh();

public:
	_ulong				Get_Numvertices() const { return m_dwNumvertices; }
	_ulong				Get_Stride() const { return m_dwStride; }
	const _vec3*		Get_VtxPos() const { return m_pVtxPos; }


public:
	HRESULT				Ready_Meshs(wstring pFilePath, wstring pFileName);
	void				Render_Mesh();
	void				Render_Mesh(LPD3DXEFFECT pEffect);

private:
	_bool				Find_Alpha(const char* pFileName);

private:
	LPD3DXMESH			m_pMesh = nullptr;
	LPD3DXBUFFER		m_pAdjacency = nullptr;
	LPD3DXBUFFER		m_pSubsetBuffer = nullptr;
	D3DXMATERIAL*		m_pSubsets = nullptr;
	_ulong				m_dwSubsetCnt = 0;

	LPDIRECT3DTEXTURE9*	m_ppTextures = nullptr;

	LPD3DXMESH			m_pOriMesh = nullptr;
	_ulong				m_dwNumvertices = 0;
	_ulong				m_dwStride = 0;
	D3DXVECTOR3*		m_pVtxPos = nullptr;

public:
	static CStaticMesh*	Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring pFilePath, wstring pFileName);

public:
	virtual	CResources*	Clone();
	virtual void	   	Free();
};

END

#endif // !StaticMesh_h__
