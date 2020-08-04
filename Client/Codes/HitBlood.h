#ifndef HitBlood_h__
#define HitBlood_h__

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

class CHitBlood : public Engine::CGameObject
{
private:
	explicit				CHitBlood(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual					~CHitBlood();

public:
	virtual HRESULT			Ready_Object(_matrix* pmatPos);
	virtual _int			Update_Object(const _float& fTimeDelta) override;
	virtual void			Render_Object() override;

private:
	HRESULT					Add_Component();
	void					Billboard(_matrix* pmatWorld);
	HRESULT					SetUp_ConstantTable(LPD3DXEFFECT pEffect);

private:
	Engine::CRcTex*			m_pBufferCom	= nullptr;
	Engine::CTexture*		m_pTextureCom[4] = { nullptr, nullptr, nullptr, nullptr };
	Engine::CRenderer*		m_pRendererCom	= nullptr;
	Engine::CTransform*		m_pTransCom		= nullptr;
	Engine::CShader*		m_pShaderCom	= nullptr;

private:
	_float					m_fViewTime		= 0.f;
	_int					m_iTexIndex		= 0;
	bool					m_bKillTime		= false;
	_int					m_iArrayIndex	= 0;

public:
	static CHitBlood*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _matrix* pmatPos);
	virtual void			Free() override;
};

#endif // !HitBlood_h__
