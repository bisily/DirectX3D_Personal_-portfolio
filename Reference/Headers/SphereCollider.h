#ifndef SphereCollider_h__
#define SphereCollider_h__

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CSphereCollider : public CComponent
{
	typedef	vector<SPHERE_COL>		VEC_SPHERE_COL;
	typedef map<wstring, _float>	MAP_BORENAMES;

private:
	explicit						CSphereCollider(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual							~CSphereCollider();

public:
	VEC_SPHERE_COL					Get_SphereList() { return m_vecSphere; }

public:
	HRESULT							Ready_SphereCollider(D3DXMATRIX* matWorld, D3DXFRAME* pRootFrame, MAP_BORENAMES* pBoreNames);
	void							Render_SphereCollider();

private:
	LPDIRECT3DDEVICE9				m_pGraphicDev	= nullptr;
	LPDIRECT3DTEXTURE9				m_pTexture;

	D3DXMATRIX*						m_matWorld;
	VEC_SPHERE_COL					m_vecSphere;
	MAP_BORENAMES					m_mapBoreNames;
	D3DXFRAME*						m_pRootFrame	= nullptr;

public:
	static CSphereCollider*			Create(LPDIRECT3DDEVICE9 pGraphicDev,
										D3DXFRAME* pRootFrame,
										MAP_BORENAMES* pBoreNames,
										D3DXMATRIX* matWorld);

	virtual void					Free();
};


END
#endif // SphereCollider_h__
