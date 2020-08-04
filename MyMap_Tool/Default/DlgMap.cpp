// DlgMap.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MyMap_Tool.h"
#include "DlgMap.h"
#include "afxdialogex.h"

#include "Export_Function.h"

#include "MyMap_ToolDoc.h"
#include "MyMap_ToolView.h"
#include "Tool_Stage.h"

// CDlgMap 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgMap, CDialog)

CDlgMap::CDlgMap(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DIALOG1, pParent)
	, m_Navi_X(0.f)
	, m_Navi_Y(0.f)
	, m_Navi_Z(0.f)
	, m_TerrainPos_X(0.f)
	, m_TerrainPos_Z(0.f)
	, m_fHeight(0.f)
	, m_fFindY(0)
{

}

CDlgMap::~CDlgMap()
{
	for (auto i : m_MapImage)
	{
		i.second->Destroy();
		Engine::Safe_Delete(i.second);
	}
	m_MapImage.clear();
}

void CDlgMap::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, XCount);
	DDX_Control(pDX, IDC_EDIT2, ZCount);
	DDX_Control(pDX, IDC_EDIT3, ITV_Value);
	DDX_Control(pDX, IDC_EDIT4, m_Detail);

	DDX_Control(pDX, IDC_CHECKBOX01, m_bSold);
	DDX_Control(pDX, IDC_CHECKBOX02, m_bWireFrame);

	DDX_Control(pDX, IDC_TREE1, m_NaviList);
	DDX_Control(pDX, IDC_CHECK1, m_AddNavi);

	DDX_Control(pDX, IDC_NAVI_X, m_Edit_Navi_X);
	DDX_Control(pDX, IDC_NAVI_Y, m_Edit_Navi_Y);
	DDX_Control(pDX, IDC_NAVI_Z, m_Edit_Navi_Z);
	DDX_Control(pDX, IDC_CHECK2, m_NaviEdit);
	DDX_Control(pDX, IDC_EDIT5, m_Pos_1);
	DDX_Control(pDX, IDC_EDIT6, m_Pos_2);
	DDX_Control(pDX, IDC_LIST1, m_NeighborList);
	DDX_Control(pDX, IDC_LIST3, m_FindList);
	DDX_Control(pDX, IDC_EDIT7, m_Find_Z);
	DDX_Control(pDX, IDC_EDIT10, m_Find_X);
	DDX_Control(pDX, IDC_EDIT8, m_Find_Y);
	DDX_Control(pDX, IDC_EDIT11, m_FindCount);
	DDX_Control(pDX, IDC_EDIT12, m_NaviCount);
	DDX_Control(pDX, IDC_TERRAINI_X, m_TerrainX);
	DDX_Control(pDX, IDC_TERRAINI_Z, m_TerrainZ);
	DDX_Control(pDX, IDC_EDIT13, m_FindCount_t);
	DDX_Control(pDX, IDC_NAVI_Y2, m_FindY);
	DDX_Control(pDX, IDC_LIST4, m_FindYList);
	DDX_Control(pDX, IDC_EDIT22, m_SetUpY);
	DDX_Control(pDX, IDC_NAVI_Y3, m_Delete_Index);
}

BEGIN_MESSAGE_MAP(CDlgMap, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CDlgMap::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CDlgMap::Navi_Pop_Back)
	ON_WM_DROPFILES()
	ON_LBN_SELCHANGE(IDC_LIST1, &CDlgMap::OnLbnSelchangeList1)
	ON_BN_CLICKED(IDC_CHECKBOX01, &CDlgMap::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECKBOX02, &CDlgMap::OnBnClickedCheck2)
	ON_BN_CLICKED(IDC_BUTTON4, &CDlgMap::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON6, &CDlgMap::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_CHECK1, &CDlgMap::OnAddNaviState)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, &CDlgMap::OnNavi_X)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN2, &CDlgMap::OnNavi_Y)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN3, &CDlgMap::OnNavi_Z)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, &CDlgMap::OnNaviTreeSelected)
	ON_BN_CLICKED(IDC_BUTTON7, &CDlgMap::TerrinSave)
	ON_BN_CLICKED(IDC_BUTTON9, &CDlgMap::TerrainLoad)
	ON_BN_CLICKED(IDC_BUTTON12, &CDlgMap::Navi_Save)
	ON_BN_CLICKED(IDC_BUTTON11, &CDlgMap::Navi_Load)
	ON_BN_CLICKED(IDC_REFRESH, &CDlgMap::OnReFresh)
	ON_BN_CLICKED(IDC_BUTTON8, &CDlgMap::Find_Index)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN7, &CDlgMap::TerrainPos_X)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN9, &CDlgMap::TerrainPos_Z)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN10, &CDlgMap::Edit_Compore_Point)
	ON_BN_CLICKED(IDC_CHECK_VALUE, &CDlgMap::Get_Pos_1_Value)
	ON_BN_CLICKED(IDC_BUTTON16, &CDlgMap::Link_Celling)
	ON_BN_CLICKED(IDC_FIND_BUTTON, &CDlgMap::Find_Y)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN11, &CDlgMap::FindY_Value)
	ON_BN_CLICKED(IDC_BUTTON18, &CDlgMap::Apply_Y_Value)
	ON_BN_CLICKED(IDC_FIND_BUTTON2, &CDlgMap::Delete_Index)
END_MESSAGE_MAP()

BOOL CDlgMap::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	CMyMap_ToolView* MainFrame = (CMyMap_ToolView*)AfxGetMainWnd();

	m_pManagement = Engine::Get_Management();

	SetDlgItemText(IDC_EDIT1, L"129");	// X 갯수
	SetDlgItemText(IDC_EDIT2, L"129");	// Z 갯수
	SetDlgItemText(IDC_EDIT3, L"1");	// 인터벌
	SetDlgItemText(IDC_EDIT4, L"20");	// 디테일
	m_bSold.SetCheck(true);

	pNaviObject = m_pManagement->Get_Object(L"Layer_GameLogic", L"Navi_Mesh");
	NULL_CHECK_RETURN(pNaviObject, E_FAIL);

	pNaviComponent = dynamic_cast<Engine::CNaviMesh*>(m_pManagement->Get_Component(L"Layer_GameLogic", L"Navi_Mesh", L"Com_NaviMesh", Engine::CComponent::ID_STATIC));
	NULL_CHECK_RETURN(pNaviComponent, E_FAIL);


	Engine::CTransform* pComponent = dynamic_cast<Engine::CTransform*>(m_pManagement->Get_Component(L"Layer_GameLogic", L"Terrain", L"Com_Transform", Engine::CComponent::COMPONENTID::ID_DYNAMIC));
	NULL_CHECK_RETURN(pComponent, E_FAIL);

	m_TerrainPos_X = pComponent->m_vInfo[Engine::CTransform::INFO_POS].x;
	m_TerrainPos_Z = pComponent->m_vInfo[Engine::CTransform::INFO_POS].z;

	CString strTem;

	strTem.Format(L"%f", m_TerrainPos_X);
	SetDlgItemText(IDC_TERRAINI_X, strTem);
	
	strTem.Format(L"%f", m_TerrainPos_Z);
	SetDlgItemText(IDC_TERRAINI_Z, strTem);

	if (!CheckNaviList())
	{
		return FALSE;
	}

	SetDlgItemText(IDC_EDIT11, L"0.000000");

	Invalidate(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

// CDlgMap 메시지 처리기입니다.
void CDlgMap::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.	 
	UpdateData(TRUE);

	CString strTem_1, strTem_2, strTem_3;
	
	GetDlgItemText(IDC_EDIT1, strTem_1);	// X 갯수
	GetDlgItemText(IDC_EDIT2, strTem_2);	// Z 갯수
	GetDlgItemText(IDC_EDIT3, strTem_3);	// 인터벌
	
	Engine::_ulong dwXCnt = _ttoi(strTem_1);
	Engine::_ulong dwZCnt = _ttoi(strTem_2);
	Engine::_ulong dwItv = _ttoi(strTem_3);
	
	strTem_1 = L"";

	GetDlgItemText(IDC_EDIT4, strTem_1);
	
	if (!strTem_1.IsEmpty())
	{
		Engine::_ulong dwDetail = _ttoi(strTem_1);
		Engine::CComponent* pComponent = m_pManagement->Get_Component(L"Layer_GameLogic", L"Terrain", L"Com_Buffer", Engine::CComponent::COMPONENTID::ID_STATIC);
		dynamic_cast<Engine::CTerrainTex*>(pComponent)->SetUp_VtxCnt(dwXCnt, dwZCnt, dwItv, dwDetail);

		GetDlgItemText(IDC_TERRAINI_Z, strTem_1);
		pComponent = Engine::Get_Component(L"Layer_GameLogic", L"Terrain", L"Com_Transform", Engine::CComponent::COMPONENTID::ID_DYNAMIC);
		dynamic_cast<Engine::CTransform*>(pComponent)->m_vInfo[Engine::CTransform::INFO_POS].z = (float)_ttof(strTem_1);
	}
	else
	{
		Engine::CComponent* pComponent = m_pManagement->Get_Component(L"Layer_GameLogic", L"Terrain", L"Com_Buffer", Engine::CComponent::COMPONENTID::ID_STATIC);
		dynamic_cast<Engine::CTerrainTex*>(pComponent)->SetUp_VtxCnt(dwXCnt, dwZCnt, dwItv);

		GetDlgItemText(IDC_TERRAINI_Z, strTem_1);
		pComponent = Engine::Get_Component(L"Layer_GameLogic", L"Terrain", L"Com_Transform", Engine::CComponent::COMPONENTID::ID_DYNAMIC);
		dynamic_cast<Engine::CTransform*>(pComponent)->m_vInfo[Engine::CTransform::INFO_POS].z = (float)_ttof(strTem_1);
	}

	UpdateData(FALSE);
}

bool CDlgMap::CheckNaviList()
{
	Engine::CNaviMesh* pComponent = dynamic_cast<Engine::CNaviMesh*>(m_pManagement->Get_Component(L"Layer_GameLogic", L"Navi_Mesh", L"Com_NaviMesh", Engine::CComponent::ID_STATIC));
	NULL_CHECK_RETURN(pComponent, FALSE);

	m_vecNaviList = *pComponent->Get_CellList();

	vector<Engine::CCell*>::iterator iter = m_vecNaviList.begin();

	m_NaviList.DeleteAllItems();

	CString strIndex = L"";
	CString strInputPos = L"";
	for (int i = 0; iter != m_vecNaviList.end(); ++i, ++iter)
	{
		strIndex.Format(_T("%d"), i);
		HTREEITEM NaviTop = m_NaviList.InsertItem((LPCTSTR)strIndex, TVI_ROOT, TVI_LAST);

		Engine::_vec3 vPosA = *(*iter)->Get_Point(Engine::CCell::POINT_A);
		strInputPos.Format(_T("_%d"), 0);

		m_NaviList.InsertItem((LPCTSTR)strInputPos, NaviTop, TVI_LAST);

		Engine::_vec3 vPosB = *(*iter)->Get_Point(Engine::CCell::POINT_B);
		strInputPos.Format(_T("_%d"), 1);

		m_NaviList.InsertItem((LPCTSTR)strInputPos, NaviTop, TVI_LAST);

		Engine::_vec3 vPosC = *(*iter)->Get_Point(Engine::CCell::POINT_C);
		strInputPos.Format(_T("_%d"), 2);

		m_NaviList.InsertItem((LPCTSTR)strInputPos, NaviTop, TVI_LAST);
	}

	SetNaviCount();

	return TRUE;
}

void CDlgMap::Navi_Pop_Back()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	Engine::CGameObject* pGameObject = Engine::Get_Management()->Get_Object(L"Layer_GameLogic", L"Navi_Mesh");

	dynamic_cast<CNavi*>(pGameObject)->Set_PopBack();
}

void CDlgMap::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	UpdateData(TRUE);

	int iFileNumber = DragQueryFile(hDropInfo, -1, NULL, 0);

	if (0 > iFileNumber)
	{
		return;
	}

	TCHAR szFullPath[128] = L"";

	for (int i = 0; i < iFileNumber; ++i)
	{
		DragQueryFile(hDropInfo, i, (LPWSTR)szFullPath, 128);

		wstring wstrRelativePath = ConvertRelatviePath(szFullPath);
		
		CString	strImageName;
		strImageName = PathFindFileName(wstrRelativePath.c_str());	 // 경로를 제외한 파일명만 남기는 함수

		PathRemoveExtension((LPWSTR)strImageName.operator LPCWSTR()); // 확장자 명을 제거하는 함수

		auto iter = m_MapImage.find(strImageName);

		if (iter == m_MapImage.end())
		{
			CImage* pImage = new CImage;
			pImage->Load(wstrRelativePath.c_str());

			m_MapImage.insert(make_pair(strImageName, pImage));
			m_ListCtrl.AddString(strImageName);
		}
	}

	UpdateData(FALSE);
	
	CDialog::OnDropFiles(hDropInfo);
}

wstring CDlgMap::ConvertRelatviePath(wstring wstrFullPath)
{
	TCHAR szRelativePath[128] = L"";
	TCHAR szDirPath[128] = L"";

	// vcproj 파일이 있는 위치까지의 경로를 얻어 옴
	GetCurrentDirectory(sizeof(szDirPath), szDirPath);

	// 유의 사항 : 같은 드라이브 내에서만 사용 가능
	PathRelativePathTo((LPWSTR)szRelativePath,
		szDirPath, FILE_ATTRIBUTE_DIRECTORY,
		wstrFullPath.c_str(), FILE_ATTRIBUTE_NORMAL);
	
	return szRelativePath;
}

void CDlgMap::SetNaviCount()
{
	CString strTem;
	SetDlgItemTextW(IDC_EDIT12, L"");

	size_t iCount = pNaviComponent->Get_CellList()->size();

	strTem.Format(L"%d", iCount);

	SetDlgItemTextW(IDC_EDIT12, strTem);
}

void CDlgMap::OnLbnSelchangeList1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);

	CString		strSelectName;

	int			iSelectIndex = m_ListCtrl.GetCurSel(); //  클릭한 셀의 인덱스를 반환하는 함수

	if (0 > iSelectIndex)
		return;

	m_ListCtrl.GetText(iSelectIndex, strSelectName);

	auto iter = m_MapImage.find(strSelectName);

	if (iter == m_MapImage.end())
		return;

	UpdateData(FALSE);

}

void CDlgMap::OnBnClickedCheck1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);

	m_bSold.SetCheck(TRUE);
	m_bWireFrame.SetCheck(FALSE);
	
	Engine::CGameObject* pObject = m_pManagement->Get_Object(L"Layer_GameLogic", L"Terrain");
	dynamic_cast<CTerrain*>(pObject)->SetRenderState(false);

	UpdateData(FALSE);
}

void CDlgMap::OnBnClickedCheck2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);

	m_bSold.SetCheck(FALSE);
	m_bWireFrame.SetCheck(TRUE);

	Engine::CGameObject* pObject = m_pManagement->Get_Object(L"Layer_GameLogic", L"Terrain");
	dynamic_cast<CTerrain*>(pObject)->SetRenderState(true);

	UpdateData(FALSE);
}

// Nevi_Add
void CDlgMap::OnBnClickedButton4()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	
	if (KEY_DOWN(VK_LBUTTON))
	{
		return;
	}

	if (CheckNaviList())
	{
		return;
	}

}

// Nevi_Delete
void CDlgMap::OnBnClickedButton6()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	HTREEITEM hItem = nullptr;
	CString strRoot = L"";

	// 선택된 아이템을 찾는다
	hItem = m_NaviList.GetNextItem(NULL, TVGN_CARET);
	
	// 선택된 아이템의 부모를 찾는다
	hItem = m_NaviList.GetNextItem(hItem, TVGN_PARENT);
	
	// 해당 아이템의 텍스트를 가져온다.
	strRoot = m_NaviList.GetItemText(hItem);

	// Null 체크
	if (strRoot == L"")
	{
		return;
	}
	
	auto iter = m_vecNaviList.begin();

	m_vecNaviList.erase(iter + _ttoi(strRoot));

	Engine::CNaviMesh* pComponent = dynamic_cast<Engine::CNaviMesh*>(m_pManagement->Get_Component(L"Layer_GameLogic", L"Navi_Mesh", L"Com_NaviMesh", Engine::CComponent::ID_STATIC));
	NULL_CHECK(pComponent);

	pComponent->Set_CellList(&m_vecNaviList);
	
	pComponent->ReSet_Index();

	m_NaviList.DeleteAllItems();

	if (CheckNaviList())
	{
		return;
	}

	SetNaviCount();

	UpdateData(FALSE);

}

void CDlgMap::OnAddNaviState()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// AddNavi를 선택하였을 때
	if (1 == m_AddNavi.GetCheck())
	{
		dynamic_cast<CNavi*>(pNaviObject)->Set_NeviMode(true);
	}
	else
	{
		dynamic_cast<CNavi*>(pNaviObject)->Set_NeviMode(false);
	}
}


void CDlgMap::OnNavi_X(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if (1 > m_NaviEdit.GetCheck())
	{
		return;
	}

	CString strTemp;
	m_Edit_Navi_X.GetWindowTextW(strTemp);

	m_Navi_X = (float)_ttof(strTemp.operator LPCWSTR());

	if (pNMUpDown->iDelta < 0)
	{
		m_Navi_X = m_Navi_X + 0.1f;
	}
	else
	{
		m_Navi_X = m_Navi_X - 0.1f;
	}

	strTemp.Format(L"%.4f", m_Navi_X);

	SetDlgItemText(IDC_NAVI_X, strTemp);
	
	m_vecNaviList[m_iSelectParantIndex]->Get_Pos(Engine::CCell::POINT(m_iSelectChildIndex))->x = m_Navi_X;
	
	*pResult = 0;
}


void CDlgMap::OnNavi_Y(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if (1 > m_NaviEdit.GetCheck())
	{
		return;
	}

	CString strTemp;
	m_Edit_Navi_Y.GetWindowTextW(strTemp);

	m_Navi_Y = (float)_ttof(strTemp.operator LPCWSTR());

	if (pNMUpDown->iDelta < 0)
	{
		m_Navi_Y = m_Navi_Y + 0.1f;
	}
	else
	{
		m_Navi_Y = m_Navi_Y - 0.1f;
	}


	strTemp.Format(L"%.4f", m_Navi_Y);

	SetDlgItemText(IDC_NAVI_Y, strTemp);

	m_vecNaviList[m_iSelectParantIndex]->Get_Pos(Engine::CCell::POINT(m_iSelectChildIndex))->y = m_Navi_Y;

	*pResult = 0;
}


void CDlgMap::OnNavi_Z(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if (1 > m_NaviEdit.GetCheck())
	{
		return;
	}

	CString strTemp;
	m_Edit_Navi_Z.GetWindowTextW(strTemp);

	m_Navi_Z = (float)_ttof(strTemp.operator LPCWSTR());

	if (pNMUpDown->iDelta < 0)
	{
		m_Navi_Z = m_Navi_Z + 0.1f;
	}
	else
	{
		m_Navi_Z = m_Navi_Z - 0.1f;
	}

	strTemp.Format(L"%.4f", m_Navi_Z);

	SetDlgItemText(IDC_NAVI_Z, strTemp);

	m_vecNaviList[m_iSelectParantIndex]->Get_Pos(Engine::CCell::POINT(m_iSelectChildIndex))->z = m_Navi_Z;
	
	*pResult = 0;
}


void CDlgMap::OnNaviTreeSelected(NMHDR *pNMHDR, LRESULT *pResult)
{
	if (KEY_DOWN(VK_LBUTTON))
	{
		return;
	}

	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	HTREEITEM hItem = nullptr;
	HTREEITEM hIndex = nullptr;
	CString	strRoot = L"", strIndex = L"";

	// 선택된 아이템을 찾는다
	hItem = m_NaviList.GetNextItem(NULL, TVGN_CARET);
	
	// 선택된 아이템의 부모를 찾는다
	hIndex = m_NaviList.GetNextItem(hItem, TVGN_PARENT);

	// 해당 아이템의 텍스트를 가져온다.
	strRoot = m_NaviList.GetItemText(hIndex);
	strIndex = m_NaviList.GetItemText(hItem);
	
	m_NeighborList.ResetContent();
	
	SetNaviCount();

	Engine::CCell* pCell = nullptr;
	CString strNeighbor = L"";
	Engine::_ulong iTempIndex = 0;

	pCell = m_vecNaviList[_ttoi(strRoot)]->Get_Neighbor(Engine::CCell::NEIGHBOR_AB);

	if (pCell != nullptr)
	{
		iTempIndex = *pCell->Get_Index();

		strNeighbor.Format(L"AB [%d] [A,B]", iTempIndex);

		m_NeighborList.AddString(strNeighbor);
	}
	else
	{
		strNeighbor = L"AB NULL";

		m_NeighborList.AddString(strNeighbor);
	}

	pCell = m_vecNaviList[_ttoi(strRoot)]->Get_Neighbor(Engine::CCell::NEIGHBOR_BC);

	if (pCell != nullptr)
	{
		iTempIndex = *pCell->Get_Index();

		strNeighbor.Format(L"BC [%d] [B,C]", iTempIndex);

		m_NeighborList.AddString(strNeighbor);
	}
	else
	{
		strNeighbor = L"BC NULL";

		m_NeighborList.AddString(strNeighbor);
	}

	pCell = m_vecNaviList[_ttoi(strRoot)]->Get_Neighbor(Engine::CCell::NEIGHBOR_CA);

	if (pCell != nullptr)
	{
		iTempIndex = *pCell->Get_Index();

		strNeighbor.Format(L"CA [%d] [C,A]", iTempIndex);

		m_NeighborList.AddString(strNeighbor);
	}
	else
	{
		strNeighbor = L"CA NULL";

		m_NeighborList.AddString(strNeighbor);
	}


	// Null 체크
	if (strIndex == L"")
	{
		SetDlgItemText(IDC_NAVI_X, L"");
		SetDlgItemText(IDC_NAVI_Y, L"");
		SetDlgItemText(IDC_NAVI_Z, L"");

		return;
	}

	CString strX, strY, strZ;

	strIndex = strIndex.Mid(1, 1);
	
	m_Navi_X = m_vecNaviList[_ttoi(strRoot)]->Get_Point(Engine::CCell::POINT(_ttoi(strIndex)))->x;
	m_Navi_Y = m_vecNaviList[_ttoi(strRoot)]->Get_Point(Engine::CCell::POINT(_ttoi(strIndex)))->y;
	m_Navi_Z = m_vecNaviList[_ttoi(strRoot)]->Get_Point(Engine::CCell::POINT(_ttoi(strIndex)))->z;

	strX.Format(L"%.4f", m_Navi_X);
	strY.Format(L"%.4f", m_Navi_Y);
	strZ.Format(L"%.4f", m_Navi_Z);

	SetDlgItemText(IDC_NAVI_X, strX);
	SetDlgItemText(IDC_NAVI_Y, strY);
	SetDlgItemText(IDC_NAVI_Z, strZ);

	m_iSelectParantIndex = _ttoi(strRoot);
	m_iSelectChildIndex = _ttoi(strIndex);

	*pResult = 0;
}

void CDlgMap::TerrinSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CString strTerrianCntX, strTerrianCntZ, strTerrianCntITV, strTerrianCntDetail, strTerrainPosZ;
	int iTerrainX, iTerrainZ, iTerrainITV, iTerrainDetail;
	float fTerrainPosZ;

	GetDlgItemText(IDC_EDIT1, strTerrianCntX);
	GetDlgItemText(IDC_EDIT2, strTerrianCntZ);
	GetDlgItemText(IDC_EDIT3, strTerrianCntITV);
	GetDlgItemText(IDC_EDIT4, strTerrianCntDetail);
	GetDlgItemText(IDC_TERRAINI_Z, strTerrainPosZ);

	DWORD	dwByte = 0;

	iTerrainX		= _ttoi(strTerrianCntX);
	iTerrainZ		= _ttoi(strTerrianCntZ);
	iTerrainITV		= _ttoi(strTerrianCntITV);
	iTerrainDetail	= _ttoi(strTerrianCntDetail);
	fTerrainPosZ	= (float)_ttof(strTerrainPosZ);

	HANDLE	hFile = CreateFile(L"../../Data/Terrain.dat", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	WriteFile(hFile, &iTerrainX, sizeof(int), &dwByte, NULL);
	WriteFile(hFile, &iTerrainZ, sizeof(int), &dwByte, NULL);
	WriteFile(hFile, &iTerrainITV, sizeof(int), &dwByte, NULL);
	WriteFile(hFile, &iTerrainDetail, sizeof(int), &dwByte, NULL);
	WriteFile(hFile, &fTerrainPosZ, sizeof(float), &dwByte, NULL);

	CloseHandle(hFile);

}

void CDlgMap::TerrainLoad()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	HANDLE	hFile = CreateFile(L"../../Data/Terrain.dat", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	CString strTerrianCntX, strTerrianCntZ, strTerrianCntITV, strTerrianCntDetail, strTerrainPosZ;
	int iTerrainX, iTerrainZ, iTerrainITV, iTerrainDetail;
	float fTerrainPosZ;
	DWORD dwByte = 0;

	ReadFile(hFile, &iTerrainX, sizeof(int), &dwByte, NULL);
	ReadFile(hFile, &iTerrainZ, sizeof(int), &dwByte, NULL);
	ReadFile(hFile, &iTerrainITV, sizeof(int), &dwByte, NULL);
	ReadFile(hFile, &iTerrainDetail, sizeof(int), &dwByte, NULL);
	ReadFile(hFile, &fTerrainPosZ, sizeof(float), &dwByte, NULL);

	strTerrianCntX.Format(L"%d", iTerrainX);
	strTerrianCntZ.Format(L"%d", iTerrainZ);
	strTerrianCntITV.Format(L"%d", iTerrainITV);
	strTerrianCntDetail.Format(L"%d", iTerrainDetail);
	strTerrainPosZ.Format(L"%f", fTerrainPosZ);

	SetDlgItemText(IDC_EDIT1, strTerrianCntX);
	SetDlgItemText(IDC_EDIT2, strTerrianCntZ);
	SetDlgItemText(IDC_EDIT3, strTerrianCntITV);
	SetDlgItemText(IDC_EDIT4, strTerrianCntDetail);
	SetDlgItemText(IDC_TERRAINI_Z, strTerrainPosZ);

	CloseHandle(hFile);

	OnBnClickedButton1();
}


void CDlgMap::Navi_Save()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	HANDLE	hFile = CreateFile(L"../../Data/Navi.dat", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	DWORD	dwByte = 0;
	D3DXVECTOR3 vecTem;

	for(auto iter : m_vecNaviList)
	{
		vecTem = D3DXVECTOR3(iter->Get_Point(Engine::CCell::POINT_A)->x,
							iter->Get_Point(Engine::CCell::POINT_A)->y,
							iter->Get_Point(Engine::CCell::POINT_A)->z);

		WriteFile(hFile, &vecTem, sizeof(D3DXVECTOR3), &dwByte, NULL);

		vecTem = D3DXVECTOR3(iter->Get_Point(Engine::CCell::POINT_B)->x,
							iter->Get_Point(Engine::CCell::POINT_B)->y,
							iter->Get_Point(Engine::CCell::POINT_B)->z);

		WriteFile(hFile, &vecTem, sizeof(D3DXVECTOR3), &dwByte, NULL);

		vecTem = D3DXVECTOR3(iter->Get_Point(Engine::CCell::POINT_C)->x,
							iter->Get_Point(Engine::CCell::POINT_C)->y,
							iter->Get_Point(Engine::CCell::POINT_C)->z);

		WriteFile(hFile, &vecTem, sizeof(D3DXVECTOR3), &dwByte, NULL);
	}

	SetNaviCount();

	CloseHandle(hFile);
}


void CDlgMap::Navi_Load()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	HANDLE	hFile = CreateFile(L"../../Data/Navi.dat", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	DWORD	dwByte = 99;
	int		iNavi_index = 0;
	D3DXVECTOR3 vecPosA, vecPosB, vecPosC;
	CString strIndex, strPos;

	m_NaviList.DeleteAllItems();

	pNaviComponent->Clear_List();

	while (true)
	{

		ReadFile(hFile, &vecPosA, sizeof(D3DXVECTOR3), &dwByte, NULL);

		if (dwByte == 0)
		{
			break;
		}
		
		ReadFile(hFile, &vecPosB, sizeof(D3DXVECTOR3), &dwByte, NULL);
		ReadFile(hFile, &vecPosC, sizeof(D3DXVECTOR3), &dwByte, NULL);
		
		pNaviComponent->Add_NeviMesh(&vecPosA, &vecPosB, &vecPosC);

		strIndex.Format(_T("%d"), iNavi_index);
		HTREEITEM NaviTop = m_NaviList.InsertItem((LPCTSTR)strIndex, TVI_ROOT, TVI_LAST);

		strPos.Format(_T("_%d"), 0);
		m_NaviList.InsertItem((LPCTSTR)strPos, NaviTop, TVI_LAST);

		strPos.Format(_T("_%d"), 1);
		m_NaviList.InsertItem((LPCTSTR)strPos, NaviTop, TVI_LAST);

		strPos.Format(_T("_%d"), 2);
		m_NaviList.InsertItem((LPCTSTR)strPos, NaviTop, TVI_LAST);
	}

	CheckNaviList();

	CloseHandle(hFile);
}

// 인접한 점들의 Y값을 일괄적으로 수정해주는 함수
void CDlgMap::OnReFresh()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CString strEditVale, strFindIndex, strPosX, strPosY, strPosZ;
	int iIndex = 0;

	GetDlgItemText(IDC_EDIT11, strEditVale);
	
	if (0.f == (float)_ttof(strEditVale))
	{
		return;
	}

	iIndex = m_FindList.GetCount();

	if (iIndex < 0)
	{
		return;
	}

	GetDlgItemText(IDC_EDIT10, strPosX);
	GetDlgItemText(IDC_EDIT8, strPosY);
	GetDlgItemText(IDC_EDIT7, strPosZ);

	D3DXVECTOR3 vCompare((float)_ttof(strPosX), (float)_ttof(strPosY), (float)_ttof(strPosZ));

	for (int i = 0; i < iIndex; ++i)
	{
		m_FindList.GetText(i, strFindIndex);

		if (vCompare.x == m_vecNaviList[_ttoi(strFindIndex)]->Get_Point(Engine::CCell::POINT_A)->x &&
			vCompare.z == m_vecNaviList[_ttoi(strFindIndex)]->Get_Point(Engine::CCell::POINT_A)->z)
		{
			m_vecNaviList[_ttoi(strFindIndex)]->Get_Pos(Engine::CCell::POINT_A)->y = (float)_ttof(strEditVale);
		}

		if (vCompare.x == m_vecNaviList[_ttoi(strFindIndex)]->Get_Point(Engine::CCell::POINT_B)->x &&
			vCompare.z == m_vecNaviList[_ttoi(strFindIndex)]->Get_Point(Engine::CCell::POINT_B)->z)
		{
			m_vecNaviList[_ttoi(strFindIndex)]->Get_Pos(Engine::CCell::POINT_B)->y = (float)_ttof(strEditVale);
		}

		if (vCompare.x == m_vecNaviList[_ttoi(strFindIndex)]->Get_Point(Engine::CCell::POINT_C)->x &&
			vCompare.z == m_vecNaviList[_ttoi(strFindIndex)]->Get_Point(Engine::CCell::POINT_C)->z)
		{
			m_vecNaviList[_ttoi(strFindIndex)]->Get_Pos(Engine::CCell::POINT_C)->y = (float)_ttof(strEditVale);
		}
	}

}

BOOL CDlgMap::PreTranslateMessage(MSG* pMsg)
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

void CDlgMap::Find_Index()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CString strPos_1_Value;

	GetDlgItemText(IDC_EDIT10, strPos_1_Value);

	if (strPos_1_Value.IsEmpty())
	{
		return;
	}

	CString strTemp;
	
	GetDlgItemText(IDC_EDIT5, strTemp);

	if (strTemp.IsEmpty())
	{
		return;
	}

	m_FindList.ResetContent();

	CString strTem2;
	D3DXVECTOR3 vTempPos;
	int iFind_Index;
	AfxExtractSubString(strTem2, strTemp, 0, ',');

	strTem2.TrimLeft(L"X:");
	
	vTempPos.x = (float)_ttof(strTem2);

	AfxExtractSubString(strTem2, strTemp, 1, ',');

	strTem2.Trim();
	strTem2.TrimLeft(L"Y:");

	vTempPos.y = (float)_ttof(strTem2);

	AfxExtractSubString(strTem2, strTemp, 2, ',');

	strTem2.Trim();
	strTem2.TrimLeft(L"Z:");

	vTempPos.z = (float)_ttof(strTem2);

	for (auto iter : m_vecNaviList)
	{
		if (vTempPos == *iter->Get_Pos(Engine::CCell::POINT_A))
		{
			iFind_Index = *iter->Get_Index();

			strTem2.Format(L"%d", iFind_Index);

			m_FindList.AddString(strTem2);
		}

		if (vTempPos == *iter->Get_Pos(Engine::CCell::POINT_B))
		{
			iFind_Index = *iter->Get_Index();

			strTem2.Format(L"%d", iFind_Index);

			m_FindList.AddString(strTem2);
		}

		if (vTempPos == *iter->Get_Pos(Engine::CCell::POINT_C))
		{
			iFind_Index = *iter->Get_Index();

			strTem2.Format(L"%d", iFind_Index);

			m_FindList.AddString(strTem2);
		}
	}

	strTem2.Format(L"%d", m_FindList.GetCount());

	SetDlgItemText(IDC_EDIT13, strTem2);
}

void CDlgMap::TerrainPos_X(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if (pNMUpDown->iDelta < 0)
	{
		m_TerrainPos_X = m_TerrainPos_X + 0.1f;
	}
	else
	{
		m_TerrainPos_X = m_TerrainPos_X - 0.1f;
	}

	Engine::CTransform* pComponent = dynamic_cast<Engine::CTransform*>(m_pManagement->Get_Component(L"Layer_GameLogic", L"Terrain", L"Com_Transform", Engine::CComponent::COMPONENTID::ID_DYNAMIC));
	NULL_CHECK(pComponent);

	pComponent->m_vInfo[Engine::CTransform::INFO_POS].x = m_TerrainPos_X;

	CString strTem;

	strTem.Format(L"%f", m_TerrainPos_X);
	SetDlgItemText(IDC_TERRAINI_X, strTem);

	*pResult = 0;
}


void CDlgMap::TerrainPos_Z(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if (pNMUpDown->iDelta < 0)
	{
		m_TerrainPos_Z = m_TerrainPos_Z + 0.1f;
	}
	else
	{
		m_TerrainPos_Z = m_TerrainPos_Z - 0.1f;
	}


	Engine::CTransform* pComponent = dynamic_cast<Engine::CTransform*>(m_pManagement->Get_Component(L"Layer_GameLogic", L"Terrain", L"Com_Transform", Engine::CComponent::COMPONENTID::ID_DYNAMIC));
	NULL_CHECK(pComponent);

	pComponent->m_vInfo[Engine::CTransform::INFO_POS].z = m_TerrainPos_Z;

	CString strTem;

	strTem.Format(L"%f", m_TerrainPos_Z);
	SetDlgItemText(IDC_TERRAINI_Z, strTem);

	*pResult = 0;
}

void CDlgMap::Edit_Compore_Point(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CString strCheckValue;

	GetDlgItemText(IDC_EDIT10, strCheckValue);

	if (strCheckValue.IsEmpty())
	{
		return;
	}

	if (pNMUpDown->iDelta < 0)
	{
		m_fHeight += 0.1f;
	}
	else
	{
		m_fHeight -= 0.1f;
	}

	CString strSetValue;

	strSetValue.Format(L"%f", m_fHeight);
	SetDlgItemText(IDC_EDIT11, strSetValue);

	*pResult = 0;
}

void CDlgMap::Get_Pos_1_Value()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CString strPos_1_Value;

	GetDlgItemText(IDC_EDIT5, strPos_1_Value);

	if (strPos_1_Value.IsEmpty())
	{
		return;
	}	
	
	CString strValue;

	AfxExtractSubString(strValue, strPos_1_Value, 0, ',');

	strValue.TrimLeft(L"X:");
	SetDlgItemText(IDC_EDIT10, strValue);

	AfxExtractSubString(strValue, strPos_1_Value, 1, ',');

	strValue.Trim();
	strValue.TrimLeft(L"Y:");
	SetDlgItemText(IDC_EDIT8, strValue);

	AfxExtractSubString(strValue, strPos_1_Value, 2, ',');

	strValue.Trim();
	strValue.TrimLeft(L"Z:");
	SetDlgItemText(IDC_EDIT7, strValue);
}


void CDlgMap::Link_Celling()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	Engine::CComponent* pComponent = Engine::Get_Component(L"Layer_GameLogic", L"Navi_Mesh", L"Com_NaviMesh", Engine::CComponent::ID_STATIC);

	NULL_CHECK(pComponent);

	dynamic_cast<Engine::CNaviMesh*>(pComponent)->Link_CellList();
}


void CDlgMap::Find_Y()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	m_FindYList.ResetContent();

	CString strFindY, strFindIndex;

	GetDlgItemText(IDC_NAVI_Y2, strFindY);

	if (strFindY.IsEmpty())
	{
		return;
	}

	m_fFindY = (float)_ttof(strFindY);

	for (auto& iter : m_vecNaviList)
	{
		if (iter->Get_Point(Engine::CCell::POINT_A)->y == m_fFindY)
		{
			strFindIndex.Format(L"%d", *iter->Get_Index());

			m_FindYList.InsertString(-1, strFindIndex);
			continue;
		}

		if (iter->Get_Point(Engine::CCell::POINT_B)->y == m_fFindY)
		{
			strFindIndex.Format(L"%d", *iter->Get_Index());

			m_FindYList.InsertString(-1, strFindIndex);
			continue;
		}

		if (iter->Get_Point(Engine::CCell::POINT_C)->y == m_fFindY)
		{
			strFindIndex.Format(L"%d", *iter->Get_Index());

			m_FindYList.InsertString(-1, strFindIndex);
			continue;
		}
	}

}


void CDlgMap::FindY_Value(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}


void CDlgMap::Apply_Y_Value()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	int iCount = m_FindYList.GetCount();

	if (iCount < 0)
	{
		return;
	}


	CString strIndex, strFindY, strSetUp_Y;
	float fFindY;

	GetDlgItemText(IDC_NAVI_Y2, strFindY);

	fFindY = (float)_ttof(strFindY);

	GetDlgItemText(IDC_EDIT22, strSetUp_Y);

	for (int i = 0; i < iCount; ++i)
	{
		m_FindYList.GetText(i, strIndex);
		
		if (m_vecNaviList[_ttoi(strIndex)]->Get_Pos(Engine::CCell::POINT_A)->y == fFindY)
		{
			m_vecNaviList[_ttoi(strIndex)]->Set_Point_Y(Engine::CCell::POINT_A, (float)_ttof(strSetUp_Y));
		}

		if (m_vecNaviList[_ttoi(strIndex)]->Get_Pos(Engine::CCell::POINT_B)->y == fFindY)
		{
			m_vecNaviList[_ttoi(strIndex)]->Set_Point_Y(Engine::CCell::POINT_B, (float)_ttof(strSetUp_Y));
		}

		if (m_vecNaviList[_ttoi(strIndex)]->Get_Pos(Engine::CCell::POINT_C)->y == fFindY)
		{
			m_vecNaviList[_ttoi(strIndex)]->Set_Point_Y(Engine::CCell::POINT_C, (float)_ttof(strSetUp_Y));
		}

	}
}


void CDlgMap::Delete_Index()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CString strDeleteIndex;

	GetDlgItemText(IDC_NAVI_Y3, strDeleteIndex);
	
	if (strDeleteIndex.IsEmpty())
	{
		return;
	}

	m_vecNaviList.erase(m_vecNaviList.begin() + _ttoi(strDeleteIndex));
	
	Engine::CComponent* pComponent = Engine::Get_Component(L"Layer_GameLogic", L"Navi_Mesh", L"Com_NaviMesh", Engine::CComponent::ID_STATIC);

	NULL_CHECK(pComponent);

	dynamic_cast<Engine::CNaviMesh*>(pComponent)->Set_CellList(&m_vecNaviList);

	dynamic_cast<Engine::CNaviMesh*>(pComponent)->ReSet_Index();

	CheckNaviList();
}
