#ifndef Player_h__
#define Player_h__

#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CDynamicMesh;
	class CTransform;
	class CRenderer;
	class CCalculator;
	class CCollider;
	class CCubeTex;
	class CNaviMesh;
}

class CPlayer : public Engine::CGameObject
{
private:
	explicit CPlayer(LPDIRECT3DDEVICE9	pGraphicDev);
public:
	virtual	~CPlayer(void);

public:
	virtual	HRESULT	Ready_Object(void);
	virtual Engine::_int	Update_Object(const Engine::_float& fTimeDelta);
	virtual	void	Render_Object(void);

	void	SetUp_Animation(Engine::_int iIndex);
	void	SetPlay_Animation(bool bPlay) { bPlayAnimation = bPlay; }
	void	SetUp_PlaySpeed(float fPlaySpeed) { m_fPlayrSpeed = fPlaySpeed; }

private:
	HRESULT			Add_Component(void);
	void			Key_Check(const Engine::_float& fTimeDelta);
	void			SetUp_OnTerrain(void);
	Engine::_vec3			PickUp_OnTerrain(void);

private:
	Engine::CTransform*			m_pTransCom			= nullptr;
	Engine::CDynamicMesh*		m_pMeshCom			= nullptr;
	Engine::CRenderer*			m_pRendererCom		= nullptr;
	Engine::CCalculator*		m_pCalculatorCom	= nullptr;
	Engine::CCollider*			m_pColliderCom		= nullptr;
	Engine::CCubeTex*			m_pColBuffer		= nullptr;
	Engine::CNaviMesh*			m_pNaviMeshCom		= nullptr;
	bool						bPlayAnimation		= false;	
	float						m_fPlayrSpeed		= 0.f;

public:
	static	CPlayer*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual	void	Free(void);
};

#endif // Player_h__
