#ifndef Terrain_h__
#define Terrain_h__

#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CRenderer;
	class CTexture;
	class CTransform;
	class CTerrainTex;
}

class CTerrain : public Engine::CGameObject
{
private:
	explicit CTerrain(LPDIRECT3DDEVICE9	pGraphicDev);
public:
	virtual	~CTerrain();

public:
	virtual	HRESULT			Ready_Object();
	virtual Engine::_int	Update_Object(const Engine::_float& fTimeDelta);
	virtual	void			Render_Object(void);
	void					SetRenderState(bool bValue) { bWireframe = bValue; }

private:
	HRESULT			Add_Component();
	HRESULT			SetUp_Material();

private:
	Engine::CTransform*			m_pTransCom = nullptr;
	Engine::CTexture*			m_pTextureCom = nullptr;
	Engine::CTerrainTex*		m_pBufferCom = nullptr;
	Engine::CRenderer*			m_pRendererCom = nullptr;

	bool						bWireframe = false;

public:
	static	CTerrain*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	virtual	void	Free(void);
};

#endif // Player_h__
