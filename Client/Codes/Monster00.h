#ifndef Monster00_h__
#define Monster00_h__

#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CDynamicMesh;
	class CTransform;
	class CRenderer;
	class CCalculator;
	class CSphereCollider;
	class CObjState;
	class CShader;
	class COptimization;
	class CCollider;
}

class CMonster00 : public Engine::CGameObject
{
private:
	explicit					CMonster00(LPDIRECT3DDEVICE9	pGraphicDev);
public:
	virtual						~CMonster00();

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
	_int						Get_AniIndex() { return m_iAniIndex; }

	vector<Engine::CCollider*>* Get_vecColl() { return &m_vecCollider; }

private:
	HRESULT						Add_Component(wstring _tagMeshName, int _iObjIndex);
	HRESULT						SetUp_ConstantTable(LPD3DXEFFECT pEffect);
	HRESULT						Make_BoreNames(map<wstring, _float>* pmapBore);
	void						Get_Target();
	void						Search_Length(const _float& fTimeDelta);
	_vec3						Target_Look(_vec3* vecDest, const _float& fTimeDelta);
	void						Calc_Render();
	void						Check_Nan();
	HRESULT						Make_SquareCollision();
	void						Push_Overlap(const _float& fTimeDelta);
	void						Calc_AttackAngle();
	void						Check_Hit(const Engine::_float& fTimeDelta);
	void						Push_Player();

private:
	Engine::CTransform*			m_pTransCom			= nullptr;
	Engine::CDynamicMesh*		m_pMeshCom			= nullptr;
	Engine::CRenderer*			m_pRendererCom		= nullptr;
	Engine::CCalculator*		m_pCalculatorCom	= nullptr;
	Engine::CSphereCollider*	m_pSphereCollider	= nullptr;
	Engine::CObjState*			m_pStateCom			= nullptr;
	Engine::CShader*			m_pShaderCom		= nullptr;
	Engine::CShader*			m_pShaderAlphaCom	= nullptr;
	Engine::COptimization*		m_pOptimizationCom	= nullptr;

	Engine::CTransform*			m_pTargetCom		= nullptr;
	
	int							m_iIndex;
	int							m_iAniIndex			= 0;
	wstring						m_strName;
	_double						m_Time				= 0.0;
	_float						m_dHitDelay			= 0.f;
	_float						m_fAttackCoolTime	= 0.f;
	_float						m_fAttactAngle		= 0.f;
	bool						m_bAttackCoolTime	= false;
	Engine::_bool				m_bIsDraw			= false;

	bool						m_bAttack			= false;
	bool						m_bAttack_Revert	= false;
	_float						fHeight				= 0.f;
	_vec3						OriginPos;
	_vec3						OriginRight;
	_vec3						OriginvUp;
	_vec3						OriginAngle;

	bool						m_bHitEffect		= false;
	
	vector<wstring>				m_vecBoreName;
	vector<Engine::CCollider*>	m_vecCollider;

	_vec3						vCollPos			= _vec3(0.f, 0.f, 0.f);
	_int						m_iState			= 0;
	_float						m_fMoveTime			= 0.f;
	bool						m_bMovePattern		= false;

	_float						m_vOriginHeight		= 0.f;
	_float						m_fAttackLang		= 5.f;
	_float						m_fMaxSearch		= 10.f;
	_float						m_fMinSearch		= 2.5f;

	_float						fOriginX			= 0.1f;
	_float						fOriginY			= 0.f;

	_int						animationIndex[6]			= { 0,1,2,3,4,5 };
	_float						fDelay = 0.f;
	_float						fCheckDelay = 0.3f;
	_int						iZero = 0;
	_float						fPushLang = 1.f;
	_float						fRotateSpeef = 3.5f;

public:
	static CMonster00*			Create(LPDIRECT3DDEVICE9 _pGraphicDev
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

#endif // Monster00_h__
