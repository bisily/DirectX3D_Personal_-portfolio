#ifndef Cell_h__
#define Cell_h__

#include "Engine_Defines.h"
#include "Base.h"

BEGIN(Engine)

class CLine;
class ENGINE_DLL CCell : public CBase 
{
public:
	enum POINT	  { POINT_A, POINT_B, POINT_C, POINT_END };
	enum NEIGHBOR {	NEIGHBOR_AB, NEIGHBOR_BC, NEIGHBOR_CA,NEIGHBOR_END	};
	enum LINE { LINE_AB, LINE_BC, LINE_CA, LINE_END };
	enum COMPARE { COMPARE_MOVE, COMPARE_STAY };

private:
	explicit CCell(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCell(void);

public: // Getter
	const	_vec3*	Get_Point(POINT eType)	 { return &m_vPoint[eType]; }
	CCell*	Get_Neighbor(NEIGHBOR eType)	 { return m_pNeighbor[eType]; }
	const _ulong*	Get_Index(void)			 { return &m_dwIndex; }
	
public: // Setter
	void	Set_Neighbor(NEIGHBOR eType, CCell* pNeighbor) { m_pNeighbor[eType] = pNeighbor; }
	void	Set_Point_X(POINT eType, _float fPosX) { m_vPoint[eType].x = fPosX; }
	void	Set_Point_Y(POINT eType, _float fPosY) { m_vPoint[eType].y = fPosY; }
	void	Set_Point_Z(POINT eType, _float fPosZ) { m_vPoint[eType].z = fPosZ; }
	void	ReSet_Index(int iIndex);

public: // For_Tool
	_vec3*	Get_Pos(POINT eType) { return &m_vPoint[eType]; }

public:
	HRESULT	Ready_Cell(LPDIRECT3DDEVICE9 pGraphicDev,
						const _ulong& dwIndex,
						const _vec3* pPointA, 
						const _vec3* pPointB, 
						const _vec3* pPointC);

	_bool	Compare_Point(const _vec3* pPointA, const _vec3* pPointB, CCell* pCell);
	COMPARE	Compare(const _vec3* pEndPos, _ulong* pCellIndex);
	void	Render_Cell(void);

	void	SetUp_PosText(_bool bRender) { bRenderPosText = bRender; }
	void	SetUp_Index(_bool bRender) { bRenderIndex = bRender; }

private:
	_vec3				m_vPoint[POINT_END]{};
	CCell*				m_pNeighbor[NEIGHBOR_END]{};
	CLine*				m_pLine[LINE_END]{};

	LPD3DXLINE			m_pD3DXLine		= nullptr;
	LPDIRECT3DDEVICE9	m_pGraphicDev	= nullptr;
	_ulong				m_dwIndex		= 0;

	LPDIRECT3DTEXTURE9	m_pTexture;
	LPD3DXMESH			m_Text			= nullptr;
	LPD3DXMESH			m_PosA			= nullptr;
	LPD3DXMESH			m_PosB			= nullptr;
	LPD3DXMESH			m_PosC			= nullptr;
	_tchar				szIndex[12];
	_bool				bRenderPosText = FALSE;
	_bool				bRenderIndex = TRUE;

public:
	static CCell* Create(LPDIRECT3DDEVICE9	pGraphicDev, const _ulong& dwIndex, const _vec3* pPointA, const _vec3* pPointB, const _vec3* pPointC);
	virtual void Free(void);
};

END
#endif // Cell_h__
