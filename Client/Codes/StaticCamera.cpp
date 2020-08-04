#include "stdafx.h"
#include "StaticCamera.h"

#include "Export_Function.h"

CStaticCamera::CStaticCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CCamera(pGraphicDev)
{

}

CStaticCamera::~CStaticCamera()
{

}

HRESULT CStaticCamera::Ready_Object(const _vec3* pEye,
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

	m_fCameraSpeed = 10.f;
	m_fTargetDistance = 3.f;

	if (FAILED(Engine::CCamera::Ready_Object()))
		return E_FAIL;

	return S_OK;
}

_int CStaticCamera::Update_Object(const _float& fTimeDelta)
{
	Get_Target();
	
	//POINT		ptCenter = { WINCX >> 1, WINCY >> 1 };

	//ClientToScreen(g_hWnd, &ptCenter);

	//SetCursorPos(ptCenter.x, ptCenter.y);

	//Key_Check(fTimeDelta);
	
	Target_Renewal(fTimeDelta);

	/*vTargetPos.y += 5.f; */
	/*_vec3 vTargetPos, vTargetLook;
	memcpy(&vTargetPos, &m_pTarget_Transform->m_matWorld.m[3][0], sizeof(_vec3));
	m_vAt = vTargetPos;*/
	
	_uint	iExit = Engine::CCamera::Update_Object(fTimeDelta);

	return 0;
}

void CStaticCamera::Get_Target()
{
	if (nullptr == m_pTarget_Transform)
	{
		m_pTarget_Transform = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Com_Transform", Engine::CComponent::ID_DYNAMIC));
		NULL_CHECK(m_pTarget_Transform);
	}
}

void CStaticCamera::Target_Renewal(const _float& fTimeDelta)
{
	m_vEye = m_pTarget_Transform->m_vInfo[Engine::CTransform::INFO_LOOK];
	D3DXVec3Normalize(&m_vEye, &m_vEye);
	
	m_vEye *= 6.f;

	_vec3 vRight;
	memcpy(&vRight, &m_pTarget_Transform->m_matWorld.m[0][0], sizeof(float) * 3);
	
	_matrix			matRotAxis;
	D3DXMatrixRotationAxis(&matRotAxis, &vRight, D3DXToRadian(m_fAngle) * fTimeDelta);
	D3DXVec3TransformNormal(&m_vEye, &m_vEye, &matRotAxis);

	_vec3 vecTarget_Pos;
	m_pTarget_Transform->Get_Infomation(Engine::CTransform::INFO_POS, &vecTarget_Pos);

	m_vEye += vecTarget_Pos;
	m_vAt = vecTarget_Pos;

	m_vEye.y = 3.f + vecTarget_Pos.y;
}

void CStaticCamera::Key_Check(const Engine::_float & fTimeDelta)
{
	_matrix			matWorld;
	D3DXMatrixInverse(&matWorld, NULL, &m_matView);

	_long	dwMouseMove = 0;

	if (dwMouseMove = Engine::Get_DIMouseMove(Engine::CDXInput::DIMS_Y))
	{
		_vec3		vRight;
		memcpy(&vRight, &matWorld.m[0][0], sizeof(_vec3));

		_vec3			vLook = m_vAt - m_vEye;
		D3DXVec3Normalize(&vLook, &vLook);

		_matrix		matRot;
		D3DXMatrixRotationAxis(&matRot, &vRight, D3DXToRadian(dwMouseMove / 10.f));
		D3DXVec3TransformNormal(&vLook, &vLook, &matRot);

		m_vEye = vLook;
	}

	if (dwMouseMove = Engine::Get_DIMouseMove(Engine::CDXInput::DIMS_X))
	{
		_vec3		vAxis{ 0.f, 1.f, 0.f };

		_vec3			vLook = m_vAt - m_vEye;
		D3DXVec3Normalize(&vLook, &vLook);

		_matrix		matRot;
		D3DXMatrixRotationAxis(&matRot, &vAxis, D3DXToRadian(dwMouseMove / 10.f));
		D3DXVec3TransformNormal(&vLook, &vLook, &matRot);

		m_vEye = vLook;
	}
	
}

CStaticCamera* CStaticCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev,
										const _vec3* pEye,
										const _vec3* pAt,
										const _vec3* pUp,
										_float _fFovY,
										_float _fAspect,
										_float _fNear,
										_float _fFar)
{
	CStaticCamera*	pInstance = new CStaticCamera(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pEye, pAt, pUp, _fFovY, _fAspect, _fNear, _fFar)))
	{
		ERR_BOX(L"StaticCamera Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

void CStaticCamera::Free()
{
	Engine::CCamera::Free();
}

