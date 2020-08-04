#include "stdafx.h"
#include "StepFog.h"

#include "Export_Function.h"

CStepFog::CStepFog(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{

}

CStepFog::~CStepFog()
{
}

HRESULT CStepFog::Ready_Object(_matrix* pmatPos, _vec3* pDir)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	memcpy(&m_vPos, &pmatPos->m[3][0], sizeof(_vec3));
	m_vPos.y += 0.4f;

	m_vLook = *pDir;
	D3DXVec3Normalize(&m_vLook, &m_vLook);

	return S_OK;
}

HRESULT CStepFog::Add_Component()
{
	Engine::CComponent*		pComponent = nullptr;

	// For.Buffer
	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(Engine::Clone_Resource(RESOURCE_STATIC, L"Buffer_RcTex_Center"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::CComponent::ID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Buffer", pComponent));

	// For.Renderer
	pComponent = m_pRendererCom = Engine::Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->AddRef();
	m_mapComponent[Engine::CComponent::ID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Renderer", pComponent));

	// For.Texture 
	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone_Resource(RESOURCE_STAGE, L"Texture_Smoke"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::CComponent::ID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture", pComponent));

	// For.Shader
	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_Alpha"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::CComponent::ID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Shader", pComponent));

	return S_OK;
}

Engine::_int CStepFog::Update_Object(const Engine::_float & fTimeDelta)
{
	if (m_pTargetTransCom == nullptr)
	{
		m_pTargetTransCom = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Com_Transform", Engine::CComponent::ID_DYNAMIC));
		NULL_CHECK_RETURN(m_pTargetTransCom, -1);
	}
	
	Engine::_uint iExit = Engine::CGameObject::Update_Object(fTimeDelta);

	m_vPos += m_vLook * fTimeDelta;

	m_fLifeTime += fTimeDelta;

	if (1.f < m_fLifeTime)
	{
		return 0;
	}
	else
	{
		m_pRendererCom->Add_RenderGroup(Engine::CRenderer::RENDER_ALPHABLEND, this);
	}

	return 0;
}

void CStepFog::Render_Object()
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	//m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);

	//m_pGraphicDev->SetRenderState(D3DRS_POINTSIZE, TRUE);

	/*m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
	m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(255, 255, 255, 255));*/

	_matrix matWorld, matScale, matTrans, matBillBord;

	D3DXMatrixIdentity(&matScale);
	D3DXMatrixIdentity(&matTrans);
	D3DXMatrixIdentity(&matBillBord);

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matBillBord);
	
	memset(&matBillBord.m[3][0], 0, sizeof(_vec3));

	D3DXMatrixInverse(&matBillBord, nullptr, &matBillBord);

	memcpy(&matBillBord.m[3][0], &m_vPos, sizeof(_vec3));
	D3DXMatrixScaling(&matScale, 0.8f, 0.8f, 0.f);

	matWorld = matScale * matBillBord;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);

	m_pTextureCom->Set_Texture();
	m_pBufferCom->Render_Buffer();

	/*m_pGraphicDev->SetRenderState(D3DRS_POINTSIZE, FALSE);*/

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	//m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

HRESULT CStepFog::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	pEffect->AddRef();

	_matrix matWorld, matScale, matTrans, matBillBord, matView, matProj;

	D3DXMatrixIdentity(&matScale);
	D3DXMatrixIdentity(&matTrans);
	D3DXMatrixIdentity(&matBillBord);

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matBillBord);

	memset(&matBillBord.m[3][0], 0, sizeof(_vec3));

	D3DXMatrixInverse(&matBillBord, nullptr, &matBillBord);
	
	if (m_bTime == true)
	{
		m_bTime = false;
		memcpy(&m_vPos, &m_pTargetTransCom->m_matWorld.m[3][0], sizeof(_vec3));
	}

	memcpy(&matBillBord.m[3][0], &m_vPos, sizeof(_vec3));

	D3DXMatrixScaling(&matScale, 0.8f, 0.5f, 0.f);

	matWorld = matScale * matBillBord;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	float fPower = 0.5f;

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);
	pEffect->SetFloat("g_fPower", fPower);

	m_pTextureCom->Set_Texture(pEffect, "g_BaseTexture", _uint(0));

	Engine::SetUp_OnShader(pEffect, L"Target_Depth", "g_DepthTexture");

	Engine::Safe_Release(pEffect);

	return S_OK;
}

CStepFog * CStepFog::Create(LPDIRECT3DDEVICE9 pGraphicDev, _matrix* pmatPos, _vec3* pDir)
{
	CStepFog*	pInstance = new CStepFog(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pmatPos, pDir)))
	{
		ERR_BOX(L"StepFog Create Failed");
		Engine::Safe_Release(pInstance);
	}
	
	return pInstance;
}

void CStepFog::Free()
{
	Engine::CGameObject::Free();
}
