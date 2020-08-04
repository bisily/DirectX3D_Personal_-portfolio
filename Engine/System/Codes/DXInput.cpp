#include "DXInput.h"

USING(Engine)
IMPLEMENT_SINGLETON(CDXInput)

Engine::CDXInput::CDXInput()
{
	ZeroMemory(m_byKeyState, sizeof(_byte) * 256);
}

Engine::CDXInput::~CDXInput()
{
	Free();
}

HRESULT Engine::CDXInput::Ready_InputDevice(HINSTANCE hInst, HWND hWnd)
{
	// 내 키보드를 생성하기 위한 사전 객체를 생성
	if(FAILED(DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInputSDK, NULL)))
		return E_FAIL;

	// 키보드 값을 대표하는 키보드 컴객체 생성
	if(FAILED(m_pInputSDK->CreateDevice(GUID_SysKeyboard, &m_pKeyBoard, NULL)))
		return E_FAIL;

	// 생성된 키보드 객체에 대한 정보를 상수 값으로 전달
	m_pKeyBoard->SetDataFormat(&c_dfDIKeyboard);

	//  Set~~ : 장치에 대한 독점권을 설정해주는 함수(클라이언트가 실행 되는 상태에서 키값을 받을지 말지 결정할 수 있다.)
	m_pKeyBoard->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);

	// 생성된 키보드 활성화 함수(Acquire : 장치에 대한 access 버전을 받아오는 함수)
	m_pKeyBoard->Acquire();
	


	// 마우스 객체
	if(FAILED(m_pInputSDK->CreateDevice(GUID_SysMouse, &m_pMouse, NULL)))
		return E_FAIL;
		
	m_pMouse->SetDataFormat(&c_dfDIMouse);
	m_pMouse->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
	m_pMouse->Acquire();

	return S_OK;
}

void Engine::CDXInput::SetUp_InputState()
{
	m_pKeyBoard->GetDeviceState(256, m_byKeyState);
	m_pMouse->GetDeviceState(sizeof(m_tMouseState), &m_tMouseState);
}

void Engine::CDXInput::Free()
{
	Engine::Safe_Release(m_pKeyBoard);
	Engine::Safe_Release(m_pMouse);
	Engine::Safe_Release(m_pInputSDK);
}

