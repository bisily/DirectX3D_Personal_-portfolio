#ifndef Logo_h__
#define Logo_h__

#include "Defines.h"
#include "Scene.h"
#include "Loading.h"

class CLogo : public Engine::CScene
{
private:
	explicit			CLogo(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	virtual				~CLogo();

public:
	virtual HRESULT		Ready_Scene();
	virtual _int		Update_Scene(const _float& fTimeDelta) override;
	virtual void		Render_Scene() override;

private:
	HRESULT				Ready_Layer_GameLogic(const _tchar* pLayerTag);
	HRESULT				Ready_Resources();
	HRESULT				SetUp_DefaultSetting();

private:
	CLoading*			m_pLoading		= nullptr;

public:
	static CLogo*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void		Free() override;

};

#endif // Logo_h__
