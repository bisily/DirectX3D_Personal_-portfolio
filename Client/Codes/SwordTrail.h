#ifndef SwordTrail_h__
#define SwordTrail_h__

#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CTransform;
	class CRenderer;
	class CTrailTex;
	class CShader;
	class CTexture;
	class CObjState;
}

class CSwordTrail : public Engine::CGameObject
{
private:
	explicit						CSwordTrail(LPDIRECT3DDEVICE9	pGraphicDev);
public:
	virtual							~CSwordTrail();

public:
	virtual	HRESULT					Ready_Object(wstring strObjKey, wstring strWeaponName, wstring strCloneTexFile, _int iIndex = 0);
	virtual _int					Update_Object(const _float& fTimeDelta);
	virtual	void					Render_Object();

	void							SetDead() { m_bDead = true; }

private:
	HRESULT							Add_Component(wstring strCloneTexFile);
	void							PointMemorize(const _float & fTimeDelta);
	void							PointMemorizeWave(const _float & fTimeDelta);

private:
	Engine::CTransform*				m_pTransCom				= nullptr;
	Engine::CRenderer*				m_pRendererCom			= nullptr;

	Engine::CShader*				m_pShaderCom			= nullptr;

	Engine::CTrailTex*				m_pBufferCom			= nullptr;
	Engine::CTexture*				m_pTextureCom			= nullptr;

	Engine::CObjState*				m_pPlayerStateCom		= nullptr;

	const _matrix*					m_pParentWorldMatrix	= nullptr;
	_float							m_fAccTime				= 0.f;
	list<_vec3>						m_Pointlist;
	_float							m_fLength				= 0.f;
	wstring							m_strObjkey, m_strWeaponName, m_strCloneTexFile;

	_int							m_iIndex				= 0;
	bool							m_bDead					= false;

public:
	static	CSwordTrail*			Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring strObjKey, wstring strWeaponName, wstring strColneTexFile, _int iIndex = 0);
private:
	virtual	void					Free();
};

#endif // SwordTrail_h__
