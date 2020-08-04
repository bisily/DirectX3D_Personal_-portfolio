#ifndef ObjState_h__
#define ObjState_h__

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CObjState : public CComponent
{
public:
	enum NOW_STATE { NOW_IDLE, NOW_ATTACK, NOW_HITED, NOW_DEAD, NOW_END };

private:
	explicit				CObjState();
public:
	virtual					~CObjState();

public: // Get
	_float					Get_Hp() const { return m_fHp; }
	_float					Get_Damage() const { return m_fDamge; }
	NOW_STATE				Get_State() const { return m_eState; }

public: // Set
	void					Set_Damge(_float fDamage) { m_fHp -= fDamage; }
	void					Set_State(NOW_STATE eState) { m_eState = eState; }

public:
	HRESULT					Ready_ObjState(_float fHp, _float fDamge);
	
public:
	_float					m_fHp		= 0.f;
	_float					m_fMaxHp	= 0.f;
	_float					m_fDamge	= 0.f;
	NOW_STATE				m_eState	= NOW_IDLE;

public:
	static	CObjState*		Create(_float fHp, _float fDamge);

private:
	virtual void			Free();
};

END
#endif // ObjState_h__
