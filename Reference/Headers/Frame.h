#ifndef Frame_h__
#define Frame_h__


#include "Engine_Defines.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CFrame : public CBase
{
private:
	explicit				CFrame();
public:
	virtual					~CFrame();

public:
	_bool					isPermit_Call(const _float& fTimeDelta);

public:
	HRESULT					Ready_Frame(const _float& fCallLimit);

private:
	_float					m_fCallLimit	= 0.f;
	_float					m_fAccTimeDelta = 0;

public:
	static		CFrame*		Create(const _float& fCallLimit);
	virtual		void		Free();
};


END
#endif // Frame_h__
