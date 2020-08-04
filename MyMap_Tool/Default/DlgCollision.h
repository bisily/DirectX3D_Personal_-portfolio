#pragma once
#include "afxwin.h"


// CDlgCollision 대화 상자입니다.

class CDlgCollision : public CDialog
{
	DECLARE_DYNAMIC(CDlgCollision)

	enum COLLTYPE { COLL_FALSE, COLL_TRUE, COLL_END };

public:
	CDlgCollision(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgCollision();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLGCOLLISION };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL	OnInitDialog();
	afx_msg void	SelectObject();
	afx_msg void	Select_Bore();
	afx_msg void	EditRadius(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void	Select_Cancle();
	afx_msg void	CreateSphere();
	afx_msg void	Edit();
	afx_msg void	Select_List();
	afx_msg void	EditSelect();

	D3DXVECTOR3		Get_BorePosion(const char* szBoreName, CString strParent);

	CListBox		m_BoreNames;
	CComboBox		m_ObjectBox;
	CEdit			m_TotalCount;

	CString			m_OldSelectName = L"Player";
	
	CEdit			m_Select_Number;
	CEdit			m_SelectBore;
	CEdit			m_Radius;
	CEdit			m_Select_Add_Bore;
	CButton			m_EditCol;
	CListBox		m_AddList;

	float			m_fRadus;

	bool bIsSelectBore = false;
	afx_msg void Create_Square();
	CEdit m_MaxSize;
	CEdit m_MinSize;
	afx_msg void Square_Edit();
};
