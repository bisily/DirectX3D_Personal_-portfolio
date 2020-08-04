#ifndef Boss_Wave_h__
#define Boss_Wave_h__

#include "Defines.h"
#include "GameObject.h"
#include "Boss.h"

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

class CBoss_Wave : public Engine::CGameObject
{
private:
	explicit					CBoss_Wave(LPDIRECT3DDEVICE9	pGraphicDev);
public:
	virtual						~CBoss_Wave();

public:
	virtual	HRESULT				Ready_Object(_int iIndex, _vec3 vPos);
	virtual _int				Update_Object(const _float& fTimeDelta);
	virtual	void				Render_Object();
	void						Set_Dead() { bIs_Dead = true; }

private:
	HRESULT						Add_Component();
	void						Check_Target();
	
private:
	Engine::CTransform*			m_pTransCom			= nullptr;
	Engine::CRenderer*			m_pRendererCom		= nullptr;
	Engine::CCalculator*		m_pCalculatorCom	= nullptr;
	Engine::CStaticMesh*		m_pMeshCom			= nullptr;
	Engine::CCollider*			m_pColliderCom		= nullptr;
	Engine::CObjState*			m_pStateCom			= nullptr;

private:	
	bool						bIs_Dead			= false;
	_vec3						m_vDir;
	_float						m_fAcc				= 0.f;
	_float						fDrawTime			= 0.f;
	_int						m_iIndex			= 0;

public:
	static	CBoss_Wave*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _int iIndex, _vec3 vPos);

private:
	virtual	void				Free();
};

#endif // Boss_Wave_h__
