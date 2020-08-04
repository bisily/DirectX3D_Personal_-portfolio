#ifndef Collider_h__
#define Collider_h__

#include "Component.h"
#include "Export_Resources.h"
#include "Transform.h"

BEGIN(Engine)

class ENGINE_DLL CCollider : public CComponent
{
public:
	enum COLLTYPE { COLL_FALSE, COLL_TRUE, COLL_END };

private:
	explicit					CCollider(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual						~CCollider();

public:
	const _vec3*				Get_Min() const { return &m_vMin;  }
	const _vec3*				Get_Max() const { return &m_vMax; }
	const _matrix*				Get_ColliderWorldMatrix() const { return &m_matWorld;  }

public:
	HRESULT						Ready_Collider(const _vec3* pPosition, const _ulong& dwNumVertices, const _ulong& dwStride);
	
	HRESULT						Ready_Collider(const _ulong& dwNumVertices,
												const _ulong& dwStride,
												const _vec3* vMin,
												const _vec3* vMax);

	HRESULT						Ready_Collider(const _vec3* vMin,
												const _vec3* vMax);

	void						Render_Collider(COLLTYPE eType, const _matrix* pColliderMatrix);
	

private:
	_vec3						m_vMin;
	_vec3						m_vMax;
	D3DXMATRIX					m_matWorld;

	LPDIRECT3DDEVICE9			m_pGraphicDev	= nullptr;
	LPDIRECT3DVERTEXBUFFER9		m_pVB			= nullptr;
	LPDIRECT3DINDEXBUFFER9		m_pIB			= nullptr;
	LPDIRECT3DTEXTURE9			m_pTexture[COLL_END];

public:
	static CCollider*			Create(LPDIRECT3DDEVICE9 pGraphicDev, 
										const _vec3* pPosition, 
										const _ulong& dwNumVertices, 
										const _ulong& dwStride);

	static CCollider*			Create(LPDIRECT3DDEVICE9 pGraphicDev,
										const _ulong& dwNumVertices,
										const _ulong& dwStride,
										const _vec3* vMin,
										const _vec3* vMax);

	static CCollider*			Create(LPDIRECT3DDEVICE9 pGraphicDev,
										const _vec3* vMin,
										const _vec3* vMax);

	virtual void				Free();
};


END
#endif // Collider_h__
