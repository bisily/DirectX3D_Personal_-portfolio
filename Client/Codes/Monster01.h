#ifndef Monster01_h__
#define Monster01_h__

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
	class CObjState;
	class CShader;
	class COptimization;
}

class CMonster01 : public Engine::CGameObject
{
private:
	explicit						CMonster01(LPDIRECT3DDEVICE9	pGraphicDev);
public:
	virtual							~CMonster01();

public:
	virtual HRESULT					Ready_Object(wstring _tagMeshName
										, _vec3 * _temp_Right
										, _vec3 * _temp_Up
										, _vec3 * _temp_Look
										, _vec3 * _temp_Pos
										, _vec3 * _temp_Scale
										, _vec3 * _temp_Angle
										, int _iObjIndex);

	virtual _int					Update_Object(const _float& fTimeDelta);
	virtual	void					Render_Object();
	virtual _int					GetIndex() { return m_iIndex; }
	_int							Get_AniIndex() { return m_iAniIndex; }

private:
	HRESULT							Add_Component(wstring _tagMeshName, int _iObjIndex);
	HRESULT							SetUp_ConstantTable(LPD3DXEFFECT pEffect);
	HRESULT							Make_BoreNames(map<wstring, _float>* pmapBore);
	void							Get_Target();
	void							Search_Target(const _float& fTimeDelta);
	_vec3							Target_Look(_vec3* vecDest);
	void							Calc_Render();
	void							Check_Nan();
	void							Push_Overlap(const _float& fTimeDelta);
	void							Push_Player();
	HRESULT							Make_SquareCollision();

private:
	Engine::CTransform*				m_pTransCom			= nullptr;
	Engine::CDynamicMesh*			m_pMeshCom			= nullptr;
	Engine::CRenderer*				m_pRendererCom		= nullptr;
	Engine::CCalculator*			m_pCalculatorCom	= nullptr;
	Engine::CCollider*				m_pColliderCom		= nullptr;
	Engine::CNaviMesh*				m_pNaviMeshCom		= nullptr;
	Engine::CSphereCollider*		m_pSphereCollider	= nullptr;
	Engine::CObjState*				m_pStateCom			= nullptr;
	Engine::CTransform*				m_pTargetCom		= nullptr;
	Engine::CShader*				m_pShaderCom		= nullptr;
	Engine::COptimization*			m_pOptimizationCom	= nullptr;

	Engine::_bool					m_bIsDraw = false;
	
	int								m_iIndex;
	int								m_iAniIndex			= 0;
	wstring							m_strName;
	_float							m_fAttackTime		= 0.f;
	_float							m_dHitDelay			= 0.f;
	_float							fHeight				= 0.f;
	bool							m_bFire				= false;
	int								iCnt				= 0;
	int								iOriginCnt			= 0;
	_vec3							OriginPos;
	_vec3							OriginRight;
	_vec3							OriginvUp;
	_vec3							OriginAngle;

	vector<wstring>					m_vecBoreName;
	vector<Engine::CCollider*>		m_vecCollider;
	_vec3							vCollPos			= _vec3(0.f, 0.f, 0.f);

	bool							m_bHitEffect		= false;

	_float							m_Time				= 0.f;
	_float							m_vOriginHeight		= 0.f;

public:
	static CMonster01*				Create(LPDIRECT3DDEVICE9 _pGraphicDev
									, wstring _tagMeshName
									, _vec3 * _temp_Right
									, _vec3 * _temp_Up
									, _vec3 * _temp_Look
									, _vec3 * _temp_Pos
									, _vec3 * _temp_Scale
									, _vec3 * _temp_Angle
									, int _iObjIndex);
private:
	virtual	void					Free();
};

#endif // Monster01_h__
