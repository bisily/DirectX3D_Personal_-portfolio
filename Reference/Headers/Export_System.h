#ifndef Export_System_h__
#define Export_System_h__

#include "Engine_Defines.h"
#include "GraphicDev.h"
#include "TimeMgr.h"
#include "FrameMgr.h"
#include "DXInput.h"
#include "FontMgr.h"

BEGIN(Engine)

//Graphic Instance
//Getter
inline LPDIRECT3DDEVICE9		GetDevice();
//Setter
//General

inline HRESULT					Ready_GraphicDev(HWND hWnd,
								Engine::CGraphicDev::WINMODE eMode,
								const _uint& iSizeX,
								const _uint& iSizeY,
								Engine::CGraphicDev** ppGraphicDev);

inline 	ID3DXFont*				Get_Font();
inline	ID3DXLine*				Get_Line();

//TimeMgr Instance
//Getter
inline _float					Get_TimeDelta(const _tchar* pTimerTag);
//Setter
inline void						Set_TimeDelta(const _tchar* pTimerTag);
//General
inline HRESULT					Ready_Timer(const _tchar* pTimerTag);


//FrameMgr Instance
//Getter
inline _bool					isPermit_Call(const _tchar* pFrameTag, const _float& fTimeDelta);
//Setter

//General
inline HRESULT					Ready_Frame(const _tchar* pFrameTag, const _float& fCallLimit);

//DXInput Instance
//Getter
inline _byte					Get_DIKeyState(_ubyte byKeyID);
inline _byte					Get_DIMouseState(CDXInput::MOUSEKEYSTATE byMouseID);
inline _long					Get_DIMouseMove(CDXInput::MOUSEMOVESTATE	byMouseMoveState);
//Setter
inline void						SetUp_InputState();
//General
inline HRESULT					Ready_InputDevice(HINSTANCE hInst, HWND hWnd);

//FontMgr Instance
//Getter
//Setter
//General
inline HRESULT					Ready_Font(LPDIRECT3DDEVICE9 pGraphicDev,
								const Engine::_tchar* pFontTag,
								const Engine::_tchar* pFontType,
								const Engine::_uint& iWidth,
								const Engine::_uint& iHeight,
								const Engine::_uint& iWeight);

inline void						Render_Font(const Engine::_tchar* pFontTag,
								const Engine::_tchar* pString,
								const Engine::_vec2* pPostion,
								D3DXCOLOR Color);

inline void						Release_System();

#include "Export_System.inl"

END



#endif // Export_System_h__
