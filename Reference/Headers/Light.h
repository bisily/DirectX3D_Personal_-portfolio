#ifndef Light_h__
#define Light_h__

#include "Engine_Defines.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CLight : public CBase
{
private:
	explicit CLight(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLight();

public:
	const D3DLIGHT9*			Get_LightInfo() { return &m_tLightInfo; }

public:
	HRESULT						Ready_Light(const D3DLIGHT9* pLightInfo, const _uint& iIndex);
	void						Render(LPD3DXEFFECT pEffect);

private:
	LPDIRECT3DDEVICE9				m_pGraphicDev = nullptr;
	D3DLIGHT9						m_tLightInfo;
	D3DMATERIAL9					m_tMtrlInfo;
	_uint							m_iIndex = 0;

	LPDIRECT3DVERTEXBUFFER9			m_pVB = nullptr;
	LPDIRECT3DINDEXBUFFER9			m_pIB = nullptr;

public:
	static		CLight*	Create(LPDIRECT3DDEVICE9 pGraphicDev, 
							const D3DLIGHT9* pLightInfo, 
							const _uint& iIndex);
private:
	virtual		void	Free();	
};
END
#endif // Light_h__
