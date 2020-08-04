#ifndef GraphicDev_h__
#define GraphicDev_h__

#include "Engine_Defines.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CGraphicDev : public CBase
{
	DECLARE_SINGLETON(CGraphicDev)

public:
	enum	WINMODE { MODE_FULL, MODE_WIN };

private:
	explicit				CGraphicDev();
	virtual					~CGraphicDev();

public:
	LPDIRECT3DDEVICE9		GetDevice() { return m_pGraphicDev; }

public:
	HRESULT					Ready_GraphicDev(HWND hWnd,
											WINMODE eMode,
											const _uint& iSizeX,
											const _uint& iSizeY,
											Engine::CGraphicDev** ppGraphicDev);

	void					Render_Begin(D3DXCOLOR Color);
	void					Render_End(void);

private:
	LPDIRECT3D9				m_pSDK			= nullptr;
	LPDIRECT3DDEVICE9		m_pGraphicDev	= nullptr;

public:
	inline	virtual void	Free();
};

END
#endif // GraphicDev_h__
