#include "AnimationCtrl.h"

USING(Engine)

Engine::CAnimationCtrl::CAnimationCtrl(LPD3DXANIMATIONCONTROLLER pAniCtrl)
	: m_pAniCtrl(pAniCtrl)
	, m_iCurrentTrack(0)
	, m_iNewTrack(1)
	, m_fAccTime(0.f)
	, m_iOldAniIdx(99)
{
	m_pAniCtrl->AddRef();
}

Engine::CAnimationCtrl::CAnimationCtrl(const CAnimationCtrl & rhs)
	: m_iCurrentTrack(rhs.m_iCurrentTrack)
	, m_iNewTrack(rhs.m_iNewTrack)
	, m_fAccTime(rhs.m_fAccTime)
	, m_iOldAniIdx(rhs.m_iOldAniIdx)
{
	rhs.m_pAniCtrl->CloneAnimationController(rhs.m_pAniCtrl->GetMaxNumAnimationOutputs(), // 메쉬가 지닌 총 애니메이션 개수( 복제 시 원본 객체에서 제공되고 있는 애니메이션의 개수)
											rhs.m_pAniCtrl->GetMaxNumAnimationSets(),     // 구동이 가능한 애니메이션 개수(1인자, 2인자 값이 거의 같음)
											rhs.m_pAniCtrl->GetMaxNumTracks(),	// 구동할 수 있는 최대 트랙의 개수, 정수형의 값, 대부분 한 개를 사용하고 많이 사용해봐야 두 개를 사용
											rhs.m_pAniCtrl->GetMaxNumEvents(), // 현재 메쉬에 적용되는 독특한 효과, 현재 우리는 사용하지 못한다.
											&m_pAniCtrl);
}

Engine::CAnimationCtrl::~CAnimationCtrl()
{
}

HRESULT Engine::CAnimationCtrl::Ready_AnimationCtrl()
{
	return S_OK;
}

// 문자열로도 받을 수 있음
void Engine::CAnimationCtrl::Set_AnimationSet(const _uint & iIdx)
{
	if (m_iOldAniIdx == iIdx)
		return;

	m_iNewTrack = (m_iCurrentTrack == 0 ? 1 : 0);

	// 애니메이션 셋 정보를 저장하는 객체
	LPD3DXANIMATIONSET			pAS = nullptr;

	m_pAniCtrl->GetAnimationSet(iIdx, &pAS);
	//m_pAniCtrl->GetAnimationSetByName();

	// 애니메이션 셋의 전체 재생시간을 체크하기 위한 작업
	m_dPeriod = pAS->GetPeriod();			// 시간을 반환, 현재 애니메이션 트랙의 포지션과 일치하는 값
											//m_pAniCtrl->GetTrackDesc(iIdx, nullptr);	// 현재 재생되는 트랙의 정보를 가져와주는 함수

											// 0번째 트랙에 애니메이션 셋을 세팅한다.
	m_pAniCtrl->SetTrackAnimationSet(m_iNewTrack, pAS);

	// 사용하지 않고 있는 이벤트들 때문에 보간이 안되는 경우가 발생할 수 있어서 넣어주는 코드(이벤트가 발생하지 않도록 처리하는 함수)
	m_pAniCtrl->UnkeyAllTrackEvents(m_iCurrentTrack);
	m_pAniCtrl->UnkeyAllTrackEvents(m_iNewTrack);

	// 현재의 모션을 꺼버리겠다는 의미(3인자 : 언제부터 키 프레임을 해제 할 것인가)
	m_pAniCtrl->KeyTrackEnable(m_iCurrentTrack, FALSE, m_fAccTime + 0.08);

	// 해제되는 시간 동안 현재 프레임은 어떤 속도로 움직일 것인지에 대한 함수(속도의 상수 값은 각자 1)
	m_pAniCtrl->KeyTrackSpeed(m_iCurrentTrack, 1.f, m_fAccTime, 0.08, D3DXTRANSITION_LINEAR);

	// 
	m_pAniCtrl->KeyTrackWeight(m_iCurrentTrack, 0.1f, m_fAccTime, 0.08, D3DXTRANSITION_LINEAR);

	// 위쪽에서 0.25라는 시간의 딜레이를 준 상태에서 트랙을 활성화하고 있다.
	// 이것은 두 모션이 0.25라는 시간동안 동시에 돌고 있다는 것을 의미하며, 이 시간 사이의 애니메이션을 보간하려는 것이다.
	m_pAniCtrl->SetTrackEnable(m_iNewTrack, TRUE);
	m_pAniCtrl->KeyTrackSpeed(m_iNewTrack, 1.f, m_fAccTime, 0.08, D3DXTRANSITION_LINEAR);
	m_pAniCtrl->KeyTrackWeight(m_iNewTrack, 0.9f, m_fAccTime, 0.08, D3DXTRANSITION_LINEAR);

	// 현재 애니메이션이 재생되고 있었던 시간을 초기화 한다.
	m_pAniCtrl->ResetTime();
	m_fAccTime = 0.f;

	// 기존의 재생 중이던 트랙에서 새로운 트랙이 등장하게 되었을 때, 0초부터 시작하도록 지시하는 함수
	m_pAniCtrl->SetTrackPosition(m_iNewTrack, 0.0);

	m_iOldAniIdx = iIdx;

	m_iCurrentTrack = m_iNewTrack;
}

void Engine::CAnimationCtrl::Play_AnimationSet(const _float & fTimeDelta)
{
	m_pAniCtrl->AdvanceTime(fTimeDelta, NULL);	// 2인자 : 애니메이션 동작에 따른 사운드나 이펙트에 대한 처리를 담당하는 객체 주소, 하지만 직접만들어서 사용해야하는데 이유는 사용 제약이 심해서 오히려 코드의 가중만 커지기 때문
	m_fAccTime += fTimeDelta;
}

CAnimationCtrl * Engine::CAnimationCtrl::Create(LPD3DXANIMATIONCONTROLLER pAniCtrl)
{
	CAnimationCtrl*		pInstance = new CAnimationCtrl(pAniCtrl);

	if (FAILED(pInstance->Ready_AnimationCtrl()))
	{
		ERR_BOX(L"Animation Create Failed");
		Engine::Safe_Release(pInstance);
	}

	return pInstance;
}

CAnimationCtrl * Engine::CAnimationCtrl::Create(const CAnimationCtrl & rhs)
{
	CAnimationCtrl*		pInstance = new CAnimationCtrl(rhs);

	if (FAILED(pInstance->Ready_AnimationCtrl()))
	{
		ERR_BOX(L"Animation Create Failed");
		Engine::Safe_Release(pInstance);
	}

	return pInstance;
}

void Engine::CAnimationCtrl::Free()
{
	Engine::Safe_Release(m_pAniCtrl);
}

// 현재 트랙이 끝났을 경우 TRUE를 반환하는 함수
_bool Engine::CAnimationCtrl::Is_AnimationSetEnd()
{
	D3DXTRACK_DESC			TrackInfo;
	ZeroMemory(&TrackInfo, sizeof(D3DXTRACK_DESC));

	m_pAniCtrl->GetTrackDesc(m_iCurrentTrack, &TrackInfo);

	if (TrackInfo.Position >= m_dPeriod - 0.1)
		return true;

	return false;
}

_bool CAnimationCtrl::Is_AniSetEnd()
{
	D3DXTRACK_DESC			TrackInfo;
	ZeroMemory(&TrackInfo, sizeof(D3DXTRACK_DESC));

	m_pAniCtrl->GetTrackDesc(m_iCurrentTrack, &TrackInfo);

	if (TrackInfo.Position >= m_dPeriod)
		return true;

	return false;
}

_double CAnimationCtrl::Get_Position()
{
	D3DXTRACK_DESC			TrackInfo;
	ZeroMemory(&TrackInfo, sizeof(D3DXTRACK_DESC));

	m_pAniCtrl->GetTrackDesc(m_iCurrentTrack, &TrackInfo);

	return TrackInfo.Position;
}

_double CAnimationCtrl::Get_Percent()
{
	D3DXTRACK_DESC			TrackInfo;
	ZeroMemory(&TrackInfo, sizeof(D3DXTRACK_DESC));

	m_pAniCtrl->GetTrackDesc(m_iCurrentTrack, &TrackInfo);

	_double iResult = _double((TrackInfo.Position / m_dPeriod) * 100);

	return iResult;
}
