#ifndef GameObject_h__
#define GameObject_h__

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CGameObject : public CBase
{
protected:
	explicit									CGameObject(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual										~CGameObject();

public: // Getter
	CComponent*									Get_Component(const wstring pComponentTag, 
															Engine::CComponent::COMPONENTID eID);

	CComponent*									Find_Component(const wstring pComponentTag, 
															Engine::CComponent::COMPONENTID eID);

	virtual _int								GetIndex() { return 0; }

	_float										Get_ViewZ() const { return m_fViewZ; }
	void										Compute_ViewZ(const _vec3* pPosition);
	
	virtual _bool								Get_IsHit() { return IsbHit; }
	virtual void								Set_Hit(_bool bHit) { IsbHit = bHit; }

public:
	virtual HRESULT								Ready_Object();
	virtual _int								Update_Object(const _float& fTimeDelta);
	virtual void								Render_Object();

protected:
	LPDIRECT3DDEVICE9							m_pGraphicDev = nullptr;
	
	typedef map<const wstring, CComponent*>		MAPCOMPONENT;
	MAPCOMPONENT								m_mapComponent[CComponent::ID_END];

	_float										m_fViewZ		= 0.f;
	_bool										IsbHit			= false;

public:
	virtual void								Free();
};

END
#endif // GameObject_h__
