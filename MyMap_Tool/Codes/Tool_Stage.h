#ifndef Tool_Stage_h__
#define Tool_Stage_h__

#include "Include.h"
#include "Scene.h"

#include "Terrain.h"
#include "DynamicCamera.h"
#include "SkyBox.h"
#include "Box.h"
#include "Player.h"
#include "Tool_StaticMesh.h"
#include "Tool_Monster00.h"
#include "Tool_Monster01.h"
#include "Tool_Boss.h"
#include "Tool_Boss_WeaPon.h"
#include "Navi.h"
#include "Player_WeaPon.h"
#include "Monster00Sword.h"
#include "Tool_Monster01.h"
#include "Monster01Arrow.h"
#include "Tool_Boss_Wave.h"

class CTool_Stage : public Engine::CScene
{
private:
	explicit CTool_Stage(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CTool_Stage();

public:
	virtual HRESULT		Ready_Scene(void);
	virtual int Update_Scene(const float& fTimeDelta) override;
	virtual void Render_Scene(void) override;

public:
	void Add_GameObject(wstring pLayerTag
						, wstring tagName
						, Engine::CComponent::COMPONENTID eID
						, int iIndex = 0
						, Engine::_vec3 vPos = Engine::_vec3(0.f, 0.f, 0.f)
						, Engine::_vec3 vScale = Engine::_vec3(0.01f, 0.01f, 0.01f));

	void Load_GameObject(wstring _pLayerTag
						, wstring _ptagName
						, Engine::CComponent::COMPONENTID _eID
						, Engine::_vec3 * _temp_Right
						, Engine::_vec3 * _temp_Up
						, Engine::_vec3 * _temp_Look
						, Engine::_vec3 * _temp_Pos
						, Engine::_vec3 * _temp_Scale
						, Engine::_vec3 * _temp_Angle
						, int _iObjIndex);
	void		Set_WireFrame(bool bState) { m_bWireFrame = bState; }

private:
	HRESULT		Ready_Layer_GameLogic(const Engine::_tchar* pLayerTag);
	HRESULT		SetUp_DefaultSetting(void);
	HRESULT		Ready_LightInfo(void);
	
private:
	bool		m_bWireFrame = false;

public:
	static CTool_Stage*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void Free(void) override;

};

#endif // !Tool_Stage_h__
