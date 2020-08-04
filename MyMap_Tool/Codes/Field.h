#ifndef Field_h__
#define Field_h__

#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CRcTerrain;
	class CResourcesMgr;
	class CTimeMgr;

	class CTexture;
	class CVIBuffer;
	class CTransform;
	class CTerrainTex;
	class CRenderer;
}

class CField : public Engine::CGameObject
{
private:
	explicit CField(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	~CField();

public:
	virtual HRESULT		Ready_Object(void);
	virtual int			Update_Object(const float& fTimeDelta);
	virtual void		Render_Object(void);

	void	Release();
	static CField*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	const Engine::VTXTEX*		GetTerrainVertex(void) { return m_pVertex; }

private:
	HRESULT	Initialize();
	HRESULT	AddComponent();

private:
	Engine::CResourcesMgr*		m_pResourcesMgr = nullptr;
	Engine::CTimeMgr*			m_pTimeMgr = nullptr;

	Engine::CTexture*			m_pTexture = nullptr;
	Engine::CTerrainTex*		m_pBuffer = nullptr;

	Engine::CTransform*			m_pInfo = nullptr;
	Engine::CRenderer*			m_pRendererCom = nullptr;

private:
	Engine::VTXTEX*				m_pVertex = nullptr;
	Engine::VTXTEX*				m_pConvertVertex = nullptr;
	DWORD						m_dwVtxCnt = 0;

private:
	float						m_fSpeed = 0.f;
};

#endif // !Field_h__
