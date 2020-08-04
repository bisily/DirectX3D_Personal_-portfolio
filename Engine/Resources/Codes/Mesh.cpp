#include "Mesh.h"

USING(Engine)

CMesh::CMesh(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CResources(pGraphicDev)
	, m_bClone(false)
{
}

CMesh::CMesh(const CMesh & rhs)
	: Engine::CResources(rhs)
	, m_bClone(true)
{
}

CMesh::~CMesh()
{
}

void CMesh::Free()
{
	Engine::CResources::Free();
}
