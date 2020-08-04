#ifndef NaviMesh_h__
#define NaviMesh_h__

#include "Mesh.h"
#include "Cell.h"

BEGIN(Engine)

class ENGINE_DLL CNaviMesh : public CMesh
{
private:
	explicit CNaviMesh(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CNaviMesh(const CNaviMesh& rhs);
	virtual ~CNaviMesh(void);

public: // setter
	void		Set_CurrentCellIndex(const _ulong& dwIndex) { m_dwIndex = dwIndex; }

public:
	HRESULT		Add_NeviMesh(const _vec3* PositionA, const _vec3* PositionB, const _vec3* PositionC);

	HRESULT		Ready_NaviMesh(int iFlage);
	void		Render_NaviCell(void); 
	_vec3		Move_OnNaviMesh(const _vec3* pTargetPos, const _vec3* pTargetDir);
	void		Get_Height(_vec3* vecEnPos, _ulong dwIndex);

public:
	const vector<CCell*>*		Get_CellList() { return &m_vecCell; }

public: // For_Tool
	void		Clear_List() { 
		for_each(m_vecCell.begin(), m_vecCell.end(), CDeleteObj());
		m_vecCell.clear(); 
	}

	void		ReSet_Index();
	void		Link_CellList() { Link_Cell(); }
	void		SetUp_PosText(_bool bRender);
	void		SetUp_Index(_bool bRender);

public:
	void						Set_CellList(vector<CCell*>* pvecCell) { m_vecCell = *pvecCell; }

private:
	HRESULT		Link_Cell(void);	

private:
	typedef	vector<CCell*>		VECCELL;
	VECCELL						m_vecCell;

	_ulong						m_dwIndex = 0;

public:
	static	CNaviMesh*	Create(LPDIRECT3DDEVICE9 pGraphicDev, int iFlage);
	virtual CResources*	Clone(void);
	virtual void		Free(void);
};

END
#endif // NaviMesh_h__
