#ifndef Renderer_h__
#define Renderer_h__

#include "Engine_Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CRenderer : public CComponent
{
	DECLARE_SINGLETON(CRenderer)

public:
	enum RENDERID	{ RENDER_PRIORITY, RENDER_NONALPHA, RENDER_ALPHATEST, RENDER_ALPHABLEND, RENDER_UI, RENDER_END };
private:
	explicit	CRenderer();
	virtual		~CRenderer();

public:
	void	Add_RenderGroup(RENDERID eGroup, CGameObject* pGameObject);
	void	Render_GameObject(LPDIRECT3DDEVICE9 pGraphicDev);
	HRESULT	Ready_Renderer(LPDIRECT3DDEVICE9 pGraphicDev);
	void	Render_ToolObject(LPDIRECT3DDEVICE9 pGraphicDev);

	void	Clear_RenderGroup();

private:
	void	Render_Priority(LPDIRECT3DDEVICE9 pGraphicDev);
	void	Render_NoneAlpha(LPDIRECT3DDEVICE9 pGraphicDev);
	void	Render_AlphaTest(LPDIRECT3DDEVICE9 pGraphicDev);
	void	Render_AlphaBlend(LPDIRECT3DDEVICE9 pGraphicDev);
	void	Render_Ui(LPDIRECT3DDEVICE9 pGraphicDev);

	void	Render_Defferd(LPDIRECT3DDEVICE9 pGraphicDev);
	void	Render_LightAcc(LPDIRECT3DDEVICE9 pGraphicDev);
	void	Render_Blend(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	typedef	list<CGameObject*>		RENDERGROUP;
	RENDERGROUP						m_RenderGroup[RENDER_END];

private:
	LPDIRECT3DVERTEXBUFFER9			m_pVB = nullptr;
	LPDIRECT3DINDEXBUFFER9			m_pIB = nullptr;

private:
	virtual		void		Free();
};

END
#endif // Renderer_h__
