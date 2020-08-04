#ifndef CollisionSphere_h__
#define CollisionSphere_h__

#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CRenderer;
}

class CCollisionSphere: public Engine::CGameObject
{
private:
	explicit CCollisionSphere(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual	~CCollisionSphere(void);

public:
	virtual	HRESULT			Ready_Object(D3DXVECTOR3* pVecPos, wstring strParentName, wstring strBoreName, int iIndex);
	virtual Engine::_int	Update_Object(const Engine::_float& fTimeDelta);
	virtual	void			Render_Object(void);

public: // Getter
	float		GetRadius() { return m_fRadius; }
	virtual		Engine::_int	GetIndex() { return m_iIndex; }
	wstring		Get_BoreName() { return m_strBoreName; }

public:	// Setter
	void		SetRadius(float fRadius);
private:
	HRESULT					Add_Component();

private:
	Engine::CRenderer*			m_pRendererCom = nullptr;

	LPDIRECT3DTEXTURE9			m_pTexture;
	LPD3DXMESH					m_pMesh = nullptr;
	LPDIRECT3DVERTEXBUFFER9*	m_ppTempVtxBuffer = nullptr;
	D3DXVECTOR3*				m_pVtxPos = nullptr;
	
	D3DXVECTOR3					m_vPos;
	wstring						m_strParentName;
	wstring						m_strBoreName;
	const Engine::_matrix*		m_pParentBoneMatrix = nullptr;
	const Engine::_matrix*		m_pParentWorldMatrix = nullptr;
	int							m_iIndex = 0;
	float						m_fRadius = 0.f;					

public:
	static	CCollisionSphere*		Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3* pVecPos, wstring strParentName, wstring strBoreName, int iIndex);
private:
	virtual	void	Free(void);
};

#endif // !CollisionSphere_h__
