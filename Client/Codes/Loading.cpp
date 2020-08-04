#include "stdafx.h"
#include "Loading.h"
#include "Export_Function.h"


CLoading::CLoading(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
{
	m_pGraphicDev->AddRef();
}

CLoading::~CLoading()
{
	
}

unsigned int CALLBACK CLoading::Thread_Main(void* pArg)
{
	CLoading*		pLoading = (CLoading*)pArg;

	_uint  iFlag = 0;

	EnterCriticalSection(pLoading->Get_CriticalSection());

	switch(pLoading->Get_LoadingID())
	{
	case LOADING_STAGE:
		iFlag = pLoading->Loading_ForStage();
		break;
	}

	LeaveCriticalSection(pLoading->Get_CriticalSection());
	
	return iFlag;
}

HRESULT CLoading::Ready_Loading(LOADINGID eLoadingID)
{
	InitializeCriticalSection(&m_Crt);

	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, Thread_Main, this, 0, nullptr);

	m_eLoadingID = eLoadingID;

	return S_OK;
}

_uint CLoading::Loading_ForTexture()
{
	wifstream	LoadFile;

	LoadFile.open(L"../../Data/TextureList.txt", ios::in);

	Engine::_tchar	szTextrueTag[MIN_STR] = L"";
	Engine::_tchar	szTextruePath[MIN_STR] = L"";
	Engine::_tchar	szTextrueCnt[MIN_STR] = L"";

	wstring strTextrueTag;
	wstring strTextruePath;

	while (!LoadFile.eof())
	{
		LoadFile.getline(szTextrueTag, MIN_STR, '|');
		strTextrueTag = szTextrueTag;

		LoadFile.getline(szTextruePath, MIN_STR, '|');
		strTextruePath = szTextruePath;

		LoadFile.getline(szTextrueCnt, MIN_STR);

		if (strTextrueTag.empty())
		{
			break;
		}

		// 텍스쳐 로딩
		if (FAILED(Engine::Ready_Textures(m_pGraphicDev,
			RESOURCE_STAGE,
			strTextrueTag,
			Engine::CTexture::TYPE_NORMAL,
			strTextruePath,
			_ttoi(szTextrueCnt))))
		{
			ERR_BOX(L"Terrain_Texture, Loading Failed");
			return -1;
		}

	}

	LoadFile.close();

	return 0;
}

_uint CLoading::Loading_ForStage()
{

	lstrcpy(m_szLoadingString, L"Texture Loading");
	if (0 != Loading_ForTexture())
	{
		ERR_BOX(L"Texture Text, Loading Failed");
	}

	lstrcpy(m_szLoadingString, L"File Loading");
	if (0 != Load_From_TextFile())
	{
		ERR_BOX(L"TextFile, Loading Failed");
	}

	// Mesh Loading

	lstrcpy(m_szLoadingString, L"Mesh Loading");

	// Navi
	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev, RESOURCE_STAGE,
		L"Mesh_Navi",
		Engine::CMesh::TYPE_NAVI,
		L"", L"", 1)))
	{
		ERR_BOX(L"Mesh_Navi, Loading Failed");
		return -1;
	}

	m_bFinish = true;
	lstrcpy(m_szLoadingString, L"Loading Complete");

	return 0;
}

Engine::_uint CLoading::Load_From_TextFile()
{
	lstrcpy(m_szLoadingString, L"Static Mesh Loading");
	// StaticMesh File
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

			ERR_BOX(szMeshTag);
			return E_FAIL;
		}
	}

	LoadFile.close();
	
	lstrcpy(m_szLoadingString, L"Dynamic Mesh Loading");
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

			ERR_BOX(szMeshTag);
			return E_FAIL;
		}
	}

	LoadFile.close();

	return 0;
}

CLoading* CLoading::Create(LPDIRECT3DDEVICE9 pGraphicDev, LOADINGID eLoadingID)
{
	CLoading*	pInstance = new CLoading(pGraphicDev);

	if(FAILED(pInstance->Ready_Loading(eLoadingID)))
	{
		ERR_BOX(L"Loading Created Failed");
		Engine::Safe_Release(pInstance);
	}

	return pInstance;
}

void CLoading::Free()
{
	WaitForSingleObject(m_hThread, INFINITE);
	CloseHandle(m_hThread);
	DeleteCriticalSection(&m_Crt);

	Engine::Safe_Release(m_pGraphicDev);
}

