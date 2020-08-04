// DlgObject.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MyMap_Tool.h"
#include "DlgObject.h"
#include "afxdialogex.h"

#include "Export_Function.h"

#include "MyMap_ToolDoc.h"
#include "MyMap_ToolView.h"
#include "Tool_Stage.h"

// CDlgObject 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgObject, CDialog)

CDlgObject::CDlgObject(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DIALOG3, pParent)
{

	m_StaticObj_Cnt = 0;
	m_DynamicObj_Cnt = 0;
}

CDlgObject::~CDlgObject()
{
}

void CDlgObject::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_PositionX);
	DDX_Control(pDX, IDC_EDIT6, m_PositionY);
	DDX_Control(pDX, IDC_EDIT5, m_PositionZ);
	DDX_Control(pDX, IDC_EDIT7, m_RotationX);
	DDX_Control(pDX, IDC_EDIT9, m_RotationY);
	DDX_Control(pDX, IDC_EDIT8, m_RotationZ);
	DDX_Control(pDX, IDC_EDIT10, m_ScaleX);
	DDX_Control(pDX, IDC_EDIT12, m_ScaleY);
	DDX_Control(pDX, IDC_EDIT11, m_ScaleZ);
	DDX_Control(pDX, IDC_TREE1, m_TreeCtrl);
	DDX_Control(pDX, IDC_EDIT2, m_SelectObject);
	DDX_Control(pDX, IDC_LIST1, m_AddStaticList);
	DDX_Control(pDX, IDC_EDIT18, m_CreatePosX);
	DDX_Control(pDX, IDC_EDIT17, m_CreatePosY);
	DDX_Control(pDX, IDC_EDIT15, m_CreatePosZ);
	DDX_Control(pDX, IDC_EDIT20, m_CreateScaleX);
	DDX_Control(pDX, IDC_EDIT19, m_CreateScaleY);
	DDX_Control(pDX, IDC_EDIT16, m_CreateScaleZ);
	DDX_Control(pDX, IDC_EDIT13, m_PickObject);
	DDX_Control(pDX, IDC_LIST2, m_AddDynamicList);
	DDX_Control(pDX, IDC_EDIT14, m_PickDynamicObject);
	DDX_Control(pDX, IDC_RADIO5, m_Transform_Selected[0]);
	DDX_Control(pDX, IDC_RADIO6, m_Transform_Selected[1]);
	DDX_Control(pDX, IDC_CHECK2, m_SetViewState);
	DDX_Control(pDX, IDC_DYNAMIC_CNT, m_DynamicObjectCnt);
}


BEGIN_MESSAGE_MAP(CDlgObject, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CDlgObject::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CDlgObject::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CDlgObject::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CDlgObject::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CDlgObject::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CDlgObject::OnBnClickedButton6)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, &CDlgObject::OnTvnSelchangedTree1)
	ON_LBN_SELCHANGE(IDC_LIST1, &CDlgObject::OnLbnSelchangeList1)
	ON_LBN_SELCHANGE(IDC_LIST2, &CDlgObject::OnLbnSelchangeList2)
	ON_BN_CLICKED(IDC_BUTTON7, &CDlgObject::StaticMesh_Save)
	ON_BN_CLICKED(IDC_BUTTON8, &CDlgObject::StaticMesh_Load)
	ON_BN_CLICKED(IDC_BUTTON9, &CDlgObject::DynamicMesh_Save)
	ON_BN_CLICKED(IDC_BUTTON10, &CDlgObject::DynamicMesh_Load)
	ON_BN_CLICKED(IDC_BUTTON13, &CDlgObject::Delete_StaticObj)
	ON_BN_CLICKED(IDC_BUTTON14, &CDlgObject::Delete_DynamicObj)
	ON_BN_CLICKED(IDC_CHECK2, &CDlgObject::Set_ViewState)
END_MESSAGE_MAP()


// CDlgObject 메시지 처리기입니다.

BOOL CDlgObject::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	m_pManagement = Engine::Get_Management();

	SetDlgItemText(IDC_EDIT1, L"0");
	SetDlgItemText(IDC_EDIT6, L"0");
	SetDlgItemText(IDC_EDIT5, L"0");

	SetDlgItemText(IDC_EDIT7, L"0");
	SetDlgItemText(IDC_EDIT9, L"0");
	SetDlgItemText(IDC_EDIT8, L"0");

	SetDlgItemText(IDC_EDIT10, L"0.01");
	SetDlgItemText(IDC_EDIT12, L"0.01");
	SetDlgItemText(IDC_EDIT11, L"0.01");


	HTREEITEM StaticTop = m_TreeCtrl.InsertItem(L"Static_Mesh", TVI_ROOT, TVI_LAST);
	HTREEITEM DynamicTop = m_TreeCtrl.InsertItem(L"Dynamic_Mesh", TVI_ROOT, TVI_LAST);

	// BagSack
		// BagSack.X
	HTREEITEM Static_BagSack = m_TreeCtrl.InsertItem(L"BagSack", StaticTop, TVI_LAST);
	m_TreeCtrl.InsertItem(L"BagSack.X", Static_BagSack, TVI_LAST);
	
	// Box
		// Box
	HTREEITEM Static_Box = m_TreeCtrl.InsertItem(L"Box", StaticTop, TVI_LAST);
	m_TreeCtrl.InsertItem(L"Box.X", Static_Box, TVI_LAST);

	// Building
	HTREEITEM Static_Building = m_TreeCtrl.InsertItem(L"Building", StaticTop, TVI_LAST);
	
		// Building_Parts00
			// Parts00.X
	m_TreeCtrl.InsertItem(L"Parts00.X", Static_Building, TVI_LAST);

		// Building_Parts01
			// Parts01.X
	m_TreeCtrl.InsertItem(L"Parts01.X", Static_Building, TVI_LAST);

		// Building_Pass00
			// Pass00.X
	m_TreeCtrl.InsertItem(L"Pass00.X", Static_Building, TVI_LAST);

		// Building00
			// Building00.X
	m_TreeCtrl.InsertItem(L"Building00.X", Static_Building, TVI_LAST);

		// Building01
			// Building01.X
	m_TreeCtrl.InsertItem(L"Building01.X", Static_Building, TVI_LAST);

		// Building02
			// Building02.X
	m_TreeCtrl.InsertItem(L"Building02.X", Static_Building, TVI_LAST);

		// Building03
			// Building03.X
	m_TreeCtrl.InsertItem(L"Building03.X", Static_Building, TVI_LAST);

		// Building04
			// Building04.X
	m_TreeCtrl.InsertItem(L"Building04.X", Static_Building, TVI_LAST);

		// Tunnel
			// Tunnel.X
	m_TreeCtrl.InsertItem(L"Tunnel.X", Static_Building, TVI_LAST);

		// Pile_of_debris
			// Pile_of_debris.X
	m_TreeCtrl.InsertItem(L"Pile_of_debris.X", Static_Building, TVI_LAST);


	// Castle_Door
		// Castle_Door.X
	HTREEITEM Static_Castle = m_TreeCtrl.InsertItem(L"Castle_Door", StaticTop, TVI_LAST);
	m_TreeCtrl.InsertItem(L"Castle_Door.X", Static_Castle, TVI_LAST);

	// Castle
		// Castle.X
	m_TreeCtrl.InsertItem(L"Castle.X", Static_Castle, TVI_LAST);

	// Cloth
		// Cloth.X
	HTREEITEM Static_Cloth = m_TreeCtrl.InsertItem(L"Cloth", StaticTop, TVI_LAST);
	m_TreeCtrl.InsertItem(L"Cloth.X", Static_Cloth, TVI_LAST);

	// Door
		// Door.X
	HTREEITEM Static_Door = m_TreeCtrl.InsertItem(L"Door", StaticTop, TVI_LAST);
	m_TreeCtrl.InsertItem(L"Door.X", Static_Door, TVI_LAST);

	// Door_Guide
		// Door_Guide.X
	HTREEITEM Door_Guide = m_TreeCtrl.InsertItem(L"Door_Guide", StaticTop, TVI_LAST);
	m_TreeCtrl.InsertItem(L"Door_Guide.X", Door_Guide, TVI_LAST);

	// Drums
		// Drum00.x
	HTREEITEM Static_Drums = m_TreeCtrl.InsertItem(L"Drum", StaticTop, TVI_LAST);
	m_TreeCtrl.InsertItem(L"Drum00.X", Static_Drums, TVI_LAST);

		// Drums
			// Drums.X
	m_TreeCtrl.InsertItem(L"Drums.X", Static_Drums, TVI_LAST);

	// FloorWood
		// FloorWood.X
	HTREEITEM Static_FloorWood = m_TreeCtrl.InsertItem(L"FloorWood", StaticTop, TVI_LAST);
	m_TreeCtrl.InsertItem(L"FloorWood.X", Static_FloorWood, TVI_LAST);

	// Ground
		// Ground00.X
	HTREEITEM Static_Ground = m_TreeCtrl.InsertItem(L"Ground", StaticTop, TVI_LAST);
	m_TreeCtrl.InsertItem(L"Ground00.X", Static_Ground, TVI_LAST);
	m_TreeCtrl.InsertItem(L"Ground01.X", Static_Ground, TVI_LAST);
	m_TreeCtrl.InsertItem(L"Ground02.X", Static_Ground, TVI_LAST);
	m_TreeCtrl.InsertItem(L"Ground03.X", Static_Ground, TVI_LAST);

	// Ordnance.X
		// Ordnance.X.X
	HTREEITEM Static_Ordnance = m_TreeCtrl.InsertItem(L"Ordnance", StaticTop, TVI_LAST);
	m_TreeCtrl.InsertItem(L"Ordnance.X", Static_Ordnance, TVI_LAST);

	// Rubble
	HTREEITEM Static_Rubble = m_TreeCtrl.InsertItem(L"Rubble", StaticTop, TVI_LAST);
	
		// Rubble00
			// Rubble00.X
	HTREEITEM Static_Rubble00 = m_TreeCtrl.InsertItem(L"Rubble00", Static_Rubble, TVI_LAST);
	m_TreeCtrl.InsertItem(L"Rubble00.X", Static_Rubble00, TVI_LAST);

		// Rubble01
			// Rubble01.X
	HTREEITEM Static_Rubble01 = m_TreeCtrl.InsertItem(L"Rubble01", Static_Rubble, TVI_LAST);
	m_TreeCtrl.InsertItem(L"Rubble01.X", Static_Rubble01, TVI_LAST);

		// Rubble02
			// Rubble02.X
	HTREEITEM Static_Rubble02 = m_TreeCtrl.InsertItem(L"Rubble02", Static_Rubble, TVI_LAST);
	m_TreeCtrl.InsertItem(L"Rubble02.X", Static_Rubble02, TVI_LAST);

	// Wagon
		// Wagon.X
	HTREEITEM Static_Wagon = m_TreeCtrl.InsertItem(L"Wagon", StaticTop, TVI_LAST);
	m_TreeCtrl.InsertItem(L"Wagon.X", Static_Wagon, TVI_LAST);

	// Wall
		// Wall00.X
	HTREEITEM Static_Wall = m_TreeCtrl.InsertItem(L"Wall", StaticTop, TVI_LAST);
	m_TreeCtrl.InsertItem(L"Wall.X", Static_Wall, TVI_LAST);

	// Monster
		// Monster00
			// Monster00.X
	HTREEITEM Static_Monster00 = m_TreeCtrl.InsertItem(L"Monster00", DynamicTop, TVI_LAST);
	m_TreeCtrl.InsertItem(L"Monster00.X", Static_Monster00, TVI_LAST);

	// Monster
		// Monster01
			// Monster01.X
	HTREEITEM Static_Monster01 = m_TreeCtrl.InsertItem(L"Monster01", DynamicTop, TVI_LAST);
	m_TreeCtrl.InsertItem(L"Monster01.X", Static_Monster01, TVI_LAST);

	HTREEITEM Static_Boss = m_TreeCtrl.InsertItem(L"Boss", DynamicTop, TVI_LAST);
	m_TreeCtrl.InsertItem(L"Boss.X", Static_Boss, TVI_LAST);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

// SetPosition
void CDlgObject::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);
	
	strSelectObjKind = L"";

	SelectObjKind(&strSelectObjKind);
	
	if (nullptr != GetPickComponent(strSelectObjKind) && CheckEdit(strSelectObjKind))
	{
		strTemp = L"";
		GetDlgItemText(IDC_EDIT1, strTemp);
		float fX = (float)_tstof(strTemp.GetString());

		strTemp = L"";
		GetDlgItemText(IDC_EDIT6, strTemp);
		float fY = (float)_tstof(strTemp.GetString());

		strTemp = L"";
		GetDlgItemText(IDC_EDIT5, strTemp);
		float fZ = (float)_tstof(strTemp.GetString());

		m_pComponent = Get_Transform(strSelectObjKind);
		
		NULL_CHECK(m_pComponent);

		dynamic_cast<Engine::CTransform*>(m_pComponent)->Set_Position(D3DXVECTOR3(fX, fY, fZ));

		m_pComponent = nullptr;
	}

	UpdateData(FALSE);

}

// SetRotation_X
void CDlgObject::OnBnClickedButton2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);

	strSelectObjKind = L"";

	SelectObjKind(&strSelectObjKind);
	
	if (nullptr != GetPickComponent(strSelectObjKind) && CheckEdit(strSelectObjKind))
	{
		strTemp = L"";
		GetDlgItemText(IDC_EDIT7, strTemp);
		float fX = (float)_tstof(strTemp.GetString());

		m_pComponent = Get_Transform(strSelectObjKind);

		NULL_CHECK(m_pComponent);

		dynamic_cast<Engine::CTransform*>(m_pComponent)->m_vAngle.x = fX;

		m_pComponent = nullptr;
	}


	UpdateData(FALSE);
}

// SetRotation_Y
void CDlgObject::OnBnClickedButton4()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);
	
	strSelectObjKind = L"";

	SelectObjKind(&strSelectObjKind);

	if (nullptr != GetPickComponent(strSelectObjKind) && CheckEdit(strSelectObjKind))
	{
		strTemp = L"";
		GetDlgItemText(IDC_EDIT9, strTemp);
		float fY = (float)_tstof(strTemp.GetString());

		m_pComponent = Get_Transform(strSelectObjKind);

		NULL_CHECK(m_pComponent);

		dynamic_cast<Engine::CTransform*>(m_pComponent)->m_vAngle.y = fY;

		m_pComponent = nullptr;
	}

	UpdateData(FALSE);
}

// SetRotation_Z
void CDlgObject::OnBnClickedButton5()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);
	
	strSelectObjKind = L"";

	SelectObjKind(&strSelectObjKind);

	if (nullptr != GetPickComponent(strSelectObjKind) && CheckEdit(strSelectObjKind))
	{
		strTemp = L"";
		GetDlgItemText(IDC_EDIT8, strTemp);
		float fZ = (float)_tstof(strTemp.GetString());

		m_pComponent = Get_Transform(strSelectObjKind);

		NULL_CHECK(m_pComponent);

		dynamic_cast<Engine::CTransform*>(m_pComponent)->m_vAngle.z = fZ;

		m_pComponent = nullptr;
	}

	UpdateData(FALSE);
}

// SetScale
void CDlgObject::OnBnClickedButton3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);

	strSelectObjKind = L"";

	SelectObjKind(&strSelectObjKind);

	if (nullptr != GetPickComponent(strSelectObjKind) && CheckEdit(strSelectObjKind))
	{

		strTemp = L"";
		GetDlgItemText(IDC_EDIT10, strTemp);
		float fX = (float)_tstof(strTemp.GetString());

		strTemp = L"";
		GetDlgItemText(IDC_EDIT12, strTemp);
		float fY = (float)_tstof(strTemp.GetString());

		strTemp = L"";
		GetDlgItemText(IDC_EDIT11, strTemp);
		float fZ = (float)_tstof(strTemp.GetString());

		m_pComponent = Get_Transform(strSelectObjKind);

		NULL_CHECK(m_pComponent);

		dynamic_cast<Engine::CTransform*>(m_pComponent)->Set_Scale(fX, fY, fZ);

		m_pComponent = nullptr;		
	}

	UpdateData(FALSE);
}

void CDlgObject::OnBnClickedButton6()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);

	strTemp = L"";
	CString strRoot = L"";

	HTREEITEM hItem = nullptr;
	HTREEITEM hParent = nullptr;

	hItem = m_TreeCtrl.GetNextItem(NULL, TVGN_CARET);
	strTemp = m_TreeCtrl.GetItemText(hItem);

	hParent = m_TreeCtrl.GetNextItem(hItem, TVGN_PARENT);

	while (true)
	{	
		hParent = m_TreeCtrl.GetNextItem(hParent, TVGN_PARENT);
		strRoot = m_TreeCtrl.GetItemText(hParent);
		
		if (!strRoot.Compare(L"Static_Mesh") || !strRoot.Compare(L"Dynamic_Mesh"))
		{
			break;
		}
	}

	int iStarting_point = 0;

	if (-1 != strTemp.Find(L".X", 0))
	{
		if (!strRoot.Compare(L"Static_Mesh"))
		{
			m_AddStaticList.AddString(strTemp);
		}
		else
		{
			m_AddDynamicList.AddString(strTemp);
		}

		AfxExtractSubString(strTemp, strTemp, 0, L'.');
		
		wstring tagName = L"Mesh_";
		tagName = tagName + strTemp.operator LPCWSTR();
	
		Engine::_vec3 vPos{}, vScale{};

		GetDlgItemText(IDC_EDIT18, strTemp);
		if (!strTemp.IsEmpty() && CheckAvailableNumber(IDC_EDIT18))
		{
			vPos.x = (float)_wtof(strTemp);
			strTemp = L"";

			GetDlgItemText(IDC_EDIT17, strTemp);
			if (!strTemp.IsEmpty() && CheckAvailableNumber(IDC_EDIT17))
			{
				vPos.y = (float)_wtof(strTemp);
				strTemp = L"";

				GetDlgItemText(IDC_EDIT15, strTemp);
				if (!strTemp.IsEmpty() && CheckAvailableNumber(IDC_EDIT15))
				{
					vPos.z = (float)_wtof(strTemp);
					iStarting_point = 1;
				}
				else
				{
					iStarting_point = 0;
				}
			}
			else
			{
				iStarting_point = 0;
			}
		}
		else
		{
			iStarting_point = 0;
		}

		if (1 == iStarting_point)
		{
			strTemp = L"";
			GetDlgItemText(IDC_EDIT20, strTemp);
			if (!strTemp.IsEmpty() && CheckAvailableNumber(IDC_EDIT20))
			{
				vScale.x = (float)_wtof(strTemp);
				strTemp = L"";

				GetDlgItemText(IDC_EDIT19, strTemp);
				if (!strTemp.IsEmpty() && CheckAvailableNumber(IDC_EDIT19))
				{
					vScale.y = (float)_wtof(strTemp);
					strTemp = L"";

					GetDlgItemText(IDC_EDIT16, strTemp);
					if (!strTemp.IsEmpty() && CheckAvailableNumber(IDC_EDIT16))
					{
						vScale.z = (float)_wtof(strTemp);

						iStarting_point = 2;
					}
					else
					{
						iStarting_point = 1;
					}
				}
				else
				{
					iStarting_point = 1;
				}
			}
			else
			{
				iStarting_point = 1;
			}
		}


		if (!strRoot.Compare(L"Static_Mesh"))
		{
			CreateStaticObject(iStarting_point, tagName, &vPos, &vScale);
		}
		else
		{
			CreateDynamicObject(iStarting_point, tagName, &vPos, &vScale);
		}
	}

	UpdateData(FALSE);
}

void CDlgObject::OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	strTemp = L"";
	HTREEITEM htSelectItem;
	htSelectItem = m_TreeCtrl.GetNextItem(NULL, TVGN_CARET);

	strTemp = m_TreeCtrl.GetItemText(htSelectItem);

	if (-1 != strTemp.Find(L".X", 0))
	{
		m_SelectObject.SetWindowTextW(strTemp);

		m_CreatePosX.EnableWindow(TRUE);
		m_CreatePosY.EnableWindow(TRUE);
		m_CreatePosZ.EnableWindow(TRUE);
		m_CreateScaleX.EnableWindow(TRUE);
		m_CreateScaleY.EnableWindow(TRUE);
		m_CreateScaleZ.EnableWindow(TRUE);
	}
	else
	{
		m_CreatePosX.EnableWindow(FALSE);
		m_CreatePosY.EnableWindow(FALSE);
		m_CreatePosZ.EnableWindow(FALSE);
		m_CreateScaleX.EnableWindow(FALSE);
		m_CreateScaleY.EnableWindow(FALSE);
		m_CreateScaleZ.EnableWindow(FALSE);
	}

	*pResult = 0;
}

bool CDlgObject::CheckAvailableNumber(int iID)
{
	bool bCheck = false;
	strTemp = L"";
	GetDlgItem(iID)->GetWindowTextW(strTemp);
	
	int iSize = strTemp.GetLength();

	for (int i = 0; i < iSize; ++i)
	{
		TCHAR temp = strTemp.GetAt(i);

		if (temp >= '0' && 
			temp <= '9')
		{
			bCheck = true;
		}
		else if (temp == '.')
		{
			bCheck = true;
		}
		else
		{
			bCheck = false;
			
			break;
		}
	}

	return bCheck;
}

bool CDlgObject::CheckEdit(CString _strSelectObjKind)
{
	CString strCheck = L"";

	if (!_strSelectObjKind.Compare(_T("StaticObject")))
	{
		GetDlgItemText(IDC_EDIT13, strCheck);
	}
	else
	{
		GetDlgItemText(IDC_EDIT14, strCheck);
	}
	
	if (strCheck.IsEmpty())
	{
		return false;
	}

	return true;
}

void CDlgObject::SelectObjKind(CString* pStrSelect)
{
	for (int i = 0; i < 2; ++i)
	{
		if (m_Transform_Selected[i].GetCheck())
		{
			m_Transform_Selected[i].GetWindowTextW(*pStrSelect);

			break;
		}
	}
}

Engine::CComponent * CDlgObject::GetPickComponent(CString _strSelectObjKind)
{
	CString strPickObjName = L"";
	int m_iSelectObjectIndex = 0;


	if (!_strSelectObjKind.Compare(_T("StaticObject")))
	{
		m_iSelectObjectIndex = m_AddStaticList.GetCurSel();
		GetDlgItemText(IDC_EDIT13, strPickObjName);
	}
	else if(!_strSelectObjKind.Compare(_T("DynamicObject")))
	{
		m_iSelectObjectIndex = m_AddDynamicList.GetCurSel();
		GetDlgItemText(IDC_EDIT14, strPickObjName);
	}

	if (!strPickObjName.IsEmpty())
	{
		AfxExtractSubString(strPickObjName, strPickObjName, 0, '.');
	}
	else
	{
		return nullptr;
	}
	
	wstring strMeshName = L"Mesh_" + strPickObjName;

	m_pComponent = Engine::Get_ComponentIndex(L"Layer_GameLogic", strMeshName.c_str(), L"Com_Transform", Engine::CComponent::COMPONENTID::ID_DYNAMIC, m_iSelectObjectIndex);

	NULL_CHECK_RETURN(m_pComponent, nullptr);

	return m_pComponent;
}

void CDlgObject::CreateStaticObject(int _Start_Point, 
									wstring _tagName, 
									Engine::_vec3* _vPos, 
									Engine::_vec3* _vScale)
{
	Engine::CScene* pScene = m_pManagement->Get_CurrentScene();

	int m_IObjectSize = m_AddStaticList.GetCount();

	switch (_Start_Point)
	{
	case 0:
		dynamic_cast<CTool_Stage*>(pScene)->Add_GameObject(L"Layer_GameLogic", _tagName, Engine::CComponent::ID_STATIC, m_IObjectSize);
		break;

	case 1:
		dynamic_cast<CTool_Stage*>(pScene)->Add_GameObject(L"Layer_GameLogic", _tagName, Engine::CComponent::ID_STATIC, m_IObjectSize, *_vPos);
		break;

	case 2:
		dynamic_cast<CTool_Stage*>(pScene)->Add_GameObject(L"Layer_GameLogic", _tagName, Engine::CComponent::ID_STATIC, m_IObjectSize, *_vPos, *_vScale);
		break;

	case 3:
		dynamic_cast<CTool_Stage*>(pScene)->Add_GameObject(L"Layer_GameLogic", _tagName, Engine::CComponent::ID_STATIC, m_IObjectSize, *_vPos, *_vScale);
		break;
	}
	
	LoadObjCnt(L"Static");
}

void CDlgObject::CreateDynamicObject(int _Start_Point, wstring _tagName, Engine::_vec3* _vPos, Engine::_vec3* _vScale)
{
	Engine::CScene* pScene = m_pManagement->Get_CurrentScene();

	int m_IObjectSize = m_AddDynamicList.GetCount() - 1;

	switch (_Start_Point)
	{
	case 0:
		dynamic_cast<CTool_Stage*>(pScene)->Add_GameObject(L"Layer_GameLogic", _tagName, Engine::CComponent::ID_DYNAMIC, m_IObjectSize);
		break;

	case 1:
		dynamic_cast<CTool_Stage*>(pScene)->Add_GameObject(L"Layer_GameLogic", _tagName, Engine::CComponent::ID_DYNAMIC, m_IObjectSize, *_vPos);
		break;

	case 2:
		dynamic_cast<CTool_Stage*>(pScene)->Add_GameObject(L"Layer_GameLogic", _tagName, Engine::CComponent::ID_DYNAMIC, m_IObjectSize, *_vPos, *_vScale);
		break;
	}

	LoadObjCnt(L"Dynamic");
}

void CDlgObject::LoadCreateStaicObject(CString MeshName, 
										Engine::_vec3 * temp_Right, 
										Engine::_vec3 * temp_Up, 
										Engine::_vec3 * temp_Look, 
										Engine::_vec3 * temp_Pos, 
										Engine::_vec3 * temp_Scale,
										Engine::_vec3 * temp_Angle, 
										int iObjIndex)
{
	Engine::CScene* pScene = m_pManagement->Get_CurrentScene();

	dynamic_cast<CTool_Stage*>(pScene)->Load_GameObject(L"Layer_GameLogic",
														MeshName.operator LPCWSTR(),
														Engine::CComponent::ID_STATIC,
														temp_Right,
														temp_Up,
														temp_Look,
														temp_Pos,
														temp_Scale,
														temp_Angle,
														iObjIndex);
	
}

void CDlgObject::LoadCreateDynamicObject(CString MeshName, 
											Engine::_vec3 * temp_Right, 
											Engine::_vec3 * temp_Up, 
											Engine::_vec3 * temp_Look, 
											Engine::_vec3 * temp_Pos, 
											Engine::_vec3 * temp_Scale, 
											Engine::_vec3 * temp_Angle, 
											int iObjIndex)
{
	Engine::CScene* pScene = m_pManagement->Get_CurrentScene();

	dynamic_cast<CTool_Stage*>(pScene)->Load_GameObject(L"Layer_GameLogic",
														MeshName.operator LPCWSTR(),
														Engine::CComponent::ID_DYNAMIC,
														temp_Right,
														temp_Up,
														temp_Look,
														temp_Pos,
														temp_Scale,
														temp_Angle,
														iObjIndex);
}

void CDlgObject::OnLbnSelchangeList1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	
	if (KEY_DOWN(VK_LBUTTON))
	{
		return;
	}

	UpdateData(TRUE);

	int iSelectObject = 0;
	CString strSelectName = L"";
	
	iSelectObject = m_AddStaticList.GetCurSel();

	if (-1 > iSelectObject)
	{
		return;
	}

	m_AddStaticList.GetText(iSelectObject, strSelectName);
	
	m_PickObject.SetWindowTextW(strSelectName);
	
	CString strCheck = L"";
	SelectObjKind(&strCheck);

	AfxExtractSubString(strSelectName, strSelectName, 0, '.');
	strSelectName = L"Mesh_" + strSelectName;

	if (nullptr != m_pGameObject)
	{
		const type_info& class_type = typeid(*m_pGameObject);
	
		if (!strcmp("class CTool_StaticMesh", class_type.name()))
		{
			dynamic_cast<CTool_StaticMesh*>(m_pGameObject)->Set_RenderState(TRUE);
		}
		else if (!strcmp("class CTool_Monster00", class_type.name()))
		{
			dynamic_cast<CTool_Monster00*>(m_pGameObject)->Set_RenderState(TRUE);
		}
		//else if (!strcmp("class CTool_Monster01", class_type.name()))
		//{
		//	dynamic_cast<CTool_Monster01*>(m_pGameObject)->Set_RenderState(TRUE);
		//}
		else if (!strcmp("class CTool_Boss", class_type.name()))
		{
			dynamic_cast<CTool_Boss*>(m_pGameObject)->Set_RenderState(TRUE);
		}
	
		m_pGameObject = nullptr;
	}

	if (!strCheck.Compare(L"StaticObject"))
	{
		m_pGameObject = m_pManagement->Get_Object(L"Layer_GameLogic", strSelectName.operator LPCWSTR(), iSelectObject);
		
		NULL_CHECK(m_pGameObject);

		dynamic_cast<CTool_StaticMesh*>(m_pGameObject)->Set_RenderState(FALSE);

		Engine::CTransform* pTransform = dynamic_cast<Engine::CTransform*>(m_pGameObject->Get_Component(L"Com_Transform", Engine::CComponent::ID_DYNAMIC));

		Set_Transform(pTransform);

	}

	UpdateData(FALSE);
}

void CDlgObject::OnLbnSelchangeList2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	
	if (KEY_DOWN(VK_LBUTTON))
	{
		return;
	}
	
	UpdateData(TRUE);

	int iSelectObject = 0;
	CString strSelectName = L"";

	iSelectObject = m_AddDynamicList.GetCurSel();

	if (-1 > iSelectObject)
	{
		return;
	}

	m_AddDynamicList.GetText(iSelectObject, strSelectName);

	m_PickDynamicObject.SetWindowTextW(strSelectName);

	CString strCheck = L"";
	SelectObjKind(&strCheck);

	if (nullptr != m_pGameObject)
	{
		const type_info& class_type = typeid(*m_pGameObject);

		if (!strcmp("class CTool_StaticMesh", class_type.name()))
		{
			dynamic_cast<CTool_StaticMesh*>(m_pGameObject)->Set_RenderState(TRUE);
		}
		else if (!strcmp("class CTool_Monster00", class_type.name()))
		{
			dynamic_cast<CTool_Monster00*>(m_pGameObject)->Set_RenderState(TRUE);
		}
		else if (!strcmp("class CTool_Monster01", class_type.name()))
		{
			dynamic_cast<CTool_Monster01*>(m_pGameObject)->Set_RenderState(TRUE);
		}
		else if (!strcmp("class CTool_Boss", class_type.name()))
		{
			dynamic_cast<CTool_Boss*>(m_pGameObject)->Set_RenderState(TRUE);
		}

		m_pGameObject = nullptr;
	}

	if (!strCheck.Compare(L"DynamicObject"))
	{
		AfxExtractSubString(strSelectName, strSelectName, 0, L'.');

		wstring tagName = L"Mesh_";
		tagName = tagName + strSelectName.operator LPCWSTR();
		int iSelect = m_AddDynamicList.GetCurSel();

		m_pGameObject = m_pManagement->Get_Object(L"Layer_GameLogic", tagName, iSelect);

		NULL_CHECK(m_pGameObject);

		if (!tagName.compare(L"Mesh_Monster00"))
		{
			dynamic_cast<CTool_Monster00*>(m_pGameObject)->Set_RenderState(FALSE);
		}
		else if (!tagName.compare(L"Mesh_Monster01"))
		{
			dynamic_cast<CTool_Monster01*>(m_pGameObject)->Set_RenderState(FALSE);
		}
		else if (!tagName.compare(L"Mesh_Boss"))
		{
			dynamic_cast<CTool_Boss*>(m_pGameObject)->Set_RenderState(FALSE);
		}

		Engine::CTransform* pTransform = dynamic_cast<Engine::CTransform*>(m_pGameObject->Get_Component(L"Com_Transform", Engine::CComponent::ID_DYNAMIC));

		Set_Transform(pTransform);
	}

	UpdateData(FALSE);
}


void CDlgObject::StaticMesh_Save()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	HANDLE	hFile = CreateFile(L"../../Data/StaticMesh.dat", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	
	multimap<const wstring, Engine::CGameObject*> mul_ObjectList;

	mul_ObjectList = m_pManagement->Get_CurrentScene()->Get_Layer(L"Layer_GameLogic")->Get_ObjectMultiMap();
	
	DWORD	dwByte = 0;
	TCHAR	szBuffer[MIN_STR] = L"";
	int iIndex = m_AddStaticList.GetCount(), iCnt = 0;

	for (auto& iter : mul_ObjectList)
	{
		const type_info& class_type = typeid(*iter.second);

		if (!strcmp("class CTool_StaticMesh", class_type.name()))
		{
			Engine::CTransform tempTransform = *dynamic_cast<CTool_StaticMesh*>(iter.second)->Get_pTransCom();

			// Pos
			for (int i = 0; i < Engine::CTransform::INFO_END; ++i)
			{
				WriteFile(hFile, &tempTransform.m_vInfo[i], sizeof(D3DXVECTOR3), &dwByte, NULL);
			}

			// Sacle
			WriteFile(hFile, &tempTransform.m_vScale, sizeof(D3DXVECTOR3), &dwByte, NULL);
			
			// Angle
			WriteFile(hFile, &tempTransform.m_vAngle, sizeof(D3DXVECTOR3), &dwByte, NULL);
			
			// MeshName
			WriteFile(hFile, &*dynamic_cast<CTool_StaticMesh*>(iter.second)->Get_MeshName().c_str(), MIN_STR, &dwByte, NULL);

			swprintf_s(szBuffer, L"%d", iIndex - (iIndex - iCnt));

			iCnt++;

			// Index
			WriteFile(hFile, &szBuffer, MIN_STR, &dwByte, NULL);

		}
	}

	CloseHandle(hFile);

}


void CDlgObject::StaticMesh_Load()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);

	HANDLE	hFile = CreateFile(L"../../Data/StaticMesh.dat", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	Engine::_vec3 temp_Right, temp_Up, temp_Look, temp_Pos;
	Engine::_vec3 temp_Scale, temp_Angle;
	TCHAR MeshName[MIN_STR] = L"", Index[MIN_STR] = L"";
	DWORD dwByte = 99;

	m_AddStaticList.ResetContent();

	while (true)
	{
		ReadFile(hFile, temp_Right, sizeof(D3DXVECTOR3), &dwByte, NULL);

		if (dwByte == 0)
		{
			break;
		}

		ReadFile(hFile, temp_Up, sizeof(D3DXVECTOR3), &dwByte, NULL);
		ReadFile(hFile, temp_Look, sizeof(D3DXVECTOR3), &dwByte, NULL);
		ReadFile(hFile, temp_Pos, sizeof(D3DXVECTOR3), &dwByte, NULL);

		ReadFile(hFile, temp_Scale, sizeof(D3DXVECTOR3), &dwByte, NULL);

		ReadFile(hFile, temp_Angle, sizeof(D3DXVECTOR3), &dwByte, NULL);

		ReadFile(hFile, MeshName, MIN_STR, &dwByte, NULL);

		ReadFile(hFile, Index, MIN_STR, &dwByte, NULL);

		LoadCreateStaicObject(MeshName, &temp_Right, &temp_Up, &temp_Look, &temp_Pos, &temp_Scale, &temp_Angle, _ttoi(Index));

		CString strConvert = MeshName;

		int retval = 0;
		retval = strConvert.Find('_');
		strConvert = strConvert.Mid(retval + 1);

		strConvert += L".X";

		m_AddStaticList.InsertString(-1, strConvert);
	}

	CloseHandle(hFile);

	UpdateData(FALSE);

	LoadObjCnt(L"Static");
}


void CDlgObject::DynamicMesh_Save()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	HANDLE	hFile = CreateFile(L"../../Data/DynamicMesh.dat", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	multimap<const wstring, Engine::CGameObject*> mul_ObjectList;

	mul_ObjectList = m_pManagement->Get_CurrentScene()->Get_Layer(L"Layer_GameLogic")->Get_ObjectMultiMap();

	DWORD	dwByte = 0;
	TCHAR	szBuffer[MIN_STR] = L"";
	int iIndex = m_AddStaticList.GetCount(), iCnt = 0;

	for (auto& iter : mul_ObjectList)
	{
		const type_info& class_type = typeid(*iter.second);

		if (!strcmp("class CTool_Monster00", class_type.name()))
		{
			Engine::CTransform tempTransform = *dynamic_cast<CTool_Monster00*>(iter.second)->Get_pTransCom();
			
			// Pos
			for (int i = 0; i < Engine::CTransform::INFO_END; ++i)
			{
				WriteFile(hFile, &tempTransform.m_vInfo[i], sizeof(D3DXVECTOR3), &dwByte, NULL);
			}

			// Sacle
			WriteFile(hFile, &tempTransform.m_vScale, sizeof(D3DXVECTOR3), &dwByte, NULL);

			// Angle
			WriteFile(hFile, &tempTransform.m_vAngle, sizeof(D3DXVECTOR3), &dwByte, NULL);

			TCHAR tagName[64]; 
			wstring strName = dynamic_cast<CTool_Monster00*>(iter.second)->Get_MeshName();

			lstrcpy(tagName, strName.c_str());

			// MeshName
			WriteFile(hFile, tagName, MIN_STR, &dwByte, NULL);

			swprintf_s(szBuffer, L"%d", iIndex - (iIndex - iCnt));

			iCnt++;

			// Index
			WriteFile(hFile, &szBuffer, MIN_STR, &dwByte, NULL);
		}
		else if (!strcmp("class CTool_Monster01", class_type.name()))
		{
			Engine::CTransform tempTransform = *dynamic_cast<CTool_Monster01*>(iter.second)->Get_pTransCom();
	
			// Pos
			for (int i = 0; i < Engine::CTransform::INFO_END; ++i)
			{
				WriteFile(hFile, &tempTransform.m_vInfo[i], sizeof(D3DXVECTOR3), &dwByte, NULL);
			}

			// Sacle
			WriteFile(hFile, &tempTransform.m_vScale, sizeof(D3DXVECTOR3), &dwByte, NULL);

			// Angle
			WriteFile(hFile, &tempTransform.m_vAngle, sizeof(D3DXVECTOR3), &dwByte, NULL);

			TCHAR tagName[64];
			wstring strName = dynamic_cast<CTool_Monster01*>(iter.second)->Get_MeshName();

			lstrcpy(tagName, strName.c_str());

			// MeshName
			WriteFile(hFile, tagName, MIN_STR, &dwByte, NULL);

			swprintf_s(szBuffer, L"%d", iIndex - (iIndex - iCnt));

			iCnt++;
			// Index
			WriteFile(hFile, &szBuffer, MIN_STR, &dwByte, NULL);
		}
		else if (!strcmp("class CTool_Boss", class_type.name()))
		{
			Engine::CTransform tempTransform = *dynamic_cast<CTool_Boss*>(iter.second)->Get_pTransCom();

			// Pos
			for (int i = 0; i < Engine::CTransform::INFO_END; ++i)
			{
				WriteFile(hFile, &tempTransform.m_vInfo[i], sizeof(D3DXVECTOR3), &dwByte, NULL);
			}

			// Sacle
			WriteFile(hFile, &tempTransform.m_vScale, sizeof(D3DXVECTOR3), &dwByte, NULL);

			// Angle
			WriteFile(hFile, &tempTransform.m_vAngle, sizeof(D3DXVECTOR3), &dwByte, NULL);

			TCHAR tagName[64];
			wstring strName = dynamic_cast<CTool_Boss*>(iter.second)->Get_MeshName();

			lstrcpy(tagName, strName.c_str());

			// MeshName
			WriteFile(hFile, tagName, MIN_STR, &dwByte, NULL);

			swprintf_s(szBuffer, L"%d", iIndex - (iIndex - iCnt));

			iCnt++;
			// Index
			WriteFile(hFile, &szBuffer, MIN_STR, &dwByte, NULL);
		}
	}

	CloseHandle(hFile);
}


void CDlgObject::DynamicMesh_Load()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);

	HANDLE	hFile = CreateFile(L"../../Data/DynamicMesh.dat", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	Engine::_vec3 temp_Right, temp_Up, temp_Look, temp_Pos;
	Engine::_vec3 temp_Scale, temp_Angle;
	TCHAR MeshName[MIN_STR] = L"", Index[MIN_STR] = L"";
	DWORD dwByte = 99;

	m_AddDynamicList.ResetContent();

	while (true)
	{
		ReadFile(hFile, temp_Right, sizeof(D3DXVECTOR3), &dwByte, NULL);

		if (dwByte == 0)
		{
			break;
		}

		ReadFile(hFile, temp_Up, sizeof(D3DXVECTOR3), &dwByte, NULL);
		ReadFile(hFile, temp_Look, sizeof(D3DXVECTOR3), &dwByte, NULL);
		ReadFile(hFile, temp_Pos, sizeof(D3DXVECTOR3), &dwByte, NULL);

		ReadFile(hFile, temp_Scale, sizeof(D3DXVECTOR3), &dwByte, NULL);

		ReadFile(hFile, temp_Angle, sizeof(D3DXVECTOR3), &dwByte, NULL);

		ReadFile(hFile, MeshName, MIN_STR, &dwByte, NULL);

		ReadFile(hFile, Index, MIN_STR, &dwByte, NULL);

		LoadCreateDynamicObject(MeshName, &temp_Right, &temp_Up, &temp_Look, &temp_Pos, &temp_Scale, &temp_Angle, _ttoi(Index));

		CString strConvert = MeshName;

		int retval = 0;
		retval = strConvert.Find('_');
		strConvert = strConvert.Mid(retval + 1);

		strConvert += L".X";

		m_AddDynamicList.InsertString(-1, strConvert);
	}

	CloseHandle(hFile);

	CString strCnt;

	int iCnt = m_AddDynamicList.GetCount();

	strCnt.Format(L"%d", iCnt);

	SetDlgItemText(IDC_DYNAMIC_CNT, strCnt);

	UpdateData(FALSE);
}

void CDlgObject::Set_Transform(Engine::CTransform * pTransform)
{
	CString strTem;

	// Pos
	strTem.Format(L"%.4f", pTransform->m_vInfo[Engine::CTransform::INFO_POS].x);
	SetDlgItemText(IDC_EDIT1, strTem);

	strTem.Format(L"%.4f", pTransform->m_vInfo[Engine::CTransform::INFO_POS].y);
	SetDlgItemText(IDC_EDIT6, strTem);

	strTem.Format(L"%.4f", pTransform->m_vInfo[Engine::CTransform::INFO_POS].z);
	SetDlgItemText(IDC_EDIT5, strTem);

	// Scale
	D3DXVECTOR3 vecTem = pTransform->m_vScale;
	
	strTem.Format(L"%.4f", vecTem.x);
	SetDlgItemText(IDC_EDIT10, strTem);

	strTem.Format(L"%.4f", vecTem.y);
	SetDlgItemText(IDC_EDIT12, strTem);

	strTem.Format(L"%.4f", vecTem.z);
	SetDlgItemText(IDC_EDIT11, strTem);
}

Engine::CComponent * CDlgObject::Get_Transform(CString strCompare)
{
	CString strTemp;

	if (!strCompare.Compare(L"StaticObject"))
	{
		m_AddStaticList.GetText(m_AddStaticList.GetCurSel(), strTemp);

		AfxExtractSubString(strTemp, strTemp, 0, '.');

		strTemp = L"Mesh_" + strTemp;

		m_pGameObject = m_pManagement->Get_Object(L"Layer_GameLogic", strTemp.operator LPCWSTR(), m_AddStaticList.GetCurSel());
		NULL_CHECK_RETURN(m_pGameObject, nullptr);
	}
	else if((!strCompare.Compare(L"DynamicObject")))
	{
		m_AddDynamicList.GetText(m_AddDynamicList.GetCurSel(), strTemp);

		AfxExtractSubString(strTemp, strTemp, 0, '.');

		strTemp = L"Mesh_" + strTemp;

		m_pGameObject = m_pManagement->Get_Object(L"Layer_GameLogic", strTemp.operator LPCWSTR(), m_AddDynamicList.GetCurSel());
		NULL_CHECK_RETURN(m_pGameObject, nullptr);
	}

	

	return m_pGameObject->Get_Component(L"Com_Transform", Engine::CComponent::ID_DYNAMIC);
}

void CDlgObject::LoadObjCnt(CString pType)
{
	multimap<const wstring, Engine::CGameObject*> pLayer = m_pManagement->Get_CurrentScene()->Get_Layer(L"Layer_GameLogic")->Get_ObjectMultiMap();

	int iSize = 0;
	CString strTemp;

	if (!pType.Compare(L"Static"))
	{
		for (auto iter : pLayer)
		{
			const type_info& class_type = typeid(*iter.second);

			if (!strcmp("class CTool_StaticMesh", class_type.name()))
			{
				iSize++;
			}
		}
		strTemp.Format(L"%d", iSize);
		SetDlgItemTextW(IDC_STATIC_CNT, strTemp.operator LPCWSTR());
	}
	else if(!pType.Compare(L"Dynamic"))
	{
		for (auto iter : pLayer)
		{
			const type_info& class_type = typeid(*iter.second);

			if (!strcmp("class CTool_Monster00", class_type.name()))
			{
				iSize++;
			}

			else if (!strcmp("class CTool_Monster01", class_type.name()))
			{
				iSize++;
			}

			else if (!strcmp("class CTool_Boss", class_type.name()))
			{
				iSize++;
			}
		}
		strTemp.Format(L"%d", iSize);
		SetDlgItemTextW(IDC_DYNAMIC_CNT, strTemp.operator LPCWSTR());
	}
}

BOOL CDlgObject::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
		{
			return TRUE;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}


void CDlgObject::Delete_StaticObj()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	int iSelect = m_AddStaticList.GetCurSel();

	if (-1 > iSelect &&
		-1 > m_Transform_Selected[0].GetCheck())
	{
		return;
	}

	CString strTemp;

	m_AddStaticList.GetText(iSelect, strTemp);

	AfxExtractSubString(strTemp, strTemp, 0, '.');

	strTemp = L"Mesh_" + strTemp;

	m_pManagement->Get_CurrentScene()->Get_Layer(L"Layer_GameLogic")->DeleteObj(iSelect, strTemp.operator LPCWSTR());

	m_AddStaticList.DeleteString(iSelect);
	
	LoadObjCnt(L"Static");
}


void CDlgObject::Delete_DynamicObj()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	int iSelect = m_AddDynamicList.GetCurSel();

	if (-1 > iSelect &&
		-1 > m_Transform_Selected[1].GetCheck())
	{
		return;
	}

	CString strTemp;

	m_AddDynamicList.GetText(iSelect, strTemp);

	AfxExtractSubString(strTemp, strTemp, 0, '.');

	strTemp = L"Mesh_" + strTemp;

	m_pManagement->Get_CurrentScene()->Get_Layer(L"Layer_GameLogic")->DeleteObj(iSelect, strTemp.operator LPCWSTR());

	m_AddDynamicList.DeleteString(iSelect);
}


void CDlgObject::Set_ViewState()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);
	
	int iSelect = m_SetViewState.GetCheck();
	
	auto Multi = Engine::Get_Management()->Get_CurrentScene()->Get_Layer(L"Layer_GameLogic")->Get_ObjectMultiMap();
	
	if (iSelect == 1)
	{
		for (auto iter : Multi)
		{
			const type_info& class_type = typeid(*iter.second);

			if (!strcmp("class CTool_StaticMesh", class_type.name()))
			{
				dynamic_cast<CTool_StaticMesh*>(iter.second)->Set_RenderState(FALSE);
			}
		}
	}
	else
	{
		for (auto iter : Multi)
		{
			const type_info& class_type = typeid(*iter.second);

			if (!strcmp("class CTool_StaticMesh", class_type.name()))
			{
				dynamic_cast<CTool_StaticMesh*>(iter.second)->Set_RenderState(TRUE);
			}
		}

	}

	UpdateData(FALSE);
}
