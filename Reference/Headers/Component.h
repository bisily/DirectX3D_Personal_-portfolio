#ifndef Component_h__
#define Component_h__

#include "Engine_Defines.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CComponent : public CBase
{
public:
	// DYNAMIC = �� �����Ӹ��� ������Ʈ �Ǿ���� ������Ʈ ����
	// STATIC  = �� �����Ӹ��� ������Ʈ�� �ʿ����� ���� ������Ʈ ����
	enum COMPONENTID{ ID_DYNAMIC, ID_STATIC, ID_END	};


protected:
	explicit CComponent();
public:
	virtual ~CComponent();

public:
	virtual void			Update_Component(const _float& fTimeDelta);
	virtual	CComponent*		Clone() { return nullptr; }

protected:
	virtual void Free()PURE;
};

END
#endif // Component_h__
