#ifndef Monster00Sword_h__
#define Monster00Sword_h__

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

class CMonster00Sword : public Engine::CGameObject
{
private:
	explicit CMonster00Sword(LPDIRECT3DDEVICE9	pGraphicDev);
public:
	virtual	~CMonster00Sword();

public:
	virtual	HRESULT					Ready_Object(wstring m_strName, Engine::_int iIndex);
	virtual Engine::_int			Update_Object(const Engine::_float& fTimeDelta);
	virtual	void					Render_Object();
	virtual Engine::_int			GetIndex() { return m_iIndex; }

private:
	HRESULT							Add_Component();
	HRESULT							Make_BoreNames(map<wstring, Engine::_float>* pmapBore);
	HRESULT							Make_CollisionSquare();
	void							Get_ParentBoneMatrix();

private:
	Engine::CTransform*				m_pTransCom				= nullptr;
	Engine::CRenderer*				m_pRendererCom			= nullptr;
	Engine::CCalculator*			m_pCalculatorCom		= nullptr;
	Engine::CDynamicMesh*			m_pMeshCom				= nullptr;
	Engine::CCollider*				m_pColliderCom			= nullptr;
	Engine::CShader*				m_pShaderCom			= nullptr;
	Engine::CObjState*				m_pStateCom				= nullptr;
	Engine::CObjState*				m_pStateParentCom		= nullptr;
	//Engine::CSphereCollider*		m_pSphereColliderCom	= nullptr;

private:
	const Engine::_matrix*			m_pParentBoneMatrix = nullptr;
	const Engine::_matrix*			m_pParentWorldMatrix = nullptr;
	const Engine::_matrix*			m_pMyBore = nullptr;

	int								m_iIndex = 0;
	wstring							m_strName;

public:
	static	CMonster00Sword*		Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring m_strName, Engine::_int iIndex);
private:
	virtual	void	Free();
};

#endif // Monster00Sword_h__
