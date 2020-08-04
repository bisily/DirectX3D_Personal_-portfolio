#pragma once

#include "Include.h"
#include "afxwin.h"
#include "afxcmn.h"

// CDlgObject 대화 상자입니다.

namespace Engine
{
	class CManagement;
	class CComponent;
	class CGameObject;
	class CTransform;
}

class CDlgObject : public CDialog
{
	DECLARE_DYNAMIC(CDlgObject)

public:
	CDlgObject(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgObject();

public:
	bool				CheckAvailableNumber(int iID);
	bool				CheckEdit(CString _strSelectObjKind);
	void				SelectObjKind(CString* pStrSelect);
	Engine::CComponent*	GetPickComponent(CString _strSelectObjKind);
	void				CreateStaticObject(int _Start_Point, wstring _tagName, Engine::_vec3* _vPos, Engine::_vec3* _vScale);
	void				CreateDynamicObject(int _Start_Point, wstring _tagName, Engine::_vec3* _vPos, Engine::_vec3* _vScale);
	void				LoadCreateStaicObject(CString MeshName,	
												Engine::_vec3* temp_Right, 
												Engine::_vec3* temp_Up,	
												Engine::_vec3* temp_Look, 
												Engine::_vec3* temp_Pos, 
												Engine::_vec3* temp_Scale, 
												Engine::_vec3* temp_Angle, 
												int iObjIndex);
	void				LoadCreateDynamicObject(CString MeshName, 
												Engine::_vec3* temp_Right, 
												Engine::_vec3* temp_Up, 
												Engine::_vec3* temp_Look, 
												Engine::_vec3* temp_Pos, 
												Engine::_vec3* temp_Scale, 
												Engine::_vec3* temp_Angle, 
												int iObjIndex);
	void				Set_Transform(Engine::CTransform* pTransform);
	Engine::CComponent* Get_Transform(CString strCompare);
	void				LoadObjCnt(CString pType);

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG3 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLbnSelchangeList1();
	afx_msg void OnLbnSelchangeList2();
	afx_msg void StaticMesh_Save();
	afx_msg void StaticMesh_Load();
	afx_msg void DynamicMesh_Save();
	afx_msg void DynamicMesh_Load();
	afx_msg void Delete_StaticObj();
	afx_msg void Delete_DynamicObj();
	afx_msg void Set_ViewState();

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();

	Engine::CComponent*		m_pComponent = nullptr;
	Engine::CGameObject*	m_pGameObject = nullptr;
	Engine::CManagement*	m_pManagement = nullptr;

	CString		strSelectObjKind;
	CString		strTemp;
	CListBox	m_AddStaticList;
	CListBox	m_AddDynamicList;
	CTreeCtrl	m_TreeCtrl;
	CEdit		m_PositionX;
	CEdit		m_PositionY;
	CEdit		m_PositionZ;
	CEdit		m_RotationX;
	CEdit		m_RotationY;
	CEdit		m_RotationZ;
	CEdit		m_ScaleX;
	CEdit		m_ScaleY;
	CEdit		m_ScaleZ;
	CEdit		m_SelectObject;
	CEdit		m_CreatePosX;
	CEdit		m_CreatePosY;
	CEdit		m_CreatePosZ;
	CEdit		m_CreateScaleX;
	CEdit		m_CreateScaleY;
	CEdit		m_CreateScaleZ;
	CEdit		m_PickObject;
	CEdit		m_PickDynamicObject;
	CButton		m_Transform_Selected[2];
	CButton		m_SetViewState;
	CEdit		m_DynamicObjectCnt;

	int			m_StaticObj_Cnt;
	int			m_DynamicObj_Cnt;
};
