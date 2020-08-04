//Management Instance
Engine::CManagement*		Engine::Get_Management()
{
	return Engine::CManagement::GetInstance();
}

//Getter
Engine::CComponent*			Engine::Get_Component(const Engine::_tchar*		pLayerTag,
													const Engine::_tchar*		pObjTag,
													const Engine::_tchar*		pComponentTag,
													Engine::CComponent::COMPONENTID eID)
{
	return Engine::CManagement::GetInstance()->Get_Component(pLayerTag, pObjTag, pComponentTag, eID);
}

Engine::CComponent*			Get_ComponentIndex(const Engine::_tchar*		pLayerTag,
												const Engine::_tchar*		pObjTag,
												const Engine::_tchar*		pComponentTag,
												Engine::CComponent::COMPONENTID eID,
												int iObjectIndex)
{
	return Engine::CManagement::GetInstance()->Get_Component(pLayerTag, pObjTag, pComponentTag, eID, iObjectIndex);
}

//Setter
//General
HRESULT	Engine::Create_Management(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CManagement** ppManagement)
{
	CManagement*		pManagement = CManagement::GetInstance();

	if (nullptr == pManagement)
		return E_FAIL;

	if (FAILED(pManagement->Ready_Management(pGraphicDev)))
		return E_FAIL;

	*ppManagement = pManagement;

	return S_OK;
}

HRESULT	Engine::SetUp_CurrentScene(Engine::CScene*	pCurrentScene)
{
	return Engine::CManagement::GetInstance()->SetUp_CurrentScene(pCurrentScene);
}

// Renderer Instance
//Getter
Engine::CRenderer*	Engine::Get_Renderer(void)
{
	return Engine::CRenderer::GetInstance();
}

//Setter
HRESULT	Engine::Ready_Renderer(LPDIRECT3DDEVICE9 pGraphicDev)
{
	return Engine::CRenderer::GetInstance()->Ready_Renderer(pGraphicDev);
}
//General

// LightMgr Instance
//Getter
const	D3DLIGHT9*	Get_LightInfo(const _uint& iIndex)
{
	return CLightMgr::GetInstance()->Get_LightInfo(iIndex);
}
//Setter
//General
void		Render_Light(LPD3DXEFFECT pEffect)
{
	CLightMgr::GetInstance()->Render_Light(pEffect);
}

HRESULT		Engine::Ready_Light(LPDIRECT3DDEVICE9 pGraphicDev,
	const D3DLIGHT9* pLightInfo,
	const Engine::_uint& iIndex)
{
	return Engine::CLightMgr::GetInstance()->Ready_Light(pGraphicDev, pLightInfo, iIndex);
}

// PrototypeMgr Instance
//Getter
//Setter
//General
HRESULT		Engine::Ready_Prototype(const Engine::_tchar* pProtTag, Engine::CComponent* pInstance)
{
	return Engine::CPrototypeMgr::GetInstance()->Ready_Prototype(pProtTag, pInstance);
}

Engine::CComponent* Engine::Clone_Prototype(const Engine::_tchar* pProtoTag)
{
	return Engine::CPrototypeMgr::GetInstance()->Clone_Prototype(pProtoTag);
}

//// RenderTargetMgr Instance
////Getter
Engine::CRenderTargetMgr*	Get_RenderTarget()
{
	return Engine::CRenderTargetMgr::GetInstance();
}

// Setter
void	Engine::SetUp_OnShader(LPD3DXEFFECT pEffect, const Engine::_tchar* pTargetTag, const char* pContantName)
{
	Engine::CRenderTargetMgr::GetInstance()->SetUp_OnShader(pEffect, pTargetTag, pContantName);
}

//General
HRESULT		Engine::Ready_RenderTarget(const _tchar* pTargetTag,
								LPDIRECT3DDEVICE9 pGraphicDev,
								const Engine::_uint& iWidth,
								const Engine::_uint& iHeight,
								D3DFORMAT Format,
								D3DXCOLOR Color)
{
	return Engine::CRenderTargetMgr::GetInstance()->Ready_RenderTarget(pTargetTag, pGraphicDev, iWidth, iHeight, Format, Color);
}

HRESULT		Engine::Ready_MRT(const Engine::_tchar* pMRTTag, const Engine::_tchar* pTargetTag)
{
	return Engine::CRenderTargetMgr::GetInstance()->Ready_MRT(pMRTTag, pTargetTag);
}

HRESULT		Engine::Ready_DebugBuffer(const Engine::_tchar* pTargetTag,
										const Engine::_float& fX,
										const Engine::_float& fY,
										const Engine::_float& fSizeX,
										const Engine::_float& fSizeY)
{
	return Engine::CRenderTargetMgr::GetInstance()->Ready_DebugBuffer(pTargetTag, fX, fY, fSizeX, fSizeY);
}

void	Engine::Render_DebugBuffer(const Engine::_tchar* pTargetTag)
{
	CRenderTargetMgr::GetInstance()->Render_DebugBuffer(pTargetTag);
}

void	Engine::Release_Utility(void)
{
	Engine::CRenderTargetMgr::GetInstance()->DestroyInstance();
	Engine::CPrototypeMgr::GetInstance()->DestroyInstance();
	Engine::CLightMgr::GetInstance()->DestroyInstance();
	Engine::CRenderer::GetInstance()->DestroyInstance();
	Engine::CManagement::GetInstance()->DestroyInstance();
}

