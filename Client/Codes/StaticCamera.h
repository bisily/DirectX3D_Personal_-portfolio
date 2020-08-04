#ifndef StaticCamera_h__
#define StaticCamera_h__

#include "Defines.h"
#include "Camera.h"
#include "GameObject.h"

namespace Engine
{
	class CCamera;
	class CTransform;
}

class CStaticCamera : public Engine::CCamera
{
private:
	explicit						CStaticCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual							~CStaticCamera();

public:
	HRESULT							Ready_Object(const _vec3* pEye,
													const _vec3* pAt,
													const _vec3* pUp,
													_float _fFovY,
													_float _fAspect,
													_float _fNear,
													_float _fFar);

	virtual _int					Update_Object(const _float& fTimeDelta) override;
	_float							Get_Angle() { return m_fAngle; }
	void							Get_Target();

private:
	void							Target_Renewal(const _float& fTimeDelta);
	void							Key_Check(const Engine::_float& fTimeDelta);

private:
	Engine::CTransform*				m_pTarget_Transform		= nullptr;
	float							m_fCameraSpeed;
	float							m_fTargetDistance;
	float							m_fAngle				= 90.f;

public:
	static		CStaticCamera*		Create(LPDIRECT3DDEVICE9 pGraphicDev,
											const _vec3* pEye,
											const _vec3* pAt,
											const _vec3* pUp,
											_float _fFovY = D3DXToRadian(60.f),
											_float _fAspect = _float(WINCX) / _float(WINCY),
											_float _fNear = 1.f,
											_float _fFar = 1000.f);
private:
	virtual void Free() override;

};

#endif // StaticCamera_h__
