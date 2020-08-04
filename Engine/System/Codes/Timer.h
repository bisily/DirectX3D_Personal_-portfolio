#ifndef Timer_h__
#define Timer_h__

#include "Engine_Defines.h"
#include "../SuperClass/Base.h"

BEGIN(Engine)

class CTimer : public CBase
{
private:
	explicit				CTimer();
public:
	virtual					~CTimer();

public:
	_float					Get_TimeDelta() const { return m_fTimeDelta; }

public:
	HRESULT					Ready_Timer();
	void					Set_TimeDelta();

private:
	LARGE_INTEGER			m_FrameTime{};
	LARGE_INTEGER			m_FixTime{};
	LARGE_INTEGER			m_LastTime{};
	LARGE_INTEGER			m_CpuTick{};

	_float					m_fTimeDelta = 0.f;

public:
	static	CTimer*			Create();
	virtual void			Free();
};

END

#endif // Timer_h__