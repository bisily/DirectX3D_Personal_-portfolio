#ifndef Mesh_h__
#define Mesh_h__

#include "Resources.h"

BEGIN(Engine)

class ENGINE_DLL CMesh : public CResources
{
public:
	enum MESHTYPE { TYPE_STATIC, TYPE_DYNAMIC, TYPE_NAVI };

protected:
	explicit CMesh(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CMesh(const CMesh& rhs);
	virtual ~CMesh();

public:
	bool		m_bClone;

public:
	virtual CResources*		Clone()PURE;

public:
	virtual void Free();

};

END

#endif // !Mesh_h__
