#ifndef DynamicCamera_h__
#define DynamicCamera_h__

#include "Defines.h"
#include "Camera.h"

namespace Engine
{
	class CCamera;
}

class CDynamicCamera : public Engine::CCamera
{
private:
	explicit		CDynamicCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual			~CDynamicCamera();

public:
	HRESULT			Ready_Object(const _vec3* pEye,
							const _vec3* pAt,
							const _vec3* pUp,
							_float _fFovY,
							_float _fAspect,
							_float _fNear,
							_float _fFar);

	virtual _int	Update_Object(const _float& fTimeDelta) override;

private:
	void			Check_InputDev(const _float& fTimeDelta);

public:
	static			CDynamicCamera*		Create(LPDIRECT3DDEVICE9 pGraphicDev,
											const _vec3* pEye,
											const _vec3* pAt,
											const _vec3* pUp,
											_float _fFovY = D3DXToRadian(60.f),
											_float _fAspect = _float(WINCX) / _float(WINCY),
											_float _fNear = 1.f,
											_float _fFar = 1000.f);
private:
	virtual void	Free() override;

};

#endif // DynamicCamera_h__
