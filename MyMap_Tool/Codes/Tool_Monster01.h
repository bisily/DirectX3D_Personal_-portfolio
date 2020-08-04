#ifndef Tool_Monster01_h__
#define Tool_Monster01_h__

#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CDynamicMesh;
	class CTransform;
	class CRenderer;
	class CCalculator;
	class CCollider;
}

class CTool_Monster01 : public Engine::CGameObject
{
private:
	explicit CTool_Monster01(LPDIRECT3DDEVICE9	pGraphicDev);
public:
	virtual	~CTool_Monster01(void);

public:
	virtual	HRESULT	Ready_Object(wstring _tagMeshName
		, int _iObjIndex = 0
		, Engine::_vec3 vPosition = Engine::_vec3(0.f, 0.f, 0.f)
		, Engine::_vec3 vScale = Engine::_vec3(0.01f, 0.01f, 0.01f));

	HRESULT			Ready_Load_Object(wstring _tagMeshName
		, Engine::_vec3 * _temp_Right
		, Engine::_vec3 * _temp_Up
		, Engine::_vec3 * _temp_Look
		, Engine::_vec3 * _temp_Pos
		, Engine::_vec3 * _temp_Scale
		, Engine::_vec3 * _temp_Angle
		, int _iObjIndex);

	virtual Engine::_int	Update_Object(const Engine::_float& fTimeDelta);
	virtual	void	Render_Object(void);

public: // Getter
	virtual Engine::_int	GetIndex() { return m_iObjIndex; }

public: // Setter
	void			Set_RenderState(bool _bState) { m_bWireFrame = _bState; }
	void			SetUp_Animation(Engine::_int iIndex);
	void			SetPlay_Animation(bool bPlay) { bPlayAnimation = bPlay; }

	// ÀúÀå¿ë
	const Engine::CTransform*		Get_pTransCom() { return m_pTransCom; }
	const wstring					Get_MeshName() { return m_strMeshName; }
	const int						Get_ObjIndex() { return m_iObjIndex; }

private:
	HRESULT					Add_Component(void);
	void					SetUp_OnTerrain(void);
	Engine::_vec3			PickUp_OnTerrain(void);

private:
	Engine::CTransform*			m_pTransCom = nullptr;
	Engine::CDynamicMesh*		m_pMeshCom = nullptr;
	Engine::CRenderer*			m_pRendererCom = nullptr;
	Engine::CCalculator*		m_pCalculatorCom = nullptr;
	Engine::CCollider*			m_pColliderCom = nullptr;
	bool						m_bWireFrame;
	int							m_iObjIndex;
	wstring						m_strMeshName;
	bool						bPlayAnimation = false;

public:
	static	CTool_Monster01*		Create(LPDIRECT3DDEVICE9 pGraphicDev
		, wstring _tagMeshName
		, int iObjIndex = 0
		, Engine::_vec3 vPosition = Engine::_vec3(0.f, 0.f, 0.f)
		, Engine::_vec3 vScale = Engine::_vec3(0.01f, 0.01f, 0.01f));

	static CTool_Monster01*			CreateLoad(LPDIRECT3DDEVICE9 _pGraphicDev
		, wstring _tagMeshName
		, Engine::_vec3 * _temp_Right
		, Engine::_vec3 * _temp_Up
		, Engine::_vec3 * _temp_Look
		, Engine::_vec3 * _temp_Pos
		, Engine::_vec3 * _temp_Scale
		, Engine::_vec3 * _temp_Angle
		, int _iObjIndex);
private:
	virtual	void	Free(void);
};

#endif // Tool_Monster01_h__
