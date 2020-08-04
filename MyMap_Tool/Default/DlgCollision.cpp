// DlgCollision.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MyMap_Tool.h"
#include "DlgCollision.h"
#include "afxdialogex.h"

#include "Export_Function.h"

#include "MyMap_ToolDoc.h"
#include "MyMap_ToolView.h"
#include "Tool_Stage.h"
#include "CollisionSphere.h"
#include "Square.h"

// CDlgCollision 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgCollision, CDialog)

CDlgCollision::CDlgCollision(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DLGCOLLISION, pParent)
	, m_fRadus(0.f)
{

}

CDlgCollision::~CDlgCollision()
{
}

	void CDlgCollision::DoDataExchange(CDataExchange* pDX)
{
		CDialog::DoDataExchange(pDX);
		DDX_Control(pDX, IDC_LIST2, m_BoreNames);
		DDX_Control(pDX, IDC_COMBO1, m_ObjectBox);
		DDX_Control(pDX, IDC_EDIT10, m_TotalCount);
		DDX_Control(pDX, IDC_EDIT5, m_Select_Number);
		DDX_Control(pDX, IDC_EDIT4, m_SelectBore);
		DDX_Control(pDX, IDC_EDIT1, m_Radius);
		DDX_Control(pDX, IDC_LIST1, m_AddList);
		DDX_Control(pDX, IDC_CHECK1, m_EditCol);
		DDX_Control(pDX, IDC_EDIT2, m_Select_Add_Bore);
		DDX_Control(pDX, IDC_EDIT13, m_MaxSize);
		DDX_Control(pDX, IDC_EDIT54, m_MinSize);
	}


BEGIN_MESSAGE_MAP(CDlgCollision, CDialog)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CDlgCollision::SelectObject)
	ON_LBN_SELCHANGE(IDC_LIST2, &CDlgCollision::Select_Bore)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, &CDlgCollision::EditRadius)
	ON_BN_CLICKED(IDC_BUTTON1, &CDlgCollision::Select_Cancle)
	ON_BN_CLICKED(IDC_BUTTON4, &CDlgCollision::CreateSphere)
	ON_BN_CLICKED(IDC_BUTTON2, &CDlgCollision::Edit)
	ON_LBN_SELCHANGE(IDC_LIST1, &CDlgCollision::Select_List)
	ON_BN_CLICKED(IDC_CHECK1, &CDlgCollision::EditSelect)
	ON_BN_CLICKED(IDC_BUTTON3, &CDlgCollision::Create_Square)
	ON_BN_CLICKED(IDC_BUTTON11, &CDlgCollision::Square_Edit)
END_MESSAGE_MAP()


// CDlgCollision 메시지 처리기입니다.


BOOL CDlgCollision::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	m_ObjectBox.AddString(L"Player");
	m_ObjectBox.AddString(L"Boss");
	m_ObjectBox.AddString(L"Monster00");
	m_ObjectBox.AddString(L"Monster01");
	m_ObjectBox.AddString(L"Player_WeaPon");
	m_ObjectBox.AddString(L"Boss_WeaPon");
	m_ObjectBox.AddString(L"Monster00Sword");
	
	m_ObjectBox.SetWindowTextW(L"Player");

	m_OldSelectName = L"Player";

	SetDlgItemText(IDC_EDIT10, L"0");

	m_Radius.EnableWindow(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CDlgCollision::SelectObject()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);

	CString strTemp;
	int iSelect = m_ObjectBox.GetCurSel();

	m_ObjectBox.GetLBText(iSelect, strTemp);

	if (m_OldSelectName != strTemp || m_BoreNames.GetCount() == 0)
	{
		m_BoreNames.ResetContent();
		SetDlgItemText(IDC_EDIT10, L"0");

		m_OldSelectName = strTemp;
		
		Engine::CDynamicMesh* pDynamicMesh = dynamic_cast<Engine::CDynamicMesh*>(Engine::Get_Component(L"Layer_GameLogic", m_OldSelectName, L"Com_Mesh", Engine::CComponent::ID_STATIC));
		
		if (pDynamicMesh != nullptr)
		{
			for (auto& iter : pDynamicMesh->Get_BoreNames())
			{
				m_BoreNames.AddString(iter.c_str());
			}

			strTemp.Format(L"%d", m_BoreNames.GetCount());
			SetDlgItemText(IDC_EDIT10, strTemp);
		}
	}

	UpdateData(FALSE);
}

void CDlgCollision::Select_Bore()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	int iSelect = m_BoreNames.GetCurSel();

	if (-1 < iSelect)
	{
		UpdateData(TRUE);

		CString strSelectBore;
		CString strParent;
		CString strSelectObject;
		CString strTemp;
		CStringA strConver;
	
		m_BoreNames.GetText(iSelect, strSelectObject);
		GetDlgItemText(IDC_COMBO1, strSelectObject);

		SetDlgItemText(IDC_EDIT4, strSelectObject);

		strTemp.Format(L"%d", iSelect + 1);

		SetDlgItemText(IDC_EDIT5, strTemp);
		
		m_BoreNames.GetText(iSelect, strSelectBore);
		GetDlgItemText(IDC_COMBO1, strParent);

		strConver = strSelectBore;

		const char* szBoreName = strConver;

		D3DXVECTOR3 vecPosion;
		vecPosion = Get_BorePosion(szBoreName, strParent);
		
		Engine::Get_Management()->Get_CurrentScene()->Get_Layer(L"Layer_GameLogic")->DeleteObj(0, L"Mesh_PreView");

		Engine::CGameObject* pGameObject = CCollisionSphere::Create(Engine::GetDevice(), &vecPosion, strParent.operator LPCWSTR(), strSelectBore.operator LPCWSTR(), 0);
		NULL_CHECK(pGameObject);

		Engine::Get_Management()->Get_CurrentScene()->Get_Layer(L"Layer_GameLogic")->Add_GameObject(L"Mesh_PreView", pGameObject);
		
		UpdateData(FALSE);
	}
}


void CDlgCollision::EditRadius(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
		
	if (bIsSelectBore != true)
	{
		return;
	}

	if (pNMUpDown->iDelta < 0)
	{
		m_fRadus = m_fRadus + 0.1f;
	}
	else
	{
		m_fRadus = m_fRadus - 0.1f;
	}

	CString strTemp;

	strTemp.Format(L"%.4f", m_fRadus);

	SetDlgItemText(IDC_EDIT1, strTemp);

	*pResult = 0;
}

void CDlgCollision::Select_Cancle()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);

	Engine::Get_Management()->Get_CurrentScene()->Get_Layer(L"Layer_GameLogic")->DeleteObj(0, L"Mesh_PreView");

	bIsSelectBore = false;

	m_SelectBore.SetWindowTextW(L"");
	m_Select_Number.SetWindowTextW(L"");
	
	UpdateData(FALSE);
}


void CDlgCollision::CreateSphere()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CString strSelectBore;
	CString strParent;
	CString strSaveName;
	CStringA strConver;
	int iSelect = m_BoreNames.GetCurSel();
	int iIndex = m_AddList.GetCount();

	m_BoreNames.GetText(iSelect, strSelectBore);
	GetDlgItemText(IDC_COMBO1, strParent);

	strConver = strSelectBore;

	const char* szBoreName = strConver;

	D3DXVECTOR3 vecPosion;
	vecPosion = Get_BorePosion(szBoreName, strParent);

	Engine::CGameObject* pGameObject = nullptr;
	
	pGameObject = CCollisionSphere::Create(Engine::GetDevice(), &vecPosion, strParent.operator LPCWSTR(), strSelectBore.operator LPCWSTR(), iIndex);
	NULL_CHECK(pGameObject);

	strSaveName = szBoreName;

	Engine::Get_Management()->Get_CurrentScene()->Get_Layer(L"Layer_GameLogic")->Add_GameObject(strSaveName.operator LPCWSTR(), pGameObject);

	m_AddList.AddString(strSaveName);
}


void CDlgCollision::Edit()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	int iSelect = m_AddList.GetCurSel();

	CString strSelectName;
	CString strRadius, strPos;

	m_AddList.GetText(iSelect, strSelectName);

	Engine::CGameObject* pGameObject = Engine::Get_Management()->Get_Object(L"Layer_GameLogic", strSelectName.operator LPCWSTR(), iSelect);
	NULL_CHECK(pGameObject);

	GetDlgItemText(IDC_EDIT1, strRadius);
	m_fRadus = (float)_wtof(strRadius);
	dynamic_cast<CCollisionSphere*>(pGameObject)->SetRadius(m_fRadus);
}


void CDlgCollision::Select_List()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	int iSelect = m_AddList.GetCurSel();

	if (iSelect < -1)
	{
		return;
	}

	CString strSelect;
	wstring strBoreName;

	m_AddList.GetText(iSelect, strSelect);

	Engine::CGameObject* pGameObject = Engine::Get_Management()->Get_Object(L"Layer_GameLogic", strSelect.operator LPCWSTR(), iSelect);
	NULL_CHECK(pGameObject);

	const type_info& class_type = typeid(*pGameObject);

	if ((!strcmp("class CCollisionSphere", class_type.name())))
	{
		strBoreName = dynamic_cast<CCollisionSphere*>(pGameObject)->Get_BoreName();
		SetDlgItemText(IDC_EDIT2, strBoreName.c_str());

		m_fRadus = dynamic_cast<CCollisionSphere*>(pGameObject)->GetRadius();
	}
	else if ((!strcmp("class CSquare", class_type.name())))
	{
		strBoreName = dynamic_cast<CSquare*>(pGameObject)->Get_BoreName();
	}

	strSelect.Format(L"%.4f", m_fRadus);
	SetDlgItemText(IDC_EDIT1, strSelect);
	
}

D3DXVECTOR3 CDlgCollision::Get_BorePosion(const char* szBoreName, CString strParent)
{
	Engine::CDynamicMesh* pDynamicMesh = dynamic_cast<Engine::CDynamicMesh*>(Engine::Get_Component(L"Layer_GameLogic", m_OldSelectName, L"Com_Mesh", Engine::CComponent::ID_STATIC));

	const Engine::D3DXFRAME_DERIVED*		pFrame = pDynamicMesh->Get_FrameByName(szBoreName);

	D3DXMATRIX matBore = pFrame->CombinedTransformationMatrix;

	D3DXMATRIX m_matParent = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(L"Layer_GameLogic", strParent, L"Com_Transform", Engine::CComponent::ID_DYNAMIC))->m_matWorld;

	D3DXMATRIX m_matWorld;
	D3DXMatrixIdentity(&m_matWorld);

	m_matWorld = matBore * m_matParent;

	D3DXVECTOR3 vecOutput = { 0.f, 0.f, 0.f };

	D3DXVec3TransformCoord(&vecOutput, &vecOutput, &m_matWorld);

	return vecOutput;
}


void CDlgCollision::EditSelect()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	int iSelect = m_AddList.GetCurSel();

	if (iSelect == -1)
	{
		m_EditCol.SetCheck(FALSE);
		return;
	}

	UpdateData(TRUE);

	if (0 == m_EditCol.GetCheck())
	{
		bIsSelectBore = FALSE;

		m_Radius.EnableWindow(FALSE);
	}
	else if (1 == m_EditCol.GetCheck())
	{
		bIsSelectBore = TRUE;

		m_Radius.EnableWindow(TRUE);
	}

	UpdateData(FALSE);

}


void CDlgCollision::Create_Square()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CString strSelectBore;
	CString strParent;
	CString strSaveName;
	CStringA strConver;
	int iSelect = m_BoreNames.GetCurSel();
	int iIndex = m_AddList.GetCount();

	m_BoreNames.GetText(iSelect, strSelectBore);
	GetDlgItemText(IDC_COMBO1, strParent);

	strConver = strSelectBore;

	const char* szBoreName = strConver;

	D3DXVECTOR3 vecPosion;
	vecPosion = Get_BorePosion(szBoreName, strParent);

	Engine::CGameObject* pGameObject = nullptr;

	_vec3 fMin{ -10.f, -10.f, -10.f }, fMax{ 10.f, 10.f, 10.f };

	pGameObject = CSquare::Create(Engine::GetDevice(), &vecPosion, strParent.operator LPCWSTR(), strSelectBore.operator LPCWSTR(), fMin, fMax, iIndex);
	NULL_CHECK(pGameObject);

	strSaveName = szBoreName;

	Engine::Get_Management()->Get_CurrentScene()->Get_Layer(L"Layer_GameLogic")->Add_GameObject(strSaveName.operator LPCWSTR(), pGameObject);

	m_AddList.AddString(strSaveName);
}


void CDlgCollision::Square_Edit()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	int iSelect = m_AddList.GetCurSel();

	CString strSelectName;
	CString strMinX, strMinY, strMinZ, strMaxX, strMaxY, strMaxZ;

	m_AddList.GetText(iSelect, strSelectName);

	if (strSelectName.IsEmpty())
	{
		return;
	}

	Engine::CGameObject* pGameObject = Engine::Get_Management()->Get_Object(L"Layer_GameLogic", strSelectName.operator LPCWSTR(), iSelect);
	NULL_CHECK(pGameObject);

	GetDlgItemText(IDC_EDIT13, strMaxX);
	GetDlgItemText(IDC_EDIT21, strMaxY);
	GetDlgItemText(IDC_EDIT56, strMaxZ);
	GetDlgItemText(IDC_EDIT54, strMinX);
	GetDlgItemText(IDC_EDIT55, strMinY);
	GetDlgItemText(IDC_EDIT57, strMinZ);

	_vec3 vecMin((float)_ttof(strMinX), (float)_ttof(strMinY), (float)_ttof(strMinZ));
	_vec3 vecMax((float)_ttof(strMaxX), (float)_ttof(strMaxY), (float)_ttof(strMaxZ));

	dynamic_cast<CSquare*>(pGameObject)->SetSquare(vecMin, vecMax);
}
