#include "stdafx.h"
#include "CameraObserver.h"
#include "Export_Function.h"

CCameraObserver::CCameraObserver(void)
{
	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixIdentity(&m_matProj);
}

CCameraObserver::~CCameraObserver(void)
{
	
}

const	D3DXMATRIX* CCameraObserver::GetView(void)
{
	return &m_matView;
}

const D3DXMATRIX* CCameraObserver::GetProj(void)
{
	return &m_matProj;
}

CCameraObserver* CCameraObserver::Create(void)
{
	return new CCameraObserver;
}

void CCameraObserver::Update(int iMessage)
{
	list<void*>*		pDataList = m_pInfoSubject->GetDataList(iMessage);
	NULL_CHECK(pDataList);

	switch(iMessage)
	{
	case D3DTS_VIEW:
		m_matView = *((D3DXMATRIX*)pDataList->front());
		break;

	case D3DTS_PROJECTION:
		m_matProj = *((D3DXMATRIX*)pDataList->front());
		break;
	}
}

