#pragma once
#include "afxwin.h"


// DlgAnmation 대화 상자입니다.

#include "Export_Function.h"
#include "afxcmn.h"

class DlgAnmation : public CDialog
{
	DECLARE_DYNAMIC(DlgAnmation)

public:
	DlgAnmation(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~DlgAnmation();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLGANMATION };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	
	Engine::CAnimationCtrl*	GetAniCtrl(CString strSelectObj);

	virtual BOOL OnInitDialog();
	
	afx_msg void Select_Xfile();
	afx_msg void Player_Anmation();
	afx_msg void StopAnimation();
	afx_msg void SelectAnmation();
	
	
	CComboBox m_XFileKind;
	CListBox m_AnimatioList;
	
	CEdit m_AnimationCount;
	CEdit m_SelectAnimation;
	
	int m_iAnmationCnt;
	CButton m_AnimationMode;
	afx_msg void OnBnClickedCheck1();
};
