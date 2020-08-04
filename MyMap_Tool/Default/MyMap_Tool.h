
// MyMap_Tool.h : MyMap_Tool ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.

#include "Engine_Defines.h"

namespace Engine
{
	class CTimeMgr;
}

// CMyMap_ToolApp:
// �� Ŭ������ ������ ���ؼ��� MyMap_Tool.cpp�� �����Ͻʽÿ�.
//

class CMyMap_ToolView;
class CMyMap_ToolApp : public CWinAppEx
{
public:
	CMyMap_ToolApp();


// �������Դϴ�.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// �����Դϴ�.
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	
	CMyMap_ToolView*			m_pView = nullptr;
	Engine::CTimeMgr*			m_pTimeMgr = nullptr;
	virtual BOOL OnIdle(LONG lCount);
};

