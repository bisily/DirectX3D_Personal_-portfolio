#ifndef LifeBar_h__
#define LifeBar_h__

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

class CLifeBar : public Engine::CGameObject
{
private:
	explicit				CLifeBar(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual					~CLifeBar();

public:
	virtual HRESULT			Ready_Object() override;
	virtual _int			Update_Object(const _float& fTimeDelta) override;
	virtual void			Render_Object() override;

private:
	HRESULT					Add_Component();

private:
	Engine::CRcTex*			m_pBufferCom		= nullptr;
	Engine::CTransform*		m_pTransCom			= nullptr;
	Engine::CTexture*		m_pTextureCom		= nullptr;
	Engine::CTexture*		m_pTextureHpBarCom	= nullptr;
	Engine::CRenderer*		m_pRendererCom		= nullptr;
	Engine::CObjState*		m_pTargetCom		= nullptr;

private:
	_matrix					m_matProj;
	_float					m_fX				= 0.f;
	_float					m_fY				= 0.f;
	_float					m_fSizeX			= 0.f;
	_float					m_fSizeY			= 0.f;
	_float					m_fSizeHpX			= 0.f;
	_tchar					m_szHp[128]			= L"";

public:
	static CLifeBar*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void			Free() override;
};

#endif // !LifeBar_h__
