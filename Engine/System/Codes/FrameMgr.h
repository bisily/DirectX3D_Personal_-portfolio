#ifndef FrameMgr_h__
#define FrameMgr_h__

#include "Engine_Defines.h"
#include "Base.h"

BEGIN(Engine)

class CFrame;
class ENGINE_DLL CFrameMgr : public CBase
{
	DECLARE_SINGLETON(CFrameMgr)

private:
	explicit								CFrameMgr();
	virtual									~CFrameMgr();

public:
	_bool									isPermit_Call(const _tchar* pFrameTag, const _float& fTimeDelta);
	HRESULT									Ready_Frame(const _tchar* pFrameTag, const _float& fCallLimit);

private:
	map<const _tchar*, CFrame*>				m_mapFrame;
	typedef map<const _tchar*, CFrame*>		MAPFRAME;

private:
	CFrame*									Find_Frame(const _tchar* pFrameTag);
public:
	virtual	void							Free();
};

END
#endif // FrameMgr_h__
