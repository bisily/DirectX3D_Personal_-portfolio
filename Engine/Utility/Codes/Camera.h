#ifndef Camera_h__
#define Camera_h__

#include "Engine_Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CCamera : public CGameObject
{
protected:
	explicit				CCamera(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual					~CCamera();

public:
	virtual		HRESULT		Ready_Object();
	virtual		_int		Update_Object(const _float& fTimeDelta);

protected:
	_vec3					m_vEye;
	_vec3					m_vAt;
	_vec3					m_vUp;
	_matrix					m_matView;

	_float					m_fFovY;
	_float					m_fAspect; 
	_float					m_fNear; 
	_float					m_fFar;
	_matrix					m_matProj;

protected:
	virtual		void		Free();
};

END

#endif // !Camera_h__
