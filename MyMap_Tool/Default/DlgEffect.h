#pragma once


// CDlgEffect ��ȭ �����Դϴ�.

class CDlgEffect : public CDialog
{
	DECLARE_DYNAMIC(CDlgEffect)

public:
	CDlgEffect(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlgEffect();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};
