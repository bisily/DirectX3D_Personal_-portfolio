#ifndef Transform_h__
#define Transform_h__

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTransform : public CComponent
{
public:
	enum INFO { INFO_RIGHT, INFO_UP, INFO_LOOK, INFO_POS, INFO_END };
	enum ROTATION { ROT_X, ROT_Y, ROT_Z, ROT_END };

private:
	explicit						CTransform();
public:
	virtual							~CTransform();

public: // Get
	void							Get_Infomation(INFO eType, _vec3* pInfomation) { memcpy(pInfomation, &m_matWorld.m[eType][0], sizeof(_vec3)); }

	void							Get_WorldMatrix(_matrix* pWorldMatrix) const { *pWorldMatrix = m_matWorld; }

	const _matrix*					Get_NRotWorldMatrix(_matrix* pWorldMatrix) const;
	const _matrix*					Get_WorldMatrixPointer() const { return &m_matWorld; }

	const _matrix*					Compute_LookAtTarget(const _vec3* pTargetPos);

	const _vec3*					Get_PosPointer() { return &m_vInfo[INFO_POS]; }

public: // Set
	void							SetTransform(LPDIRECT3DDEVICE9 pGraphicDev) const { pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld); }

	void							Set_Scale(const _float& fX, const _float& fY, const _float& fZ) { m_vScale = _vec3(fX, fY, fZ); }

	void							Set_Position(_vec3 vPosition) { m_vInfo[INFO_POS] = vPosition; }

	void							Set_Position(const _float& fX, const _float& fY, const _float& fZ) { m_vInfo[INFO_POS] = _vec3(fX, fY, fZ); }
	
	void							Set_ParentMatrix(const _matrix* pParentMatrix) { m_matWorld *= *pParentMatrix; }

	void							Set_WorldMatrix(const _matrix* pMatrix) { m_matWorld = *pMatrix; }

public:

	void							Move_Position(const _vec3* const pDirection) { m_vInfo[INFO_POS] += *pDirection; }

	void							Rotation(ROTATION eType, const _float& fAngle) { *(((float*)&m_vAngle) + eType) += fAngle; }

	void	Move_Forward(const _float& fSpeed, const _float& fTimeDelta)
	{
		_vec3		vDirection;
		memcpy(&vDirection, &m_matWorld.m[2][0], sizeof(_vec3));
		D3DXVec3Normalize(&vDirection, &vDirection);

		m_vInfo[INFO_POS] += vDirection * fTimeDelta * fSpeed;
	}

	void							Move_TargetPos(const _vec3* pTargetPos, const _float& fSpeed, const _float& fTimeDelta);

public:
	void							Target_Chase(const _vec3* pTargetPos, const _float& fSpeed);


public:
	HRESULT							Ready_Transform();
	virtual void					Update_Component(const _float& fTimeDelta);

public:
	_vec3							m_vInfo[INFO_END];
	_vec3							m_vScale;
	_vec3							m_vAngle;
	_matrix							m_matWorld;
	_matrix							m_matNRotWorld;

public:
	static	CTransform*				Create();

private:
	virtual void					Free();
};

END
#endif // Transform_h__
