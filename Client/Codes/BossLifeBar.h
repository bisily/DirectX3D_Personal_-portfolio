#ifndef BossLifeBar_h__
#define BossLifeBar_h__

#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CRcTex;
	class CTransform;
	class CTexture;
	class CRenderer;
	class CObjState;
}

class CBossLifeBar : public Engine::CGameObject
{
private:
	explicit					CBossLifeBar(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual						~CBossLifeBar();

public:
	virtual HRESULT				Ready_Object() override;
	virtual _int				Update_Object(const _float& fTimeDelta) override;
	virtual void				Render_Object() override;
	
	void						SetUp_Trigger(Engine::_bool bTrigger) { m_bTrigger = bTrigger; }

private:
	HRESULT						Add_Component();

private:
	Engine::CRcTex*				m_pBufferCom		= nullptr;
	Engine::CTransform*			m_pTransCom			= nullptr;
	Engine::CTexture*			m_pTextureCom		= nullptr;
	Engine::CTexture*			m_pTextureIconCom	= nullptr;
	Engine::CTexture*			m_pTextureTextCom	= nullptr;
	Engine::CTexture*			m_pTextureHpBarCom	= nullptr;
	Engine::CRenderer*			m_pRendererCom		= nullptr;
	Engine::CObjState*			m_pTargetCom		= nullptr;

	Engine::_bool				m_bTrigger			= false;
	_matrix						m_matProj;
	_float						m_fX				= 0.f; 
	_float						m_fY				= 0.f;
	_float						m_fSizeX			= 0.f;
	_float						m_fSizeY			= 0.f;
	_float						m_fSizeHpX			= 0.f;

public:
	static CBossLifeBar*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void				Free() override;
};

#endif // !BossLifeBar_h__
