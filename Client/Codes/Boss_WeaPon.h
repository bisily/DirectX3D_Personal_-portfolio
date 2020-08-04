#ifndef Boss_WeaPon_h__
#define Boss_WeaPon_h__

#include "Defines.h"
#include "GameObject.h"
#include "Boss.h"

namespace Engine
{
	class CTransform;
	class CRenderer;
	class CCalculator;
	class CDynamicMesh;
	class CCollider;
	class COptimization;
	class CObjState;
}

class CBoss_WeaPon : public Engine::CGameObject
{
private:
	explicit					CBoss_WeaPon(LPDIRECT3DDEVICE9	pGraphicDev);
public:
	virtual						~CBoss_WeaPon();

public:
	virtual	HRESULT				Ready_Object();
	virtual _int				Update_Object(const _float& fTimeDelta);
	virtual	void				Render_Object();
	void						Set_Dead() { bIs_Dead = true; }

private:
	HRESULT						Add_Component();
	void						Fix_Pos(_matrix* pmatPos);
	void						Get_Object();
	void						Check_Target();
	HRESULT						Make_CollisionSquare();
	
private:
	Engine::CTransform*			m_pTransCom				= nullptr;
	Engine::CRenderer*			m_pRendererCom			= nullptr;
	Engine::CCalculator*		m_pCalculatorCom		= nullptr;
	Engine::CDynamicMesh*		m_pMeshCom				= nullptr;
	Engine::CCollider*			m_pColliderCom			= nullptr;
	Engine::COptimization*		m_pOptimizationCom		= nullptr;
	Engine::CObjState*			m_pStateCom				= nullptr;

private:
	const _matrix*				m_pParentBoneMatrix		= nullptr;
	const _matrix*				m_pParentWorldMatrix	= nullptr;
	CBoss*						m_pBoss					= nullptr;
	
	Engine::_bool				m_bIsDraw				= false;
	bool						bIs_Dead				= false;

public:
	static	CBoss_WeaPon*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual	void				Free();
};

#endif // Boss_WeaPon_h__
