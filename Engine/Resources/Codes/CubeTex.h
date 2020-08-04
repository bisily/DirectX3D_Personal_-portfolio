#ifndef CubeTex_h__
#define CubeTex_h__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CCubeTex : public CVIBuffer
{
private:
	explicit CCubeTex(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CCubeTex(const CCubeTex& rhs);
	virtual ~CCubeTex();

public:
	HRESULT	Ready_Buffer();
	virtual void Render_Buffer();

public:
	static CCubeTex*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CResources*		Clone();
	virtual void	Free();
};

END


#endif // CubeTex_h__
