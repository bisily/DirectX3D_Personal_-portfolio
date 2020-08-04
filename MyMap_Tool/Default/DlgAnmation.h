#pragma once
#include "afxwin.h"


// DlgAnmation ��ȭ �����Դϴ�.

#include "Export_Function.h"
#include "afxcmn.h"

class DlgAnmation : public CDialog
{
	DECLARE_DYNAMIC(DlgAnmation)

public:
	DlgAnmation(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~DlgAnmation();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLGANMATION };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

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
