#ifndef TerrainTex_h__
#define TerrainTex_h__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CTerrainTex : public CVIBuffer
{
private:
	explicit CTerrainTex(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTerrainTex(const CTerrainTex& rhs);
	virtual ~CTerrainTex(void);

public:
	const	_vec3* Get_VtxPos(void) const { return m_pPosition; }
	const	_ulong Get_VtxCntX(void) const { return m_iH.biWidth; }
	const	_ulong Get_VtxCntZ(void) const { return m_iH.biHeight; }

public:
	void	SetUp_VtxCnt(_ulong& dwCntX, _ulong& dwCntZ, _ulong& dwVtxItv, _ulong Detail = 20.f);

	void	CopyIndexBuffer(Engine::INDEX32* pIndex, const _ulong& dwTriCnt);

public:
	HRESULT	Ready_Buffer(const _ulong& dwCntX,
						const _ulong& dwCntZ,
						const _ulong& dwVtxItv,
						const _ulong& dwVtxDetail);
	virtual void Render_Buffer(void);

private:
	HANDLE				m_hFile = nullptr;
	BITMAPFILEHEADER	m_fH;
	BITMAPINFOHEADER	m_iH;
	_vec3*				m_pPosition;
	_bool				m_bIsClon;

public:
	static CTerrainTex*	Create(LPDIRECT3DDEVICE9 pGraphicDev, 
								const _ulong& dwCntX, 
								const _ulong& dwCntZ, 
								const _ulong& dwVtxItv,
								const _ulong& dwVtxDetail);
	virtual CResources*		Clone(void);
	virtual void	Free(void);

};
END

#endif // TerrainTex_h__