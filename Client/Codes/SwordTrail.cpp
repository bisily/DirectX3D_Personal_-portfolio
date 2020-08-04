#include "stdafx.h"
#include "SwordTrail.h"

#include "Export_Function.h"

CSwordTrail::CSwordTrail(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CSwordTrail::~CSwordTrail()
{
}

HRESULT CSwordTrail::Ready_Object(wstring strObjKey, wstring strWeaponName, wstring strCloneTexFile, _int iIndex)
{
	if (FAILED(Add_Component(strCloneTexFile)))
	{
		ERR_BOX(L"SwordTrail, AddComponent Failed");

		return E_FAIL;
	}

	if (iIndex != 0)
	{
		m_iIndex = iIndex;
	}

	m_strObjkey = strObjKey;
	m_strWeaponName = strWeaponName;

	return S_OK;
}

_int CSwordTrail::Update_Object(const _float & fTimeDelta)
{
	if (m_pParentWorldMatrix == nullptr)
	{
		Engine::CTransform* pSwordInfo = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(L"Layer_GameLogic", m_strWeaponName.c_str(), L"Com_Transform", Engine::CComponent::ID_DYNAMIC));

		NULL_CHECK_RETURN(pSwordInfo, -1);
		m_pParentWorldMatrix = &pSwordInfo->m_matWorld;

		if (lstrcmp(L"Boss_Wave_Up", m_strObjkey.c_str()) && lstrcmp(L"Boss_Wave_Down", m_strObjkey.c_str()))
		{
			Engine::CObjState* pObjState = dynamic_cast<Engine::CObjState*>(Engine::Get_Component(L"Layer_GameLogic", m_strObjkey.c_str(), L"Com_State", Engine::CComponent::ID_DYNAMIC));
			NULL_CHECK_RETURN(pObjState, -1);
			m_pPlayerStateCom = pObjState;
		}

	}

	if (m_bDead == true)
	{
		return -1;
	}

	Engine::_int iExit = Engine::CGameObject::Update_Object(fTimeDelta);


	if (!lstrcmp(L"Sword", m_strWeaponName.c_str()) || !lstrcmp(L"Boss_Sword", m_strWeaponName.c_str()))
	{
		PointMemorize(fTimeDelta);
	}
	else
	{
		PointMemorizeWave(fTimeDelta);
	}

	m_pRendererCom->Add_RenderGroup(Engine::CRenderer::RENDER_ALPHABLEND, this);

	return 0;
}

void CSwordTrail::Render_Object()
{
	if (m_pPlayerStateCom != nullptr)
	{
		if (m_pPlayerStateCom->Get_State() != Engine::CObjState::NOW_ATTACK)
		{
			return;
		}
	}
	
	if (m_Pointlist.size() <= 3)
	{
		return;
	}

	auto		iter = m_Pointlist.begin();
	auto		iter_end = m_Pointlist.end();

	_vec3*		pPoint = new _vec3[m_Pointlist.size() / 2];
	_vec3*		pDir = new _vec3[m_Pointlist.size() / 2];

	int		iIndex = 0;
	for (; iter != iter_end; ++iter)
	{
		pPoint[iIndex] = (*iter);
		++iter;

		pDir[iIndex] = (*iter);
		++iIndex;
	}

	m_pBufferCom->SetVertexTrail(pPoint, pDir, m_fLength, m_Pointlist.size(), &m_pTransCom->m_matWorld);
	m_pTextureCom->Set_Texture(0);

	m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(255, 255, 255, 255));
	m_pBufferCom->Render_Buffer();

	Engine::Safe_Delete_Array(pPoint);
	Engine::Safe_Delete_Array(pDir);
}

HRESULT CSwordTrail::Add_Component(wstring strCloneTexFile)
{
	Engine::CComponent*		pComponent = nullptr;

	// For.Transform
	pComponent = m_pTransCom = Engine::CTransform::Create();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::CComponent::ID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));

	// For.Buffer
	pComponent = m_pBufferCom = dynamic_cast<Engine::CTrailTex*>(Engine::Clone_Resource(RESOURCE_STATIC, L"Buffer_TrailTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::CComponent::ID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Buffer", pComponent));

	// For.Texture
	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone_Resource(RESOURCE_STAGE, strCloneTexFile));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::CComponent::ID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture", pComponent));

	// For.Renderer
	pComponent = m_pRendererCom = Engine::Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->AddRef();
	m_mapComponent[Engine::CComponent::ID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Renderer", pComponent));

	return S_OK;
}

void CSwordTrail::PointMemorize(const _float & fTimeDelta)
{
	while (m_Pointlist.size() >= 50)
	{
		m_Pointlist.pop_front();
		m_Pointlist.pop_front();
	}

	m_fAccTime += fTimeDelta;


	if (0.02f < m_fAccTime)
	{
		D3DXVECTOR3		vPoint[2];

		if (!lstrcmp(L"Sword", m_strWeaponName.c_str()))
		{
			vPoint[0] = D3DXVECTOR3(0.f, 0.f, 0.f);
			vPoint[1] = D3DXVECTOR3(0.f, 0.f, -150.f);
		}
		else
		{
			vPoint[0] = D3DXVECTOR3(0.f, 0.f, 0.f);
			vPoint[1] = D3DXVECTOR3(0.f, 0.f, -500.f);
		}

		D3DXVECTOR3		vDir, vPos;
		vDir = vPoint[1] - vPoint[0];
		m_fLength = D3DXVec3Length(&vDir) / 2.f;
		D3DXVec3Normalize(&vDir, &vDir);

		vPos = vDir * m_fLength;

		D3DXVec3TransformCoord(&vPos, &vPos, m_pParentWorldMatrix);
		m_Pointlist.push_back(vPos);

		D3DXVec3TransformNormal(&vDir, &vDir, m_pParentWorldMatrix);
		m_Pointlist.push_back(vDir);

		m_fAccTime = 0.f;
	}
}

void CSwordTrail::PointMemorizeWave(const _float & fTimeDelta)
{

	while (m_Pointlist.size() >= 40)
	{
		m_Pointlist.pop_front();
		m_Pointlist.pop_front();
	}

	m_fAccTime += fTimeDelta;

	if (0.02f < m_fAccTime)
	{
		D3DXVECTOR3		vPoint[2];

		if (!lstrcmp(L"Boss_Wave_Up", m_strObjkey.c_str()))
		{
			vPoint[0] = D3DXVECTOR3(0.f, 50.f, 0.f);
			vPoint[1] = D3DXVECTOR3(0.f, 0.f, 0.f);
		}
		else
		{
			vPoint[0] = D3DXVECTOR3(0.f, 0.f, 0.f);
			vPoint[1] = D3DXVECTOR3(0.f, 50.f, 0.f);
		}

		D3DXVECTOR3		vDir, vPos;
		vDir = vPoint[1] - vPoint[0];
		m_fLength = D3DXVec3Length(&vDir) / 2.f;
		D3DXVec3Normalize(&vDir, &vDir);

		vPos = vDir * m_fLength;

		D3DXVec3TransformCoord(&vPos, &vPos, m_pParentWorldMatrix);
		m_Pointlist.push_back(vPos);

		D3DXVec3TransformNormal(&vDir, &vDir, m_pParentWorldMatrix);
		m_Pointlist.push_back(vDir);
		
		m_fAccTime = 0.f;
	}
}

CSwordTrail * CSwordTrail::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring strObjKey, wstring strWeaponName, wstring strCloneTexFile, _int iIndex)
{
	CSwordTrail*	pInstance = new CSwordTrail(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(strObjKey, strWeaponName, strCloneTexFile, iIndex)))
	{
		ERR_BOX(L"SwordTrail, Create Failed");
		Engine::Safe_Release(pInstance);
	}

	return pInstance;
}

void CSwordTrail::Free()
{
	Engine::CGameObject::Free();
}
