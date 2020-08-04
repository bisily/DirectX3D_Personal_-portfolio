#include "SphereCollider.h"

USING(Engine)

Engine::CSphereCollider::CSphereCollider(LPDIRECT3DDEVICE9 pGraphicDev)
{

	m_pGraphicDev = pGraphicDev;
	m_pGraphicDev->AddRef();
	
}

Engine::CSphereCollider::~CSphereCollider()
{

}

HRESULT Engine::CSphereCollider::Ready_SphereCollider(D3DXMATRIX* matWorld, D3DXFRAME* pRootFrame, MAP_BORENAMES* pBoreNames)
{
	m_matWorld = matWorld;
	m_pRootFrame = pRootFrame;
	m_mapBoreNames = *pBoreNames;
	

	m_pGraphicDev->CreateTexture(1, 1, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &m_pTexture, NULL);

	D3DLOCKED_RECT			LockRect;
	ZeroMemory(&LockRect, sizeof(D3DLOCKED_RECT));

	m_pTexture->LockRect(0, &LockRect, NULL, 0);

	*((Engine::_ulong*)LockRect.pBits) = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);
	m_pTexture->UnlockRect(0);

	for (auto iter : m_mapBoreNames)
	{	
		string strTemp;
		strTemp.assign(iter.first.begin(), iter.first.end());

		vector<char> writable(strTemp.begin(), strTemp.end());

		writable.push_back('\0');
		char* ptr = &writable[0];

		Engine::D3DXFRAME_DERIVED*		pFrame = (D3DXFRAME_DERIVED*)D3DXFrameFind(m_pRootFrame, ptr);

		_vec3 vPos(pFrame->CombinedTransformationMatrix._41, pFrame->CombinedTransformationMatrix._42, pFrame->CombinedTransformationMatrix._43);

		SPHERE_COL tagSphere;
		tagSphere.pmatPos = &pFrame->CombinedTransformationMatrix;

		D3DXCreateSphere(m_pGraphicDev, iter.second, 10, 10, &tagSphere.pMesh, NULL);
		
		_ulong dwFvF = tagSphere.pMesh->GetFVF();

		tagSphere.pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&tagSphere.pVertices);

		D3DXComputeBoundingSphere(tagSphere.pVertices, tagSphere.pMesh->GetNumVertices(), D3DXGetFVFVertexSize(dwFvF), &vPos, &tagSphere.fRadius);

		tagSphere.pMesh->UnlockVertexBuffer();

		m_vecSphere.push_back(tagSphere);
	}
	return S_OK;
}

void Engine::CSphereCollider::Render_SphereCollider()
{
	_matrix matWorld;
	_vec3	vecPos;

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	
	for (auto& iter : m_vecSphere)
	{
		if (!m_vecSphere.empty())
		{
			D3DXMatrixIdentity(&matWorld);

			memcpy(&vecPos, &(*iter.pmatPos * *m_matWorld).m[3][0], sizeof(_vec3));
		
			D3DXMatrixScaling(&matWorld, iter.fScaling, iter.fScaling, iter.fScaling);
			D3DXMatrixTranslation(&matWorld, vecPos.x, vecPos.y, vecPos.z);

			m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
			m_pGraphicDev->SetTexture(0, m_pTexture);
			iter.pMesh->DrawSubset(0);
		}
	}

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}

Engine::CSphereCollider* Engine::CSphereCollider::Create(LPDIRECT3DDEVICE9 pGraphicDev,
															D3DXFRAME* pRootFrame,
															MAP_BORENAMES* pBoreNames,
															D3DXMATRIX* matWorld)
{
	CSphereCollider*	pInstance = new CSphereCollider(pGraphicDev);

	if (FAILED(pInstance->Ready_SphereCollider(matWorld, pRootFrame, pBoreNames)))
	{
		ERR_BOX(L"SphereCollider Create Failed");
		Engine::Safe_Release(pInstance);
	}

	return pInstance;
}

void Engine::CSphereCollider::Free()
{

	Engine::Safe_Release(m_pGraphicDev);
	Engine::Safe_Release(m_pTexture);

}

