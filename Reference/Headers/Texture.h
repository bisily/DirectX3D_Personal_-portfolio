#ifndef Texture_h__
#define Texture_h__

#include "Resources.h"

BEGIN(Engine)

class ENGINE_DLL CTexture : public CResources
{
public:
	enum TEXTURETYPE { TYPE_NORMAL, TYPE_CUBE, TYPE_END };

protected:
	explicit CTexture(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTexture(const CTexture& rhs);

public:
	virtual ~CTexture();

public:	// setter
	void		Set_Texture(const _uint& iIndex = 0);
	void		Set_Texture(LPD3DXEFFECT pEffect, const char* pShaderName, const _uint& iIndex = 0);

public:
	virtual	HRESULT	Ready_Texture(wstring pPath, 
									TEXTURETYPE eType, 
									const _uint& iCnt);

private:
	typedef	vector<IDirect3DTexture9*>		VECTEXTURE;
	VECTEXTURE								m_vecTexture;

public:
	virtual CResources*		Clone();
	static	CTexture*		Create(LPDIRECT3DDEVICE9 pGraphicDev, 
									wstring pPath, 
									TEXTURETYPE eType, 
									const _uint& iCnt = 1);
public:
	virtual void	Free();

};

END
#endif // Texture_h__
