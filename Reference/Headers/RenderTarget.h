#ifndef RenderTarget_h__
#define RenderTarget_h__

#include "Engine_Defines.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CRenderTarget : public CBase
{
private:
	explicit CRenderTarget(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CRenderTarget();

public:
	HRESULT	Ready_RenderTarget(const _uint& iWidth, const _uint& iHeight, D3DFORMAT Format, D3DXCOLOR Color);
	void	SetUp_OnGraphicDev(const _uint& iIndex);
	void	SetUp_OnShader(LPD3DXEFFECT pEffect, const char* pContantName);

	void	Clear_RenderTarget(void);
	void	Release_OnGraphicDev(const _uint& iIndex);

	HRESULT		Ready_DebugBuffer(const _float& fX,
									const _float& fY,
									const _float& fSizeX,
									const _float& fSizeY);

	void		Render_DebugBuffer(void);
private:
	LPDIRECT3DDEVICE9				m_pGraphicDev = nullptr;
	LPDIRECT3DTEXTURE9				m_pTargetTexture = nullptr;
	LPDIRECT3DSURFACE9				m_pTargetSurface = nullptr;		// RenderTarget : 텍스쳐에서 뽑아내 출력하는 실제적인 렌더 타겟
	LPDIRECT3DSURFACE9				m_pOldTargetSurface = nullptr;  // RenderTarget : 위의 서피스와 동일화 시키려는 서피스

	D3DXCOLOR						m_ClearColor;

	LPDIRECT3DVERTEXBUFFER9			m_pVB = nullptr;
	LPDIRECT3DINDEXBUFFER9			m_pIB = nullptr;

public:
	static CRenderTarget*		Create(LPDIRECT3DDEVICE9 pGraphicDev,
										const _uint& iWidth,
										const _uint& iHeight,
										D3DFORMAT Format, D3DXCOLOR Color);

private:
	virtual void Free(void);

};

END
#endif // RenderTarget_h__
