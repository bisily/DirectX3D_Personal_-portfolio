#ifndef Scene_h__
#define Scene_h__

#include "Engine_Defines.h"
#include "Base.h"
#include "Layer.h"

BEGIN(Engine)


class ENGINE_DLL CScene : public CBase
{
protected:
	explicit								CScene(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual									~CScene();

public: // Getter
	CComponent*								Get_Component(const wstring		pLayerTag,
															const wstring		pObjTag,
															const wstring		pComponentTag,
															Engine::CComponent::COMPONENTID eID);

	CComponent*								Get_Component(const wstring		pLayerTag,
															const wstring		pObjTag,
															const wstring		pComponentTag,
															Engine::CComponent::COMPONENTID eID,
															_int iObjectIndex);

	CGameObject*							Get_Object(const wstring pLayerTag, const wstring pObjTag, _long iObjIndex = -1);

	CLayer*									Get_Layer(const wstring pLayerTag);

public:
	virtual HRESULT							Ready_Scene();
	virtual _int							Update_Scene(const _float& fTimeDelta);
	virtual void							Render_Scene()PURE;

protected:
	LPDIRECT3DDEVICE9						m_pGraphicDev = nullptr;
	map<const wstring, CLayer*>				m_mapLayer;
	typedef	map<const wstring, CLayer*>		MAPLAYER;

public:
	virtual	void							Free();
};

END
#endif // Scene_h__
