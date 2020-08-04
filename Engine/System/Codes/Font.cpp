#include "Font.h"

USING(Engine)

Engine::CFont::CFont(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
{
	m_pGraphicDev->AddRef();
}

Engine::CFont::~CFont()
{

}

HRESULT Engine::CFont::Ready_Font(const _tchar* pFontType, 
								const _uint& iWidth, 
								const _uint& iHeight, 
								const _uint& iWeight)
{
	D3DXFONT_DESC			FontDesc;
	ZeroMemory(&FontDesc, sizeof(D3DXFONT_DESC));

	FontDesc.CharSet = HANGUL_CHARSET;
	lstrcpy(FontDesc.FaceName, pFontType);
	FontDesc.Width = iWidth;
	FontDesc.Height = iHeight;
	FontDesc.Weight = iWeight;

	if(FAILED(D3DXCreateFontIndirect(m_pGraphicDev, &FontDesc, &m_pFont)))
	{
		ERR_BOX(L"Font Create Failed");
		return E_FAIL;
	}

	if(FAILED(D3DXCreateSprite(m_pGraphicDev, &m_pSprite)))
	{
		ERR_BOX(L"Sprite Create Failed");
		return E_FAIL;
	}


	return S_OK;
}

Engine::CFont* Engine::CFont::Create(LPDIRECT3DDEVICE9 pGraphicDev, 
									const _tchar* pFontType, 
									const _uint& iWidth, 
									const _uint& iHeight, 
									const _uint& iWeight)
{
	CFont*	pInstance = new CFont(pGraphicDev);

	if(FAILED(pInstance->Ready_Font(pFontType, iWidth, iHeight, iWeight)))
	{
		ERR_BOX(L"Font Create Failed");
		Engine::Safe_Release(pInstance);
	}

	return pInstance;
}

void Engine::CFont::Free()
{
	Engine::Safe_Release(m_pFont);
	Engine::Safe_Release(m_pSprite);
	Engine::Safe_Release(m_pGraphicDev);
}

void Engine::CFont::Render_Font(const _tchar* pString, 
							const _vec2* pPosition, 
							D3DXCOLOR Color)
{
	RECT	rc{ _long(pPosition->x), _long(pPosition->y) };

	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);

	m_pFont->DrawTextW(m_pSprite, pString, lstrlen(pString), &rc, DT_NOCLIP, Color);

	m_pSprite->End();
}

