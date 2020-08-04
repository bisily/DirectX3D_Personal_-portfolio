#ifndef Shader_h__
#define Shader_h__

#include "Engine_Defines.h"
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CShader : public CComponent
{
private:
	explicit CShader(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CShader(const CShader& rhs);
	virtual ~CShader();

public:
	LPD3DXEFFECT		Get_EffectHandle() { return m_pEffect; }

public:
	HRESULT		Ready_Shader(const _tchar* pFilePath);

private:
	LPDIRECT3DDEVICE9		m_pGraphicDev	= nullptr;
	LPD3DXEFFECT			m_pEffect		= nullptr;	// 특정 쉐이더를 대표하는 객체	
	LPD3DXBUFFER			m_pErrMsg		= nullptr;	// 단순한 임의의 정보를 담기 위한 객체

public:
	static	CShader*		Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pFilePath);
	virtual CComponent*		Clone();
	virtual void			Free();
};


END
#endif // Shader_h__


