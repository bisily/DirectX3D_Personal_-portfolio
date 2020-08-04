#ifndef PrototypeMgr_h__
#define PrototypeMgr_h__

#include "Engine_Defines.h"
#include "Base.h"
#include "Optimization.h"
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CPrototypeMgr : public CBase
{
	DECLARE_SINGLETON(CPrototypeMgr)

private:
	explicit CPrototypeMgr();
	virtual ~CPrototypeMgr();

public:
	HRESULT		Ready_Prototype(const wstring pProtoTag, CComponent* pInstance);
	CComponent*	Clone_Prototype(const wstring pProtoTag);

private:
	CComponent*		Find_Prototype(const wstring pProtoTag);

private:
	typedef	map<const wstring, CComponent*>			MAPPROTOTYPE;
	MAPPROTOTYPE									m_mapPrototype;

public:
	virtual		void	Free();
};

END
#endif // PrototypeMgr_h__