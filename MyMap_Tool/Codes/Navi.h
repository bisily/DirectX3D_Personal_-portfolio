#ifndef Navi_h__
#define Navi_h__

#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CCalculator;
	class CRenderer;
	class CNaviMesh;
	class CCell;
}

class CNavi : public Engine::CGameObject
{
public:
	enum COLLTYPE { COLL_CLICK, COLL_FALSE, COLL_TRUE, COLL_END };

private:
	explicit CNavi(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual	~CNavi(void);

public:
	virtual	HRESULT			Ready_Object(void);
	virtual Engine::_int	Update_Object(const Engine::_float& fTimeDelta);
	virtual	void			Render_Object(void);

	void					Set_PopBack();

public: // Getter
	
public:	// Setter
	void		Set_NeviMode(bool bState) { m_bNeviMode = bState; }

private:
	HRESULT					Add_Component();
	void					KeyCheck();

private:
	D3DXVECTOR3					vTemp;
	vector<D3DXVECTOR3>			m_vecPos;
	vector<Engine::CCell*>		m_DrawCellList;
	Engine::CCalculator*		m_pCalculatorCom = nullptr;
	Engine::CRenderer*			m_pRendererCom = nullptr;
	Engine::CNaviMesh*			m_pNaviMeshCom = nullptr;

	bool						m_bNeviMode = false;
	LPDIRECT3DTEXTURE9			m_pTexture[COLL_END];

	LPD3DXMESH					m_pMesh = nullptr;
	D3DXVECTOR3*				m_pVtxPos = nullptr;
	

public:
	static	CNavi*		Create(LPDIRECT3DDEVICE9 _pGraphicDev);
private:
	virtual	void	Free(void);
};

#endif // !Navi_h__
