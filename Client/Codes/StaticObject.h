#ifndef StaticObject_h__
#define StaticObject_h__

#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CStaticMesh;
	class CTransform;
	class CRenderer;
	class CShader;
	class COptimization;
}

class CStaticObject : public Engine::CGameObject
{
private:
	explicit				CStaticObject(LPDIRECT3DDEVICE9	pGraphicDev);
public:
	virtual					~CStaticObject();

public:
	HRESULT					Ready_Object(wstring tagMeshName
										, Engine::_vec3 * temp_Right
										, Engine::_vec3 * temp_Up
										, Engine::_vec3 * temp_Look
										, Engine::_vec3 * temp_Pos
										, Engine::_vec3 * temp_Scale
										, Engine::_vec3 * temp_Angle
										, int _iObjIndex);

	virtual Engine::_int	Update_Object(const Engine::_float& fTimeDelta);
	virtual	void			Render_Object();

public: // Getter
	virtual Engine::_int	GetIndex() { return m_iObjIndex; }

public:	//Setter
	void					Set_ObjectIndex(int iObjIndex) { m_iObjIndex = iObjIndex; }

private:
	HRESULT					Add_Component(const Engine::_tchar* tagMeshName);
	HRESULT					SetUp_ConstantTable(LPD3DXEFFECT pEffect);

private:
	Engine::CTransform*		m_pTransCom			= nullptr;
	Engine::CStaticMesh*	m_pMeshCom			= nullptr;
	Engine::CRenderer*		m_pRendererCom		= nullptr;
	Engine::CShader*		m_pShaderCom		= nullptr;
	Engine::COptimization*	m_pOptimizationCom	= nullptr;

	int						m_iObjIndex;
	wstring					m_strMeshName;
	Engine::_bool			m_bIsDraw			= false;
	Engine::_bool			m_bCastle			= false;

public:
	static	CStaticObject*	Create(LPDIRECT3DDEVICE9 pGraphicDev
									, wstring tagMeshName
									, Engine::_vec3 * temp_Right
									, Engine::_vec3 * temp_Up
									, Engine::_vec3 * temp_Look
									, Engine::_vec3 * temp_Pos
									, Engine::_vec3 * temp_Scale
									, Engine::_vec3 * temp_Angle
									, int iObjIndex);

private:
	virtual	void			Free();
};

#endif // !StaticObject_h__
