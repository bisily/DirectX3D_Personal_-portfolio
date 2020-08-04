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
}

class CMonster01Arrow : public Engine::CGameObject
{
private:
	explicit CMonster01Arrow(LPDIRECT3DDEVICE9	pGraphicDev);
public:
	virtual	~CMonster01Arrow();

public:
	virtual	HRESULT					Ready_Object();
	virtual Engine::_int			Update_Object(const Engine::_float& fTimeDelta);
	virtual	void					Render_Object();
	virtual Engine::_int			GetIndex() { return m_iIndex; }

private:
	HRESULT							Add_Component();
	void							Get_ParentBoneMatrix();
	void							Collision_Player();
	void							Angle_Chekc();

private:
	Engine::CTransform*				m_pTransCom			= nullptr;
	Engine::CRenderer*				m_pRendererCom		= nullptr;
	Engine::CCalculator*			m_pCalculatorCom	= nullptr;
	Engine::CCollider*				m_pColliderCom		= nullptr;
	Engine::CShader*				m_pShaderCom		= nullptr;
	Engine::CStaticMesh*			m_pMeshCom			= nullptr;

private:
	const Engine::_matrix*			m_pParentBoneMatrix = nullptr;
	const Engine::_matrix*			m_pParentWorldMatrix = nullptr;
	const Engine::_matrix*			m_pMyBore = nullptr;

	Engine::_bool					m_bIsDraw = false;

	int								m_iIndex = 0;
	wstring							m_strName;
	Engine::_float							m_fSpeed = 0.f;
	bool							m_bFire = false;
	bool							m_bPosSet = false;
	bool							m_bSetAngle = false;
	Engine::_vec3					vTargetLook;
	Engine::_vec3					vSelfPos;
	Engine::_float					fHeight = 0.f;
	Engine::_float					fLookAngle = 0.f;
	
public:
	static	CMonster01Arrow*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual	void	Free();
};

#endif // Monster01Arrow_h__
