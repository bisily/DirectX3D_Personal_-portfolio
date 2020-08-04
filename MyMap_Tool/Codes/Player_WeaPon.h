#ifndef Player_WeaPon_h__
#define Player_WeaPon_h__

#include "Include.h"
#include "GameObject.h"

namespace Engine
{
	class CTransform;
	class CRenderer;
	class CCalculator;
	class CDynamicMesh;
	class CCollider;
	class CShader;
	class CObjState;
}

class CPlayer_WeaPon : public Engine::CGameObject
{
private:
	explicit CPlayer_WeaPon(LPDIRECT3DDEVICE9	pGraphicDev);
public:
	virtual	~CPlayer_WeaPon();

public:
	virtual	HRESULT				Ready_Object();
	virtual Engine::_int		Update_Object(const Engine::_float& fTimeDelta);
	virtual	void				Render_Object();

private:
	HRESULT						Add_Component();
	HRESULT						SetUp_ConstantTable(LPD3DXEFFECT pEffect);
private:
	Engine::CTransform*			m_pTransCom				= nullptr;
	Engine::CRenderer*			m_pRendererCom			= nullptr;
	Engine::CCalculator*		m_pCalculatorCom		= nullptr;
	Engine::CDynamicMesh*		m_pMeshCom				= nullptr;
	Engine::CCollider*			m_pColliderCom			= nullptr;
	Engine::CShader*			m_pShaderCom			= nullptr;
	Engine::CObjState*			m_pStateCom				= nullptr;

private:
	const Engine::_matrix*		m_pParentBoneMatrix				= nullptr;
	const Engine::_matrix*		m_pParentWorldMatrix			= nullptr;
	
public:
	static	CPlayer_WeaPon*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual	void	Free();
};

#endif // Player_WeaPon_h__
