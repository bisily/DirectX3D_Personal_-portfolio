#include "Texture.h"

USING(Engine)

Engine::CTexture::CTexture(LPDIRECT3DDEVICE9 pGraphicDev)
	: CResources(pGraphicDev)
{

}

Engine::CTexture::CTexture(const CTexture& rhs)
	: CResources(rhs)
{
	_uint		iContainerSize = rhs.m_vecTexture.size();
	m_vecTexture.reserve(iContainerSize);

	m_vecTexture = rhs.m_vecTexture;
	
	for(_uint i = 0; i < iContainerSize; ++i)
		m_vecTexture[i]->AddRef();

}

Engine::CTexture::~CTexture()
{

}

void Engine::CTexture::Set_Texture(const _uint& iIndex /*= 0*/)
{
	if(m_vecTexture.size() < iIndex)
		return;

	m_pGraphicDev->SetTexture(0, m_vecTexture[iIndex]);
}

void CTexture::Set_Texture(LPD3DXEFFECT pEffect, const char * pShaderName, const _uint & iIndex)
{
	if (m_vecTexture.size() < iIndex)
		return;

	pEffect->SetTexture(pShaderName, m_vecTexture[iIndex]);
}

HRESULT Engine::CTexture::Ready_Texture(wstring pPath, 
										TEXTURETYPE eType, 
										const _uint& iCnt)
{
	m_vecTexture.reserve(iCnt);

	IDirect3DTexture9*				pTexture = nullptr;

	for(_uint i = 0; i < iCnt; ++i)
	{
		TCHAR			szFileName[128] = L"";

		wsprintf(szFileName, pPath.c_str(), i);

		switch(eType)
		{
		case TYPE_NORMAL:
			if(FAILED(D3DXCreateTextureFromFile(m_pGraphicDev, szFileName, (LPDIRECT3DTEXTURE9*)&pTexture)))
				return E_FAIL;

			break;

		case TYPE_CUBE:
			if(FAILED(D3DXCreateCubeTextureFromFile(m_pGraphicDev, szFileName, (LPDIRECT3DCUBETEXTURE9*)&pTexture)))
				return E_FAIL;
			break;
		}

		m_vecTexture.push_back(pTexture);
	}

	return S_OK;
}

Engine::CResources* Engine::CTexture::Clone()
{
	return new CTexture(*this);
}

Engine::CTexture* Engine::CTexture::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring pPath, TEXTURETYPE eType, const _uint& iCnt /*= 1*/)
{
	CTexture*		pInstance = new CTexture(pGraphicDev);

	if(FAILED(pInstance->Ready_Texture(pPath, eType, iCnt)))
	{
		ERR_BOX(L"Texture Created Failed");
		Engine::Safe_Release(pInstance);
	}

	return pInstance;
}

void Engine::CTexture::Free()
{
	Engine::CResources::Free();

	for(auto& i : m_vecTexture)
		Engine::Safe_Release(i);

	m_vecTexture.clear();
}

