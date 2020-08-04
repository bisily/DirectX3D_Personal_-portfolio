#ifndef SkyBox_h__
#define SkyBox_h__

#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CTexture;
	class CTransform;
	class CRenderer;
	class CCubeTex;
}

class CSkyBox : public Engine::CGameObject
{
private:
	explicit CSkyBox(LPDIRECT3DDEVICE9	pGraphicDev);
public:
	virtual	~CSkyBox(void);

public:
	virtual	HRESULT	Ready_Object(void);
	virtual Engine::_int	Update_Object(const Engine::_float& fTimeDelta);
	virtual	void	Render_Object(void);

private:
	HRESULT			Add_Component(void);

private:
	Engine::CTransform*			m_pTransCom = nullptr;
	Engine::CTexture*			m_pTextureCom = nullptr;
	Engine::CRenderer*			m_pRendererCom = nullptr;
	Engine::CCubeTex*			m_pBufferCom = nullptr;

public:
	static	CSkyBox*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual	void	Free(void);
};

#endif // !SkyBox_h__
