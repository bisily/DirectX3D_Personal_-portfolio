#ifndef VIBuffer_h__
#define VIBuffer_h__

#include "Resources.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer : public CResources
{
public:
	enum BUFFERID { BUFFER_RCCOL, BUFFER_RCTEX, BUFFER_TERRAINTEX, BUFFER_CUBETEX, BUFFER_SWORDTRAIL };

protected:
	explicit CVIBuffer(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CVIBuffer(const CVIBuffer& rhs);
	virtual ~CVIBuffer();

public: // Setter
	void	SetVtxInfo(void* pVertex);

public:
	_ulong	Get_TriCnt() const { return m_dwTriCnt; }
public:
	HRESULT CreateBuffer();

protected:
	LPDIRECT3DVERTEXBUFFER9			m_pVB = nullptr;
	LPDIRECT3DINDEXBUFFER9			m_pIB = nullptr;
	
	_ulong			m_dwVtxSize = 0;
	_ulong			m_dwVtxCnt = 0;
	_ulong			m_dwVtxFVF = 0;
	_ulong			m_dwTriCnt = 0;

	_ulong			m_dwIdxSize = 0;
	D3DFORMAT		m_IdxFmt;
public:
	virtual void Free();
	virtual CResources*		Clone()PURE;
	virtual void Render_Buffer()PURE;
};

END
#endif // VIBuffer_h__
