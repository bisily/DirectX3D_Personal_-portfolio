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
	class CShader;
	class COptimization;
}

class CTerrain : public Engine::CGameObject
{
private:
	explicit					CTerrain(LPDIRECT3DDEVICE9	pGraphicDev);
public:
	virtual						~CTerrain();

public:
	virtual	HRESULT				Ready_Object();
	virtual Engine::_int		Update_Object(const Engine::_float& fTimeDelta);
	virtual	void				Render_Object();

private:
	HRESULT						Add_Component();
	HRESULT						SetUp_ConstantTable(LPD3DXEFFECT pEffect);

private:
	Engine::CTransform*			m_pTransCom			= nullptr;
	
	Engine::CTexture*			m_pTextureCom		= nullptr;
	Engine::COptimization*		m_pOptimizationCom	= nullptr;

	Engine::CTerrainTex*		m_pBufferCom		= nullptr;
	Engine::CRenderer*			m_pRendererCom		= nullptr;
	Engine::CShader*			m_pShaderCom		= nullptr;
	
	Engine::INDEX32*			m_pIndex			= nullptr;
	_ulong						m_dwTriCnt			= 0;

public:
	static	CTerrain*			Create(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	virtual	void				Free();
};

#endif // Player_h__
