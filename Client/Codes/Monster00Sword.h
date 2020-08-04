#ifndef Monster00Sword_h__
#define Monster00Sword_h__

#include "Defines.h"
#include "GameObject.h"
#include "Monster00.h"

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
	class COptimization;
}

class CMonster00Sword : public Engine::CGameObject
{
private:
	explicit						CMonster00Sword(LPDIRECT3DDEVICE9	pGraphicDev);
public:
	virtual							~CMonster00Sword();

public:
	virtual	HRESULT					Ready_Object(wstring m_strName, _int iIndex);
	virtual _int					Update_Object(const _float& fTimeDelta);
	virtual	void					Render_Object();
	virtual _int					GetIndex() { return m_iIndex; }

private:
	HRESULT							Add_Component();
	HRESULT							SetUp_ConstantTable(LPD3DXEFFECT pEffect);
	HRESULT							Make_BoreNames(map<wstring, _float>* pmapBore);
	HRESULT							Make_CollisionSquare();
	void							Get_ParentBoneMatrix();
	void							Check_Target();
	void							Fixed_Pos(_matrix* pmatPos);

private:
	Engine::CTransform*				m_pTransCom				= nullptr;
	Engine::CRenderer*				m_pRendererCom			= nullptr;
	Engine::CCalculator*			m_pCalculatorCom		= nullptr;
	Engine::CDynamicMesh*			m_pMeshCom				= nullptr;
	Engine::CCollider*				m_pColliderCom			= nullptr;
	Engine::CShader*				m_pShaderCom			= nullptr;
	Engine::CObjState*				m_pStateCom				= nullptr;
	Engine::CObjState*				m_pStateParentCom		= nullptr;
	Engine::COptimization*			m_pOptimizationCom		= nullptr;
	CMonster00*						m_pParent				= nullptr;

private:
	const _matrix*					m_pParentBoneMatrix		= nullptr;
	const _matrix*					m_pParentWorldMatrix	= nullptr;
	const _matrix*					m_pMyBore				= nullptr;
	
	Engine::_bool					m_bIsDraw				= false;

	_int							m_iIndex				= 0;
	wstring							m_strName;

public:
	static	CMonster00Sword*		Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring m_strName, _int iIndex);
private:
	virtual	void	Free();
};

#endif // Monster00Sword_h__
