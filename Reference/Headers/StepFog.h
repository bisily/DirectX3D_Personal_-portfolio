#ifndef StepFog_h__
#define StepFog_h__

#include "Particle.h"

BEGIN(Engine)

class ENGINE_DLL CStepFog : public CParticle
{
protected:
	explicit CStepFog(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CStepFog();
	
private:
	HRESULT	Ready_StepFog(MyBBox* pBBox, int numiParticles);
	
public:
	virtual void		ResetParticle(PARTICLEATTRIBUTE * pAttribute) override;
	virtual void		Update_Component(const _float& fTimeDelta);

public:
	static CStepFog*	Create(LPDIRECT3DDEVICE9 pGraphicDev, MyBBox* pBBox, int numiPrticles);

public:
	// CParticle을(를) 통해 상속됨
	virtual void Free() override;
};

END
#endif // StepFog_h__
