// DlgAnmation.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MyMap_Tool.h"
#include "DlgAnmation.h"
#include "afxdialogex.h"
#include "Player.h"
#include "Tool_Boss.h"
#include "Tool_Monster00.h"
#include "Tool_Monster01.h"

// DlgAnmation ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(DlgAnmation, CDialog)

DlgAnmation::DlgAnmation(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DLGANMATION, pParent)
	, m_iAnmationCnt(0)
{

}

DlgAnmation::~DlgAnmation()
{
}

void DlgAnmation::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_XFileKind);
	DDX_Control(pDX, IDC_LIST2, m_AnimatioList);
	DDX_Control(pDX, IDC_EDIT1, m_AnimationCount);
	DDX_Control(pDX, IDC_EDIT2, m_SelectAnimation);
	DDX_Control(pDX, IDC_CHECK1, m_AnimationMode);
}


BEGIN_MESSAGE_MAP(DlgAnmation, CDialog)
	ON_CBN_SELCHANGE(IDC_COMBO1, &DlgAnmation::Select_Xfile)
	ON_BN_CLICKED(IDC_BUTTON2, &DlgAnmation::Player_Anmation)
	ON_BN_CLICKED(IDC_BUTTON1, &DlgAnmation::StopAnimation)
	ON_LBN_SELCHANGE(IDC_LIST2, &DlgAnmation::SelectAnmation)
	ON_BN_CLICKED(IDC_CHECK1, &DlgAnmation::OnBnClickedCheck1)
END_MESSAGE_MAP()


// DlgAnmation �޽��� ó�����Դϴ�.


void DlgAnmation::Select_Xfile()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);

	CString	strSelectXfile;

	GetDlgItemText(IDC_COMBO1, strSelectXfile);

	if (strSelectXfile.IsEmpty())
	{
		return;
	}

	Engine::CAnimationCtrl* pAniCtrl = GetAniCtrl(strSelectXfile);
	
	CString strCnt, strIndex;

	int iAniCnt = pAniCtrl->Get_AnimationCtrl()->GetMaxNumAnimationOutputs();
	int iAbleAnimaion = pAniCtrl->Get_AnimationCtrl()->GetMaxNumAnimationSets();

	strCnt.Format(L"%d", iAniCnt);
	SetDlgItemText(IDC_EDIT1, strCnt);
	
	strCnt.Format(L"%d", iAbleAnimaion);
	SetDlgItemText(IDC_EDIT3, strCnt);

	for (int i = 0; i < iAbleAnimaion; ++i)
	{
		strIndex.Format(L"%d", i);
		m_AnimatioList.InsertString(-1, strIndex);
	}

	UpdateData(FALSE);
}


void DlgAnmation::Player_Anmation()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	
	CString strSelectIndex, strSelectObj, strPlaySpeed;

	GetDlgItemText(IDC_EDIT2, strSelectIndex);
	GetDlgItemText(IDC_COMBO1, strSelectObj);
	GetDlgItemText(IDC_EDIT5, strPlaySpeed);

	if (strSelectIndex.IsEmpty())
	{
		return;
	}

	Engine::CGameObject* pGameObject = Engine::Get_Management()->Get_Object(L"Layer_GameLogic", strSelectObj.operator LPCWSTR());
	
	NULL_CHECK(pGameObject);

	if (strPlaySpeed.IsEmpty())
	{
		if (!strSelectObj.Compare(L"Player"))
		{
			dynamic_cast<CPlayer*>(pGameObject)->SetPlay_Animation(true);
			dynamic_cast<CPlayer*>(pGameObject)->SetUp_Animation(_ttoi(strSelectIndex));
		}

		else if (!strSelectObj.Compare(L"Boss"))
		{
			dynamic_cast<CTool_Boss*>(pGameObject)->SetPlay_Animation(true);
			dynamic_cast<CTool_Boss*>(pGameObject)->SetUp_Animation(_ttoi(strSelectIndex));
		}

		else if (!strSelectObj.Compare(L"Monster00"))
		{
			dynamic_cast<CTool_Monster00*>(pGameObject)->SetPlay_Animation(true);
			dynamic_cast<CTool_Monster00*>(pGameObject)->SetUp_Animation(_ttoi(strSelectIndex));
		}

		else if (!strSelectObj.Compare(L"Monster01"))
		{
			dynamic_cast<CTool_Monster01*>(pGameObject)->SetPlay_Animation(true);
			dynamic_cast<CTool_Monster01*>(pGameObject)->SetUp_Animation(_ttoi(strSelectIndex));
		}
	}
	else if (!strPlaySpeed.IsEmpty())
	{
		//���ڸ� �Է¹��� ����Ʈ��Ʈ�ѿ��� ���ڿ��� ���´�. 
		int iCount = strPlaySpeed.GetLength();

		for (int i = 0; i < iCount; i++) 
		{
			wchar_t szTemp = strPlaySpeed.GetAt(i);

			// ���� ó��. 
			if (i == 0 && szTemp == '-')
			{
				continue;
			}

			
			// �Էµ� Ű�� 0 ~ 9 �����ΰ��� üũ. 
			if (szTemp >= '0' && 
				szTemp <= '9')
			{
				continue;
			}

			else if (szTemp == '.')
			{
				continue;
			}

			else
			{
				return;
			}
		}

		if (!strSelectObj.Compare(L"Player"))
		{
			dynamic_cast<CPlayer*>(pGameObject)->SetUp_PlaySpeed((float)_ttof(strPlaySpeed));
			dynamic_cast<CPlayer*>(pGameObject)->SetPlay_Animation(true);
			dynamic_cast<CPlayer*>(pGameObject)->SetUp_Animation(_ttoi(strSelectIndex));
		}

		else if (!strSelectObj.Compare(L"Boss"))
		{
			dynamic_cast<CTool_Boss*>(pGameObject)->SetPlay_Animation(true);
			dynamic_cast<CTool_Boss*>(pGameObject)->SetUp_Animation(_ttoi(strSelectIndex));
		}

		else if (!strSelectObj.Compare(L"Monster00"))
		{
			dynamic_cast<CTool_Monster00*>(pGameObject)->SetPlay_Animation(true);
			dynamic_cast<CTool_Monster00*>(pGameObject)->SetUp_Animation(_ttoi(strSelectIndex));
		}

		else if (!strSelectObj.Compare(L"Monster01"))
		{
			dynamic_cast<CTool_Monster01*>(pGameObject)->SetPlay_Animation(true);
			dynamic_cast<CTool_Monster01*>(pGameObject)->SetUp_Animation(_ttoi(strSelectIndex));
		}


	}

}


void DlgAnmation::StopAnimation()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	CString strSelectIndex, strSelectObj;

	GetDlgItemText(IDC_EDIT2, strSelectIndex);
	GetDlgItemText(IDC_COMBO1, strSelectObj);

	if (strSelectIndex.IsEmpty())
	{
		return;
	}

	Engine::CGameObject* pGameObject = Engine::Get_Management()->Get_Object(L"Layer_GameLogic", strSelectObj.operator LPCWSTR());

	NULL_CHECK(pGameObject);

	if (!strSelectObj.Compare(L"Player"))
	{
		dynamic_cast<CPlayer*>(pGameObject)->SetPlay_Animation(false);
	}

	if (!strSelectObj.Compare(L"Boss"))
	{
		dynamic_cast<CTool_Boss*>(pGameObject)->SetPlay_Animation(false);
	}
}

Engine::CAnimationCtrl * DlgAnmation::GetAniCtrl(CString strSelectObj)
{
	Engine::CComponent* pComponent = Engine::Get_Component(L"Layer_GameLogic", strSelectObj.operator LPCWSTR(), L"Com_Mesh", Engine::CComponent::ID_STATIC);

	NULL_CHECK_RETURN(pComponent, NULL);

	Engine::CAnimationCtrl* pAniCtrl = dynamic_cast<Engine::CDynamicMesh*>(pComponent)->Get_AnimationCtrl();

	return pAniCtrl;
}

BOOL DlgAnmation::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	m_XFileKind.AddString(L"Player");
	m_XFileKind.AddString(L"Boss");
	m_XFileKind.AddString(L"Monster00");
	m_XFileKind.AddString(L"Monster01");


	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void DlgAnmation::SelectAnmation()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	if (m_AnimatioList.GetCount() < 0)
	{
		return;
	}

	int iSelect = m_AnimatioList.GetCurSel();
	
	CString strSelectIndex, strSelectObject, strTime;

	strSelectIndex.Format(L"%d", iSelect);

	SetDlgItemText(IDC_EDIT2, strSelectIndex);

	GetDlgItemText(IDC_COMBO1, strSelectObject);

	Engine::CAnimationCtrl* pAniCtrl = nullptr;

	pAniCtrl = GetAniCtrl(strSelectObject);


	LPD3DXANIMATIONSET			pAS = nullptr;

	pAniCtrl->Get_AnimationCtrl()->GetAnimationSet(_ttoi(strSelectIndex), &pAS);

	double dTime = pAS->GetPeriod();

	strTime.Format(L"%lf", dTime);

	SetDlgItemText(IDC_EDIT6, strTime);



}


void DlgAnmation::OnBnClickedCheck1()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}
