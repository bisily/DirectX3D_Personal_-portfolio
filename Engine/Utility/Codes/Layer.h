#ifndef Layer_h__
#define Layer_h__

#include "GameObject.h"

BEGIN(Engine)
class ENGINE_DLL CLayer : public CBase
{
private:
	explicit											CLayer();
public:
	virtual												~CLayer();

public: // Getter
	CComponent*											Get_Component(const wstring		pObjTag,
																	  const wstring		pComponentTag,
																	  Engine::CComponent::COMPONENTID eID);
	
	CGameObject*										Get_Object(const wstring pObjTag);

	CGameObject*										Get_Object(const wstring pObjTag, _long iObjIndex = -1);

	CComponent*											Get_Component(const wstring		pObjTag,
														const wstring		pComponentTag,
														Engine::CComponent::COMPONENTID eID,
														int iObjectIndex);

	const multimap<const wstring, CGameObject*>			Get_ObjectMultiMap() const { return m_mapObject; }
	void												DeleteObj(int iIndex, wstring strObjKey);

public:
	virtual HRESULT										Ready_Layer();
	virtual _int										Update_Layer(const _float& fTimeDelta);
	virtual void										Render_Layer();

public:
	HRESULT												Add_GameObject(const wstring pObjTag, CGameObject* pGameObject);

private:
	typedef multimap<const wstring, CGameObject*>		MAPOBJECT;
	MAPOBJECT											m_mapObject;

public:
	static CLayer*										Create();
	virtual void										Free();

};

END
#endif // Layer_h__
