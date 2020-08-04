#include "stdafx.h"
#include "Field.h"

#include "Export_Function.h"

CField::CField(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CField::~CField(void)
{
	Release();
}

HRESULT CField::Ready_Object(void)
{
	if (FAILED(AddComponent()))
		return E_FAIL;

	return S_OK;
}

int CField::Update_Object(const float & fTimeDelta)
{
	int iExit = Engine::CGameObject::Update_Object(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(Engine::CRenderer::RENDER_NONALPHA, this);

	return iExit;
}

void CField::Render_Object(void)
{
	m_pInfo->SetTransform(m_pGraphicDev);

	m_pTexture->Set_Texture(0);
	m_pBuffer->Render_Buffer();
}

void CField::Release(void)
{
	Engine::Safe_Delete_Array(m_pVertex);
	Engine::Safe_Delete_Array(m_pConvertVertex);
}

CField * CField::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CField*		pInstance = new CField(pGraphicDev);

	if (FAILED(pInstance->Initialize()))
		Engine::Safe_Delete(pInstance);

	return pInstance;
}

HRESULT CField::Initialize(void)
{
	
	m_dwVtxCnt = 129 * 129;

	m_pVertex = new Engine::VTXTEX[m_dwVtxCnt];
	m_pConvertVertex = new Engine::VTXTEX[m_dwVtxCnt];

	m_pResourcesMgr->GetVtxInfo(Engine::RESOURCE_STATIC, L"Buffer Terrain", m_pVertex);


	FAILED_CHECK(AddComponent());

	m_fSpeed = 100.f;

	return S_OK;
}

HRESULT CField::AddComponent(void)
{
	Engine::CComponent*		pComponent = nullptr;


	// For.Transform
	pComponent = m_pInfo = Engine::CTransform::Create();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::CComponent::ID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));
	
	// For.Buffer
	pComponent = m_pBuffer = dynamic_cast<Engine::CTerrainTex*>(Engine::Clone_Resource(1, L"Buffer_Terrain"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::CComponent::ID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Buffer", pComponent));

	// For.Texture
	pComponent = m_pTexture = dynamic_cast<Engine::CTexture*>(Engine::Clone_Resource(1, L"Texture_Terrain"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::CComponent::ID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture", pComponent));

	// For.Renderer
	pComponent = m_pRendererCom = Engine::Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->AddRef();
	m_mapComponent[Engine::CComponent::ID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Renderer", pComponent));

	return S_OK;
}

