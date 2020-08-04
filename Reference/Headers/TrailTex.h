#ifndef TrailTex_h__
#define TrailTex_h__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CTrailTex : public CVIBuffer
{
private:
	explicit			CTrailTex(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit			CTrailTex(const CTrailTex& rhs);
	virtual				~CTrailTex();

public:
	HRESULT				Ready_Buffer();
	virtual void		Render_Buffer();

	void				SetVertexTrail(const _vec3* pPos
									, const _vec3* pDir
									, const _float& fLength
									, const DWORD& dwCnt
									, _matrix* pmatWorld);

public:
	static	CTrailTex*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CResources*	Clone();
	virtual void		Free();

private:
	VTXTRAIL*			m_pVertex;
	_matrix*			m_pMatWorld = nullptr;
};

END


#endif // TrailTex_h__
