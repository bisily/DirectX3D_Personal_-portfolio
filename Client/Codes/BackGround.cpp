#include "stdafx.h"
#include "BackGround.h"

#include "Export_Function.h"

CBackGround::CBackGround(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{

}

CBackGround::~CBackGround()
{
	
}

HRESULT CBackGround::Ready_Object()
{
	if(FAILED(Add_Component()))
		return E_FAIL;

	return S_OK;
}

_int CBackGround::Update_Object(const _float& fTimeDelta)
{
	
	m_pTransCom->m_vScale.x = 2.f;
	m_pTransCom->m_vScale.y = 2.f;

	Engine::CGameObject::Update_Object(fTimeDelta);

	m_fTimer += fTimeDelta;


	if(m_bValue == false)
	{
		if (0.2f < m_fTimer)
		{
			m_fTimer = 0.f;
			m_fAlpha -= 0.05f;

			if (m_fAlpha < 0.f)
			{
				m_iIndex++;
				m_bValue = true;
			}

			if (6 < m_iIndex)
			{
				m_iIndex = 0;
			}
		}
	}
	else if (m_bValue == true)
	{
		if (0.2f < m_fTimer)
		{
			m_fTimer = 0.f;
			m_fAlpha += 0.05f;
		}

		if (1.f < m_fAlpha)
		{
			m_fAlpha = 1.f;
			
			if (1.f == m_bValue)
			{
				m_bValue = false;
			}
		}

	}

	m_pRendererCom->Add_RenderGroup(Engine::CRenderer::RENDER_PRIORITY, this);
	
	return 0;
}

void CBackGround::Render_Object()
{
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	
	pEffect->AddRef();

	_uint iPassCnt = 0;

	_matrix matWorld, matView, matProj;

 	matWorld = m_pTransCom->m_matWorld;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	pEffect->SetFloat("g_fAlph", m_fAlpha);

	m_pTextureCom->Set_Texture(pEffect, "g_BaseTexture", m_iIndex);
	
	pEffect->Begin(&iPassCnt, 0);
	pEffect->BeginPass(0);

	m_pBufferCom->Render_Buffer();

	pEffect->EndPass();
	pEffect->End();

	Engine::Safe_Release(pEffect);
}

HRESULT CBackGround::Add_Component()
{
	Engine::CComponent*			pComponent = nullptr;

	// For.Buffer
	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(Engine::Clone_Resource(RESOURCE_STATIC, L"Buffer_RcTex_Center"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::CComponent::ID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Buffer", pComponent));
	
	// For.Transform
	pComponent = m_pTransCom = Engine::CTransform::Create();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::CComponent::ID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));

	// For.Texture 
	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone_Resource(RESOURCE_LOGO, L"Texture_Logo"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::CComponent::ID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture", pComponent));
	
	// For.Renderer
	pComponent = m_pRendererCom = Engine::Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->AddRef();
	m_mapComponent[Engine::CComponent::ID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Renderer", pComponent));

	// For.Shader
	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_Sample"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::CComponent::ID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Shader", pComponent));

	return S_OK;
}

CBackGround* CBackGround::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBackGround*		pInstance = new CBackGround(pGraphicDev);

	if(FAILED(pInstance->Ready_Object()))
	{
		ERR_BOX(L"BackGround Create Failed");
		Engine::Safe_Release(pInstance);
	}

	return pInstance;
}

void CBackGround::Free()
{
	Engine::CGameObject::Free();
}

