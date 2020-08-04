#include "Line.h"

USING(Engine)

Engine::CLine::CLine(void)
{

}

Engine::CLine::~CLine(void)
{

}

HRESULT Engine::CLine::Ready_Line(const _vec2* pPointA, const _vec2* pPointB)
{
	m_vPoint[POINT_START] = *pPointA;
	m_vPoint[POINT_FINISH] = *pPointB;

	m_vDirection = m_vPoint[POINT_FINISH] - m_vPoint[POINT_START];
	m_vNormal = _vec2(m_vDirection.y * -1.f, m_vDirection.x);
	D3DXVec2Normalize(&m_vNormal, &m_vNormal);

	return S_OK;
}

Engine::CLine::COMPARE Engine::CLine::Compare(const _vec2* pEndPos)
{
	_vec2	vDest = *pEndPos - m_vPoint[POINT_START];
	D3DXVec2Normalize(&vDest, &vDest);

	_float fDot = D3DXVec2Dot(&vDest, &m_vNormal);

	if(0 <= fDot)
		return COMPARE_RIGHT;

	else
		return COMPARE_LEFT;
}

Engine::CLine* Engine::CLine::Create(const _vec2* pPointA, const _vec2* pPointB)
{
	CLine*		pInstance = new CLine;

	if(FAILED(pInstance->Ready_Line(pPointA, pPointB)))
	{
		ERR_BOX(L"Line Create Failed");
		Engine::Safe_Release(pInstance);
	}

	return pInstance;
}

void Engine::CLine::Free(void)
{

}

