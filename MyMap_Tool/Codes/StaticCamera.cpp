#include "stdafx.h"
#include "StaticCamera.h"

#include "Export_Function.h"
#include "Include.h"

CStaticCamera::CStaticCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CCamera(pGraphicDev)
	, m_pInfoSubject(Engine::Get_InfoSubject())
	, m_fTargetDistance (0.f)
	, m_fCameraSpeed(0.f)
	, m_fCameraAngle(0.f)
{

}

CStaticCamera::~CStaticCamera(void)
{

}

void CStaticCamera::SetCameraTarget(Engine::CTransform* pTargetInfo)
{
	m_pTargetInfo = pTargetInfo;
}

CStaticCamera* CStaticCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev, 
									 Engine::CTransform* pTargetInfo)
{
	CStaticCamera* pInstance = new CStaticCamera(pGraphicDev);

	if(FAILED(pInstance->Initialize()))
		Engine::Safe_Delete(pInstance);

	pInstance->SetCameraTarget(pTargetInfo);
	pInstance->SetProjectionMatrix(D3DXToRadian(45.f), float(WINCX) / WINCY, 1.f, 1000.f);

	return pInstance;
}

HRESULT CStaticCamera::Initialize(void)
{
	m_fCameraSpeed = 10.f;
	m_fTargetDistance = 10.f;
	return S_OK;
}

void CStaticCamera::Update(void)
{
	/*KeyCheck();
	TargetRenewal();
	*/

}

void CStaticCamera::KeyCheck(void)
{
	/*float fTime = Engine::CTimeMgr::GetInstance()->GetTime();*/

	//if(GetAsyncKeyState(VK_UP) & 0x8000)
	//	m_fCameraAngle += D3DXToRadian(45.f) * fTime;

	//if(GetAsyncKeyState(VK_DOWN) & 0x8000)
	//	m_fCameraAngle -= D3DXToRadian(45.f) * fTime;

	//if(GetAsyncKeyState('O') & 0x8000)
	//	m_fTargetDistance += fTime * 20.f;

	//if(GetAsyncKeyState('P') & 0x8000)
	//	m_fTargetDistance -= fTime * 20.f;
}

void CStaticCamera::TargetRenewal(void)
{

	/*m_vEye = m_pTargetInfo->m_vDir * (-1.f);*/
	D3DXVec3Normalize(&m_vEye, &m_vEye);

	m_vEye *= m_fTargetDistance;


	D3DXVECTOR3		vRight;
	memcpy(&vRight, &m_pTargetInfo->m_matWorld.m[0][0], sizeof(float) * 3);

	D3DXMATRIX		matRotAxis;
	D3DXMatrixRotationAxis(&matRotAxis, &vRight, m_fCameraAngle);
	D3DXVec3TransformNormal(&m_vEye, &m_vEye, &matRotAxis);


	D3DXVECTOR3 _TempPos;

	m_pTargetInfo->Get_Infomation(Engine::CTransform::INFO_POS, &_TempPos);
	
	m_vEye += _TempPos;
	m_vAt = _TempPos;

	Engine::CCamera::SetViewSpaceMatrix(&m_vEye, &m_vAt, &g_vUp);
	
}

