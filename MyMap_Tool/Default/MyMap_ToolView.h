
// MyMap_ToolView.h : CMyMap_ToolView Ŭ������ �������̽�
//

#pragma once

#include "Include.h"

namespace Engine
{
	class CGraphicDev;
	class CManagement;
	class CTimeMgr;
}

class CMyMap_ToolDoc;
class CMyMap_ToolView : public CView
{
protected: // serialization������ ��������ϴ�.
	CMyMap_ToolView();
	DECLARE_DYNCREATE(CMyMap_ToolView)

// Ư���Դϴ�.
public:
	CMyMap_ToolDoc* GetDocument() const;

// �۾��Դϴ�.
public:
	HRESULT						SetUp_DefaultSetting(LPDIRECT3DDEVICE9* ppGraphicDev);
	HRESULT						Ready_ResourceForStatic(LPDIRECT3DDEVICE9 pGraphicDev, RESOURCEID eMax);
	HRESULT						Ready_OpeningScene(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CManagement** ppManagementInst);
	HRESULT						Ready_Resources();
	HRESULT						Load_StaticMesh();

public:
	LPDIRECT3DDEVICE9			Get_GraphicDev() { return m_pGraphicDev; }

// �������Դϴ�.
public:
	virtual void				OnDraw(CDC* pDC);  // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual BOOL				PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL				OnPreparePrinting(CPrintInfo* pInfo);
	virtual void				OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void				OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// �����Դϴ�.
public:
	virtual ~CMyMap_ToolView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

private:
	Engine::CGraphicDev*		m_pGraphicInstance = nullptr;
	Engine::CManagement*		m_pManagement = nullptr;
	Engine::CTimeMgr*			m_pTimeMgr = nullptr;
	LPDIRECT3DDEVICE9			m_pGraphicDev = nullptr;

	Engine::_ulong				m_dwRenderCnt = 0;
	TCHAR						m_szFPS[128] = L"";
	Engine::_float				m_fTime = 0.f;

protected:

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnDestroy();

};

#ifndef _DEBUG  // MyMap_ToolView.cpp�� ����� ����
inline CMyMap_ToolDoc* CMyMap_ToolView::GetDocument() const
   { return reinterpret_cast<CMyMap_ToolDoc*>(m_pDocument); }
#endif

