//Graphic Instance
//Getter
LPDIRECT3DDEVICE9		Engine::GetDevice()
{
	return Engine::CGraphicDev::GetInstance()->GetDevice();
}
//Setter
//General

HRESULT		Engine::Ready_GraphicDev(HWND hWnd,
	Engine::CGraphicDev::WINMODE eMode,
	const _uint& iSizeX,
	const _uint& iSizeY,
	Engine::CGraphicDev** ppGraphicDev)
{
	return Engine::CGraphicDev::GetInstance()->Ready_GraphicDev(hWnd, eMode, iSizeX, iSizeY, ppGraphicDev);
}

//TimeMgr Instance
//Getter
Engine::_float		Engine::Get_TimeDelta(const _tchar* pTimerTag)
{
	return Engine::CTimerMgr::GetInstance()->Get_TimeDelta(pTimerTag);
}
//Setter
void			Engine::Set_TimeDelta(const _tchar* pTimerTag)
{
	Engine::CTimerMgr::GetInstance()->Set_TimeDelta(pTimerTag);
}
HRESULT	Engine::Ready_Timer(const _tchar* pTimerTag)
{
	return Engine::CTimerMgr::GetInstance()->Ready_Timer(pTimerTag);
}

//FrameMgr Instance
//Getter
Engine::_bool		Engine::isPermit_Call(const _tchar* pFrameTag, const _float& fTimeDelta)
{
	return Engine::CFrameMgr::GetInstance()->isPermit_Call(pFrameTag, fTimeDelta);
}
//Setter
//General
HRESULT		Engine::Ready_Frame(const _tchar* pFrameTag, const _float& fCallLimit)
{
	return Engine::CFrameMgr::GetInstance()->Ready_Frame(pFrameTag, fCallLimit);
}

//DXInput Instance
//Getter
Engine::_byte Engine::Get_DIKeyState(Engine::_ubyte byKeyID)
{
	return Engine::CDXInput::GetInstance()->Get_DIKeyState(byKeyID);
}

Engine::_byte Engine::Get_DIMouseState(Engine::CDXInput::MOUSEKEYSTATE byMouseID)
{
	return Engine::CDXInput::GetInstance()->Get_DIMouseState(byMouseID);
}

Engine::_long Engine::Get_DIMouseMove(Engine::CDXInput::MOUSEMOVESTATE byMouseMoveState)
{
	return Engine::CDXInput::GetInstance()->Get_DIMouseMove(byMouseMoveState);
}
//Setter

void Engine::SetUp_InputState()
{
	Engine::CDXInput::GetInstance()->SetUp_InputState();
}
//General
HRESULT Engine::Ready_InputDevice(HINSTANCE hInst, HWND hWnd)
{
	return Engine::CDXInput::GetInstance()->Ready_InputDevice(hInst, hWnd);
}

//FontMgr Instance
//Getter
//Setter
//General
HRESULT	 Engine::Ready_Font(LPDIRECT3DDEVICE9 pGraphicDev,
	const Engine::_tchar* pFontTag,
	const Engine::_tchar* pFontType,
	const Engine::_uint& iWidth,
	const Engine::_uint& iHeight,
	const Engine::_uint& iWeight)
{
	return Engine::CFontMgr::GetInstance()->Ready_Font(pGraphicDev, pFontTag, pFontType, iWidth, iHeight, iWeight);
}

void	Engine::Render_Font(const Engine::_tchar* pFontTag,
	const Engine::_tchar* pString,
	const Engine::_vec2* pPostion,
	D3DXCOLOR Color)
{
	Engine::CFontMgr::GetInstance()->Render_Font(pFontTag, pString, pPostion, Color);
}

void Engine::Release_System()
{
	Engine::CFontMgr::GetInstance()->DestroyInstance();
	Engine::CDXInput::GetInstance()->DestroyInstance();
	Engine::CFrameMgr::GetInstance()->DestroyInstance();
	Engine::CTimerMgr::GetInstance()->DestroyInstance();
	Engine::CGraphicDev::GetInstance()->DestroyInstance();
}