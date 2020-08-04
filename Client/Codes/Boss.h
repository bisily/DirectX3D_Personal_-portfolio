#ifndef Boss_h__
#define Boss_h__

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

class CBoss : public Engine::CGameObject
{
private:
	explicit					CBoss(LPDIRECT3DDEVICE9	pGraphicDev);
public:
	virtual						~CBoss();

public:
	virtual HRESULT				Ready_Object(wstring _tagMeshName
										, Engine::_vec3 * _temp_Right
										, Engine::_vec3 * _temp_Up
										, Engine::_vec3 * _temp_Look
										, Engine::_vec3 * _temp_Pos
										, Engine::_vec3 * _temp_Scale
										, Engine::_vec3 * _temp_Angle
										, int _iObjIndex);

	virtual Engine::_int		Update_Object(const Engine::_float& fTimeDelta);
	virtual	void				Render_Object();
	virtual _int				GetIndex() { return m_iIndex; }
	int							Get_AniIndex() { return m_iAniIndex; }

private:
	HRESULT						Add_Component();
	HRESULT						SetUp_ConstantTable(LPD3DXEFFECT pEffect);
	HRESULT						Make_BoreNames(map<wstring, _float>* pmapBore);
	void						Get_TargetCom();
	void						Patten(_float fDist, const Engine::_float& fTimeDelta);
	void						Calc_Render();
	void						Target_Look(_vec3 vDist, const Engine::_float& fTimeDelta);
	void						Is_Nan();
	void						Push_Player();
	HRESULT						Make_SquareCollision();

private:
	Engine::CTransform*			m_pTransCom			= nullptr;
	Engine::CDynamicMesh*		m_pMeshCom			= nullptr;
	Engine::CRenderer*			m_pRendererCom		= nullptr;
	Engine::CCalculator*		m_pCalculatorCom	= nullptr;
	Engine::CCollider*			m_pColliderCom		= nullptr;
	Engine::CNaviMesh*			m_pNaviMeshCom		= nullptr;
	Engine::CSphereCollider*	m_pSphereCollider	= nullptr;
	Engine::CObjState*			m_pStateCom			= nullptr;
	Engine::CTransform*			m_pTargetCom		= nullptr;
	Engine::CShader*			m_pShaderCom		= nullptr;
	Engine::COptimization*		m_pOptimizationCom	= nullptr;

	int							m_iIndex;
	int							m_iAniIndex			= 0;
	int							iValue				= 0;
	_float						m_fTimer			= 0.f;
	
	Engine::_bool				m_bIsDraw			= false;

	bool						m_bAttack5			= false;
	bool						m_bAttack5_Revert	= false;
	bool						m_bAttack7			= false;
	bool						m_bAttack7_Revert	= false;
	bool						m_bAttack4			= false;
	bool						m_bAttack4_Revert	= false;
	bool						m_bWaveAttact		= false;
	bool						bDead1				= false;
	bool						bDead2				= false;

	_float						fHeight				= 20.5f;
	_float						fAttackCoolTime		= 0.f;
	_float						m_dHitDelay			= 0.f;
	bool						bTargetRock			= false;
	bool						bAttack				= false;
	_vec3						OriginRight;
	_vec3						OriginvUp;
	_vec3						OriginvPos; 
	_vec3						OriginAngle;

	// 전방 3연타
	_float						m_fFrontAttact		= 0.8f;
	_float						m_fHight			= 0.f;
	_float						m_fFrontTimer		= 0.f;
							
	// 전방 휩쓸기
	_float						m_fFrotSweepAttact	= 1.f;
	_float						m_fSweepHight		= 0.f; 
	_float						m_fSweepTimer		= 0.f;

	// 점프 공격
	_float						m_fBeforeY			= 0.f;
	_float						m_fJumpHeiht		= 0.f;
	_float						m_fJumpTime			= 0.f;

	bool						m_fWaveAttact		= false;
	_int						m_iWaveCnt			= 1;

	vector<wstring>				m_vecBoreName;
	vector<Engine::CCollider*>	m_vecCollider;
	_vec3						vCollPos			= _vec3(0.f, 0.f, 0.f);
public:
	static CBoss*				Create(LPDIRECT3DDEVICE9 _pGraphicDev
										, wstring _tagMeshName
										, Engine::_vec3 * _temp_Right
										, Engine::_vec3 * _temp_Up
										, Engine::_vec3 * _temp_Look
										, Engine::_vec3 * _temp_Pos
										, Engine::_vec3 * _temp_Scale
										, Engine::_vec3 * _temp_Angle
										, int _iObjIndex);
private:
	virtual	void				Free();
};

#endif // Boss_h__
