#ifndef TimeMgr_h__
#define TimeMgr_h__

#include "Engine_Defines.h"
#include "../SuperClass/Base.h"

BEGIN(Engine)

class CTimer;
class ENGINE_DLL CTimerMgr : public CBase
{
	DECLARE_SINGLETON(CTimerMgr)
private:
	explicit							CTimerMgr();
	virtual								~CTimerMgr();

public:
	_float								Get_TimeDelta(const _tchar* pTimerTag) ;
	void								Set_TimeDelta(const _tchar* pTimerTag);

public:
	HRESULT								Ready_Timer(const _tchar* pTimerTag);

private:
	CTimer*								Find_Timer(const _tchar* pTimerTag);

private:
	map<const _tchar*, CTimer*>			m_mapTimers;
	typedef map<const _tchar*, CTimer*>	MAPTIMER;

public:
	virtual void						Free();
};

END
#endif // TimeMgr_h__
