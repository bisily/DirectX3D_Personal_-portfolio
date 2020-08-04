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
	rhs.m_pAniCtrl->CloneAnimationController(rhs.m_pAniCtrl->GetMaxNumAnimationOutputs(), // �޽��� ���� �� �ִϸ��̼� ����( ���� �� ���� ��ü���� �����ǰ� �ִ� �ִϸ��̼��� ����)
											rhs.m_pAniCtrl->GetMaxNumAnimationSets(),     // ������ ������ �ִϸ��̼� ����(1����, 2���� ���� ���� ����)
											rhs.m_pAniCtrl->GetMaxNumTracks(),	// ������ �� �ִ� �ִ� Ʈ���� ����, �������� ��, ��κ� �� ���� ����ϰ� ���� ����غ��� �� ���� ���
											rhs.m_pAniCtrl->GetMaxNumEvents(), // ���� �޽��� ����Ǵ� ��Ư�� ȿ��, ���� �츮�� ������� ���Ѵ�.
											&m_pAniCtrl);
}

Engine::CAnimationCtrl::~CAnimationCtrl()
{
}

HRESULT Engine::CAnimationCtrl::Ready_AnimationCtrl()
{
	return S_OK;
}

// ���ڿ��ε� ���� �� ����
void Engine::CAnimationCtrl::Set_AnimationSet(const _uint & iIdx)
{
	if (m_iOldAniIdx == iIdx)
		return;

	m_iNewTrack = (m_iCurrentTrack == 0 ? 1 : 0);

	// �ִϸ��̼� �� ������ �����ϴ� ��ü
	LPD3DXANIMATIONSET			pAS = nullptr;

	m_pAniCtrl->GetAnimationSet(iIdx, &pAS);
	//m_pAniCtrl->GetAnimationSetByName();

	// �ִϸ��̼� ���� ��ü ����ð��� üũ�ϱ� ���� �۾�
	m_dPeriod = pAS->GetPeriod();			// �ð��� ��ȯ, ���� �ִϸ��̼� Ʈ���� �����ǰ� ��ġ�ϴ� ��
											//m_pAniCtrl->GetTrackDesc(iIdx, nullptr);	// ���� ����Ǵ� Ʈ���� ������ �������ִ� �Լ�

											// 0��° Ʈ���� �ִϸ��̼� ���� �����Ѵ�.
	m_pAniCtrl->SetTrackAnimationSet(m_iNewTrack, pAS);

	// ������� �ʰ� �ִ� �̺�Ʈ�� ������ ������ �ȵǴ� ��찡 �߻��� �� �־ �־��ִ� �ڵ�(�̺�Ʈ�� �߻����� �ʵ��� ó���ϴ� �Լ�)
	m_pAniCtrl->UnkeyAllTrackEvents(m_iCurrentTrack);
	m_pAniCtrl->UnkeyAllTrackEvents(m_iNewTrack);

	// ������ ����� �������ڴٴ� �ǹ�(3���� : �������� Ű �������� ���� �� ���ΰ�)
	m_pAniCtrl->KeyTrackEnable(m_iCurrentTrack, FALSE, m_fAccTime + 0.08);

	// �����Ǵ� �ð� ���� ���� �������� � �ӵ��� ������ �������� ���� �Լ�(�ӵ��� ��� ���� ���� 1)
	m_pAniCtrl->KeyTrackSpeed(m_iCurrentTrack, 1.f, m_fAccTime, 0.08, D3DXTRANSITION_LINEAR);

	// 
	m_pAniCtrl->KeyTrackWeight(m_iCurrentTrack, 0.1f, m_fAccTime, 0.08, D3DXTRANSITION_LINEAR);

	// ���ʿ��� 0.25��� �ð��� �����̸� �� ���¿��� Ʈ���� Ȱ��ȭ�ϰ� �ִ�.
	// �̰��� �� ����� 0.25��� �ð����� ���ÿ� ���� �ִٴ� ���� �ǹ��ϸ�, �� �ð� ������ �ִϸ��̼��� �����Ϸ��� ���̴�.
	m_pAniCtrl->SetTrackEnable(m_iNewTrack, TRUE);
	m_pAniCtrl->KeyTrackSpeed(m_iNewTrack, 1.f, m_fAccTime, 0.08, D3DXTRANSITION_LINEAR);
	m_pAniCtrl->KeyTrackWeight(m_iNewTrack, 0.9f, m_fAccTime, 0.08, D3DXTRANSITION_LINEAR);

	// ���� �ִϸ��̼��� ����ǰ� �־��� �ð��� �ʱ�ȭ �Ѵ�.
	m_pAniCtrl->ResetTime();
	m_fAccTime = 0.f;

	// ������ ��� ���̴� Ʈ������ ���ο� Ʈ���� �����ϰ� �Ǿ��� ��, 0�ʺ��� �����ϵ��� �����ϴ� �Լ�
	m_pAniCtrl->SetTrackPosition(m_iNewTrack, 0.0);

	m_iOldAniIdx = iIdx;

	m_iCurrentTrack = m_iNewTrack;
}

void Engine::CAnimationCtrl::Play_AnimationSet(const _float & fTimeDelta)
{
	m_pAniCtrl->AdvanceTime(fTimeDelta, NULL);	// 2���� : �ִϸ��̼� ���ۿ� ���� ���峪 ����Ʈ�� ���� ó���� ����ϴ� ��ü �ּ�, ������ �������� ����ؾ��ϴµ� ������ ��� ������ ���ؼ� ������ �ڵ��� ���߸� Ŀ���� ����
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

// ���� Ʈ���� ������ ��� TRUE�� ��ȯ�ϴ� �Լ�
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
