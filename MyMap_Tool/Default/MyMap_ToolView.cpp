
// MyMap_ToolView.cpp : CMyMap_ToolView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "MyMap_Tool.h"
#endif

#include "Export_Function.h"

#include "MyMap_ToolDoc.h"
#include "MyMap_ToolView.h"
#include "Tool_Stage.h"

#include "KeyMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

HWND g_hWnd;
HINSTANCE g_hInst;

// CMyMap_ToolView

IMPLEMENT_DYNCREATE(CMyMap_ToolView, CView)

BEGIN_MESSAGE_MAP(CMyMap_ToolView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_DESTROY()
END_MESSAGE_MAP()

// CMyMap_ToolView 생성/소멸

CMyMap_ToolView::CMyMap_ToolView()
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CMyMap_ToolView::~CMyMap_ToolView()
{
	CKeyMgr::GetInstance()->DestroyInstance();

	Engine::Safe_Release(m_pManagement);
	Engine::Safe_Release(m_pGraphicInstance);

	Engine::Release_Utility();
	Engine::Release_Resources();
	Engine::Release_System();
}

BOOL CMyMap_ToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CMyMap_ToolView 그리기
void CMyMap_ToolView::OnDraw(CDC* /*pDC*/)
{
	CMyMap_ToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.

	++m_dwRenderCnt;

	if (m_fTime >= 1.f)
	{
		wsprintf(m_szFPS, L"FPS : %d", m_dwRenderCnt);
		m_fTime = 0.f;
		m_dwRenderCnt = 0;
	}

	Engine::Set_TimeDelta(L"Timer_Immediate");

	float fTimeImmediate = Engine::Get_TimeDelta(L"Timer_Immediate");

	if (Engine::isPermit_Call(L"Frame_100", fTimeImmediate))
	{
	
		Engine::Set_TimeDelta(L"Timer_FPS60");

		float	fTimeDelta60 = Engine::Get_TimeDelta(L"Timer_FPS60");
		
		Engine::SetUp_InputState();

		m_pGraphicInstance->Render_Begin(D3DXCOLOR(0.f, 0.f, 1.f, 1.f));

		m_pManagement->Update_Scene(fTimeDelta60);
		m_pManagement->Render_ToolScene(m_pGraphicDev);

		Engine::Render_Font(L"Font_Default", m_szFPS, &Engine::_vec2(100.f, 0.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
		
		CKeyMgr::GetInstance()->Update();

		m_pGraphicInstance->Render_End();
	}

	Invalidate(FALSE);

}


// CMyMap_ToolView 인쇄

BOOL CMyMap_ToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CMyMap_ToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CMyMap_ToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CMyMap_ToolView 진단

#ifdef _DEBUG
void CMyMap_ToolView::AssertValid() const
{
	CView::AssertValid();
}

void CMyMap_ToolView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMyMap_ToolDoc* CMyMap_ToolView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMyMap_ToolDoc)));
	return (CMyMap_ToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CMyMap_ToolView 메시지 처리기


void CMyMap_ToolView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	g_hWnd = m_hWnd;
	g_hInst = AfxGetInstanceHandle();

	RECT	rcWindow;

	CMyMap_ToolView* MainFrame = (CMyMap_ToolView*)AfxGetMainWnd();

	MainFrame->GetWindowRect(&rcWindow); // 윈도우 창 테두리의 사이즈를 얻어오는 함수
	SetRect(&rcWindow,
		0,
		0,
		rcWindow.right - rcWindow.left,
		rcWindow.bottom - rcWindow.top);


	RECT	rcMainView;
	GetClientRect(&rcMainView);		// 순수 뷰 창의 크기를 얻어오는 함수

	float	fRowFrm = float(rcWindow.right - rcMainView.right);
	float	fColFrm = float(rcWindow.bottom - rcMainView.bottom);

	MainFrame->SetWindowPos(NULL,
		100,
		100,
		int(WINCX + fRowFrm),
		int(WINCY + fColFrm),
		SWP_NOZORDER);

	if (FAILED(Engine::Ready_Timer(L"Timer_Immediate")))
	{
		MessageBox(L"Ready_Timer Immediate, Create Failed");
		return;
	}
	
	if (FAILED(Engine::Ready_Timer(L"Timer_FPS60")))
	{
		MessageBox(L"Ready_Timer FPS60, Create Failed");
		return;
	}

	if (FAILED(Engine::Ready_Frame(L"Frame_100", 50.f)))
	{
		MessageBox(L"Ready_Timer Frame_100, Create Failed");
		return;
	}

	if (FAILED(SetUp_DefaultSetting(&m_pGraphicDev)))
	{
		MessageBox(L"Device, Create Failed");
		return;
	}

	if (FAILED(Ready_ResourceForStatic(m_pGraphicDev, RESOURCE_END)))
	{
		MessageBox(L"Resource For Static, Create Failed");
		return;
	}

	if (FAILED(Ready_Resources()))
	{
		MessageBox(L"Readources, Create Failed");
		return;
	}

	if (FAILED(Ready_OpeningScene(m_pGraphicDev, &m_pManagement)))
	{
		MessageBox(L"OpeningScene, Create Failed");
		return;
	}

	Engine::Safe_Release(m_pGraphicDev);
}

HRESULT CMyMap_ToolView::SetUp_DefaultSetting(LPDIRECT3DDEVICE9 * ppGraphicDev)
{
	if (FAILED(Engine::Ready_GraphicDev(g_hWnd,
		Engine::CGraphicDev::MODE_WIN,
		WINCX, WINCY, &m_pGraphicInstance)))
	{
		AfxMessageBox(L"Failed GraphicInstance");
		return E_FAIL;
	}

	m_pGraphicInstance->AddRef();

	(*ppGraphicDev) = Engine::GetDevice();
	(*ppGraphicDev)->AddRef();

	(*ppGraphicDev)->SetRenderState(D3DRS_LIGHTING, TRUE);
	(*ppGraphicDev)->SetRenderState(D3DRS_ZENABLE, TRUE);		// Z버퍼에 무조건 기록은 하되 SORTING은 수행할 지 말지 판단,  TRUE인 경우 정렬을 하겠다는 의미
	(*ppGraphicDev)->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);  // Z버퍼에 픽셀의 깊이 값을 저장하겠다는 의미

	if (FAILED(Engine::Ready_InputDevice(g_hInst, g_hWnd)))
		return E_FAIL;

	// FontMgr
	if (FAILED(Engine::Ready_Font(m_pGraphicDev, L"Font_Default", L"바탕", 15, 20, FW_HEAVY)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMyMap_ToolView::Ready_ResourceForStatic(LPDIRECT3DDEVICE9 pGraphicDev, RESOURCEID eMax)
{
	if (FAILED(Engine::Reserve_ContainerSize(eMax)))
	{
		MessageBox(L"Reserve_ContainerSize, Create Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Buffers(m_pGraphicDev,
		RESOURCE_STATIC,
		L"Buffer_RcCol",
		Engine::CVIBuffer::BUFFER_RCCOL, 
		0)))
	{
		MessageBox(L"Buffer_RcCol, Create Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Buffers(m_pGraphicDev,
		RESOURCE_STATIC,
		L"Buffer_RcTex",
		Engine::CVIBuffer::BUFFER_RCTEX,
		0)))
	{
		MessageBox(L"Buffer_RcTex, Create Failed");
		return E_FAIL;
	}

	// SKY BOX 버퍼
	if (FAILED(Engine::Ready_Buffers(m_pGraphicDev,
		RESOURCE_STATIC,
		L"Buffer_CubeTex",
		Engine::CVIBuffer::BUFFER_CUBETEX,
		0)))
	{
		MessageBox(L"Buffer_CubeTex, Create Failed");
		return E_FAIL;
	}

	// 정점 로딩
	if (FAILED(Engine::Ready_Buffers(m_pGraphicDev,
		RESOURCE_STATIC,
		L"Buffer_Terrain",
		Engine::CVIBuffer::BUFFER_TERRAINTEX,
		0,
		129,
		129,
		1)))
	{
		MessageBox(L"Buffer_Terrain, Create Failed");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CMyMap_ToolView::Ready_OpeningScene(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CManagement ** ppManagementInst)
{
	Engine::CScene*			pOpeningScene = nullptr;

	pOpeningScene = CTool_Stage::Create(pGraphicDev);
	NULL_CHECK_RETURN(pOpeningScene, E_FAIL);

	if (FAILED(Engine::Create_Management(m_pGraphicDev, ppManagementInst)))
	{
		MessageBox(L"Create_Management, Create Failed");
		return E_FAIL;
	}

	(*ppManagementInst)->AddRef();

	if (FAILED((*ppManagementInst)->SetUp_CurrentScene(pOpeningScene)))
	{
		MessageBox(L"SetUp_CurrentScene, Create Failed");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CMyMap_ToolView::Ready_Resources()
{
	// 텍스쳐 로딩
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Texture_Terrain",
		Engine::CTexture::TYPE_NORMAL,
		L"../../Resources/Texture/Terrain/Ground.tga", 1)))
	{
		MessageBox(L"Texture_Terrain, Create Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev, 
		RESOURCE_STAGE,
		L"Mesh_Navi",
		Engine::CMesh::TYPE_NAVI,
		L"", 
		L"")))
	{
		MessageBox(L"Mesh_Navi, Loading Failed");
		return E_FAIL;
	}
	
	if (FAILED(Load_StaticMesh()))
	{
		MessageBox(L"Static_Mesh, CreateFiled");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CMyMap_ToolView::Load_StaticMesh()
{
	wifstream	LoadFile;

	LoadFile.open(L"../../Data/StaticMeshList.txt", ios::in);

	Engine::_tchar	szMeshTag[MIN_STR] = L"";
	Engine::_tchar	szMeshPath[MIN_STR] = L"";
	Engine::_tchar	szMeshName[MIN_STR] = L"";
	
	wstring strMeshTag;
	wstring strMeshPath;
	wstring strMeshName;

	while (!LoadFile.eof())
	{
		LoadFile.getline(szMeshTag, MIN_STR, '|');
		strMeshTag = szMeshTag;

		LoadFile.getline(szMeshPath, MIN_STR, '|');
		strMeshPath = szMeshPath;

		LoadFile.getline(szMeshName, MIN_STR);
		strMeshName = szMeshName;

		if (strMeshName.empty())
		{
			break;
		}

		if (FAILED(Engine::Ready_Mesh(m_pGraphicDev, 
					RESOURCE_STAGE,
					strMeshTag,
					Engine::CMesh::TYPE_STATIC,
					strMeshPath,
					strMeshName)))
		{
			lstrcat(szMeshTag, L" Create Failed");

			MessageBox(szMeshTag);
			return E_FAIL;
		}
	}

	LoadFile.close();

	LoadFile.open(L"../../Data/DynamicMeshList.txt", ios::in);

	strMeshTag = L"";
	strMeshPath = L"";
	strMeshName = L"";

	while (!LoadFile.eof())
	{
		LoadFile.getline(szMeshTag, MIN_STR, '|');
		strMeshTag = szMeshTag;

		LoadFile.getline(szMeshPath, MIN_STR, '|');
		strMeshPath = szMeshPath;

		LoadFile.getline(szMeshName, MIN_STR);
		strMeshName = szMeshName;

		if (strMeshName.empty())
		{
			break;
		}

		if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
			RESOURCE_STAGE,
			strMeshTag,
			Engine::CMesh::TYPE_DYNAMIC,
			strMeshPath,
			strMeshName)))
		{
			lstrcat(szMeshTag, L" Create Failed");

			MessageBox(szMeshTag);
			return E_FAIL;
		}
	}

	LoadFile.close();

	return S_OK;
}

void CMyMap_ToolView::OnDestroy()
{
	CView::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	
}
