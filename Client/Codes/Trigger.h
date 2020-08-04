#ifndef Trigger_h__
#define Trigger_h__

#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CTexture;
}

class CTrigger : public Engine::CGameObject
{
private:
	explicit						CTrigger(LPDIRECT3DDEVICE9	pGraphicDev);
public:
	virtual							~CTrigger();

public:
	virtual	HRESULT					Ready_Object();
	virtual Engine::_int			Update_Object(const Engine::_float& fTimeDelta);
	virtual	void					Render_Object();

private:
	HRESULT							Add_Component();
	void							Get_Object();

private:
	Engine::CTexture*				m_pTextureCom		= nullptr;
	Engine::SPHERE_COL				m_tagSphere;
	const _vec3*					m_pTargetPos		= nullptr;
	Engine::_bool					bClearCondition[4];
	_vec3							vecTriggerPos[3];
	_float							m_fAngle			= 0.f;

	vector<Engine::CComponent*>		m_vecMoveComponent;
	vector<Engine::CGameObject*>	vecBoss;
	
	const multimap<const wstring, Engine::CGameObject*>* m_mapObject;

public:
	static	CTrigger*				Create(LPDIRECT3DDEVICE9 pGraphicDev);
	

private:
	virtual	void					Free();
};

#endif // Trigger_h__
