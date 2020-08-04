#include "stdafx.h"
#include "PlayerHitBlood.h"

#include "Export_Function.h"

CPlayer_HitBlood::CPlayer_HitBlood(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{

}

CPlayer_HitBlood::~CPlayer_HitBlood()
{

}

HRESULT CPlayer_HitBlood::Ready_Object(_vec3 vPos, _int iTexIndex)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_pTransCom->Set_Scale(2.f, 2.f, 0.f);

	srand((unsigned int)time(nullptr));

	m_iTexIndex = iTexIndex;

	m_vPos = vPos;

	m_iX = rand() % 600;
	m_iY = rand() % 400;

	return S_OK;
}

_int CPlayer_HitBlood::Update_Object(const _float& fTimeDelta)
{
	m_pTransCom->m_vScale.x = 2.f;
	m_pTransCom->m_vScale.y = 2.f;

	Engine::CGameObject::Update_Object(fTimeDelta);

	Engine::CTransform* pTransform = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Com_Transform", Engine::CTransform::ID_DYNAMIC));

	memcpy(&m_pTransCom->m_vInfo[Engine::CTransform::INFO_POS], &pTransform->m_matWorld.m[3][0], sizeof(_vec3));

	/*Billboard(&m_pTransCom->m_matWorld);*/

	if (m_fAlpha < 50)
	{
		return -1;
	}
	else
	{
		m_fAlpha -= 20.f * fTimeDelta;

		m_vPos.y += 2.f * fTimeDelta;
	}
	
	m_pRendererCom->Add_RenderGroup(Engine::CRenderer::RENDER_ALPHABLEND, this);

	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0.f, 1.f);

	return 0;
}

void CPlayer_HitBlood::Render_Object()
{
	/*m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransCom->m_matWorld);

	m_pTextureCom->Set_Texture();
	m_pBufferCom->Render_Buffer();*/

	
	/*LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);

	pEffect->AddRef();

	_uint iPassCnt = 0;

	_matrix	matWorld, matView, matOldView, matOldProj;

	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixIdentity(&matView);

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matOldView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matOldProj);

	matWorld._11 = 200.f;
	matWorld._22 = 200.f;
	matWorld._33 = 1.f;
	matWorld._41 = 500 - WINCX * 0.5f;
	matWorld._42 = -500 + WINCY * 0.5f;

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &m_matProj);

	pEffect->SetFloat("g_fAlph", 1.f);

	m_pTextureCom[0]->Set_Texture(pEffect, "g_BaseTexture", 0);

	pEffect->Begin(&iPassCnt, 0);
	pEffect->BeginPass(0);

	m_pTextureCom[0]->Set_Texture();
	m_pBufferCom->Render_Buffer();

	pEffect->EndPass();
	pEffect->End();

	Engine::Safe_Release(pEffect);*/
	
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0x00000088);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);


	_matrix		matWorld, matView, matOldView, matOldProj;

	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixIdentity(&matView);

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matOldView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matOldProj);

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);

	m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB((int)m_fAlpha, 255, 255, 255));
	
	matWorld._11 = 100.f;
	matWorld._22 = 100.f;
	matWorld._33 = 1.f;
	matWorld._41 = (float)m_vPos.x - WINCX * 0.5f;
	matWorld._42 = (float)-m_vPos.y + WINCY * 0.5f;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);

	m_pTextureCom[m_iTexIndex]->Set_Texture();
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(255, 255, 255, 255));


	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matOldView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matOldProj);

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

HRESULT CPlayer_HitBlood::Add_Component()
{
	Engine::CComponent*			pComponent = nullptr;

	// For.Transform
	pComponent = m_pTransCom = Engine::CTransform::Create();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::CComponent::ID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));

	// For.Buffer
	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(Engine::Clone_Resource(RESOURCE_STATIC, L"Buffer_RcTex_Center"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::CComponent::ID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Buffer", pComponent));

	// For.Texture 
	for (int i = 0; i < 4; ++i)
	{
		string strTem = to_string(i);
		wstring wstrTem, strCloneName = L"Texture_PlayerBlood0", strTagName = L"Com_Texture";

		wstrTem.assign(strTem.begin(), strTem.end());
		
		strCloneName += wstrTem;

		pComponent = m_pTextureCom[i] = dynamic_cast<Engine::CTexture*>(Engine::Clone_Resource(RESOURCE_STAGE, strCloneName));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		
		strTagName += wstrTem;

		m_mapComponent[Engine::CComponent::ID_STATIC].insert(MAPCOMPONENT::value_type(strTagName, pComponent));
	}

	// For.Renderer
	pComponent = m_pRendererCom = Engine::Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->AddRef();
	m_mapComponent[Engine::CComponent::ID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Renderer", pComponent));

	// For.Shader
	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_Alpha"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::CComponent::ID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Shader", pComponent));

	return S_OK;
}

void CPlayer_HitBlood::Billboard(_matrix * pmatWorld)
{
	_matrix matTemp, matScaling;
	_vec3 vecTem;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matTemp);

	D3DXMatrixScaling(&matScaling, 1.f, 1.f, 1.f);

	memcpy(&vecTem, &pmatWorld->m[3][0], sizeof(_vec3));

	vecTem.y += 1.f;

	memset(&matTemp.m[3][0], 0, sizeof(_vec3));

	D3DXMatrixInverse(&matTemp, nullptr, &matTemp);
	
	*pmatWorld = matScaling * matTemp;

	memcpy(&pmatWorld->m[3][0], &vecTem, sizeof(_vec3));
}

CPlayer_HitBlood* CPlayer_HitBlood::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos, _int iTexIndex)
{
	CPlayer_HitBlood*		pInstance = new CPlayer_HitBlood(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(vPos, iTexIndex)))
	{
		ERR_BOX(L"Player_HitBlood Create Failed");
		Engine::Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayer_HitBlood::Free()
{
	Engine::Safe_Release(m_pTextureCom[0]);
	Engine::Safe_Release(m_pTextureCom[1]);
	Engine::Safe_Release(m_pTextureCom[2]);
	Engine::Safe_Release(m_pTextureCom[3]);

	Engine::CGameObject::Free();
}

