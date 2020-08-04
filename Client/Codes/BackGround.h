#ifndef BackGround_h__
#define BackGround_h__

#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CRcTex;
	class CTexture;
	class CTransform;
	class CRenderer;
	class CShader;
}

class CBackGround : public Engine::CGameObject
{
private:
	explicit					CBackGround(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual						~CBackGround();

public:
	virtual HRESULT				Ready_Object() override;
	virtual _int				Update_Object(const _float& fTimeDelta) override;
	virtual void				Render_Object() override;
	
private:
	HRESULT						Add_Component();

private:
	Engine::CRcTex*				m_pBufferCom	= nullptr;	
	Engine::CTransform*			m_pTransCom		= nullptr;
	Engine::CTexture*			m_pTextureCom	= nullptr;
	Engine::CRenderer*			m_pRendererCom	= nullptr;
	Engine::CShader*			m_pShaderCom	= nullptr;
		
	bool						m_bValue		= false;
	_float						m_fAlpha		= 1.f;
	_float						m_fTimer		= 0.f;
	_int						m_iIndex		= 0;

public:
	static CBackGround*			Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void				Free() override;

};


#endif // BackGround_h__
