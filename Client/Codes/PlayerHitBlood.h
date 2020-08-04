#ifndef Player_HitBlood_h__
#define Player_HitBlood_h__

#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CRcTex;
	class CTransform;
	class CTexture;
	class CRenderer;
	class CShader;
}

class CPlayer_HitBlood : public Engine::CGameObject
{
private:
	explicit					CPlayer_HitBlood(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual						~CPlayer_HitBlood();

public:
	virtual HRESULT				Ready_Object(_vec3 vPos, _int iTexIndex);
	virtual _int				Update_Object(const _float& fTimeDelta) override;
	virtual void				Render_Object() override;

private:
	HRESULT						Add_Component();
	void						Billboard(_matrix* pmatWorld);

private:
	Engine::CRcTex*				m_pBufferCom		= nullptr;
	Engine::CTexture*			m_pTextureCom[4]	= { nullptr, nullptr, nullptr, nullptr };
	Engine::CRenderer*			m_pRendererCom		= nullptr;
	Engine::CTransform*			m_pTransCom			= nullptr;
	Engine::CShader*			m_pShaderCom		= nullptr;

private:
	_matrix						m_matProj;
	_float						m_fViewTime			= 0.f;
	_int						m_iTexIndex			= 0;
	bool						m_bKillTime			= false;

	_float						m_fAlpha			= 255.f;
	_int						m_iX				= 0;
	_int						m_iY				= 0;
	_vec3						m_vPos;

	_float						m_fScale			= 2.0f;
	_float						m_fDelay			= 50.f;
	_float						m_fAlphaDelay		= 20.f;
	_float						m_fYcorrection		= 2.f;

public:
	static CPlayer_HitBlood*	Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos, _int iTexIndex);
	virtual void				Free() override;
};

#endif // !Player_HitBlood_h__
