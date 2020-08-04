#ifndef Square_h__
#define Square_h__

#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CRenderer;
}

class CSquare : public Engine::CGameObject
{
private:
	explicit CSquare(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual	~CSquare();

public:
	virtual	HRESULT			Ready_Object(D3DXVECTOR3* pVecPos, wstring strParentName, wstring strBoreName, Engine::_vec3 vMin, Engine::_vec3 vMax, int iIndex);
	virtual Engine::_int	Update_Object(const Engine::_float& fTimeDelta);
	virtual	void			Render_Object();

public: // Getter
	virtual		Engine::_int	GetIndex() { return m_iIndex; }
	wstring		Get_BoreName() { return m_strBoreName; }
	void		SetSquare(Engine::_vec3 vMin, Engine::_vec3 vMax);

private:
	HRESULT						Add_Component();

private:
	Engine::CRenderer*			m_pRendererCom = nullptr;

	LPDIRECT3DDEVICE9			m_pGraphicDev = nullptr;
	LPDIRECT3DVERTEXBUFFER9		m_pVB = nullptr;
	LPDIRECT3DINDEXBUFFER9		m_pIB = nullptr;
	LPDIRECT3DTEXTURE9			m_pTexture;
	Engine::_vec3				m_vMin, m_vMax;

	D3DXMATRIX					matWorld;
	D3DXVECTOR3					m_vPos;
	wstring						m_strParentName;
	wstring						m_strBoreName;
	const Engine::_matrix*		m_pParentBoneMatrix = nullptr;
	const Engine::_matrix*		m_pParentWorldMatrix = nullptr;
	int							m_iIndex = 0;

public:
	static	CSquare*		Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXVECTOR3* pVecPos, wstring strParentName, wstring strBoreName, Engine::_vec3 vMin, Engine::_vec3 vMax, int iIndex);
private:
	virtual	void	Free(void);
};

#endif // !CSquare_h__
