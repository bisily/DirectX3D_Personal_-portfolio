#ifndef StaticCamera_h__
#define StaticCamera_h__

#include "../Engine/Utility/Codes/Camera.h"

namespace Engine
{
	class CTimeMgr;
	class CTransform;
	class CInfoSubject;
}

class  CStaticCamera : public Engine::CCamera
{
private:
	explicit CStaticCamera(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual ~CStaticCamera(void);

public:
	void	SetCameraTarget(Engine::CTransform* pTargetInfo);
	static  CStaticCamera*		Create(LPDIRECT3DDEVICE9 pGraphicDev, 
										Engine::CTransform* pTargetInfo);
public:
	virtual HRESULT	Initialize(void);
	virtual void	Update(void);

private:
	void	KeyCheck(void);
	void	TargetRenewal(void);

private:
	Engine::CTimeMgr*			m_pTimeMgr = nullptr;
	Engine::CTransform*	m_pTargetInfo = nullptr;
	Engine::CInfoSubject*		m_pInfoSubject = nullptr;

	float		m_fTargetDistance = 0.f;
	float		m_fCameraSpeed = 0.f;
	float		m_fCameraAngle = 0.f;

};

#endif // StaticCamera_h__
