#ifndef Tool_Boss_WeaPon_h__
#define Tool_Boss_WeaPon_h__

#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CTransform;
	class CRenderer;
	class CCalculator;
	class CDynamicMesh;
	class CCollider;
}

class CTool_Boss_WeaPon : public Engine::CGameObject
{
private:
	explicit CTool_Boss_WeaPon(LPDIRECT3DDEVICE9	pGraphicDev);
public:
	virtual	~CTool_Boss_WeaPon();

public:
	virtual	HRESULT	Ready_Object();
	virtual Engine::_int	Update_Object(const Engine::_float& fTimeDelta);
	virtual	void	Render_Object(void);

private:
	HRESULT			Add_Component();

private:
	Engine::CTransform*			m_pTransCom = nullptr;
	Engine::CRenderer*			m_pRendererCom = nullptr;
	Engine::CCalculator*		m_pCalculatorCom = nullptr;
	Engine::CDynamicMesh*		m_pMeshCom = nullptr;
	Engine::CCollider*			m_pColliderCom = nullptr;

private:
	const Engine::_matrix*		m_pParentBoneMatrix = nullptr;
	const Engine::_matrix*		m_pParentWorldMatrix = nullptr;


public:
	static	CTool_Boss_WeaPon*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual	void	Free();
};

#endif // Tool_Boss_WeaPon_h__
