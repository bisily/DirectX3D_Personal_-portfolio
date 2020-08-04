#ifndef Monster01Bow_h__
#define Monster01Bow_h__

#include "Defines.h"
#include "GameObject.h"

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
}

class CMonster01Bow : public Engine::CGameObject
{
private:
	explicit						CMonster01Bow(LPDIRECT3DDEVICE9	pGraphicDev);
public:
	virtual							~CMonster01Bow();

public:
	virtual	HRESULT					Ready_Object(wstring m_strName, _int iIndex);
	virtual _int					Update_Object(const _float& fTimeDelta);
	virtual	void					Render_Object();
	virtual _int					GetIndex() { return m_iIndex; }

private:
	HRESULT							Add_Component();
	HRESULT							SetUp_ConstantTable(LPD3DXEFFECT pEffect);
	void							Get_ParentBoneMatrix();

private:
	Engine::CTransform*				m_pTransCom				= nullptr;
	Engine::CRenderer*				m_pRendererCom			= nullptr;
	Engine::CCalculator*			m_pCalculatorCom		= nullptr;
	Engine::CStaticMesh*			m_pMeshCom				= nullptr;
	Engine::CCollider*				m_pColliderCom			= nullptr;
	Engine::CShader*				m_pShaderCom			= nullptr;
	Engine::CObjState*				m_pStateParentCom		= nullptr;
	Engine::COptimization*			m_pOptimizationCom		= nullptr;

	const _matrix*					m_pParentBoneMatrix		= nullptr;
	const _matrix*					m_pParentWorldMatrix	= nullptr;
	const _matrix*					m_pMyBore				= nullptr;

	Engine::_bool					m_bIsDraw				= false;

	int								m_iIndex				= 0;
	wstring							m_strName;

public:
	static	CMonster01Bow*			Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring m_strName, _int iIndex);
private:
	virtual	void					Free();
};

#endif // Monster01Bow_h__
