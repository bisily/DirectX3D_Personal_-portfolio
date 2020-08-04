#ifndef RcTex_h__
#define RcTex_h__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CRcTex : public CVIBuffer
{
private:
	explicit CRcTex(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CRcTex(const CRcTex& rhs);
	virtual ~CRcTex(void);

public:
	HRESULT	Ready_Buffer(void);
	HRESULT Ready_Buffer(_int iFlag);
	virtual void Render_Buffer(void);
	
	void	SetUp_VtxPos(_vec3 vecPos);

public:
	static CRcTex*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	static CRcTex*	Create(LPDIRECT3DDEVICE9 pGraphicDev, _int iFlag);

	virtual CResources*		Clone(void);
	virtual void	Free(void);
};

END


#endif // RcTex_h__
