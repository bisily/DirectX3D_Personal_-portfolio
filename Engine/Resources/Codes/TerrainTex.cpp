#include "TerrainTex.h"

USING(Engine)

Engine::CTerrainTex::CTerrainTex(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev)
	, m_bIsClon(false)
	, m_pPosition(nullptr)
{

}

Engine::CTerrainTex::CTerrainTex(const CTerrainTex& rhs)
	:CVIBuffer(rhs)
	, m_pPosition(rhs.m_pPosition)
	, m_bIsClon(true)
	, m_iH(rhs.m_iH)
	, m_fH(rhs.m_fH)
{

}

Engine::CTerrainTex::~CTerrainTex(void)
{

}

void CTerrainTex::SetUp_VtxCnt(_ulong & dwCntX, 
								_ulong & dwCntZ, 
								_ulong & dwVtxItv,
								_ulong Detail)
{
	m_bIsClon = false;

	m_dwTriCnt = (dwCntX - 1) * (dwCntZ - 1) * 2;
	m_dwVtxCnt = dwCntX * dwCntZ;

	m_dwVtxFVF = VTXFVF_TEX;
	m_dwVtxSize = sizeof(VTXTEX);

	m_dwIdxSize = sizeof(INDEX32);
	m_IdxFmt = D3DFMT_INDEX32;

	m_pPosition = new _vec3[m_dwVtxCnt];
	VTXTEX*		pVtxTex = nullptr;

	m_iH.biWidth = dwCntX;
	m_iH.biHeight = dwCntZ;

	if (FAILED(CVIBuffer::CreateBuffer()))
		return;

	m_pVB->Lock(0, 0, (void**)&pVtxTex, 0);

	_ulong iIndex = 0;

	for (_ulong z = 0; z < dwCntZ; ++z)
	{
		for (_ulong x = 0; x < dwCntX; ++x)
		{
			iIndex = z * dwCntX + x;

			pVtxTex[iIndex].vPosition = D3DXVECTOR3(
				float(x) * dwVtxItv,
				/*(pPixel[iIndex] & 0X000000ff) / 20.f*/ 0,
				float(z) * dwVtxItv);

			m_pPosition[iIndex] = pVtxTex[iIndex].vPosition;

			pVtxTex[iIndex].vNormal = _vec3(0.f, 0.f, 0.f);

			pVtxTex[iIndex].vTex = D3DXVECTOR2((x / (dwCntX - 1.f)) * Detail, z / ((dwCntZ - 1.f)) * Detail);
		}
	}

	INDEX32*		pIndex = NULL;

	_int iTriCnt = 0;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	for (_ulong z = 0; z < dwCntZ - 1; ++z)
	{
		for (_ulong x = 0; x < dwCntX - 1; ++x)
		{
			iIndex = z * dwCntX + x;

			pIndex[iTriCnt]._1 = iIndex + dwCntX;
			pIndex[iTriCnt]._2 = iIndex + dwCntX + 1;
			pIndex[iTriCnt]._3 = iIndex + 1;

			_vec3		vDest, vSour, vNormal;

			vDest = pVtxTex[pIndex[iTriCnt]._2].vPosition - pVtxTex[pIndex[iTriCnt]._1].vPosition;
			vSour = pVtxTex[pIndex[iTriCnt]._3].vPosition - pVtxTex[pIndex[iTriCnt]._2].vPosition;
			D3DXVec3Cross(&vNormal, &vDest, &vSour);

			pVtxTex[pIndex[iTriCnt]._1].vNormal += vNormal;
			pVtxTex[pIndex[iTriCnt]._2].vNormal += vNormal;
			pVtxTex[pIndex[iTriCnt]._3].vNormal += vNormal;

			++iTriCnt;

			pIndex[iTriCnt]._1 = iIndex + dwCntX;
			pIndex[iTriCnt]._2 = iIndex + 1;
			pIndex[iTriCnt]._3 = iIndex;

			vDest = pVtxTex[pIndex[iTriCnt]._2].vPosition - pVtxTex[pIndex[iTriCnt]._1].vPosition;
			vSour = pVtxTex[pIndex[iTriCnt]._3].vPosition - pVtxTex[pIndex[iTriCnt]._2].vPosition;
			D3DXVec3Cross(&vNormal, &vDest, &vSour);

			pVtxTex[pIndex[iTriCnt]._1].vNormal += vNormal;
			pVtxTex[pIndex[iTriCnt]._2].vNormal += vNormal;
			pVtxTex[pIndex[iTriCnt]._3].vNormal += vNormal;

			++iTriCnt;
		}
	}

	m_pVB->Unlock();
	m_pIB->Unlock();
}

HRESULT Engine::CTerrainTex::Ready_Buffer(const _ulong& dwCntX,
										const _ulong& dwCntZ, 
										const _ulong& dwVtxItv,
										const _ulong& dwVtxDetail)
{
	////////////////////////////////////////////// 직접 Texture를 만들 일이 있다면 사용할 코드
	/*LPDIRECT3DTEXTURE9			pTexture = nullptr;

	D3DXCreateTexture(m_pGraphicDev, 129, 129, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &pTexture);

	D3DLOCKED_RECT		LockRect;

	pTexture->LockRect(0, &LockRect, NULL, 0);

	for(_int i = 0; i < 129; ++i)
	{
	for(_int j = 0; j < 129; ++j)
	{
	_int		iIndex = i * 129 + j;

	*(((_ulong*)LockRect.pBits) + iIndex) = D3DCOLOR_ARGB(255, 255, 0, 0);
	}
	}

	pTexture->UnlockRect(0);

	D3DXSaveTextureToFile(L"../Bin/Resources/Texture/Terrain/MyHeight.bmp", D3DXIFF_BMP, pTexture, NULL);*/

	//////////////////////////////////////////////

	m_dwTriCnt = (dwCntX - 1) * (dwCntZ - 1) * 2;
	m_dwVtxCnt = dwCntX * dwCntZ;

	m_dwVtxFVF = VTXFVF_TEX;
	m_dwVtxSize = sizeof(VTXTEX);

	m_dwIdxSize = sizeof(INDEX32);
	m_IdxFmt = D3DFMT_INDEX32;

	m_pPosition = new _vec3[m_dwVtxCnt];

	if(FAILED(CVIBuffer::CreateBuffer()))
		return E_FAIL;
	
	m_iH.biWidth = dwCntX;
	m_iH.biHeight = dwCntZ;

	//_ulong	dwByte = 0;

	//m_hFile = CreateFile(L"../../Resources/Texture/Terrain/Height.bmp", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	//ReadFile(m_hFile, &m_fH, sizeof(BITMAPFILEHEADER), &dwByte, NULL);
	//ReadFile(m_hFile, &m_iH, sizeof(BITMAPINFOHEADER), &dwByte, NULL);

	//_ulong*	pPixel = new _ulong[m_iH.biWidth * m_iH.biHeight];

	//ReadFile(m_hFile, pPixel, sizeof(_ulong) * m_iH.biWidth * m_iH.biHeight, &dwByte, NULL);

	//CloseHandle(m_hFile);

	VTXTEX*		pVtxTex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVtxTex, 0);

	_ulong iIndex = 0;

	for (_ulong z = 0; z < dwCntZ; ++z)
	{
		for (_ulong x = 0; x < dwCntX; ++x)
		{
			iIndex = z * dwCntX + x;

			pVtxTex[iIndex].vPosition = D3DXVECTOR3(
				float(x) * dwVtxItv,
				0,
				float(z) * dwVtxItv);

			m_pPosition[iIndex] = pVtxTex[iIndex].vPosition;

			pVtxTex[iIndex].vNormal = _vec3(0.f, 0.f, 0.f);

			pVtxTex[iIndex].vTex = D3DXVECTOR2((x / (dwCntX - 1.f)) * dwVtxDetail, z / ((dwCntZ - 1.f)) * 20.f);
		}
	}

	INDEX32*		pIndex = NULL;

	_int iTriCnt = 0;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	for(_ulong z = 0; z < dwCntZ - 1; ++z)
	{
		for(_ulong x = 0; x < dwCntX - 1; ++x)
		{
			iIndex = z * dwCntX + x;

			pIndex[iTriCnt]._1 = iIndex + dwCntX;
			pIndex[iTriCnt]._2 = iIndex + dwCntX + 1;
			pIndex[iTriCnt]._3 = iIndex + 1;

			_vec3		vDest, vSour, vNormal;

			vDest = pVtxTex[pIndex[iTriCnt]._2].vPosition - pVtxTex[pIndex[iTriCnt]._1].vPosition;
			vSour = pVtxTex[pIndex[iTriCnt]._3].vPosition - pVtxTex[pIndex[iTriCnt]._2].vPosition;
			D3DXVec3Cross(&vNormal, &vDest, &vSour);

			pVtxTex[pIndex[iTriCnt]._1].vNormal += vNormal;
			pVtxTex[pIndex[iTriCnt]._2].vNormal += vNormal;
			pVtxTex[pIndex[iTriCnt]._3].vNormal += vNormal;

			++iTriCnt;

			pIndex[iTriCnt]._1 = iIndex + dwCntX;
			pIndex[iTriCnt]._2 = iIndex + 1;
			pIndex[iTriCnt]._3 = iIndex;

			vDest = pVtxTex[pIndex[iTriCnt]._2].vPosition - pVtxTex[pIndex[iTriCnt]._1].vPosition;
			vSour = pVtxTex[pIndex[iTriCnt]._3].vPosition - pVtxTex[pIndex[iTriCnt]._2].vPosition;
			D3DXVec3Cross(&vNormal, &vDest, &vSour);

			pVtxTex[pIndex[iTriCnt]._1].vNormal += vNormal;
			pVtxTex[pIndex[iTriCnt]._2].vNormal += vNormal;
			pVtxTex[pIndex[iTriCnt]._3].vNormal += vNormal;

			++iTriCnt;
		}
	}

	m_pVB->Unlock();
	m_pIB->Unlock();

	return S_OK;
}

void Engine::CTerrainTex::Render_Buffer(void)
{
	m_pGraphicDev->SetStreamSource(0, m_pVB, 0, m_dwVtxSize);
	m_pGraphicDev->SetFVF(m_dwVtxFVF);
	m_pGraphicDev->SetIndices(m_pIB);
	m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_dwVtxCnt, 0, m_dwTriCnt);
}

Engine::CTerrainTex* Engine::CTerrainTex::Create(LPDIRECT3DDEVICE9 pGraphicDev, 
													const _ulong& dwCntX, 
													const _ulong& dwCntZ, 
													const _ulong& dwVtxItv, 
													const _ulong& dwVtxDetail)
{
	CTerrainTex*		pInstance = new CTerrainTex(pGraphicDev);

	if(FAILED(pInstance->Ready_Buffer(dwCntX, dwCntZ, dwVtxItv, dwVtxDetail)))
	{
		ERR_BOX(L"TerrainTex Create Failed");
		Engine::Safe_Release(pInstance);
	}

	return pInstance;
}

Engine::CResources* Engine::CTerrainTex::Clone(void)
{
	return new CTerrainTex(*this);
}

void Engine::CTerrainTex::Free(void)
{
	Engine::CVIBuffer::Free();

	if (false == m_bIsClon)
	{
		Engine::Safe_Delete_Array(m_pPosition);
	}
}

void Engine::CTerrainTex::CopyIndexBuffer(Engine::INDEX32* pIndex, const _ulong& dwTriCnt)
{
	Engine::INDEX32*	pOrigin = nullptr;

	m_pIB->Lock(0, 0, (void**)&pOrigin, 0);

	memcpy(pOrigin, pIndex, sizeof(Engine::INDEX32) * dwTriCnt);

	m_pIB->Unlock();

	m_dwTriCnt = dwTriCnt;
}