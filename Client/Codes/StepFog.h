#ifndef StepFog_h__
#define StepFog_h__

#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CTransform;
	class CRenderer;
	class CRcTex;
	class CTexture;
	class CShader;
}

class CStepFog : public Engine::CGameObject
{
private:
	explicit					CStepFog(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual						~CStepFog();
	
private:
	HRESULT						Add_Component();

public:
	virtual	HRESULT				Ready_Object(_matrix* pmatPos, _vec3* pDir);
	virtual Engine::_int		Update_Object(const Engine::_float& fTimeDelta);
	virtual	void				Render_Object();

private:
	HRESULT						SetUp_ConstantTable(LPD3DXEFFECT pEffect);

private:
	Engine::CTransform*			m_pTargetTransCom	= nullptr;
	Engine::CRenderer*			m_pRendererCom		= nullptr;
	Engine::CTexture*			m_pTextureCom		= nullptr;
	Engine::CRcTex*				m_pBufferCom		= nullptr;
	Engine::CShader*			m_pShaderCom		= nullptr;

	_float						m_fLifeTime			= 0.f;
	_vec3						m_vPos, m_vLook;
	bool						m_bTime				= false;

public:
	static CStepFog*			Create(LPDIRECT3DDEVICE9 pGraphicDev, _matrix* pmatPos, _vec3* pDir);

public:
	virtual void				Free() override;
};

#endif // StepFog_h__
