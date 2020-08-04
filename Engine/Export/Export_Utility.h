#ifndef Export_Utility_h__
#define Export_Utility_h__

#include "Engine_Defines.h"
#include "Management.h"

#include "Transform.h"
#include "Renderer.h"
#include "LightMgr.h"
#include "Calculator.h"
#include "Collider.h"
#include "SphereCollider.h"
#include "PrototypeMgr.h"
#include "RenderTargetMgr.h"
#include "ObjState.h"

BEGIN(Engine)

//Management Instance
inline CManagement*			Get_Management();

//Getter
inline 	CComponent*			Get_Component(const _tchar*		pLayerTag,
											const _tchar*		pObjTag,
											const _tchar*		pComponentTag,
											Engine::CComponent::COMPONENTID eID);

inline 	CComponent*			Get_ComponentIndex(const _tchar*		pLayerTag,
												const _tchar*		pObjTag,
												const _tchar*		pComponentTag,
												Engine::CComponent::COMPONENTID eID,
												int iObjectIndex);

//Setter
//General
inline HRESULT				Create_Management(LPDIRECT3DDEVICE9 pGraphicDev, CManagement** ppManagement);
inline HRESULT				SetUp_CurrentScene(CScene*	pCurrentScene);

// Renderer Instance
//Getter
inline CRenderer*			Get_Renderer();

//Setter
//General
inline HRESULT	Ready_Renderer(LPDIRECT3DDEVICE9 pGraphicDev);

///////////////////////////////////////////////////////////////////////////////////////////////////////
// LightMgr Instance
// LightMgr Instance
//Getter
inline const	D3DLIGHT9*	Get_LightInfo(const _uint& iIndex = 0);
//Setter
inline void		Render_Light(LPD3DXEFFECT pEffect);
//General
inline HRESULT				Ready_Light(LPDIRECT3DDEVICE9 pGraphicDev,
										const D3DLIGHT9* pLightInfo,
										const _uint& iIndex);

///////////////////////////////////////////////////////////////////////////////////////////////////////
// PrototypeMgr Instance
//Getter
//Setter
//General
inline HRESULT				Ready_Prototype(const _tchar* pProtoTag, CComponent* pInstance);
inline CComponent*			Clone_Prototype(const _tchar* pProtoTag);

///////////////////////////////////////////////////////////////////////////////////////////////////////
// RenderTargetMgr Instance
//Getter
inline CRenderTargetMgr*	Get_RenderTarget(void);
//Setter
inline void					SetUp_OnShader(LPD3DXEFFECT pEffect, const _tchar* pTargetTag, const char* pContantName);
//General
inline 	HRESULT				Ready_RenderTarget(const _tchar* pTargetTag,
												LPDIRECT3DDEVICE9 pGraphicDev,
												const _uint& iWidth,
												const _uint& iHeight,
												D3DFORMAT Format,
												D3DXCOLOR Color);

inline HRESULT				Ready_MRT(const _tchar* pMRTTag, const _tchar* pTargetTag);

inline HRESULT				Ready_DebugBuffer(const _tchar* pTargetTag,
												const _float& fX,
												const _float& fY,
												const _float& fSizeX,
												const _float& fSizeY);

inline void					Render_DebugBuffer(const _tchar* pTargetTag);

inline void					Release_Utility();

#include "Export_Utility.inl"

END

#endif // Export_Utility_h__
