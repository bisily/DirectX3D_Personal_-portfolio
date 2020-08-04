#pragma once
#include "afxcmn.h"

#include "DlgMap.h"
#include "DlgEffect.h"
#include "DlgObject.h"
#include "DlgCollision.h"
#include "DlgAnmation.h"

// CMyForm �� ���Դϴ�.

class CMyForm : public CFormView
{
	DECLARE_DYNCREATE(CMyForm)

protected:
	CMyForm();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CMyForm();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MYFORM };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:	
	CDlgMap*		m_pDlgMap		= nullptr;
	CDlgObject*		m_pDlgObject	= nullptr;
	CDlgEffect*		m_pDlgEffect	= nullptr;
	CDlgCollision*	m_pDlgCol		= nullptr;
	DlgAnmation*	m_pDlgAnimation = nullptr;

	virtual void OnInitialUpdate();
	virtual void OnDraw(CDC* /*pDC*/);
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton15();
	afx_msg void AnimationTool();
};


