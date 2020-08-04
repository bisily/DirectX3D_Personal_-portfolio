#ifndef MonsterLifeBar_h__
#define MonsterLifeBar_h__

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

class CMonsterLifeBar : public Engine::CGameObject
{
private:
	explicit				CMonsterLifeBar(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual					~CMonsterLifeBar();

public:
	virtual HRESULT			Ready_Object(wstring strObjKey, _int iIndex = 0);
	virtual _int			Update_Object(const _float& fTimeDelta) override;
	virtual void			Render_Object() override;

	virtual _int			GetIndex() { return m_iIndex; }
	void					SetUp_Trigger(Engine::_bool bTrigger) { m_bTrigger = bTrigger; }
	void					SetUp_Dead() { m_bIsDead = true; }
private:
	HRESULT					Add_Component();
	void					Billboard(_matrix* pmatWorld, _matrix* pmatWorldHp);

private:
	Engine::CRcTex*			m_pBufferCom		= nullptr;
	Engine::CTexture*		m_pTextureCom		= nullptr;
	Engine::CTexture*		m_pTextureHpBarCom	= nullptr;
	Engine::CRenderer*		m_pRendererCom		= nullptr;
	Engine::CObjState*		m_pTargetStateCom	= nullptr;
	Engine::CTransform*		m_pTargetPosCom		= nullptr;

private:
	Engine::_bool			m_bTrigger			= false; 
	Engine::_bool			m_bRender			= false;
	Engine::_bool			m_bIsDead;
	_float					m_fViewTime			= 0.f;
	_float					m_fSizeX;
	_float					m_fSizeY;
	_matrix					m_matWorld;
	_matrix					m_matWorldHp;
	_int					m_iIndex			= 0;
	wstring					m_strObjKey;


public:
	static CMonsterLifeBar*	Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring strObjKey, _int iIndex = 0);
	virtual void			Free() override;
};

#endif // !MonsterLifeBar_h__
