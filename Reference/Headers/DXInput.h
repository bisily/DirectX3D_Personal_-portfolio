#ifndef DXInput_h__
#define DXInput_h__

#include "Engine_Defines.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CDXInput : public CBase
{
	DECLARE_SINGLETON(CDXInput)

public:
	enum MOUSEKEYSTATE { DIM_LB, DIM_RB, DIM_HB, DIM_END };
	enum MOUSEMOVESTATE { DIMS_X, DIMS_Y, DIMS_Z, DIMS_END };

private:
	explicit						CDXInput();
	virtual							~CDXInput();

public:
	_byte							Get_DIKeyState(_ubyte byKeyID) { return m_byKeyState[byKeyID]; }
	_byte							Get_DIMouseState(MOUSEKEYSTATE byMouseID) { return m_tMouseState.rgbButtons[byMouseID]; }
	_long							Get_DIMouseMove(MOUSEMOVESTATE	byMouseMoveState) {	return *(((_long*)(&m_tMouseState)) + byMouseMoveState); }

public:
	HRESULT							Ready_InputDevice(HINSTANCE hInst, HWND hWnd);
	void							SetUp_InputState();

private:
	LPDIRECTINPUT8					m_pInputSDK		= nullptr;

	LPDIRECTINPUTDEVICE8			m_pKeyBoard		= nullptr;
	LPDIRECTINPUTDEVICE8			m_pMouse		= nullptr;

private:
	_byte							m_byKeyState[256];
	DIMOUSESTATE					m_tMouseState;

public:
	virtual void Free();

};

END
#endif // DXInput_h__
