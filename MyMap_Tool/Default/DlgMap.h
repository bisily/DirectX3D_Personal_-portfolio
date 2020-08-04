#pragma once

#include "Include.h"
#include "afxwin.h"
#include "afxcmn.h"

// CDlgMap 대화 상자입니다.

namespace Engine
{
	class CManagement;
	class CNaviMesh;
	class CCell;
	class CGameObject;
}

class CDlgMap : public CDialog
{
	DECLARE_DYNAMIC(CDlgMap)

public:
	CDlgMap(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgMap();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	
	wstring		 ConvertRelatviePath(wstring wsrtFullPath);

public:
	// Map
	afx_msg void OnBnClickedButton1();

	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnLbnSelchangeList1();
	afx_msg void Navi_Pop_Back();
	
	// Terrain
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedCheck2();
	afx_msg void TerrinSave();
	afx_msg void TerrainLoad();
	afx_msg void TerrainPos_X(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void TerrainPos_Z(NMHDR *pNMHDR, LRESULT *pResult);

	// Nevi
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnAddNaviState();
	afx_msg void OnNavi_X(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNavi_Y(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNavi_Z(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNaviTreeSelected(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void Navi_Save();
	afx_msg void Navi_Load();
	afx_msg void Find_Index();
	afx_msg void OnReFresh();
	afx_msg void Edit_Compore_Point(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void Get_Pos_1_Value();


	bool	     CheckNaviList();
	void	     SetNaviCount();

	virtual BOOL PreTranslateMessage(MSG* pMsg);

	// Terrain
	CEdit							XCount;
	CEdit							ZCount;
	CEdit							ITV_Value;
	CEdit							m_Pos_1;
	CEdit							m_Pos_2;
	CEdit							m_TerrainX;
	CEdit							m_TerrainZ;

	CButton							m_bSold;
	CButton							m_bWireFrame;
	
	float							m_TerrainPos_X;
	float							m_TerrainPos_Z;


	// Navi
	CEdit							m_Edit_Navi_X;
	CEdit							m_Edit_Navi_Y;
	CEdit							m_Edit_Navi_Z;
	CEdit							m_Detail;
	CEdit							m_Find_X;
	CEdit							m_Find_Y;
	CEdit							m_Find_Z;
	CEdit							m_FindCount;
	CEdit							m_NaviCount;
	CEdit							m_FindCount_t;

	CButton							m_NaviEdit;
	CButton							m_AddNavi;
	
	float							m_Navi_X;
	float							m_Navi_Y;
	float							m_Navi_Z;
	float							m_fHeight;
	int 							m_iSelectChildIndex, m_iSelectParantIndex;
	CTreeCtrl						m_NaviList;

	CListBox						m_NeighborList;
	CListBox						m_FindList;
	
	Engine::CGameObject*			pNaviObject = nullptr;
	Engine::CNaviMesh*				pNaviComponent = nullptr;
	vector<Engine::CCell*>			m_vecNaviList;

	CListBox						m_ListCtrl;
	map<CString, CImage*>			m_MapImage;
	Engine::CManagement*			m_pManagement = nullptr;
	
	afx_msg void Link_Celling();
	afx_msg void Find_Y();
	CEdit m_FindY;
	float m_fFindY;
	afx_msg void FindY_Value(NMHDR *pNMHDR, LRESULT *pResult);
	CListBox m_FindYList;
	afx_msg void Apply_Y_Value();
	CEdit m_SetUpY;
	CEdit m_Delete_Index;
	afx_msg void Delete_Index();
};
