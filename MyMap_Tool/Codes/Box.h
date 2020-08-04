#ifndef Box_h__
#define Box_h__

#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CStaticMesh;
	class CTransform;
	class CRenderer;
	class CCalculator;
	class CCollider;
}

class CBox : public Engine::CGameObject
{
private:
	explicit CBox(LPDIRECT3DDEVICE9	pGraphicDev);
public:
	virtual	~CBox(void);

public:
	virtual	HRESULT	Ready_Object(void);
	virtual Engine::_int	Update_Object(const Engine::_float& fTimeDelta);
	virtual	void	Render_Object(void);

private:
	HRESULT			Add_Component(void);
	void			SetUp_OnTerrain(void);

private:
	Engine::CTransform*			m_pTransCom = nullptr;
	Engine::CStaticMesh*		m_pMeshCom = nullptr;
	Engine::CRenderer*			m_pRendererCom = nullptr;
	Engine::CCalculator*		m_pCalculatorCom = nullptr;
	Engine::CCollider*			m_pColliderCom = nullptr;

public:
	static	CBox*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual	void	Free(void);
};

#endif // !Box_h__
