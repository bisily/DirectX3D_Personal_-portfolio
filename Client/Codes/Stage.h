#ifndef Stage_h__
#define Stage_h__

#include "Defines.h"
#include "Scene.h"
#include "ObjectHeaders.h"

class CStage : public Engine::CScene
{
private:
	explicit		CStage(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual			~CStage();

public:
	virtual HRESULT	Ready_Scene();
	virtual int		Update_Scene(const float& fTimeDelta) override;
	virtual void	Render_Scene() override;

private:
	HRESULT			Ready_Layer_GameLogic(const Engine::_tchar* pLayerTag);
	HRESULT			SetUp_DefaultSetting();
	HRESULT			Ready_LightInfo();
	
	HRESULT			Load_StaticObject(Engine::CLayer* pLayer, Engine::CGameObject* pGameObject);
	HRESULT			Load_DynamicObject(Engine::CLayer* pLayer, Engine::CGameObject* pGameObject);
	HRESULT			LoadTerrainData(Engine::CLayer* pLayer, Engine::CGameObject* pGameObject);

public:
	static CStage*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void	Free() override;

};

#endif // !Stage_h__
