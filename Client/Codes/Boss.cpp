#include "stdafx.h"
#include "Boss.h"

#include "Export_Function.h"
#include "Boss_WeaPon.h"
#include "Boss_Wave.h"
#include "SwordTrail.h"
#include "Player.h"

CBoss::CBoss(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{

}

CBoss::~CBoss()
{

}

HRESULT CBoss::Ready_Object(wstring _tagMeshName
								, Engine::_vec3 * _temp_Right
								, Engine::_vec3 * _temp_Up
								, Engine::_vec3 * _temp_Look
								, Engine::_vec3 * _temp_Pos
								, Engine::_vec3 * _temp_Scale
								, Engine::_vec3 * _temp_Angle
								, int _iObjIndex)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_pTransCom->m_vInfo[0] = *_temp_Right;
	m_pTransCom->m_vInfo[1] = *_temp_Up;
	m_pTransCom->m_vInfo[2] = *_temp_Look;
	m_pTransCom->m_vInfo[3] = *_temp_Pos;

	m_pTransCom->Set_Scale(_temp_Scale->x, _temp_Scale->y, _temp_Scale->z);
	m_pTransCom->m_vAngle = *_temp_Angle;

	m_iIndex = _iObjIndex;

	m_pTransCom->Set_Scale(0.005f, 0.005f, 0.005f);

	return S_OK;
}

Engine::_int CBoss::Update_Object(const Engine::_float& fTimeDelta)
{
	Get_TargetCom();
	
	Is_Nan();
	
	Push_Player();

	Engine::_uint iExit = Engine::CGameObject::Update_Object(fTimeDelta);

	if (m_pStateCom->Get_Hp() <= 0)
	{
		m_pStateCom->Set_State(Engine::CObjState::NOW_DEAD);
	}

	if (m_pStateCom->Get_Hp() <= 0 &&
		m_pStateCom->Get_State() == Engine::CObjState::NOW_DEAD)
	{
		if (m_iAniIndex != 11 &&
			bDead1 == false &&
			bDead2 == false)
		{
			m_pMeshCom->Set_AnimationSet(11);
			m_iAniIndex = 11;
			bDead1 = true;
		}

		if (m_iAniIndex == 11 &&
			90 < m_pMeshCom->Get_Percent() &&
			bDead1 == true &&
			bDead2 == false)
		{
			bDead1 = false;
			bDead2 = true;

			m_pMeshCom->Set_AnimationSet(10);
			m_iAniIndex = 10;
		}

		// 죽는 모션 1
		if (m_iAniIndex == 11 &&
			!m_pMeshCom->Is_AnimationSetEnd() &&
			bDead1 == true &&
			bDead2 == false)
		{
			m_pMeshCom->Play_AnimationSet(fTimeDelta);
			m_pRendererCom->Add_RenderGroup(Engine::CRenderer::RENDER_NONALPHA, this);
			m_pTransCom->m_matWorld.m[3][1] = fHeight;
		}
		
		if (m_iAniIndex == 10 &&
			!m_pMeshCom->Is_AnimationSetEnd() &&
			bDead1 == false &&
			bDead2 == true)
		{
			fHeight = fHeight - fTimeDelta;

			if (fHeight < 19.f)
			{
				fHeight = 19.f;
			}

			m_pTransCom->m_matWorld.m[3][1] = fHeight;

			m_pMeshCom->Play_AnimationSet(fTimeDelta);
			m_pRendererCom->Add_RenderGroup(Engine::CRenderer::RENDER_NONALPHA, this);
		}

		if (bDead1 == false &&
			bDead2 == true &&
			m_pMeshCom->Is_AnimationSetEnd() == true)
		{
			CBoss_WeaPon* pBoss_WeaPon = dynamic_cast<CBoss_WeaPon*>(Engine::Get_Management()->Get_Object(L"Layer_GameLogic", L"Boss_Sword"));

			NULL_CHECK_RETURN(pBoss_WeaPon, -1);

			pBoss_WeaPon->Set_Dead();

			return -1;
		}

		return 0;
	}

	// 화면 출력
	Calc_Render();

	// 맞은 후 애니메이션 & 딜레이
	if (IsbHit == true)
	{
		m_dHitDelay += fTimeDelta;

		if (m_iAniIndex != 12 &&
			m_pStateCom->Get_State() != Engine::CObjState::NOW_HITED)
		{
			m_pMeshCom->Set_AnimationSet(12);
			m_iAniIndex = 12;

			iValue = 0;
			
			m_pTransCom->m_vInfo[Engine::CTransform::INFO_POS].y = 19.f;

			m_pStateCom->Set_State(Engine::CObjState::NOW_HITED);

			fAttackCoolTime = 0.f;
		}

		if (0.5f < m_dHitDelay)
		{
			m_dHitDelay = 0.f;
			IsbHit = false;

			m_bAttack5 = false;
			m_bAttack5_Revert = false;
			m_bAttack7 = false;
			m_bAttack7_Revert = false;
			m_bAttack4 = false;
			m_bAttack4_Revert = false;
			m_bWaveAttact = false;
		}
	}

	_vec3 vDist = m_pTargetCom->m_vInfo[Engine::CTransform::INFO_POS] - m_pTransCom->m_vInfo[Engine::CTransform::INFO_POS];
	_float fDist = D3DXVec3Length(&vDist);
	
	Target_Look(vDist, fTimeDelta);

	Patten(fDist, fTimeDelta);

	m_pMeshCom->Play_AnimationSet(fTimeDelta);
	
	if (m_bAttack5 == false &&
		m_bAttack5_Revert == false &&
		m_bAttack7 == false &&
		m_bAttack7_Revert == false &&
		m_bAttack4 == false &&
		m_bAttack4_Revert == false &&
		m_bWaveAttact == false &&
		m_pMeshCom->Is_AnimationSetEnd())
	{
		m_pMeshCom->Set_AnimationSet(0);
		m_iAniIndex = 0;
	}

	if (fDist < 15.f)
	{
		if (iValue == 0)
		{
			if (bAttack == false)
			{
				bTargetRock = true;
			}
		}
	}

	if (bAttack == true)
	{
		fAttackCoolTime += fTimeDelta;

		if (4.f < fAttackCoolTime)
		{
			fAttackCoolTime = 0.f;
			bAttack = false;
		}
	}

	m_pRendererCom->Add_RenderGroup(Engine::CRenderer::RENDER_NONALPHA, this);

	return 0;
}

void CBoss::Render_Object()
{
	if (false == m_bIsDraw)
		return;

	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);

	pEffect->AddRef();

	SetUp_ConstantTable(pEffect);

	_uint iPassCnt = 0;

	pEffect->Begin(&iPassCnt, 0);
	pEffect->BeginPass(0);

	m_pMeshCom->Render_Mesh(pEffect);

	pEffect->EndPass();
	pEffect->End();

	Engine::Safe_Release(pEffect);
}

HRESULT CBoss::Add_Component()
{
	Engine::CComponent*		pComponent = nullptr;

	// For.Transform
	pComponent = m_pTransCom = Engine::CTransform::Create();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::CComponent::ID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));

	// For.Renderer
	pComponent = m_pRendererCom = Engine::Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->AddRef();
	m_mapComponent[Engine::CComponent::ID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Renderer", pComponent));

	// For.Calculator
	pComponent = m_pCalculatorCom = Engine::CCalculator::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::CComponent::ID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Calculator", pComponent));

	// For.Mesh
	pComponent = m_pMeshCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Clone_Resource(RESOURCE_STAGE, L"Mesh_Boss"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::CComponent::ID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Mesh", pComponent));

	// For.NaviMesh
	pComponent = m_pNaviMeshCom = dynamic_cast<Engine::CNaviMesh*>(Engine::Clone_Resource(RESOURCE_STAGE, L"Mesh_Navi"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::CComponent::ID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_NaviMesh", pComponent));

	map<wstring, _float> mapBore;
	Make_BoreNames(&mapBore);

	// For.SphereCollider
	pComponent = m_pSphereCollider = Engine::CSphereCollider::Create(m_pGraphicDev, m_pMeshCom->Get_RootBore(), &mapBore, &m_pTransCom->m_matWorld);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::CComponent::ID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_SphereCol", pComponent));
	
	Make_SquareCollision();

	// For.ObjState
	pComponent = m_pStateCom = Engine::CObjState::Create(100.f, 10.f);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::CComponent::ID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Com_State", pComponent));

	// For.Shader
	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_Mesh"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::CComponent::ID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Shader", pComponent));

	// For.Optimization
	pComponent = m_pOptimizationCom = dynamic_cast<Engine::COptimization*>(Engine::Clone_Prototype(L"Proto_Optimization"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::CComponent::ID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Optimization", pComponent));

	return S_OK;
}

HRESULT CBoss::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	pEffect->AddRef();

	_matrix		matWorld, matView, matProj;

	m_pTransCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	const D3DLIGHT9*		pLightInfo = Engine::Get_LightInfo();

	_vec4 vDir;

	memcpy(&vDir, &m_pTransCom->m_vInfo[Engine::CTransform::INFO_LOOK], sizeof(_vec3));

	vDir.w = 0.f;

	D3DXVec4Normalize(&vDir, &vDir);

	pEffect->SetVector("g_vLightDir", &vDir);
	pEffect->SetVector("g_vLightDiffuse", (_vec4*)&pLightInfo->Diffuse);
	pEffect->SetVector("g_vLightSpecular", (_vec4*)&pLightInfo->Specular);
	pEffect->SetVector("g_vLightAmbient", (_vec4*)&pLightInfo->Ambient);


	D3DMATERIAL9			MtrlInfo;
	ZeroMemory(&MtrlInfo, sizeof(D3DMATERIAL9));

	MtrlInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	MtrlInfo.Specular = D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.f);
	MtrlInfo.Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.f);
	MtrlInfo.Emissive = D3DXCOLOR(0.f, 0.f, 0.f, 1.f);

	MtrlInfo.Power = 20.f;

	pEffect->SetVector("g_vMtrlDiffuse", (_vec4*)&MtrlInfo.Diffuse);
	pEffect->SetVector("g_vMtrlSpecular", (_vec4*)&MtrlInfo.Specular);
	pEffect->SetVector("g_vMtrlAmbient", (_vec4*)&MtrlInfo.Ambient);
	pEffect->SetFloat("g_fPower", MtrlInfo.Power);

	D3DXMatrixInverse(&matView, nullptr, &matView);
	pEffect->SetVector("g_vCamPosition", (_vec4*)&matView.m[3][0]);

	Engine::Safe_Release(pEffect);

	return S_OK;
}

HRESULT CBoss::Make_BoreNames(map<wstring, _float>* pmapBore)
{
	TCHAR szBoreName[64] = L"", szBoreSize[64] = L"";
	DWORD dwByte = 99;

	wifstream	LoadFile;

	LoadFile.open(L"../../Data/Collision_Boss.txt", ios::in);

	while (!LoadFile.eof())
	{
		LoadFile.getline(szBoreName, 64, '|');
		LoadFile.getline(szBoreSize, 64);

		if (!lstrcmp(szBoreName, L""))
		{
			break;
		}

		pmapBore->insert(make_pair(szBoreName, (float)_ttof(szBoreSize)));
	}

	LoadFile.close();

	return S_OK;
}

void CBoss::Get_TargetCom()
{
	if (m_pTargetCom == nullptr)
	{
		Engine::CTransform* pTransform = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Com_Transform", Engine::CComponent::ID_DYNAMIC));

		NULL_CHECK(pTransform);

		m_pTargetCom = pTransform;
	}
}

void CBoss::Patten(_float fDist, const Engine::_float& fTimeDelta)
{
	if (bTargetRock == true)
	{
		if (fDist < 4.f)
		{
			iValue = 1;
		}

		else if (5.f < fDist &&
			fDist < 8.f)
		{
			iValue = 3;
		}
		else if (9.f < fDist &&
			fDist < 12.f)
		{
			iValue = 2;
		}

		else if (11.f < fDist &&
			fDist < 15.f)
		{
			iValue = 4;
		}

		bTargetRock = false;
	}

	switch (iValue)
	{
	case 1:
		// 전방 휩쓸기 공격
		if (m_bAttack7 == false &&
			m_iAniIndex != 7 &&
			m_bAttack7_Revert == false)
		{

			m_pMeshCom->Set_AnimationSet(7);
			m_iAniIndex = 7;
			m_bAttack7 = true;

			m_pTransCom->m_vInfo[Engine::CTransform::INFO_POS].y = 19.f;

		}

		if (m_iAniIndex == 7 &&
			20 < m_pMeshCom->Get_Percent())
		{
			m_pStateCom->Set_State(Engine::CObjState::NOW_ATTACK);
		}

		if (m_iAniIndex == 7 &&
			50 < m_pMeshCom->Get_Percent() &&
			m_pMeshCom->Get_Percent() < 51)
		{
			CSoundMgr::GetInstance()->StopSound(CSoundMgr::CHANNEL_BOSS_SWING);
			CSoundMgr::GetInstance()->PlaySound(L"Boss_Swing.wav", CSoundMgr::CHANNEL_BOSS_SWING);
		}

		if (m_iAniIndex == 7 &&
			m_pMeshCom->Get_Percent() < 35)
		{
			m_fSweepTimer += fTimeDelta;

			if (0.01f < m_fSweepTimer)
			{
				m_fSweepTimer = 0.f;
				m_fSweepHight += 0.005f;

				if (0.015f < m_fSweepHight)
				{
					m_fSweepHight = 0.015f;
				}
			}
						
			m_pTransCom->m_vInfo[Engine::CTransform::INFO_POS].y -= m_fSweepHight;
		}


		if (m_iAniIndex == 7 &&
			80 < m_pMeshCom->Get_Percent())
		{
			m_bAttack7 = false;
		}

		if (m_iAniIndex == 7 &&
			m_bAttack7 == false &&
			m_bAttack7_Revert == false)
		{
			m_pStateCom->Set_State(Engine::CObjState::NOW_IDLE);
			m_pMeshCom->Set_AnimationSet(8);
			m_iAniIndex = 8;
			
			m_fSweepHight = 0.f;

			m_bAttack7_Revert = true;
		}
		
		if (m_iAniIndex == 8 &&
			75 < m_pMeshCom->Get_Percent())
		{

			m_fSweepTimer += fTimeDelta;

			if (0.01f < m_fSweepTimer)
			{
				m_fSweepTimer = 0.f;
				m_fSweepHight += 0.075f;

				if (m_pTransCom->m_vInfo[Engine::CTransform::INFO_POS].y < 19.f)
				{
					m_pTransCom->m_vInfo[Engine::CTransform::INFO_POS].y += m_fSweepHight;

					if (19.f < m_pTransCom->m_vInfo[Engine::CTransform::INFO_POS].y)
					{
						m_pTransCom->m_vInfo[Engine::CTransform::INFO_POS].y = 19.f;
					}
				}
			}

		}

		if (m_iAniIndex == 8 &&
			m_bAttack7_Revert == true &&
			m_pMeshCom->Is_AnimationSetEnd())
		{
			m_fTimer = 0.f;
			m_bAttack7_Revert = false;

			const Engine::D3DXFRAME_DERIVED* pFrame = m_pMeshCom->Get_FrameByName("M_ROOT");

			_matrix matMovePos = pFrame->CombinedTransformationMatrix;
			_matrix matPos = matMovePos * m_pTransCom->m_matWorld;

			m_pTransCom->Set_Position(_vec3(matPos.m[3][0], m_pTransCom->m_vInfo[Engine::CTransform::INFO_POS].y, matPos.m[3][2]));
			
			iValue = 0;
			
			m_fSweepHight = 0.f;

			bTargetRock = false;
			bAttack = true;
		}

		break;

	case 2:
		// 전진 3연타
		if (m_bAttack5 == false &&
			m_iAniIndex != 5 &&
			m_bAttack5_Revert == false)
		{
			m_pStateCom->Set_State(Engine::CObjState::NOW_ATTACK);
			m_pMeshCom->Set_AnimationSet(5);
			m_iAniIndex = 5;
			m_bAttack5 = true;
			m_pTransCom->m_vInfo[Engine::CTransform::INFO_POS].y = 19.f;
		}

		if (m_iAniIndex == 5 &&
			52 < m_pMeshCom->Get_Percent() &&
			m_pMeshCom->Get_Percent() < 53)
		{
			CSoundMgr::GetInstance()->StopSound(CSoundMgr::CHANNEL_BOSS_SWING);
			CSoundMgr::GetInstance()->PlaySound(L"Boss_Swing.wav", CSoundMgr::CHANNEL_BOSS_SWING);
		}

		if (m_iAniIndex == 5 &&
			70 < m_pMeshCom->Get_Percent() &&
			m_pMeshCom->Get_Percent() < 71)
		{
			CSoundMgr::GetInstance()->StopSound(CSoundMgr::CHANNEL_BOSS_SWING);
			CSoundMgr::GetInstance()->PlaySound(L"Boss_Swing.wav", CSoundMgr::CHANNEL_BOSS_SWING);
		}

		if (m_iAniIndex == 5 &&
			25 < m_pMeshCom->Get_Percent())
		{
			m_fFrontTimer += fTimeDelta;

			if (0.01f < m_fFrontTimer)
			{
				m_fFrontTimer = 0.f;
				m_fHight += 0.1f;

				if (m_fFrontAttact < m_fHight)
				{
					m_fHight = 0.8f;
				}
			}

			m_pTransCom->m_vInfo[Engine::CTransform::INFO_POS].y = 19.f;
			m_pTransCom->m_vInfo[Engine::CTransform::INFO_POS].y -= m_fHight;
		}

		if (m_iAniIndex == 5 &&
			m_pMeshCom->Is_AnimationSetEnd())
		{
			m_bAttack5 = false;
		}

		if (m_iAniIndex == 5 &&
			m_bAttack5 == false &&
			m_bAttack5_Revert == false)
		{
			m_bAttack5 = false;

			m_pMeshCom->Set_AnimationSet(6);
			m_iAniIndex = 6;

			m_bAttack5_Revert = true;
		}

		if (m_iAniIndex == 6 &&
			28 < m_pMeshCom->Get_Percent() &&
			m_pMeshCom->Get_Percent() < 30)
		{
			CSoundMgr::GetInstance()->StopSound(CSoundMgr::CHANNEL_BOSS_SWING);
			CSoundMgr::GetInstance()->PlaySound(L"Boss_Swing.wav", CSoundMgr::CHANNEL_BOSS_SWING);
		}
		
		if (m_iAniIndex == 6 &&
			m_bAttack5_Revert == true &&
			m_pMeshCom->Is_AnimationSetEnd())
		{
			m_pStateCom->Set_State(Engine::CObjState::NOW_IDLE);
			const Engine::D3DXFRAME_DERIVED* pFrame = m_pMeshCom->Get_FrameByName("M_ROOT");

			_matrix matMovePos = pFrame->CombinedTransformationMatrix;
			_matrix matPos = matMovePos * m_pTransCom->m_matWorld;

			m_pTransCom->Set_Position(_vec3(matPos.m[3][0], 19.f, matPos.m[3][2]));

			m_bAttack5_Revert = false;
			
			iValue = 0;

			m_fHight = 0.f;

			bTargetRock = false;
			bAttack = true;
		}
		
		break;

	case 3:
		// 점프공격
		if (m_bAttack4 == false &&
			m_iAniIndex != 4 &&
			m_bAttack4_Revert == false)
		{
			m_pStateCom->Set_State(Engine::CObjState::NOW_ATTACK);
			m_pMeshCom->Set_AnimationSet(4);
			m_iAniIndex = 4;
			m_bAttack4 = true;

			m_fBeforeY = m_pTransCom->m_vInfo[Engine::CTransform::INFO_POS].y;
		}

		m_fJumpTime += fTimeDelta;

		if (m_iAniIndex == 4 &&
			10 < m_pMeshCom->Get_Percent() &&
			m_pMeshCom->Get_Percent() < 20)
		{			
			if (0.01f < m_fJumpTime)
			{
				m_fJumpTime = 0.f;

				m_fJumpHeiht += 0.02f;
			}

			if (1.2f < m_fJumpHeiht)
			{
				m_fJumpHeiht = 1.2f;
			}

			m_pTransCom->m_vInfo[Engine::CTransform::INFO_POS].y = 19.f - m_fJumpHeiht;
		}

		if (m_iAniIndex == 4 &&
			20 < m_pMeshCom->Get_Percent() && 
			m_pMeshCom->Get_Percent() < 30)
		{
			m_fJumpHeiht = 0.f;
			m_fJumpTime = 0.f;
		}

		if (m_iAniIndex == 4 &&
			30 < m_pMeshCom->Get_Percent() &&
			m_pMeshCom->Get_Percent() < 55)
		{
			if (0.01f < m_fJumpTime)
			{
				m_fJumpTime = 0.f;

				m_fJumpHeiht += 0.006f;
			}

			if (17.8f < m_fJumpHeiht)
			{
				m_fJumpHeiht = 17.8f;
			}

			m_pTransCom->m_vInfo[Engine::CTransform::INFO_POS].y -= m_fJumpHeiht;

			if (m_pTransCom->m_vInfo[Engine::CTransform::INFO_POS].y < 17.8f)
			{
				m_pTransCom->m_vInfo[Engine::CTransform::INFO_POS].y = 17.8f;
			}
		}
		
		if (m_iAniIndex == 4 &&
			58 < m_pMeshCom->Get_Percent() &&
			m_pMeshCom->Get_Percent() < 65)
		{

			if (0.01f < m_fJumpTime)
			{
				m_fJumpTime = 0.f;

				m_fJumpHeiht += 0.1f;
			}

			if (2.2f < m_fJumpHeiht)
			{
				m_fJumpHeiht = 2.2f;
			}

			m_pTransCom->m_vInfo[Engine::CTransform::INFO_POS].y += m_fJumpHeiht;

			if (20.f < m_pTransCom->m_vInfo[Engine::CTransform::INFO_POS].y)
			{
				m_pTransCom->m_vInfo[Engine::CTransform::INFO_POS].y = 20.f;
			}

		}

		if (m_iAniIndex == 4 &&
			65 < m_pMeshCom->Get_Percent() &&
			m_pMeshCom->Get_Percent() < 68)
		{
			m_fJumpHeiht = 0.f;
			m_fJumpTime = 0.f;
		}


		if (m_iAniIndex == 4 &&
			48 < m_pMeshCom->Get_Percent() &&
			m_pMeshCom->Get_Percent() < 49)
		{
			CSoundMgr::GetInstance()->StopSound(CSoundMgr::CHANNEL_BOSS_SWING);
		}

		if (m_iAniIndex == 4 &&
			50 < m_pMeshCom->Get_Percent() &&
			m_pMeshCom->Get_Percent() < 52)
		{
			CSoundMgr::GetInstance()->PlaySound(L"Boss_Swing.wav", CSoundMgr::CHANNEL_BOSS_SWING);
		}

		if (m_iAniIndex == 4 &&
			60 < m_pMeshCom->Get_Percent() &&
			m_pMeshCom->Get_Percent() < 61)
		{
			CSoundMgr::GetInstance()->StopSound(CSoundMgr::CHANNEL_BOSS_SWING);
			CSoundMgr::GetInstance()->PlaySound(L"Boss_Swing.wav", CSoundMgr::CHANNEL_BOSS_SWING);
		}

		if (m_iAniIndex == 4 &&
			70 < m_pMeshCom->Get_Percent())
		{

			if (0.01f < m_fJumpTime)
			{
				m_fJumpTime = 0.f;

				m_fJumpHeiht += 0.02f;
			}

			if (2.2f < m_fJumpHeiht)
			{
				m_fJumpHeiht = 2.2f;
			}

			m_pTransCom->m_vInfo[Engine::CTransform::INFO_POS].y -= m_fJumpHeiht;

			if (m_pTransCom->m_vInfo[Engine::CTransform::INFO_POS].y < 17.8f)
			{
				m_pTransCom->m_vInfo[Engine::CTransform::INFO_POS].y = 17.8f;
			}
		}

		if (m_iAniIndex == 4 &&
			80 < m_pMeshCom->Get_Percent())
		{
			m_bAttack4 = false;
		}

		if (m_iAniIndex == 4 &&
			m_bAttack4 == false &&
			m_bAttack4_Revert == false)
		{
			m_bAttack4 = false;

			m_pMeshCom->Set_AnimationSet(9);
			m_iAniIndex = 9;

			m_bAttack4_Revert = true;
		}

		if (m_iAniIndex == 9 &&
			m_pMeshCom->Get_Percent() < 60)
		{
			m_fJumpTime = 0.f;
			m_fJumpHeiht = 0.f;
		}

		if (m_iAniIndex == 9 &&
			60 < m_pMeshCom->Get_Percent() )
		{
			if (0.01f < m_fJumpTime)
			{
				m_fJumpTime = 0.f;

				m_fJumpHeiht += 0.02f;
			}

			m_pTransCom->m_vInfo[Engine::CTransform::INFO_POS].y += m_fJumpHeiht;

			if (19.f < m_pTransCom->m_vInfo[Engine::CTransform::INFO_POS].y)
			{
				m_pTransCom->m_vInfo[Engine::CTransform::INFO_POS].y = 19.f;
			}
		}
		
		if (m_iAniIndex == 9 &&
			m_bAttack4_Revert == true &&
			m_pMeshCom->Is_AnimationSetEnd())
		{
			m_pStateCom->Set_State(Engine::CObjState::NOW_IDLE);
			m_bAttack4_Revert = false;

			const Engine::D3DXFRAME_DERIVED* pFrame = m_pMeshCom->Get_FrameByName("M_ROOT");

			_matrix matMovePos = pFrame->CombinedTransformationMatrix;
			_matrix matPos = matMovePos * m_pTransCom->m_matWorld;

			m_pTransCom->Set_Position(_vec3(matPos.m[3][0], m_pTransCom->m_vInfo[Engine::CTransform::INFO_POS].y, matPos.m[3][2]));
			
			iValue = 0;
			m_fJumpHeiht = 0.f; 
			m_fJumpTime = 0.f;
			bTargetRock = false;
			bAttack = true;
		}

		break;

	case 4:

		if (m_bWaveAttact == false &&
			m_iAniIndex != 13 &&
			m_iAniIndex != 14)
		{
			m_pStateCom->Set_State(Engine::CObjState::NOW_ATTACK);

			m_iAniIndex = 13;
			m_pMeshCom->Set_AnimationSet(13);
		}

		if (m_iAniIndex == 13 &&
			28 < m_pMeshCom->Get_Percent())
		{
			m_pTransCom->m_vInfo[Engine::CTransform::INFO_POS].y -= 0.6f;

			if (m_pTransCom->m_vInfo[Engine::CTransform::INFO_POS].y < 18.4f)
			{
				m_pTransCom->m_vInfo[Engine::CTransform::INFO_POS].y = 18.4f;
			}
		}

		if (m_iAniIndex == 13 &&
			77 < m_pMeshCom->Get_Percent())
		{
			m_pTransCom->m_vInfo[Engine::CTransform::INFO_POS].y += 0.6f;

			if (19.f < m_pTransCom->m_vInfo[Engine::CTransform::INFO_POS].y)
			{
				m_pTransCom->m_vInfo[Engine::CTransform::INFO_POS].y = 19.f;
			}
		}

		if (m_iAniIndex == 13 &&
			90 < m_pMeshCom->Get_Percent())
		{
			if (m_iAniIndex != 14)
			{
				m_fWaveAttact = false;
				m_iAniIndex = 14;
				m_pMeshCom->Set_AnimationSet(14);
			}
		}

		if (m_iAniIndex == 14 &&
			78 < m_pMeshCom->Get_Percent() &&
			m_pMeshCom->Get_Percent() < 80)
		{
			m_pStateCom->Set_State(Engine::CObjState::NOW_IDLE);

			const Engine::D3DXFRAME_DERIVED* pFrame = m_pMeshCom->Get_FrameByName("M_ROOT");

			_matrix matMovePos = pFrame->CombinedTransformationMatrix;
			_matrix matPos = matMovePos * m_pTransCom->m_matWorld;

			m_pTransCom->Set_Position(_vec3(matPos.m[3][0], m_pTransCom->m_vInfo[Engine::CTransform::INFO_POS].y, matPos.m[3][2]));
		}

		if (m_iAniIndex == 13 &&
			70 < m_pMeshCom->Get_Percent() &&
			m_pMeshCom->Get_Percent() < 71)
		{
			CSoundMgr::GetInstance()->StopSound(CSoundMgr::CHANNEL_BOSS_WAVE);
			CSoundMgr::GetInstance()->PlaySound(L"WaveSound.wav", CSoundMgr::CHANNEL_BOSS_WAVE);
		}

		if (m_fWaveAttact == false &&
			m_iAniIndex == 13 &&
			80 < m_pMeshCom->Get_Percent() &&
			m_pMeshCom->Get_Percent() < 81)
		{
			m_fWaveAttact = true;
			_vec3 vLook;

			vLook = m_pTransCom->m_vInfo[Engine::CTransform::INFO_LOOK];

			wstring strName = L"Boss_Wave", strVal, strString, strString2;
			string strValue;

			strValue = to_string(m_iWaveCnt);

			strVal.assign(strValue.begin(), strValue.end());

			strName += strVal;

			Engine::CGameObject* pWave = CBoss_Wave::Create(m_pGraphicDev, m_iWaveCnt, m_pTransCom->m_vInfo[Engine::CTransform::INFO_POS]);
			Engine::Get_Management()->Get_CurrentScene()->Get_Layer(L"Layer_GameLogic")->Add_GameObject(strName, pWave);

			// Boss_Wave0_Up
			strString = strName + L"_Up";

			// For.SwordTrail Boss Instance
			Engine::CGameObject* pGameObject = CSwordTrail::Create(m_pGraphicDev, L"Boss_Wave_Up", strName, L"Texture_SwordTrail00", m_iWaveCnt);
			Engine::Get_Management()->Get_CurrentScene()->Get_Layer(L"Layer_GameLogic")->Add_GameObject(strString, pGameObject);

			// Boss_Wave0_Down
			strString2 = strName + L"_Down";

			// For.SwordTrail Boss Instance
			pGameObject = CSwordTrail::Create(m_pGraphicDev, L"Boss_Wave_Down", strName, L"Texture_SwordTrail00", m_iWaveCnt);
			Engine::Get_Management()->Get_CurrentScene()->Get_Layer(L"Layer_GameLogic")->Add_GameObject(strString2, pGameObject);

			iValue = 0;
			bTargetRock = false;
			bAttack = true;
			m_iWaveCnt++;

		}

		break;
	}

}

void CBoss::Push_Player()
{
	_float fPlayerDist = D3DXVec3Length(&(m_pTargetCom->m_vInfo[Engine::CTransform::INFO_POS] - m_pTransCom->m_vInfo[Engine::CTransform::INFO_POS]));

	if (5.f < fPlayerDist)
	{
		return;
	}

	_vec3 vSrcPos;

	memcpy(&vSrcPos, &m_pTargetCom->m_matWorld.m[3][0], sizeof(_vec3));

	_float fLength = D3DXVec3Length(&(m_pTransCom->m_vInfo[Engine::CTransform::INFO_POS] - vSrcPos));

	Engine::CGameObject* pGameObject = Engine::Get_Management()->Get_CurrentScene()->Get_Object(L"Layer_GameLogic", L"Player");

	vector<Engine::CCollider*>* pSrcCollider = dynamic_cast<CPlayer*>(pGameObject)->Get_Collider();

	if (m_pCalculatorCom->Collision_OBB(m_vecCollider[0]->Get_Min(), m_vecCollider[0]->Get_Max(), &m_pTransCom->m_matWorld, (*pSrcCollider)[0]->Get_Min(), (*pSrcCollider)[0]->Get_Max(), &m_pTargetCom->m_matWorld, &vCollPos))
	{
		D3DXVec3Normalize(&vCollPos, &vCollPos);
		vCollPos *= 0.1f;
		vCollPos.y = 0.f;

		dynamic_cast<Engine::CTransform*>(m_pTargetCom)->m_vInfo[Engine::CTransform::INFO_POS] += vCollPos;
	}

}

HRESULT CBoss::Make_SquareCollision()
{
	Engine::CComponent* pComponent = nullptr;

	wfstream	pFile;
	_vec3		vPoint, vMax, vMin;
	_matrix		matPos;
	_tchar		szBore[64];
	_tchar		szSizeX[64], szSizeY[64], szSizeZ[64];
	wstring		strBore;

	pFile.open(L"../../Data/Collision_Boss_Square.txt", ios::in);

	while (!pFile.eof())
	{
		pFile.getline(szBore, 64, '|');

		m_vecBoreName.push_back(szBore);

		if (!lstrcmp(szBore, L""))
		{
			break;
		}

		pFile.getline(szSizeX, 64, '|');
		pFile.getline(szSizeY, 64, '|');
		pFile.getline(szSizeZ, 64, '|');

		vMax = _vec3((float)_ttof(szSizeX), (float)_ttof(szSizeY), (float)_ttof(szSizeZ));

		pFile.getline(szSizeX, 64, '|');
		pFile.getline(szSizeY, 64, '|');
		pFile.getline(szSizeZ, 64);

		vMin = _vec3((float)_ttof(szSizeX), (float)_ttof(szSizeY), (float)_ttof(szSizeZ));

		char* pStr;
		int strSize = WideCharToMultiByte(CP_ACP, 0, szBore, -1, NULL, 0, NULL, NULL);
		pStr = new char[strSize];
		WideCharToMultiByte(CP_ACP, 0, szBore, -1, pStr, strSize, 0, 0);

		const Engine::D3DXFRAME_DERIVED*		pFrame = m_pMeshCom->Get_FrameByName(pStr);

		delete pStr;

		matPos = pFrame->CombinedTransformationMatrix * *m_pTransCom->Get_WorldMatrixPointer();

		memcpy(&vPoint, &matPos.m[3][0], sizeof(_vec3));

		// For.Collider
		pComponent = Engine::CCollider::Create(m_pGraphicDev, &vMin, &vMax);
		NULL_CHECK_RETURN(pComponent, E_FAIL);

		m_vecCollider.push_back(dynamic_cast<Engine::CCollider*>(pComponent));
	}

	return S_OK;
}


void CBoss::Calc_Render()
{
	_matrix		matView;
	_vec3		vPosition, vCamPos, vTem;
	m_pTransCom->Get_Infomation(Engine::CTransform::INFO_POS, &vPosition);

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);

	memcpy(&vCamPos, &(matView.m[3][0]), sizeof(_vec3));

	vTem.x = abs(vCamPos.x);
	vTem.y = abs(vCamPos.y);
	vTem.z = abs(vCamPos.z);

	_float fDist = D3DXVec3Length(&(vTem - vPosition));

	if (fDist < 350.f)
	{
		m_bIsDraw = m_pOptimizationCom->Is_Frustum_ForObject(&vPosition, 15.f);
	}
	else
	{
		m_bIsDraw = false;
	}
}

void CBoss::Target_Look(_vec3 vDist, const Engine::_float & fTimeDelta)
{
	_vec3 vecTem;

	vecTem = vDist;

	D3DXVec3Normalize(&vecTem, &vecTem);

	_vec3 vRight;
	m_pTransCom->Get_Infomation(Engine::CTransform::INFO_RIGHT, &vRight);
	D3DXVec3Normalize(&vRight, &vRight);

	_float fConditionAngle = D3DXVec3Dot(&vecTem, &vRight) * -1.f;

	_float fCondition = roundf(D3DXToDegree(acosf(fConditionAngle)) * 100);

	_vec3 vLook;
	m_pTransCom->Get_Infomation(Engine::CTransform::INFO_LOOK, &vLook);
	D3DXVec3Normalize(&vLook, &vLook);
	_float fAngle = D3DXVec3Dot(&vLook, &vecTem) * -1.f;

	if (9000 != (int)fCondition)
	{
		if (m_iAniIndex == 0)
		{
			// Player가 왼쪽에 있는지 검사
			if (90.f < D3DXToDegree(acosf(fConditionAngle)))
			{
				m_pTransCom->Rotation(Engine::CTransform::ROT_Y, D3DXToRadian(-acosf(fAngle)));
			}
			else
			{
				m_pTransCom->Rotation(Engine::CTransform::ROT_Y, D3DXToRadian(acosf(fAngle)));
			}
		}
	}
}

void CBoss::Is_Nan()
{
	if (isnan(m_pTransCom->m_matWorld.m[0][0]))
	{
		memcpy(&m_pTransCom->m_matWorld.m[0][0], &_vec3(1.f, 0.f, 0.f), sizeof(_vec3));
		memcpy(&m_pTransCom->m_matWorld.m[1][0], &_vec3(0.f, 1.f, 0.f), sizeof(_vec3));
		memcpy(&m_pTransCom->m_matWorld.m[2][0], &_vec3(0.f, 0.f, 1.f), sizeof(_vec3));
		
		m_pTransCom->m_vAngle = _vec3(0.f, 0.f, 0.f);
	}
	else if (!isnan(m_pTransCom->m_matWorld.m[0][0]))
	{
		memcpy(&OriginRight, &m_pTransCom->m_matWorld.m[0][0], sizeof(_vec3));
		memcpy(&OriginvUp, &m_pTransCom->m_matWorld.m[1][0], sizeof(_vec3));
		memcpy(&OriginvPos, &m_pTransCom->m_matWorld.m[3][0], sizeof(_vec3));

		OriginAngle = m_pTransCom->m_vAngle;
	}
}

CBoss * CBoss::Create(LPDIRECT3DDEVICE9 pGraphicDev, 
						wstring tagMeshName, 
						Engine::_vec3 * temp_Right, 
						Engine::_vec3 * temp_Up, 
						Engine::_vec3 * temp_Look, 
						Engine::_vec3 * temp_Pos, 
						Engine::_vec3 * temp_Scale, 
						Engine::_vec3 * temp_Angle, 
						int iObjIndex)
{
	CBoss*		pInstance = new CBoss(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(tagMeshName, temp_Right, temp_Up, temp_Look, temp_Pos, temp_Scale, temp_Angle, iObjIndex)))
	{
		Engine::Safe_Release(pInstance);
		ERR_BOX(L"DynamicMesh, Create Load Failed");
	}

	return pInstance;
}

void CBoss::Free()
{
	Engine::CGameObject::Free();
}