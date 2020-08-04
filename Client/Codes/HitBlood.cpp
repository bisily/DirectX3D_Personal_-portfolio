#include "stdafx.h"
#include "HitBlood.h"

#include "Export_Function.h"

CHitBlood::CHitBlood(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{

}

CHitBlood::~CHitBlood()
{

}

HRESULT CHitBlood::Ready_Object(_matrix* pmatPos)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_pTransCom->Set_Scale(10.f, 10.f, 10.f);
	
	memcpy(&m_pTransCom->m_vInfo[Engine::CTransform::INFO_POS], &(pmatPos->m[3][0]), sizeof(_vec3));

	srand((unsigned int)time(nullptr));

	m_iArrayIndex = rand() % 3;

	srand((unsigned int)time(NULL));

	int iValue = rand() % 5 + 1;

	int iCon = rand() % 5 + 1;

	_float fValue = (_float)iValue * 0.05f;

	switch (iCon)
	{
	case 1:
		m_pTransCom->m_vInfo[Engine::CTransform::INFO_POS].x = m_pTransCom->m_vInfo[Engine::CTransform::INFO_POS].x + fValue;
		m_pTransCom->m_vInfo[Engine::CTransform::INFO_POS].z = m_pTransCom->m_vInfo[Engine::CTransform::INFO_POS].z + fValue;
		break;

	case 2:
		m_pTransCom->m_vInfo[Engine::CTransform::INFO_POS].x = m_pTransCom->m_vInfo[Engine::CTransform::INFO_POS].x - fValue;
		m_pTransCom->m_vInfo[Engine::CTransform::INFO_POS].z = m_pTransCom->m_vInfo[Engine::CTransform::INFO_POS].z - fValue;
		break;
	}

	return S_OK;
}

_int CHitBlood::Update_Object(const _float& fTimeDelta)
{
	Engine::CGameObject::Update_Object(fTimeDelta);

	if (m_bKillTime == true)
	{
		return -1;
	}

	Billboard(&m_pTransCom->m_matWorld);
	
	m_fViewTime += fTimeDelta;

	if (0.05f< m_fViewTime)
	{
		m_iTexIndex++;
		m_fViewTime = 0.f;
	}

	if (m_iArrayIndex == 0 &&
		7 < m_iTexIndex)
	{
		m_bKillTime = true;
	}
	else if (15 < m_iTexIndex)
	{
		m_bKillTime = true;
	}
	
	if (m_bKillTime == false)
	{
		m_pRendererCom->Add_RenderGroup(Engine::CRenderer::RENDER_ALPHABLEND, this);
	}

	return 0;
}

void CHitBlood::Render_Object()
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0x00000088);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransCom->m_matWorld);

	m_pTextureCom[3]->Set_Texture(m_iTexIndex);
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

HRESULT CHitBlood::Add_Component()
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

	// For.Texture 0
	pComponent = m_pTextureCom[0] = dynamic_cast<Engine::CTexture*>(Engine::Clone_Resource(RESOURCE_STAGE, L"Texture_Blood00"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::CComponent::ID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture0", pComponent));

	// For.Texture 1
	pComponent = m_pTextureCom[1] = dynamic_cast<Engine::CTexture*>(Engine::Clone_Resource(RESOURCE_STAGE, L"Texture_Blood01"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::CComponent::ID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture1", pComponent));

	// For.Texture 2
	pComponent = m_pTextureCom[2] = dynamic_cast<Engine::CTexture*>(Engine::Clone_Resource(RESOURCE_STAGE, L"Texture_Blood02"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::CComponent::ID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture2", pComponent));

	// For.Texture 3
	pComponent = m_pTextureCom[3] = dynamic_cast<Engine::CTexture*>(Engine::Clone_Resource(RESOURCE_STAGE, L"Texture_Blood03"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::CComponent::ID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture3", pComponent));

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

void CHitBlood::Billboard(_matrix * pmatWorld)
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

HRESULT CHitBlood::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	pEffect->AddRef();

	_matrix		matWorld, matView, matProj;

	m_pTransCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	const D3DLIGHT9*		pLightInfo = Engine::Get_LightInfo();

	_vec4 vDir;

	memcpy(&vDir, &m_pTransCom->m_vInfo[Engine::CTransform::INFO_LOOK], sizeof(_vec3));

	vDir *= -1.f;
	vDir.w = 0.f;

	D3DXVec4Normalize(&vDir, &vDir);

	pEffect->SetVector("g_vLightDir", &vDir);
	pEffect->SetVector("g_vLightDiffuse", (_vec4*)&pLightInfo->Diffuse);
	//pEffect->SetVector("g_vLightSpecular", (_vec4*)&pLightInfo->Specular);
	pEffect->SetVector("g_vLightAmbient", (_vec4*)&pLightInfo->Ambient);


	D3DMATERIAL9			MtrlInfo;
	ZeroMemory(&MtrlInfo, sizeof(D3DMATERIAL9));

	MtrlInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	//MtrlInfo.Specular = D3DXCOLOR(0.f, 0.f, 0.f, 1.f);
	MtrlInfo.Ambient = D3DXCOLOR(0.f, 0.f, 0.f, 1.f);
	MtrlInfo.Emissive = D3DXCOLOR(0.f, 0.f, 0.f, 1.f);

	MtrlInfo.Power = 5.f;

	pEffect->SetVector("g_vMtrlDiffuse", (_vec4*)&MtrlInfo.Diffuse);
	//pEffect->SetVector("g_vMtrlSpecular", (_vec4*)&MtrlInfo.Specular);
	pEffect->SetVector("g_vMtrlAmbient", (_vec4*)&MtrlInfo.Ambient);
	pEffect->SetFloat("g_fPower", MtrlInfo.Power);

	D3DXMatrixInverse(&matView, nullptr, &matView);
	pEffect->SetVector("g_vCamPosition", (_vec4*)&matView.m[3][0]);

	//Engine::SetUp_OnShader(pEffect, L"Target_Depth", "g_DepthTexture");

	pEffect->SetFloat("g_fAlpha", 1.f);

	Engine::Safe_Release(pEffect);

	return S_OK;
}

CHitBlood* CHitBlood::Create(LPDIRECT3DDEVICE9 pGraphicDev, _matrix* pmatPos)
{
	CHitBlood*		pInstance = new CHitBlood(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pmatPos)))
	{
		ERR_BOX(L"HitBlood Create Failed");
		Engine::Safe_Release(pInstance);
	}

	return pInstance;
}

void CHitBlood::Free()
{
	Engine::CGameObject::Free();
}

