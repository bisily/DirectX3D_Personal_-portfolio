#ifndef Tool_Boss_Wave_h__
#define Tool_Boss_Wave_h__

#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CTransform;
	class CRenderer;
	class CCalculator;
	class CStaticMesh;
	class CCollider;
	class COptimization;
	class CObjState;
}

class CTool_Boss_Wave : public Engine::CGameObject
{
private:
	explicit					CTool_Boss_Wave(LPDIRECT3DDEVICE9	pGraphicDev);
public:
	virtual						~CTool_Boss_Wave();

public:
	virtual	HRESULT				Ready_Object();
	virtual Engine::_int		Update_Object(const Engine::_float& fTimeDelta);
	virtual	void				Render_Object();
	void						Set_Dead() { bIs_Dead = true; }

private:
	HRESULT						Add_Component();
	HRESULT						Make_CollisionSquare();

private:
	Engine::CTransform*			m_pTransCom = nullptr;
	Engine::CRenderer*			m_pRendererCom = nullptr;
	Engine::CCalculator*		m_pCalculatorCom = nullptr;
	Engine::CStaticMesh*		m_pMeshCom = nullptr;
	Engine::CCollider*			m_pColliderCom = nullptr;
	Engine::CObjState*			m_pStateCom = nullptr;

private:	
	bool						bIs_Dead = false;
	Engine::_vec3				m_vDir;

public:
	static	CTool_Boss_Wave*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual	void				Free();
};

#endif // Tool_Boss_Wave_h__
