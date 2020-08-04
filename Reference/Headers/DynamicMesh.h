#ifndef DynamicMesh_h__
#define DynamicMesh_h__

#include "Mesh.h"
#include "Engine_Defines.h"
#include "AnimationCtrl.h"
#include "HierachyLoader.h"

BEGIN(Engine)

class ENGINE_DLL CDynamicMesh : public CMesh
{
private:
	explicit CDynamicMesh(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CDynamicMesh(const CDynamicMesh& rhs);
	virtual ~CDynamicMesh();

public:	//getter
	const D3DXFRAME_DERIVED*		Get_FrameByName(const char* pFrameName)
	{
		return	(D3DXFRAME_DERIVED*)D3DXFrameFind(m_pRootFrame, pFrameName);
	}

	_bool					Is_AnimationSetEnd() { return m_pAniCtrl->Is_AnimationSetEnd(); }
	_bool					Is_AniSetEnd() { return m_pAniCtrl->Is_AniSetEnd(); }

	D3DXFRAME*				Get_RootBore() { return m_pRootFrame; }
	
	_double					Get_Position();
	_double					Get_Period();
	_double					Get_AccTime();
	_double					Get_Percent();

public:
	HRESULT					Ready_Meshes(const _tchar* pFilePath, const _tchar* pFileName);
	void					Render_Mesh();
	void					Render_Mesh(LPD3DXEFFECT	pEffect);
	void					Set_AnimationSet(const _uint& iIdx);
	void					Play_AnimationSet(const _float& fTimeDelta);

public: //For.Tool
	vector<wstring>&		Get_BoreNames() { return m_vecBoreNames; }
	CAnimationCtrl*			Get_AnimationCtrl() { return m_pAniCtrl; }

private:
	// 모든 뼈들을 순회하며 뼈들이 가지고 있는 행렬정보를 갱신하는 함수

	void Update_FrameMatrices(D3DXFRAME_DERIVED* pFrame, const _matrix* pParentMatrix);
	void SetUp_FrameMatrices(D3DXFRAME_DERIVED* pFrame);

private:
	D3DXFRAME*			m_pRootFrame	= nullptr;
	CHierachyLoader*	m_pLoader		= nullptr;
	CAnimationCtrl*		m_pAniCtrl		= nullptr;

	typedef list<D3DXMESHCONTAINER_DERIVED*> MESHCONTANIERLIST;
	MESHCONTANIERLIST m_MeshContainerList;

	vector<wstring>		m_vecBoreNames;
public:
	static CDynamicMesh*	Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring pFilePath, wstring pFileName);
	virtual CResources*		Clone();
	virtual void			Free();

};

END
#endif // !DynamicMesh_h__
