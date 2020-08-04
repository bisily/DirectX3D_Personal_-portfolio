#ifndef DynamicCamera_h__
#define DynamicCamera_h__

#include "Include.h"
#include "Camera.h"

namespace Engine
{
	class CCamera;
}

class CDynamicCamera : public Engine::CCamera
{
private:
	explicit CDynamicCamera(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual ~CDynamicCamera();

public:
	HRESULT Ready_Object(const Engine::_vec3* pEye,
							const Engine::_vec3* pAt,
							const Engine::_vec3* pUp,
							Engine::_float _fFovY,
							Engine::_float _fAspect,
							Engine::_float _fNear,
							Engine::_float _fFar);

	virtual Engine::_int	Update_Object(const Engine::_float& fTimeDelta);
	
	virtual void	Render_Object();

private:
	void		KeyCheck(const Engine::_float& fTimeDelta);

private:

public:
	static		CDynamicCamera*		Create(LPDIRECT3DDEVICE9 pGraphicDev,
											const Engine::_vec3* pEye,
											const Engine::_vec3* pAt,
											const Engine::_vec3* pUp,
											Engine::_float _fFovY = D3DXToRadian(60.f),
											Engine::_float _fAspect = Engine::_float(WINCX) / Engine::_float(WINCY),
											Engine::_float _fNear = 1.f,
											Engine::_float _fFar = 1000.f);
private:
	virtual void	Free();
};

#endif // !DynamicCamera_h__
