// MyForm.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MyMap_Tool.h"
#include "MyForm.h"
#include "MainFrm.h"

// CMyForm

IMPLEMENT_DYNCREATE(CMyForm, CFormView)

CMyForm::CMyForm()
	: CFormView(IDD_MYFORM)
{

}

CMyForm::~CMyForm()
{
	Engine::Safe_Delete(m_pDlgMap);
	Engine::Safe_Delete(m_pDlgObject);
	Engine::Safe_Delete(m_pDlgEffect);
	Engine::Safe_Delete(m_pDlgCol);
	Engine::Safe_Delete(m_pDlgAnimation);
}

void CMyForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMyForm, CFormView)
	ON_BN_CLICKED(IDC_BUTTON1, &CMyForm::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CMyForm::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CMyForm::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON15, &CMyForm::OnBnClickedButton15)
	ON_BN_CLICKED(IDC_BUTTON39, &CMyForm::AnimationTool)
END_MESSAGE_MAP()


// CMyForm �����Դϴ�.

#ifdef _DEBUG
void CMyForm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CMyForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMyForm �޽��� ó�����Դϴ�.

void CMyForm::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

}

void CMyForm::OnDraw(CDC* /*pDC*/)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

}

// Map
void CMyForm::OnBnClickedButton1()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CRect rect;

	if (m_pDlgMap == nullptr)
	{
		m_pDlgMap = new CDlgMap;
		m_pDlgMap->Create(IDD_DIALOG1);
		m_pDlgMap->GetWindowRect(&rect);
		m_pDlgMap->MoveWindow(2, 2, rect.Width(), rect.Height());
		m_pDlgMap->ShowWindow(SW_SHOW);
	}
	else
	{
		m_pDlgMap->ShowWindow(SW_SHOW);
	}
}

// Object
void CMyForm::OnBnClickedButton2()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	
	CRect rect;
	
	if (m_pDlgObject == nullptr)
	{
		m_pDlgObject = new CDlgObject;
		m_pDlgObject->Create(IDD_DIALOG3);
		m_pDlgObject->GetWindowRect(&rect);
		m_pDlgObject->MoveWindow(2, 2, rect.Width(), rect.Height());
		m_pDlgObject->ShowWindow(SW_SHOW);
	}
	else
	{
		m_pDlgObject->ShowWindow(SW_SHOW);

	}	
}

// Effect
void CMyForm::OnBnClickedButton3()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	CRect rect;

	if (m_pDlgEffect == nullptr)
	{
		m_pDlgEffect = new CDlgEffect;
		m_pDlgEffect->Create(IDD_DIALOG2);
		m_pDlgEffect->GetWindowRect(&rect);
		m_pDlgEffect->MoveWindow(2, 2, rect.Width(), rect.Height());
		m_pDlgEffect->ShowWindow(SW_SHOW);
	}
	else
	{
		m_pDlgEffect->ShowWindow(SW_SHOW);
	}	
}


void CMyForm::OnBnClickedButton15()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	CRect rect;

	if (m_pDlgCol == nullptr)
	{
		m_pDlgCol = new CDlgCollision;
		m_pDlgCol->Create(IDD_DLGCOLLISION);
		m_pDlgCol->GetWindowRect(&rect);
		m_pDlgCol->MoveWindow(2, 2, rect.Width(), rect.Height());
		m_pDlgCol->ShowWindow(SW_SHOW);
	}
	else
	{
		m_pDlgCol->ShowWindow(SW_SHOW);
	}
}


void CMyForm::AnimationTool()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	CRect rect;

	if (m_pDlgAnimation == nullptr)
	{
		m_pDlgAnimation = new DlgAnmation;
		m_pDlgAnimation->Create(IDD_DLGANMATION);
		m_pDlgAnimation->GetWindowRect(&rect);
		m_pDlgAnimation->MoveWindow(2, 2, rect.Width(), rect.Height());
		m_pDlgAnimation->ShowWindow(SW_SHOW);
	}
	else
	{
		m_pDlgAnimation->ShowWindow(SW_SHOW);
	}
}
