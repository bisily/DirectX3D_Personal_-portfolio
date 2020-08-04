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

HRESULT CDynamicCamera::Ready_Object(const _vec3* pEye,
										const _vec3* pAt,
										const _vec3* pUp,
										_float _fFovY,
										_float _fAspect,
										_float _fNear,
										_float _fFar)
{
	m_vEye = *pEye;
	m_vAt = *pAt;
	m_vUp = *pUp;

	m_fFovY = _fFovY;
	m_fAspect = _fAspect;
	m_fNear = _fNear;
	m_fFar = _fFar;

	if (FAILED(Engine::CCamera::Ready_Object()))
		return E_FAIL;

	return S_OK;
}

_int CDynamicCamera::Update_Object(const _float& fTimeDelta)
{
	//POINT		ptCenter = { WINCX >> 1, WINCY >> 1 };

	//ClientToScreen(g_hWnd, &ptCenter);

	//SetCursorPos(ptCenter.x, ptCenter.y);

	Check_InputDev(fTimeDelta);

	_uint	iExit = Engine::CCamera::Update_Object(fTimeDelta);

	return 0;
}

void CDynamicCamera::Check_InputDev(const _float& fTimeDelta)
{
	_matrix			matWorld;
	D3DXMatrixInverse(&matWorld, NULL, &m_matView);

	if (Engine::Get_DIKeyState(DIK_UP) & 0x80)
	{
		_vec3		vLook;
		memcpy(vLook, &matWorld.m[2][0], sizeof(_vec3));
		D3DXVec3Normalize(&vLook, &vLook);

		_vec3		vLength = vLook * 15.f * fTimeDelta;

		m_vEye += vLength;
		m_vAt += vLength;
	}

	if (Engine::Get_DIKeyState(DIK_DOWN) & 0x80)
	{
		_vec3		vLook;
		memcpy(vLook, &matWorld.m[2][0], sizeof(_vec3));
		D3DXVec3Normalize(&vLook, &vLook);

		_vec3		vLength = vLook * 5.f * fTimeDelta;

		m_vEye -= vLength;
		m_vAt -= vLength;
	}

	if (Engine::Get_DIKeyState(DIK_RIGHT) & 0x80)
	{
		_vec3		vLook;
		memcpy(vLook, &matWorld.m[0][0], sizeof(_vec3));
		D3DXVec3Normalize(&vLook, &vLook);

		_vec3		vLength = vLook * 5.f * fTimeDelta;

		m_vEye += vLength;
		m_vAt += vLength;
	}

	if (Engine::Get_DIKeyState(DIK_LEFT) & 0x80)
	{
		_vec3		vLook;
		memcpy(vLook, &matWorld.m[0][0], sizeof(_vec3));
		D3DXVec3Normalize(&vLook, &vLook);

		_vec3		vLength = vLook * 5.f * fTimeDelta;

		m_vEye -= vLength;
		m_vAt -= vLength;
	}

	_long	dwMouseMove = 0;
	if (Engine::Get_DIKeyState(DIK_NUMPAD0) & 0x80)
	{
		if (dwMouseMove = Engine::Get_DIMouseMove(Engine::CDXInput::DIMS_Y))
		{
			_vec3		vRight;
			memcpy(&vRight, &matWorld.m[0][0], sizeof(_vec3));

			_vec3			vLook = m_vAt - m_vEye;
			D3DXVec3Normalize(&vLook, &vLook);

			_matrix		matRot;
			D3DXMatrixRotationAxis(&matRot, &vRight, D3DXToRadian(dwMouseMove / 10.f));
			D3DXVec3TransformNormal(&vLook, &vLook, &matRot);

			m_vAt = m_vEye + vLook;
		}

		if (dwMouseMove = Engine::Get_DIMouseMove(Engine::CDXInput::DIMS_X))
		{
			_vec3		vAxis{ 0.f, 1.f, 0.f };

			_vec3			vLook = m_vAt - m_vEye;
			D3DXVec3Normalize(&vLook, &vLook);

			_matrix		matRot;
			D3DXMatrixRotationAxis(&matRot, &vAxis, D3DXToRadian(dwMouseMove / 10.f));
			D3DXVec3TransformNormal(&vLook, &vLook, &matRot);

			m_vAt = m_vEye + vLook;
		}
	}

}

CDynamicCamera* CDynamicCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev,
										const _vec3* pEye,
										const _vec3* pAt,
										const _vec3* pUp,
										_float _fFovY,
										_float _fAspect,
										_float _fNear,
										_float _fFar)
{
	CDynamicCamera*	pInstance = new CDynamicCamera(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pEye, pAt, pUp, _fFovY, _fAspect, _fNear, _fFar)))
	{
		ERR_BOX(L"DynamicCamera Created Failed");
		Engine::Safe_Release(pInstance);
	}

	return pInstance;
}

void CDynamicCamera::Free()
{
	Engine::CCamera::Free();
}

