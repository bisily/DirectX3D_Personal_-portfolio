#ifndef Player_h__
#define Player_h__

#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CDynamicMesh;
	class CTransform;
	class CRenderer;
	class CCalculator;
	class CCollider;
	class CNaviMesh;
	class CSphereCollider;
	class CShader;
	class CObjState;
	class CLayer;
}


class CPlayer : public Engine::CGameObject
{
private:
	explicit					CPlayer(LPDIRECT3DDEVICE9	pGraphicDev);
public:
	virtual						~CPlayer();

public:
	virtual	HRESULT				Ready_Object();
	virtual Engine::_int		Update_Object(const Engine::_float& fTimeDelta);
	virtual	void				Render_Object();

	vector<Engine::CCollider*>* Get_Collider() { return &m_vecCollider; }
	vector<wstring>*			Get_BoreName() { return &m_vecBoreName; }

private:
	HRESULT						Add_Component();
	void						Key_Check(const Engine::_float& fTimeDelta);
	HRESULT						SetUp_ConstantTable(LPD3DXEFFECT pEffect);
	HRESULT						Make_BoreNames(map<wstring, _float>* pmapBore);
	HRESULT						Make_SquareCollision();
	void						Play_Animation(const Engine::_float& fTimeDelta);

private:
	Engine::CTransform*			m_pTransCom				= nullptr;
	Engine::CDynamicMesh*		m_pMeshCom				= nullptr;
	Engine::CRenderer*			m_pRendererCom			= nullptr;
	Engine::CCalculator*		m_pCalculatorCom		= nullptr;
	Engine::CNaviMesh*			m_pNaviMeshCom			= nullptr;
	Engine::CSphereCollider*	m_pSphereColliderCom	= nullptr;
	Engine::CShader*			m_pShadowCom			= nullptr;
	Engine::CShader*			m_pShaderCom			= nullptr;
	Engine::CObjState*			m_pStateCom				= nullptr;

	vector<Engine::CCollider*>	m_vecCollider;
	vector<wstring>				m_vecBoreName;
	vector<int>					m_vecCombo;
	int							iIndex					= 0;
	float						fTimer					= 0.f;
	_matrix						matMovePos;						
	_float						m_dHitDelay				= 0.f;
	_float						fRunSoundTime			= 0.f;

public:
	static	CPlayer*			Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual	void				Free();
};

#endif // Player_h__
