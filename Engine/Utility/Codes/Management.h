#ifndef Management_h__
#define Management_h__

#include "Engine_Defines.h"
#include "Base.h"
#include "Scene.h"
#include "Shader.h"
#include "PrototypeMgr.h"

BEGIN(Engine)

class ENGINE_DLL CManagement : public CBase
{
	DECLARE_SINGLETON(CManagement)

private:
	explicit			CManagement();
	virtual				~CManagement();

public: // Getter
	CComponent*			Get_Component(const wstring		pLayerTag,
										const wstring		pObjTag,
										const wstring		pComponentTag,
										Engine::CComponent::COMPONENTID eID);

	CComponent*			Get_Component(const wstring		pLayerTag,
										const wstring		pObjTag,
										const wstring		pComponentTag,
										Engine::CComponent::COMPONENTID eID,
										_int iObjectIndex);


	CGameObject*		Get_Object(const wstring pLayerTag, const wstring pObjTag, _long iObjIndex = -1);

	CScene*				Get_CurrentScene() { return m_pCurrentScene; }



public: // Setter
	HRESULT				SetUp_CurrentScene(CScene* pCurrentScene);

public:
	HRESULT				Ready_Management(LPDIRECT3DDEVICE9 pGraphicDev);
	_int				Update_Scene(const _float& fTimeDelta);
	void				Render_Scene(LPDIRECT3DDEVICE9 pGraphicDev);
	void				Render_ToolScene(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	CScene*				m_pCurrentScene = nullptr;
public:
	virtual void		Free();
};

END
#endif // Management_h__
