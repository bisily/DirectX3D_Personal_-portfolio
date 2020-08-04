#include "stdafx.h"
#include "LifeBar.h"

#include "Export_Function.h"

CLifeBar::CLifeBar(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{

}

CLifeBar::~CLifeBar()
{

}

HRESULT CLifeBar::Ready_Object()
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_fX = 250.f;
	m_fY = 87.f;

	m_fSizeX = 300.f;
	m_fSizeY = 70.f;
	
	return S_OK;
}

_int CLifeBar::Update_Object(const _float& fTimeDelta)
{
	Engine::CGameObject::Update_Object(fTimeDelta);

	if (m_pTargetCom == nullptr)
	{
		Engine::CObjState*	pTransform = dynamic_cast<Engine::CObjState*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Com_State", Engine::CComponent::ID_DYNAMIC));

		NULL_CHECK_RETURN(pTransform, -1);

		m_pTargetCom = pTransform;
	}

	m_pRendererCom->Add_RenderGroup(Engine::CRenderer::RENDER_ALPHABLEND, this);

	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0.f, 1.f);

	return 0;
}

void CLifeBar::Render_Object()
{
	wsprintf(m_szHp, L"HP : %d", (int)m_pTargetCom->m_fHp);
	
	//Engine::Render_Font(L"Font_Default", m_szHp, &_vec2(600.f, 0.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

	_matrix		matWorld, matView, matOldView, matOldProj;

	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixIdentity(&matView);

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matOldView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matOldProj);

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	matWorld._11 = m_fSizeX;
	matWorld._22 = m_fSizeY;
	matWorld._33 = 1.f;
	matWorld._41 = m_fX - WINCX * 0.5f;
	matWorld._42 = -m_fY + WINCY * 0.5f;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);

	m_pTextureCom->Set_Texture(0);
	m_pBufferCom->Render_Buffer();

	matWorld._11 = m_fSizeX * (m_pTargetCom->m_fHp / m_pTargetCom->m_fMaxHp);
	matWorld._22 = m_fSizeY;
	matWorld._33 = 1.f;

	float fEditSizeX = m_fSizeX - (m_fSizeX * (m_pTargetCom->m_fHp / m_pTargetCom->m_fMaxHp));

	matWorld._41 = (m_fX - WINCX * 0.5f) - (fEditSizeX /2.f);
	matWorld._42 = -m_fY + WINCY * 0.5f;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);

	m_pTextureHpBarCom->Set_Texture(0);
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matOldView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matOldProj);
}

HRESULT CLifeBar::Add_Component()
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
	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone_Resource(RESOURCE_STAGE, L"Texture_LifeBar_F"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::CComponent::ID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture", pComponent));
	
	// For.Texture 
	pComponent = m_pTextureHpBarCom = dynamic_cast<Engine::CTexture*>(Engine::Clone_Resource(RESOURCE_STAGE, L"Texture_LifeBar_G"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::CComponent::ID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_TextureHpBar", pComponent));

	// For.Renderer
	pComponent = m_pRendererCom = Engine::Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->AddRef();
	m_mapComponent[Engine::CComponent::ID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Renderer", pComponent));

	return S_OK;
}

CLifeBar* CLifeBar::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CLifeBar*		pInstance = new CLifeBar(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		ERR_BOX(L"LifeBar Create Failed");
		Engine::Safe_Release(pInstance);
	}

	return pInstance;
}

void CLifeBar::Free()
{
	Engine::CGameObject::Free();
}

