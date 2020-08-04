#ifndef AnimationCtrl_h__
#define AnimationCtrl_h__

#include "Engine_Defines.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CAnimationCtrl : public CBase
{
private:
	explicit CAnimationCtrl(LPD3DXANIMATIONCONTROLLER pAniCtrl);
	explicit CAnimationCtrl(const CAnimationCtrl& rhs);
	virtual ~CAnimationCtrl();

public:
	LPD3DXANIMATIONCONTROLLER		Get_AnimationCtrl() { return m_pAniCtrl; }

public:
	HRESULT							Ready_AnimationCtrl();
	void							Set_AnimationSet(const _uint& iIdx);
	void							Play_AnimationSet(const _float& fTimeDelta);
	_bool							Is_AnimationSetEnd();
	_bool							Is_AniSetEnd();

	_double							Get_Position();
	_double							Get_Period() { return m_dPeriod; }
	_double							Get_AccTime() { return m_fAccTime; }
	_double							Get_Percent();

private:
	LPD3DXANIMATIONCONTROLLER		m_pAniCtrl = nullptr;
	_uint							m_iCurrentTrack = 0;
	_uint							m_iNewTrack = 0;
	_float							m_fAccTime = 0.f;
	_uint							m_iOldAniIdx = 0;
	_double							m_dPeriod = 0.0;

public:
	static	 CAnimationCtrl*		Create(LPD3DXANIMATIONCONTROLLER pAniCtrl);
	static	 CAnimationCtrl*		Create(const CAnimationCtrl& rhs);
	virtual void					Free();
};

END
#endif // AnimationCtrl_h__
