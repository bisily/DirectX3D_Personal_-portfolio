
// MyMap_ToolView.h : CMyMap_ToolView 클래스의 인터페이스
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
protected: // serialization에서만 만들어집니다.
	CMyMap_ToolView();
	DECLARE_DYNCREATE(CMyMap_ToolView)

// 특성입니다.
public:
	CMyMap_ToolDoc* GetDocument() const;

// 작업입니다.
public:
	HRESULT						SetUp_DefaultSetting(LPDIRECT3DDEVICE9* ppGraphicDev);
	HRESULT						Ready_ResourceForStatic(LPDIRECT3DDEVICE9 pGraphicDev, RESOURCEID eMax);
	HRESULT						Ready_OpeningScene(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CManagement** ppManagementInst);
	HRESULT						Ready_Resources();
	HRESULT						Load_StaticMesh();

public:
	LPDIRECT3DDEVICE9			Get_GraphicDev() { return m_pGraphicDev; }

// 재정의입니다.
public:
	virtual void				OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL				PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL				OnPreparePrinting(CPrintInfo* pInfo);
	virtual void				OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void				OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
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

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnDestroy();

};

#ifndef _DEBUG  // MyMap_ToolView.cpp의 디버그 버전
inline CMyMap_ToolDoc* CMyMap_ToolView::GetDocument() const
   { return reinterpret_cast<CMyMap_ToolDoc*>(m_pDocument); }
#endif

