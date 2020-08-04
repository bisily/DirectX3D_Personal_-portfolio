// DlgEffect.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MyMap_Tool.h"
#include "DlgEffect.h"
#include "afxdialogex.h"


// CDlgEffect 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgEffect, CDialog)

CDlgEffect::CDlgEffect(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DIALOG2, pParent)
{

}

CDlgEffect::~CDlgEffect()
{
}

void CDlgEffect::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgEffect, CDialog)
END_MESSAGE_MAP()


// CDlgEffect 메시지 처리기입니다.
