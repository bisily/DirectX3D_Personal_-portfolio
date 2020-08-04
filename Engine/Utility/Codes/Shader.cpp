#include "Shader.h"

USING(Engine)

Engine::CShader::CShader(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
{
	m_pGraphicDev->AddRef();

}

Engine::CShader::CShader(const CShader& rhs)
	: m_pGraphicDev(rhs.m_pGraphicDev)
	, m_pEffect(rhs.m_pEffect)
{
	m_pGraphicDev->AddRef();
	m_pEffect->AddRef();
}

Engine::CShader::~CShader()
{

}

HRESULT Engine::CShader::Ready_Shader(const _tchar* pFilePath)
{
	if (FAILED(D3DXCreateEffectFromFile(m_pGraphicDev,
										pFilePath,
										nullptr,
										nullptr,
										D3DXSHADER_DEBUG,
										nullptr,
										&m_pEffect,
										&m_pErrMsg)))	// 쉐이더 코드가 문제가 있는 경우
	{
		MessageBoxA(nullptr, (char*)m_pErrMsg->GetBufferPointer(), "Shader_Error", MB_OK);
		return E_FAIL;
	}


	else if (nullptr != m_pErrMsg)
	{
		MessageBoxA(nullptr, (char*)m_pErrMsg->GetBufferPointer(), "Shader_warning", MB_OK);
	}


	return S_OK;
}

Engine::CShader* Engine::CShader::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pFilePath)
{
	CShader*	pInstance = new CShader(pGraphicDev);

	if (FAILED(pInstance->Ready_Shader(pFilePath)))
	{
		ERR_BOX(L"shader created Failed");
		Engine::Safe_Release(pInstance);
	}

	return pInstance;
}

Engine::CComponent* Engine::CShader::Clone()
{
	return new CShader(*this);
}

void Engine::CShader::Free()
{
	Engine::Safe_Release(m_pGraphicDev);
	Engine::Safe_Release(m_pEffect);
}

