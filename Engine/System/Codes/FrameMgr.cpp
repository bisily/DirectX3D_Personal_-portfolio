#include "FrameMgr.h"
#include "Frame.h"

USING(Engine)
IMPLEMENT_SINGLETON(CFrameMgr)

Engine::CFrameMgr::CFrameMgr()
{

}

Engine::CFrameMgr::~CFrameMgr()
{
	Free();
}

Engine::_bool Engine::CFrameMgr::isPermit_Call(const _tchar* pFrameTag, const _float& fTimeDelta)
{
	CFrame*		pFrame = Find_Frame(pFrameTag);

	if(nullptr == pFrame)
		return false;

	return pFrame->isPermit_Call(fTimeDelta);
}

HRESULT Engine::CFrameMgr::Ready_Frame(const _tchar* pFrameTag, const _float& fCallLimit)
{
	CFrame*		pFrame = Find_Frame(pFrameTag);
	
	if(nullptr != pFrame)
		return E_FAIL;

	pFrame = CFrame::Create(fCallLimit);
	NULL_CHECK_RETURN(pFrame, E_FAIL);

	m_mapFrame.insert(MAPFRAME::value_type(pFrameTag, pFrame));

	return S_OK;
}

Engine::CFrame* Engine::CFrameMgr::Find_Frame(const _tchar* pFrameTag)
{
	auto		iter = find_if(m_mapFrame.begin(), m_mapFrame.end(), CTag_Finder(pFrameTag));
	if(iter == m_mapFrame.end())
		return nullptr;

	return iter->second;
}

void Engine::CFrameMgr::Free()
{
	for_each(m_mapFrame.begin(), m_mapFrame.end(), CDeleteMap());
	m_mapFrame.clear();
}

