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
	// �� Ű���带 �����ϱ� ���� ���� ��ü�� ����
	if(FAILED(DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInputSDK, NULL)))
		return E_FAIL;

	// Ű���� ���� ��ǥ�ϴ� Ű���� �İ�ü ����
	if(FAILED(m_pInputSDK->CreateDevice(GUID_SysKeyboard, &m_pKeyBoard, NULL)))
		return E_FAIL;

	// ������ Ű���� ��ü�� ���� ������ ��� ������ ����
	m_pKeyBoard->SetDataFormat(&c_dfDIKeyboard);

	//  Set~~ : ��ġ�� ���� �������� �������ִ� �Լ�(Ŭ���̾�Ʈ�� ���� �Ǵ� ���¿��� Ű���� ������ ���� ������ �� �ִ�.)
	m_pKeyBoard->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);

	// ������ Ű���� Ȱ��ȭ �Լ�(Acquire : ��ġ�� ���� access ������ �޾ƿ��� �Լ�)
	m_pKeyBoard->Acquire();
	


	// ���콺 ��ü
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

