#ifndef Monster01Arrow_h__
#define Monster01Arrow_h__

#include "Defines.h"
#include "GameObject.h"

class CMonster01;
namespace Engine
{
	class CTransform;
	class CRenderer;
	class CCalculator;
	class CStaticMesh;
	class CCollider;
	class CShader;
	class CObjState;
	class CSphereCollider;
	class COptimization;
	class CDynamicMesh;
	class CObjState;
}

class CMonster01Arrow : public Engine::CGameObject
{
private:
	explicit						CMonster01Arrow(LPDIRECT3DDEVICE9	pGraphicDev);
public:
	virtual							~CMonster01Arrow();

public:
	virtual	HRESULT					Ready_Object(_matrix* pmatPos, int iIndex);
	virtual _int					Update_Object(const _float& fTimeDelta);
	virtual	void					Render_Object();
	virtual _int					GetIndex() { return m_iIndex; }

private:
	HRESULT							Add_Component();
	void							Get_ParentBoneMatrix();
	void							Collision_Player();
	void							Angle_Chekc();

private:
	Engine::CTransform*				m_pTransCom				= nullptr;
	Engine::CRenderer*				m_pRendererCom			= nullptr;
	Engine::CCalculator*			m_pCalculatorCom		= nullptr;
	Engine::CStaticMesh*			m_pMeshCom				= nullptr;
	Engine::CCollider*				m_pColliderCom			= nullptr;
	Engine::CShader*				m_pShaderCom			= nullptr;
	Engine::COptimization*			m_pOptimizationCom		= nullptr;
	Engine::CDynamicMesh*			m_pParetMesh			= nullptr;
	Engine::CObjState*				m_pStateCom				= nullptr;
	CMonster01*						m_pParent				= nullptr;

	const _matrix*					m_pParentBoneMatrix		= nullptr;
	const _matrix*					m_pParentWorldMatrix	= nullptr;
	const _matrix*					m_pMyBore				= nullptr;

	Engine::_bool					m_bIsDraw				= false;

	int								m_iIndex				= 0;
	wstring							m_strName;
	_float							m_fSpeed				= 0.f;
	bool							m_bFire					= false;
	bool							m_bPosSet				= false;
	bool							m_bSetAngle				= false;
	_vec3							vTargetLook;
	_vec3							vSelfPos;
	_float							fHeight					= 0.f;
	_float							fLookAngle				= 0.f;
	bool							Is_bCollision			= false;

public:
	static	CMonster01Arrow*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _matrix* pmatPos, int iIndex);
private:
	virtual	void					Free();
};

#endif // Monster01Arrow_h__
