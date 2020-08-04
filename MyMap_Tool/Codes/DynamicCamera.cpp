#include "stdafx.h"
#include "DynamicCamera.h"

#include "Export_Function.h"

CDynamicCamera::CDynamicCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CCamera(pGraphicDev)
{
}

CDynamicCamera::~CDynamicCamera()
{
}

HRESULT CDynamicCamera::Ready_Object(const Engine::_vec3* pEye,
									const Engine::_vec3* pAt,
									const Engine::_vec3* pUp,
									Engine::_float _fFovY,
									Engine::_float _fAspect,
									Engine::_float _fNear,
									Engine::_float _fFar)
{

	m_vEye = *pEye;
	m_vAt = *pAt;
	m_vUp = *pUp;

	m_fFovY = _fFovY;
	m_fAspect = _fAspect;
	m_fNear = _fNear;
	m_fFar = _fFar;
	
	if (FAILED(Engine::CCamera::Ready_Object()))
	{
		return E_FAIL;
	}

	return S_OK;
}

Engine::_int CDynamicCamera::Update_Object(const Engine::_float & fTimeDelta)
{
	KeyCheck(fTimeDelta);

	Engine::_int iExit = Engine::CCamera::Update_Object(fTimeDelta);
	
	return 0;
}

void CDynamicCamera::Render_Object()
{	
}

void CDynamicCamera::KeyCheck(const Engine::_float& fTimeDelta)
{
	Engine::_matrix			matWorld;
	D3DXMatrixInverse(&matWorld, NULL, &m_matView);

	if (Engine::Get_DIKeyState(DIK_W) & 0x80)
	{
		Engine::_vec3		vLook;
		memcpy(vLook, &matWorld.m[2][0], sizeof(Engine::_vec3));
		D3DXVec3Normalize(&vLook, &vLook);

		Engine::_vec3		vLength = vLook * 0.2f;

		m_vEye += vLength;
		m_vAt += vLength;
	}

	if (Engine::Get_DIKeyState(DIK_S) & 0x80)
	{
		Engine::_vec3		vLook;
		memcpy(vLook, &matWorld.m[2][0], sizeof(Engine::_vec3));
		D3DXVec3Normalize(&vLook, &vLook);

		Engine::_vec3		vLength = vLook * 0.2f;

		m_vEye -= vLength;
		m_vAt -= vLength;
	}

	if (Engine::Get_DIKeyState(DIK_D) & 0x80)
	{
		Engine::_vec3		vLook;
		memcpy(vLook, &matWorld.m[0][0], sizeof(Engine::_vec3));
		D3DXVec3Normalize(&vLook, &vLook);

		Engine::_vec3		vLength = vLook * 0.2f;

		m_vEye += vLength;
		m_vAt += vLength;
	}

	if (Engine::Get_DIKeyState(DIK_A) & 0x80)
	{
		Engine::_vec3		vLook;
		memcpy(vLook, &matWorld.m[0][0], sizeof(Engine::_vec3));
		D3DXVec3Normalize(&vLook, &vLook);

		Engine::_vec3		vLength = vLook * 0.2f;

		m_vEye -= vLength;
		m_vAt -= vLength;
	}

	Engine::_long	dwMouseMove = 0;

	if (Engine::Get_DIKeyState(DIK_Z) & 0x80)
	{
		if (dwMouseMove = Engine::Get_DIMouseMove(Engine::CDXInput::DIMS_Y))
		{
			Engine::_vec3		vRight;
			memcpy(&vRight, &matWorld.m[0][0], sizeof(Engine::_vec3));

			Engine::_vec3			vLook = m_vAt - m_vEye;
			D3DXVec3Normalize(&vLook, &vLook);

			Engine::_matrix		matRot;
			D3DXMatrixRotationAxis(&matRot, &vRight, D3DXToRadian(dwMouseMove / 10.f));
			D3DXVec3TransformNormal(&vLook, &vLook, &matRot);

			m_vAt = m_vEye + vLook;
		}

		if (dwMouseMove = Engine::Get_DIMouseMove(Engine::CDXInput::DIMS_X))
		{
			Engine::_vec3		vAxis{ 0.f, 1.f, 0.f };

			Engine::_vec3			vLook = m_vAt - m_vEye;
			D3DXVec3Normalize(&vLook, &vLook);

			Engine::_matrix		matRot;
			D3DXMatrixRotationAxis(&matRot, &vAxis, D3DXToRadian(dwMouseMove / 10.f));
			D3DXVec3TransformNormal(&vLook, &vLook, &matRot);

			m_vAt = m_vEye + vLook;
		}
	}

}

CDynamicCamera * CDynamicCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev,
	const Engine::_vec3* pEye,
	const Engine::_vec3* pAt,
	const Engine::_vec3* pUp,
	Engine::_float _fFovY,
	Engine::_float _fAspect,
	Engine::_float _fNear,
	Engine::_float _fFar)
{
	CDynamicCamera* pInstance = new CDynamicCamera(pGraphicDev);

	if (pInstance->Ready_Object(pEye, pAt, pUp, _fFovY, _fAspect, _fNear, _fFar))
	{
		ERR_BOX(L"DynamicCamera Created Failed");
		Engine::Safe_Release(pInstance);
	}
	
	return pInstance;
}

void CDynamicCamera::Free()
{
	Engine::CGameObject::Free();
}