#include "Cell.h"
#include "Line.h"

USING(Engine)

Engine::CCell::CCell(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
{
	m_pGraphicDev->AddRef();
}

Engine::CCell::~CCell(void)
{

}

void CCell::ReSet_Index(int iIndex)
{
	m_dwIndex = iIndex;
	
	HDC hdc = CreateCompatibleDC(0);
	HFONT hFont;
	HFONT hFontOld;

	// 폰트 구조체?
	LOGFONT loFont;

	loFont.lfHeight = 4;
	loFont.lfWidth = 4;
	loFont.lfEscapement = 0;
	loFont.lfOrientation = 0;
	loFont.lfWeight = 500;
	loFont.lfItalic = false;
	loFont.lfStrikeOut = false;
	loFont.lfCharSet = DEFAULT_CHARSET;
	loFont.lfOutPrecision = 0;
	loFont.lfClipPrecision = 0;
	loFont.lfQuality = 0;
	loFont.lfPitchAndFamily = 0;
	loFont.lfFaceName, TEXT("맑은고딕");

	hFont = CreateFontIndirect(&loFont);
	hFontOld = (HFONT)SelectObject(hdc, hFont);

	wsprintf(szIndex, L"%d", iIndex);

	D3DXCreateText(m_pGraphicDev, hdc, szIndex, 0.001f, 0.001f, &m_Text, 0, 0);
	D3DXCreateText(m_pGraphicDev, hdc, L"_0", 0.001f, 0.001f, &m_PosA, 0, 0);
	D3DXCreateText(m_pGraphicDev, hdc, L"_1", 0.001f, 0.001f, &m_PosB, 0, 0);
	D3DXCreateText(m_pGraphicDev, hdc, L"_2", 0.001f, 0.001f, &m_PosC, 0, 0);

	SelectObject(hdc, hFontOld);
	DeleteObject(hFont);
	DeleteObject(hdc);
}

HRESULT Engine::CCell::Ready_Cell(LPDIRECT3DDEVICE9 pGraphicDev,
									const _ulong& dwIndex,
									const _vec3* pPointA, 
									const _vec3* pPointB, 
									const _vec3* pPointC)
{
	m_dwIndex = dwIndex;

	m_vPoint[POINT_A] = *pPointA;
	m_vPoint[POINT_B] = *pPointB;
	m_vPoint[POINT_C] = *pPointC;

	m_pLine[LINE_AB] = CLine::Create(&_vec2(m_vPoint[POINT_A].x, m_vPoint[POINT_A].z),
									 &_vec2(m_vPoint[POINT_B].x, m_vPoint[POINT_B].z));
	NULL_CHECK_RETURN(m_pLine[LINE_AB], E_FAIL);

	
	m_pLine[LINE_BC] = CLine::Create(&_vec2(m_vPoint[POINT_B].x, m_vPoint[POINT_B].z),
		&_vec2(m_vPoint[POINT_C].x, m_vPoint[POINT_C].z));
	NULL_CHECK_RETURN(m_pLine[LINE_BC], E_FAIL);

	
	m_pLine[LINE_CA] = CLine::Create(&_vec2(m_vPoint[POINT_C].x, m_vPoint[POINT_C].z),
		&_vec2(m_vPoint[POINT_A].x, m_vPoint[POINT_A].z));
	NULL_CHECK_RETURN(m_pLine[LINE_CA], E_FAIL);


	#ifdef _DEBUG
	if(FAILED(D3DXCreateLine(pGraphicDev, &m_pD3DXLine)))
		return E_FAIL;

	HDC hdc = CreateCompatibleDC(0);
	HFONT hFont;
	HFONT hFontOld;

	// 폰트 구조체?
	LOGFONT loFont;

	loFont.lfHeight = 2;
	loFont.lfWidth = 2;
	loFont.lfEscapement = 0;
	loFont.lfOrientation = 0;
	loFont.lfWeight = 500;
	loFont.lfItalic = false;
	loFont.lfStrikeOut = false;
	loFont.lfCharSet = DEFAULT_CHARSET;
	loFont.lfOutPrecision = 0;
	loFont.lfClipPrecision = 0;
	loFont.lfQuality = 0;
	loFont.lfPitchAndFamily = 0;
	loFont.lfFaceName, TEXT("맑은고딕");

	hFont = CreateFontIndirect(&loFont);
	hFontOld = (HFONT)SelectObject(hdc, hFont);

	wsprintf(szIndex, L"%d", m_dwIndex);

	D3DXCreateText(m_pGraphicDev, hdc, szIndex, 0.001f, 0.001f, &m_Text, 0, 0);
	D3DXCreateText(m_pGraphicDev, hdc, L"_0", 0.001f, 0.001f, &m_PosA, 0, 0);
	D3DXCreateText(m_pGraphicDev, hdc, L"_1", 0.001f, 0.001f, &m_PosB, 0, 0);
	D3DXCreateText(m_pGraphicDev, hdc, L"_2", 0.001f, 0.001f, &m_PosC, 0, 0);
	
	m_pGraphicDev->CreateTexture(1, 1, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &m_pTexture, NULL);

	D3DLOCKED_RECT			LockRect;
	ZeroMemory(&LockRect, sizeof(D3DLOCKED_RECT));
	m_pTexture->LockRect(0, &LockRect, NULL, 0);
	*((Engine::_ulong*)LockRect.pBits) = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	m_pTexture->UnlockRect(0);


	SelectObject(hdc, hFontOld);
	DeleteObject(hFont);
	DeleteObject(hdc);
	#endif

	return S_OK;
}

_bool Engine::CCell::Compare_Point(const _vec3* pPointA, const _vec3* pPointB, CCell* pCell)
{
	if(m_vPoint[POINT_A] == *pPointA)
	{
		if(m_vPoint[POINT_B] == *pPointB)
		{
			m_pNeighbor[NEIGHBOR_AB] = pCell;
			return true;
		}
		if(m_vPoint[POINT_C] == *pPointB)
		{
			m_pNeighbor[NEIGHBOR_CA] = pCell;
			return true;
		}

	}

	if(m_vPoint[POINT_B] == *pPointA)
	{
		if(m_vPoint[POINT_A] == *pPointB)
		{
			m_pNeighbor[NEIGHBOR_AB] = pCell;
			return true;
		}
		if(m_vPoint[POINT_C] == *pPointB)
		{
			m_pNeighbor[NEIGHBOR_BC] = pCell;
			return true;
		}

	}

	if(m_vPoint[POINT_C] == *pPointA)
	{
		if(m_vPoint[POINT_A] == *pPointB)
		{
			m_pNeighbor[NEIGHBOR_CA] = pCell;
			return true;
		}
		if(m_vPoint[POINT_B] == *pPointB)
		{
			m_pNeighbor[NEIGHBOR_BC] = pCell;
			return true;
		}

	}

	return false;
}

void Engine::CCell::Render_Cell(void)
{
	_vec3		vPoint[4];

	vPoint[0] = m_vPoint[POINT_A];
	vPoint[1] = m_vPoint[POINT_B];
	vPoint[2] = m_vPoint[POINT_C];
	vPoint[3] = m_vPoint[POINT_A];

	_matrix matView, matProj;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	for(_ulong i = 0; i < 4; ++i)
	{
		D3DXVec3TransformCoord(&vPoint[i], &vPoint[i], &matView);

		if(vPoint[i].z <= 0.1f)
			vPoint[i].z = 0.f;

		D3DXVec3TransformCoord(&vPoint[i], &vPoint[i], &matProj);
	}
	m_pD3DXLine->SetWidth(3.f);

	m_pGraphicDev->EndScene();
	m_pGraphicDev->BeginScene();
	
	m_pD3DXLine->Begin();
	
	_matrix		matTemp;
	D3DXMatrixIdentity(&matTemp);

	m_pD3DXLine->DrawTransform(vPoint, 4, &matTemp, D3DXCOLOR(1.f, 0.f, 0.f, 1.f)); // 3d 라인을 그려주는 함수

	m_pD3DXLine->End();

	if (bRenderIndex == TRUE)
	{
		D3DXMATRIX matPos;
		_vec3 vecPos;

		vecPos = m_vPoint[POINT_A];
		vecPos += m_vPoint[POINT_B];
		vecPos += m_vPoint[POINT_C];

		vecPos /= 3.f;

		D3DXMatrixTranslation(&matPos, vecPos.x, vecPos.y, vecPos.z);
		m_pGraphicDev->SetTransform(D3DTS_WORLD, &matPos);
	
		m_pGraphicDev->SetTexture(0, m_pTexture);
		m_Text->DrawSubset(0);
	}

	if (bRenderPosText == TRUE)
	{
		D3DXMATRIX matPoint;
		_vec3 vecPoint;

		vecPoint = m_vPoint[POINT_A];

		D3DXMatrixScaling(&matPoint, 0.01f, 0.01f, 0.01f);
		D3DXMatrixTranslation(&matPoint, vecPoint.x, vecPoint.y + 1.f, vecPoint.z);
		m_pGraphicDev->SetTransform(D3DTS_WORLD, &matPoint);

		m_pGraphicDev->SetTexture(0, m_pTexture);
		m_PosA->DrawSubset(0);

		vecPoint = m_vPoint[POINT_B];
	
		D3DXMatrixScaling(&matPoint, 0.01f, 0.01f, 0.01f);
		D3DXMatrixTranslation(&matPoint, vecPoint.x, vecPoint.y + 0.5f, vecPoint.z);
		m_pGraphicDev->SetTransform(D3DTS_WORLD, &matPoint);

		m_pGraphicDev->SetTexture(0, m_pTexture);
		m_PosB->DrawSubset(0);
	
		vecPoint = m_vPoint[POINT_C];

		D3DXMatrixScaling(&matPoint, 0.01f, 0.01f, 0.01f);
		D3DXMatrixTranslation(&matPoint, vecPoint.x, vecPoint.y + 0.1f, vecPoint.z);
		m_pGraphicDev->SetTransform(D3DTS_WORLD, &matPoint);

		m_pGraphicDev->SetTexture(0, m_pTexture);
		m_PosC->DrawSubset(0);
	}
}

Engine::CCell* Engine::CCell::Create(LPDIRECT3DDEVICE9	pGraphicDev, 
									const _ulong& dwIndex,
									const _vec3* pPointA, 
									const _vec3* pPointB, 
									const _vec3* pPointC)
{
	CCell*		pInstance = new CCell(pGraphicDev);

	if(FAILED(pInstance->Ready_Cell(pGraphicDev, dwIndex, pPointA, pPointB, pPointC)))
	{
		ERR_BOX(L"Cell Create Failed");
		Engine::Safe_Release(pInstance);
	}
	
	return pInstance;
}

void Engine::CCell::Free(void)
{
	for(_ulong i = 0; i < LINE_END; ++i)
	{
		Engine::Safe_Release(m_pLine[i]);
	}

	Engine::Safe_Release(m_pTexture);
	Engine::Safe_Release(m_Text);
	Engine::Safe_Release(m_PosA);
	Engine::Safe_Release(m_PosB);
	Engine::Safe_Release(m_PosC);
	Engine::Safe_Release(m_pD3DXLine);
	Engine::Safe_Release(m_pGraphicDev);
}

Engine::CCell::COMPARE Engine::CCell::Compare(const _vec3* pEndPos, _ulong* pCellIndex)
{
	for(_ulong i = 0; i < LINE_END; ++i)
	{
		if(CLine::COMPARE_RIGHT == m_pLine[i]->Compare(&_vec2(pEndPos->x, pEndPos->z)))
		{
			if(nullptr == m_pNeighbor[i])
				return CCell::COMPARE_STAY;

			else
			{
				*pCellIndex = *m_pNeighbor[i]->Get_Index();
				return CCell::COMPARE_MOVE;
			}
		}
	}
	
	return CCell::COMPARE_MOVE;
}

