#ifndef MeshStepFog_h__
#define MeshStepFog_h__

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

class CMeshStepFog : public Engine::CGameObject
{
private:
	explicit				CMeshStepFog(LPDIRECT3DDEVICE9	pGraphicDev);
public:
	virtual					~CMeshStepFog();

public:
	HRESULT					Ready_Object(_matrix* pmatPos, _vec3* pDir);

	virtual Engine::_int	Update_Object(const Engine::_float& fTimeDelta);
	virtual	void			Render_Object();

private:
	HRESULT					Add_Component();
	HRESULT					SetUp_ConstantTable(LPD3DXEFFECT pEffect);

private:
	Engine::CTransform*			m_pTargetTransCom = nullptr;
	Engine::CTransform*			m_pTransCom = nullptr;
	Engine::CStaticMesh*		m_pMeshCom = nullptr;
	Engine::CRenderer*			m_pRendererCom = nullptr;
	Engine::CShader*			m_pShaderCom = nullptr;
	Engine::COptimization*		m_pOptimizationCom = nullptr;
	
	_float						m_fLifeTime = 0.f;
	_vec3						m_vPos, m_vLook;
	bool						m_bTime = false;

public:
	static	CMeshStepFog*	Create(LPDIRECT3DDEVICE9 pGraphicDev, _matrix* pmatPos, _vec3* pDir);

private:
	virtual	void			Free();
};

#endif // !MeshStepFog_h__
