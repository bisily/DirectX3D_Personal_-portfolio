#ifndef Player_WeaPon_h__
#define Player_WeaPon_h__

#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CTransform;
	class CRenderer;
	class CCalculator;
	class CDynamicMesh;
	class CCollider;
	class CShader;
	class CObjState;
	class CSphereCollider;
}

class CPlayer_WeaPon : public Engine::CGameObject
{
private:
	explicit						CPlayer_WeaPon(LPDIRECT3DDEVICE9	pGraphicDev);
public:
	virtual							~CPlayer_WeaPon();

public:
	virtual	HRESULT					Ready_Object();
	virtual _int					Update_Object(const _float& fTimeDelta);
	virtual	void					Render_Object();

private:
	HRESULT							Add_Component();
	HRESULT							SetUp_ConstantTable(LPD3DXEFFECT pEffect);
	HRESULT							Make_BoreNames(map<wstring, _float>* pmapBore);
	HRESULT							Make_CollisionSquare();
	void							Get_ParentBoneMatrix();
	void							BossCollision();
	void							MonsterCollision();

private:
	Engine::CTransform*				m_pTransCom						= nullptr;
	Engine::CRenderer*				m_pRendererCom					= nullptr;
	Engine::CCalculator*			m_pCalculatorCom				= nullptr;
	Engine::CDynamicMesh*			m_pMeshCom						= nullptr;
	Engine::CCollider*				m_pColliderCom					= nullptr;
	Engine::CShader*				m_pShaderCom					= nullptr;
	Engine::CObjState*				m_pStateCom						= nullptr;
	Engine::CObjState*				m_pStateParentCom				= nullptr;
	Engine::CSphereCollider*		m_pSphereColliderCom			= nullptr;

	const _matrix*					m_pParentBoneMatrix				= nullptr;
	const _matrix*					m_pParentWorldMatrix			= nullptr;
	const _matrix*					m_pMyBore						= nullptr;
	vector<Engine::CCollider*>		m_vecCollider;
	_float							fTriggerX = 192.f;

public:
	static	CPlayer_WeaPon*			Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual	void					Free();
};

#endif // Player_WeaPon_h__
