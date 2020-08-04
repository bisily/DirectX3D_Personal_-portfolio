#ifndef SkyBox_h__
#define SkyBox_h__

#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CStaticMesh;
	class CTransform;
	class CRenderer;
	class CShader;
}

class CSkyBox : public Engine::CGameObject
{
private:
	explicit					CSkyBox(LPDIRECT3DDEVICE9	pGraphicDev);
public:
	virtual						~CSkyBox();

public:
	virtual	HRESULT				Ready_Object();
	virtual _int				Update_Object(const _float& fTimeDelta);
	virtual	void				Render_Object();

private:
	HRESULT						Add_Component();
	HRESULT						SetUp_ConstantTable(LPD3DXEFFECT pEffect);

private:
	Engine::CTransform*			m_pTransCom			= nullptr;
	Engine::CStaticMesh*		m_pMeshCom			= nullptr;
	Engine::CStaticMesh*		m_pMeshBottomCom	= nullptr;
	Engine::CRenderer*			m_pRendererCom		= nullptr;
	Engine::CShader*			m_pShaderCom		= nullptr;

public:
	static	CSkyBox*			Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual	void				Free();
};

#endif // SkyBox_h__
