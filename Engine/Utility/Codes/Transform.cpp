#include "Transform.h"

USING(Engine)

Engine::CTransform::CTransform()
	: m_vScale(1.f, 1.f, 1.f)
	, m_vAngle(0.f, 0.f, 0.f)
{

}

Engine::CTransform::~CTransform()
{

}

HRESULT Engine::CTransform::Ready_Transform()
{
	D3DXMatrixIdentity(&m_matWorld);

	for (_int i = 0; i < 4; ++i)
		memcpy(&m_vInfo[i], &m_matWorld.m[i][0], sizeof(_vec3));

	return S_OK;
}

void Engine::CTransform::Update_Component(const _float& fTimeDelta)
{
	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixIdentity(&m_matNRotWorld);

	for (_int i = 0; i < INFO_END - 1; ++i)
	{
		memcpy(&m_vInfo[i], &m_matWorld.m[i][0], sizeof(_vec3));
	}

	// 크기 변환 과정
	for (_int i = 0; i < INFO_END - 1; ++i)
	{
		D3DXVec3Normalize(&m_vInfo[i], &m_vInfo[i]);
		m_vInfo[i] *= *((float*)&m_vScale + i);
	}

	for (_int i = 0; i < 4; ++i)
		memcpy(&m_matNRotWorld.m[i][0], &m_vInfo[i], sizeof(_vec3));


	_matrix			matRot[ROT_END];

	D3DXMatrixRotationX(&matRot[ROT_X], m_vAngle.x);
	D3DXMatrixRotationY(&matRot[ROT_Y], m_vAngle.y);
	D3DXMatrixRotationZ(&matRot[ROT_Z], m_vAngle.z);

	// 회전 변환 과정
	for (_int i = 0; i < INFO_END - 1; ++i)
	{
		for (_int j = 0; j < ROT_END; ++j)
		{
			D3DXVec3TransformNormal(&m_vInfo[i], &m_vInfo[i], &matRot[j]);
		}
	}

	for (_int i = 0; i < 4; ++i)
	{
		memcpy(&m_matWorld.m[i][0], &m_vInfo[i], sizeof(_vec3));
	}

}

Engine::CTransform* Engine::CTransform::Create()
{
	CTransform*		pInstance = new CTransform;

	if (FAILED(pInstance->Ready_Transform()))
	{
		ERR_BOX(L"Transform Create Failed");
		Engine::Safe_Release(pInstance);
	}

	return pInstance;
}

void Engine::CTransform::Free()
{

}

void Engine::CTransform::Target_Chase(const _vec3* pTargetPos, const _float& fSpeed)
{
	_vec3		vDirection = *pTargetPos - m_vInfo[INFO_POS];

	D3DXVec3Normalize(&vDirection, &vDirection);

	m_vInfo[INFO_POS] += vDirection * fSpeed;

	_matrix			matScale, matRot, matTrans;

	D3DXMatrixScaling(&matScale, m_vScale.x, m_vScale.y, m_vScale.z);
	matRot = *Compute_LookAtTarget(pTargetPos);
	D3DXMatrixTranslation(&matTrans, m_vInfo[INFO_POS].x, m_vInfo[INFO_POS].y, m_vInfo[INFO_POS].z);

	m_matWorld = matScale * matRot * matTrans;

}

const Engine::_matrix* Engine::CTransform::Compute_LookAtTarget(const _vec3* pTargetPos)
{
	_vec3		vDirection = *pTargetPos - m_vInfo[INFO_POS];
	D3DXVec3Normalize(&vDirection, &vDirection);

	_vec3		vUp{};
	D3DXVec3Normalize(&vUp, &m_vInfo[INFO_UP]);

	float	fAngle = D3DXVec3Dot(&vDirection, &vUp);


	_vec3		vAxis{};
	D3DXVec3Cross(&vAxis, &m_vInfo[INFO_UP], &vDirection);

	_matrix		matRot;

	return D3DXMatrixRotationAxis(&matRot, &vAxis, acosf(fAngle));
}

void Engine::CTransform::Move_TargetPos(const _vec3* pTargetPos,
	const _float& fSpeed,
	const _float& fTimeDelta)
{
	_vec3		vDir = *pTargetPos - m_vInfo[INFO_POS];

	D3DXVec3Normalize(&vDir, &vDir);

	m_vInfo[INFO_POS] += vDir * fSpeed * fTimeDelta;
}

const Engine::_matrix* Engine::CTransform::Get_NRotWorldMatrix(_matrix* pWorldMatrix) const
{
	if (nullptr != pWorldMatrix)
		*pWorldMatrix = m_matNRotWorld;

	return &m_matNRotWorld;
}

