#ifndef Stage_h__
#define Stage_h__

#include "Defines.h"
#include "Scene.h"

class CStage : public Engine::CScene
{
private:
	explicit CStage(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CStage(void);

public:
	virtual HRESULT		Ready_Scene(void);
	virtual int Update_Scene(const float& fTimeDelta) override;
	virtual void Render_Scene(void) override;

private:
	HRESULT		Ready_Layer_GameLogic(const TCHAR* pLayerTag);
	HRESULT		Ready_Resources(void);

public:
	static CStage*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void Free(void) override;

};


#endif // Stage_h__
