#include "stdafx.h"
#include "MonsterLifeBar.h"

#include "Export_Function.h"

CMonsterLifeBar::CMonsterLifeBar(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{

}

CMonsterLifeBar::~CMonsterLifeBar()
{

}

HRESULT CMonsterLifeBar::Ready_Object(wstring strObjKey, _int iIndex)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_iIndex = iIndex;
	m_strObjKey = strObjKey;
	
	m_fSizeX = 1.f;
	m_fSizeY = 1.f;
	
	D3DXMatrixIdentity(&m_matWorld);
	return S_OK;
}

_int CMonsterLifeBar::Update_Object(const _float& fTimeDelta)
{
	if (m_bIsDead == true)
	{
		return -1;
	}

	Engine::CGameObject::Update_Object(fTimeDelta);

	if (m_pTargetStateCom == nullptr)
	{
		Engine::CObjState*	pObjState = dynamic_cast<Engine::CObjState*>(Engine::Get_ComponentIndex(L"Layer_GameLogic", m_strObjKey.c_str(), L"Com_State", Engine::CComponent::ID_DYNAMIC, m_iIndex));

		NULL_CHECK_RETURN(pObjState, -1);

		m_pTargetStateCom = pObjState;

		Engine::CTransform*	pTransform = dynamic_cast<Engine::CTransform*>(Engine::Get_ComponentIndex(L"Layer_GameLogic", m_strObjKey.c_str(), L"Com_Transform", Engine::CComponent::ID_DYNAMIC, m_iIndex));

		NULL_CHECK_RETURN(pTransform, -1);

		m_pTargetPosCom = pTransform;
	}

	m_matWorld = m_pTargetPosCom->m_matWorld;
	m_matWorldHp = m_matWorld;
	
	Billboard(&m_matWorld, &m_matWorldHp);

	if (m_bTrigger == true)
	{
		m_fViewTime += fTimeDelta;

		if (5.f < m_fViewTime)
		{
			m_fViewTime = 0.f;
			m_bTrigger = false;
			m_bRender = false;
		}
		else
		{
			m_bRender = true;
		}
	}

	if (m_pTargetStateCom->Get_Hp() <= 0)
	{
		m_bRender = false;
	}

	m_pRendererCom->Add_RenderGroup(Engine::CRenderer::RENDER_ALPHABLEND, this);

	return 0;
}

void CMonsterLifeBar::Render_Object()
{
	if (m_bRender == true)
	{
		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

		m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
		m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0x00000088);
		m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

		m_matWorld.m[3][0] -= 0.5f;

		m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);
	
		m_pTextureCom->Set_Texture(0);
		m_pBufferCom->Render_Buffer();

		m_matWorldHp.m[3][0] -= 0.5f;
		m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorldHp);
	
		m_pTextureHpBarCom->Set_Texture(0);
		m_pBufferCom->Render_Buffer();

		m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
		m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	}
}

HRESULT CMonsterLifeBar::Add_Component()
{
	Engine::CComponent*			pComponent = nullptr;

	// For.Buffer
	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(Engine::Clone_Resource(RESOURCE_STATIC, L"Buffer_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::CComponent::ID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Buffer", pComponent));

	// For.Texture 
	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone_Resource(RESOURCE_STAGE, L"Texture_EnanmyHp"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::CComponent::ID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture", pComponent));

	// For.Texture 
	pComponent = m_pTextureHpBarCom = dynamic_cast<Engine::CTexture*>(Engine::Clone_Resource(RESOURCE_STAGE, L"Texture_EnanmyHpGauge"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::CComponent::ID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_TextureHpBar", pComponent));

	// For.Renderer
	pComponent = m_pRendererCom = Engine::Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->AddRef();
	m_mapComponent[Engine::CComponent::ID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Renderer", pComponent));

	return S_OK;
}

void CMonsterLifeBar::Billboard(_matrix * pmatWorld, _matrix* pmatWorldHp)
{
	_matrix matTemp, matScaling, matScalingHp;
	_vec3 vecTem, vecTemHP;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matTemp);

	D3DXMatrixScaling(&matScaling, 1.f, 0.5f, 0.f);
	D3DXMatrixScaling(&matScalingHp, (m_pTargetStateCom->m_fHp / m_pTargetStateCom->m_fMaxHp), 0.5f, 0.f);
	
	memcpy(&vecTem, &pmatWorld->m[3][0], sizeof(_vec3));
	memcpy(&vecTemHP, &pmatWorldHp->m[3][0], sizeof(_vec3));

	vecTem.y += 2.f;
	vecTemHP.y += 2.f;

	memset(&matTemp.m[3][0], 0, sizeof(_vec3));

	D3DXMatrixInverse(&matTemp, nullptr, &matTemp);

	*pmatWorld = matScaling * matTemp;

	*pmatWorldHp = matScalingHp * matTemp;
	
	memcpy(&pmatWorld->m[3][0], &vecTem, sizeof(_vec3));
	memcpy(&pmatWorldHp->m[3][0], &vecTemHP, sizeof(_vec3));
}

CMonsterLifeBar* CMonsterLifeBar::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring strObjKey, _int iIndex)
{
	CMonsterLifeBar*		pInstance = new CMonsterLifeBar(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(strObjKey, iIndex)))
	{
		ERR_BOX(L"MonsterLifeBar Create Failed");
		Engine::Safe_Release(pInstance);
	}

	return pInstance;
}

void CMonsterLifeBar::Free()
{
	Engine::CGameObject::Free();
}

