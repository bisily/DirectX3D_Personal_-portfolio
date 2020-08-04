#ifndef Tool_StaticMesh_h__
#define Tool_StaticMesh_h__

#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CStaticMesh;
	class CTransform;
	class CRenderer;
	class CCalculator;
	class CCollider;
}

class CTool_StaticMesh : public Engine::CGameObject
{
private:
	explicit CTool_StaticMesh(LPDIRECT3DDEVICE9	pGraphicDev);
public:
	virtual	~CTool_StaticMesh(void);

public:
	virtual	HRESULT			Ready_Object(const Engine::_tchar* _tagMeshName
										, int _iIndex = 0
										, Engine::_vec3 _vPosition = Engine::_vec3(0.f, 0.f, 0.f)
										, Engine::_vec3 _vScale = Engine::_vec3(0.01f, 0.01f, 0.01f));
	
	HRESULT			Ready_Load_Object(wstring _tagMeshName
										, Engine::_vec3 * _temp_Right
										, Engine::_vec3 * _temp_Up
										, Engine::_vec3 * _temp_Look
										, Engine::_vec3 * _temp_Pos
										, Engine::_vec3 * _temp_Scale
										, Engine::_vec3 * _temp_Angle
										, int _iObjIndex);

	virtual Engine::_int	Update_Object(const Engine::_float& fTimeDelta);
	virtual	void			Render_Object(void);

public: // Getter
	virtual Engine::_int	GetIndex() { return m_iObjIndex; }

	// ÀúÀå¿ë
	const Engine::CTransform*		Get_pTransCom() { return m_pTransCom; }
	const wstring					Get_MeshName() { return m_strMeshName; }
	const int						Get_ObjIndex() { return m_iObjIndex; }

public:	//Setter
	void					Set_ObjectIndex(int _iObjIndex) { m_iObjIndex = _iObjIndex; }
	void					Set_RenderState(bool _bState) { m_bWireFrame = _bState; }

private:
	HRESULT					Add_Component(const Engine::_tchar* _tagMeshName);

private:
	Engine::CTransform*			m_pTransCom = nullptr;
	Engine::CStaticMesh*		m_pMeshCom = nullptr;
	Engine::CRenderer*			m_pRendererCom = nullptr;
	Engine::CCalculator*		m_pCalculatorCom = nullptr;

	Engine::_bool				m_bWireFrame;
	int							m_iObjIndex;
	wstring						m_strMeshName;

public:
	static	CTool_StaticMesh*		Create(LPDIRECT3DDEVICE9 _pGraphicDev
											, const Engine::_tchar* _tagMeshName
											, int _iObjIndex = 0
											, Engine::_vec3 _vPosition = Engine::_vec3(0.f, 0.f, 0.f)
											, Engine::_vec3 _vScale = Engine::_vec3(0.01f, 0.01f, 0.01f));

	static CTool_StaticMesh*		CreateLoad(LPDIRECT3DDEVICE9 _pGraphicDev
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

#endif // !Tool_StaticMesh_h__
