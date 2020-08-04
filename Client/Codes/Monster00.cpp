#include "stdafx.h"
#include "Monster00.h"

#include "Export_Function.h"
#include "HitBlood.h"
#include "MonsterLifeBar.h"
#include "Player.h"

CMonster00::CMonster00(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{

}

CMonster00::~CMonster00()
{

}

HRESULT CMonster00::Ready_Object(wstring _tagMeshName
								, Engine::_vec3 * _temp_Right
								, Engine::_vec3 * _temp_Up
								, Engine::_vec3 * _temp_Look
								, Engine::_vec3 * _temp_Pos
								, Engine::_vec3 * _temp_Scale
								, Engine::_vec3 * _temp_Angle
								, int _iObjIndex)
{

	if (FAILED(Add_Component(_tagMeshName, _iObjIndex)))
		return E_FAIL;

	m_pTransCom->m_vInfo[0] = *_temp_Right;
	m_pTransCom->m_vInfo[1] = *_temp_Up;
	m_pTransCom->m_vInfo[2] = *_temp_Look;
	m_pTransCom->m_vInfo[3] = *_temp_Pos;

	OriginPos = *_temp_Pos;
	OriginRight = *_temp_Right;
	OriginvUp = *_temp_Up;

	m_pTransCom->Set_Scale(_temp_Scale->x, _temp_Scale->y, _temp_Scale->z);
	m_pTransCom->m_vAngle = *_temp_Angle;

	OriginAngle = *_temp_Angle;

	m_iIndex = _iObjIndex;

	m_pMeshCom->Set_AnimationSet(0);

	return S_OK;
}

Engine::_int CMonster00::Update_Object(const Engine::_float& fTimeDelta)
{
	Get_Target();

	Engine::_uint iExit = Engine::CGameObject::Update_Object(fTimeDelta);

	Calc_Render();

	Check_Nan();

	Calc_AttackAngle();

	Push_Overlap(fTimeDelta);
	Push_Player();

	if (m_pStateCom->m_fHp < 0 &&
		m_pStateCom->Get_State() == Engine::CObjState::NOW_DEAD)
	{
		if (m_iAniIndex != 1)
		{
			m_pMeshCom->Set_AnimationSet(1);
			m_iAniIndex = 1;

			CSoundMgr::GetInstance()->PlaySound(L"Monster_Dead.wav", CSoundMgr::CHANNEL_EFFECT);
		}

		if (55 < m_pMeshCom->Get_Percent())
		{
			m_Time += fTimeDelta;

			if (0.05f < m_Time)
			{
				fHeight += 0.001f;
				m_Time = 0.f;
			}

			if (1.f < fHeight)
			{
				fHeight = 1.f;
			}

			m_pTransCom->m_vInfo[Engine::CTransform::INFO_POS].y -= fHeight;
			
			if (m_pTransCom->m_vInfo[Engine::CTransform::INFO_POS].y <= 0)
			{
				if (abs(m_vOriginHeight - 1.f) < abs(m_pTransCom->m_vInfo[Engine::CTransform::INFO_POS].y))
				{
					m_pTransCom->m_vInfo[Engine::CTransform::INFO_POS].y = m_vOriginHeight - 1.f;
				}
			}

			if (m_pTransCom->m_vInfo[Engine::CTransform::INFO_POS].y > 0)
			{
				_float fValue = m_pTransCom->m_vInfo[Engine::CTransform::INFO_POS].y - fHeight;
				
				if (fValue < m_vOriginHeight - 1.f)
				{
					m_pTransCom->m_vInfo[Engine::CTransform::INFO_POS].y = m_vOriginHeight - 1.f;
				}
			}
			
		}

		if (!m_pMeshCom->Is_AniSetEnd())
		{
			m_pMeshCom->Play_AnimationSet(fTimeDelta);
			m_pRendererCom->Add_RenderGroup(Engine::CRenderer::RENDER_NONALPHA, this);

			return 0;
		}
		else
		{
			Engine::CGameObject* pGameObject = Engine::Get_Management()->Get_Object(L"Layer_UI", L"MonsterLifeBar", m_iIndex);

			dynamic_cast<CMonsterLifeBar*>(pGameObject)->SetUp_Dead();

			return -1;
		}
		
	}
	else
	{
		m_vOriginHeight = m_pTransCom->m_vInfo[Engine::CTransform::INFO_POS].y;
	}

	Check_Hit(fTimeDelta);

	if (m_bAttackCoolTime == true)
	{
		m_fAttackCoolTime += fTimeDelta;

		if (15.f < m_fAttackCoolTime)
		{
			m_bAttackCoolTime = false;
		}
	}

	m_pMeshCom->Play_AnimationSet(fTimeDelta);
	
	Search_Length(fTimeDelta);
	
	if (m_pMeshCom->Is_AnimationSetEnd())
	{
		m_pMeshCom->Set_AnimationSet(0);
		m_iAniIndex = 0;
	}
	
	m_pRendererCom->Add_RenderGroup(Engine::CRenderer::RENDER_NONALPHA, this);
	
	return 0;
}

void CMonster00::Render_Object()
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

HRESULT CMonster00::Add_Component(wstring _tagMeshName, int _iObjIndex)
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
	
	string strTem;

	strTem = to_string(_iObjIndex);
	
	wstring strTem2;
	
	strTem2.assign(strTem.begin(), strTem.end());

	_tagMeshName += strTem2;

	// For.Mesh
	pComponent = m_pMeshCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Clone_Resource(RESOURCE_STAGE, _tagMeshName));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::CComponent::ID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Mesh", pComponent));
	
	map<wstring, _float>		mapBore;
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
	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_DefferdMesh"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::CComponent::ID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Shader", pComponent));

	// For.Optimization
	pComponent = m_pOptimizationCom = dynamic_cast<Engine::COptimization*>(Engine::Clone_Prototype(L"Proto_Optimization"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::CComponent::ID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Optimization", pComponent));

	return S_OK;
}

HRESULT CMonster00::Make_SquareCollision()
{
	Engine::CComponent* pComponent = nullptr;

	wfstream	pFile;
	_vec3		vPoint, vMax, vMin;
	_matrix		matPos;
	_tchar		szBore[64];
	_tchar		szSizeX[64], szSizeY[64], szSizeZ[64];
	wstring		strBore;

	pFile.open(L"../../Data/Collision_Monster00_Square.txt", ios::in);

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

void CMonster00::Push_Overlap(const _float& fTimeDelta)
{	
	_float fPlayerDist = D3DXVec3Length(&(m_pTargetCom->m_vInfo[Engine::CTransform::INFO_POS] - m_pTransCom->m_vInfo[Engine::CTransform::INFO_POS]));

	if (5.f < fPlayerDist)
	{
		return;
	}

	multimap<const wstring, Engine::CGameObject*> multiMap = Engine::Get_Management()->Get_CurrentScene()->Get_Layer(L"Layer_GameLogic")->Get_ObjectMultiMap();

	int iCount = multiMap.count(L"Monster00");
	
	if (iCount <= 1)
		return;

	auto& iter = multiMap.find(L"Monster00");
	for (int i = 0; i < iCount; ++i, ++iter)
	{
		if (iter->second->GetIndex() == m_iIndex) 
			continue;

		const _matrix matSrcWorld = dynamic_cast<Engine::CTransform*>(iter->second->Get_Component(L"Com_Transform", Engine::CComponent::ID_DYNAMIC))->m_matWorld;
		_vec3 vSrcPos;

		memcpy(&vSrcPos, &matSrcWorld.m[3][0], sizeof(_vec3));
		
		_float fLength = D3DXVec3Length(&(m_pTransCom->m_vInfo[Engine::CTransform::INFO_POS] - vSrcPos));
		if (fPushLang < fLength)
			continue;

		vector<Engine::CCollider*>* pSrcCollider = dynamic_cast<CMonster00*>(iter->second)->Get_vecColl();
	
		if (m_pCalculatorCom->Collision_OBB(m_vecCollider[0]->Get_Min(), m_vecCollider[0]->Get_Max(), &m_pTransCom->m_matWorld, (*pSrcCollider)[0]->Get_Min(), (*pSrcCollider)[0]->Get_Max(), &matSrcWorld, &vCollPos))
		{
			D3DXVec3Normalize(&vCollPos, &vCollPos);
			vCollPos *= fOriginX;
			vCollPos.y = fOriginY;

			dynamic_cast<Engine::CTransform*>(iter->second->Get_Component(L"Com_Transform", Engine::CComponent::ID_DYNAMIC))->m_vInfo[Engine::CTransform::INFO_POS] += vCollPos;
		}
	}
}

void CMonster00::Calc_AttackAngle()
{
	_vec3 vTem, vLook;
	vTem = m_pTargetCom->m_vInfo[Engine::CTransform::INFO_POS] - m_pTransCom->m_vInfo[Engine::CTransform::INFO_POS];
	D3DXVec3Normalize(&vTem, &vTem);

	vLook = m_pTransCom->m_vInfo[Engine::CTransform::INFO_LOOK];
	
	D3DXVec3Normalize(&vLook, &vLook);

	_float fConditionAngle = D3DXVec3Dot(&vTem, &vLook);

	m_fAttactAngle = D3DXToDegree(acosf(fConditionAngle));

}

void CMonster00::Check_Hit(const Engine::_float& fTimeDelta)
{
	// 맞은 후 애니메이션 & 딜레이
	if (IsbHit == true)
	{
		if (m_bHitEffect == false)
		{
			m_bHitEffect = true;

			Engine::CGameObject* pBlood = CHitBlood::Create(m_pGraphicDev, &m_pTransCom->m_matWorld);
			Engine::Get_Management()->Get_CurrentScene()->Get_Layer(L"Layer_GameLogic")->Add_GameObject(L"Hit_Blood", pBlood);
		}

		m_dHitDelay += fTimeDelta;

		if (m_iAniIndex != animationIndex[6])
		{
			m_pMeshCom->Set_AnimationSet(animationIndex[6]);
			m_iAniIndex = animationIndex[6];
		}

		if (fCheckDelay < m_dHitDelay)
		{
			m_dHitDelay = fDelay;
			IsbHit = false;

			m_bHitEffect = false;

			m_bAttack = false;
			m_bAttack_Revert = false;
		}
	}
}

void CMonster00::Push_Player()
{
	_float fPlayerDist = D3DXVec3Length(&(m_pTargetCom->m_vInfo[Engine::CTransform::INFO_POS] - m_pTransCom->m_vInfo[Engine::CTransform::INFO_POS]));

	if (m_fAttackLang < fPlayerDist)
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
		vCollPos *= fOriginX;
		vCollPos.y = fOriginY;

		dynamic_cast<Engine::CTransform*>(m_pTargetCom)->m_vInfo[Engine::CTransform::INFO_POS] += vCollPos;
	}
	
}

HRESULT CMonster00::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	pEffect->AddRef();

	_matrix		matWorld, matView, matProj;

	m_pTransCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);
	pEffect->SetFloat("g_fAlpha", 0.5f);
	
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
	MtrlInfo.Specular = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.f);
	MtrlInfo.Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.f);
	MtrlInfo.Emissive = D3DXCOLOR(0.f, 0.f, 0.f, 1.f);

	MtrlInfo.Power = 50000.f;

	pEffect->SetVector("g_vMtrlDiffuse", (_vec4*)&MtrlInfo.Diffuse);
	pEffect->SetVector("g_vMtrlSpecular", (_vec4*)&MtrlInfo.Specular);
	pEffect->SetVector("g_vMtrlAmbient", (_vec4*)&MtrlInfo.Ambient);
	pEffect->SetFloat("g_fPower", MtrlInfo.Power);

	D3DXMatrixInverse(&matView, nullptr, &matView);
	pEffect->SetVector("g_vCamPosition", (_vec4*)&matView.m[3][0]);

	Engine::Safe_Release(pEffect);

	return S_OK;
}

HRESULT CMonster00::Make_BoreNames(map<wstring, _float>* pmapBore)
{
	TCHAR szBoreName[64] = L"", szBoreSize[64] = L"";
	DWORD dwByte = 99;

	wifstream	LoadFile;

	LoadFile.open(L"../../Data/Collision_Monster00.txt", ios::in);

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

void CMonster00::Get_Target()
{
	if (m_pTargetCom == nullptr)
	{
		Engine::CComponent* pComponent = Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Com_Transform", Engine::CComponent::ID_DYNAMIC);
		NULL_CHECK(pComponent);

		m_pTargetCom = dynamic_cast<Engine::CTransform*>(pComponent);
	}
}

void CMonster00::Search_Length(const _float& fTimeDelta)
{
	// Player를 바라보는 방향벡터
	_vec3 vecDist = m_pTargetCom->m_vInfo[Engine::CTransform::INFO_POS] - m_pTransCom->m_vInfo[Engine::CTransform::INFO_POS];
	_float fDist = D3DXVec3Length(&vecDist);
	
	if (fDist < m_fMaxSearch &&
		m_fMinSearch < fDist &&
		IsbHit == false &&
		m_bAttack == false &&
		m_bAttack_Revert == false)
	{
		_vec3 vTemp;

		vTemp = Target_Look(&vecDist, fTimeDelta);

		D3DXVec3Normalize(&vTemp, &vTemp);

		Engine::_vec3 vPos, vDir, vDest;
		m_pTransCom->Get_Infomation(Engine::CTransform::INFO_POS, &vPos);
		m_pTransCom->Get_Infomation(Engine::CTransform::INFO_LOOK, &vDir);
		D3DXVec3Normalize(&vDir, &vDir);

		D3DXVec3Normalize(&vDest, &vDest);

		m_pTransCom->m_vInfo[Engine::CTransform::INFO_POS] += vTemp * fTimeDelta * fRotateSpeef;
		
		m_pMeshCom->Set_AnimationSet(animationIndex[3]);
		m_iAniIndex = animationIndex[3];
	}

	if (fDist < 2.5f &&
		IsbHit == false &&
		m_bAttack == false &&
		m_bAttack_Revert == false)
	{
		Target_Look(&vecDist, fTimeDelta);
	}

	if (fDist < m_fAttackLang)
	{
		srand((unsigned int)time(nullptr));

		m_iState = (rand() % 3) + 1;

		if (m_iState != 0 &&
			m_bMovePattern == false)
		{
			m_bMovePattern = true;
		}

	}

	switch (m_iState)
	{
	case 1:

		// 오른쪽으로 이동
		if (fDist < 5.f &&
			IsbHit == false &&
			m_iAniIndex != 3 &&
			m_iAniIndex != 2 &&
			m_bAttack == false &&
			m_bAttack_Revert == false &&
			m_iAniIndex != 5 &&
			m_bAttackCoolTime == false &&
			m_fAttactAngle < 15.f &&
			-15.f < m_fAttactAngle)
		{
			m_pStateCom->Set_State(Engine::CObjState::NOW_IDLE);
			m_pMeshCom->Set_AnimationSet(6);

			m_iAniIndex = 6;
			_vec3 vMoveDir;

			m_pTransCom->Get_Infomation(Engine::CTransform::INFO_RIGHT, &vMoveDir);
			D3DXVec3Normalize(&vMoveDir, &vMoveDir);

			m_pTransCom->m_vInfo[Engine::CTransform::INFO_POS] += vMoveDir * fTimeDelta * 0.5f;

			if (m_bMovePattern == true)
			{
				m_fMoveTime += fTimeDelta;

				if (3.f < m_fMoveTime)
				{
					m_bMovePattern = false;
				}
			}
		}
		break;

	case 2:
		// 왼쪽으로 이동
		if (fDist < 5.f &&
			IsbHit == false &&
			m_iAniIndex != 3 &&
			m_iAniIndex != 2 &&
			m_bAttack == false &&
			m_bAttack_Revert == false &&
			m_iAniIndex != 5 &&
			m_bAttackCoolTime == false &&
			m_fAttactAngle < 15.f &&
			-15.f < m_fAttactAngle)
		{
			m_pStateCom->Set_State(Engine::CObjState::NOW_IDLE);
			m_pMeshCom->Set_AnimationSet(7);

			m_iAniIndex = 6;
			_vec3 vMoveDir;

			m_pTransCom->Get_Infomation(Engine::CTransform::INFO_RIGHT, &vMoveDir);
			D3DXVec3Normalize(&vMoveDir, &vMoveDir);

			m_pTransCom->m_vInfo[Engine::CTransform::INFO_POS] -= (vMoveDir * fTimeDelta * 0.5f);

			if (m_bMovePattern == true)
			{
				m_fMoveTime += fTimeDelta;

				if (3.f < m_fMoveTime)
				{
					m_bMovePattern = false;
				}
			}
		}

		break;

	case 3:

		if (fDist < 5 &&
			IsbHit == false &&
			m_iAniIndex != 3 &&
			m_iAniIndex != 2 &&
			m_bAttack == false &&
			m_bAttack_Revert == false &&
			m_iAniIndex != 5 &&
			m_bAttackCoolTime == false &&
			m_fAttactAngle < 15.f &&
			-15.f < m_fAttactAngle)
		{
			m_pMeshCom->Set_AnimationSet(3);
			m_iAniIndex = 3;
			m_bAttack = true;

			Target_Look(&vecDist, fTimeDelta);
		}
		break;
	}

	if (m_iAniIndex == 3 &&
		50 < m_pMeshCom->Get_Percent())
	{
		m_pStateCom->Set_State(Engine::CObjState::NOW_ATTACK);
	}

	if (m_iAniIndex == 3 &&
		IsbHit == false &&
		m_bAttack == true &&
		92 < m_pMeshCom->Get_Percent())
	{
		m_pStateCom->Set_State(Engine::CObjState::NOW_IDLE);
		m_bAttack = false;

		m_pMeshCom->Set_AnimationSet(4);
		m_iAniIndex = 4;


		m_bAttack_Revert = true;

		Target_Look(&vecDist, fTimeDelta);
	}

	if (m_iAniIndex == 4 &&
		m_bAttack_Revert == true &&
		95 < m_pMeshCom->Get_Percent())
	{
		m_bAttack_Revert = false;
		m_bAttackCoolTime = true;

		const Engine::D3DXFRAME_DERIVED* pFrame = m_pMeshCom->Get_FrameByName("M_ROOT");

		_matrix matMovePos = pFrame->CombinedTransformationMatrix;
		_matrix matPos = matMovePos * m_pTransCom->m_matWorld;

		m_pTransCom->Set_Position(_vec3(matPos.m[3][0], m_pTransCom->m_vInfo[Engine::CTransform::INFO_POS].y, matPos.m[3][2]));
	}
	
	if (fDist < 3.f &&
		IsbHit == false &&
		m_iAniIndex != 4 &&
		0 < m_pStateCom->Get_Hp())
	{
		Target_Look(&vecDist, fTimeDelta);
	}

}

_vec3 CMonster00::Target_Look(_vec3* vecDest, const _float& fTimeDelta)
{
	_vec3 vecTem;

	vecTem = *vecDest;

	D3DXVec3Normalize(&vecTem, &vecTem);

	_vec3 vRight;
	m_pTransCom->Get_Infomation(Engine::CTransform::INFO_RIGHT, &vRight);
	D3DXVec3Normalize(&vRight, &vRight);

	_float fConditionAngle = D3DXVec3Dot(&vecTem, &vRight) * -1.f;

	_float fCondition = round(D3DXToDegree(acosf(fConditionAngle)) * 100);
	
	_vec3 vLook;
	m_pTransCom->Get_Infomation(Engine::CTransform::INFO_LOOK, &vLook);
	D3DXVec3Normalize(&vLook, &vLook);
	_float fAngle = D3DXVec3Dot(&vLook, &vecTem) * -1.f;

	// Player가 왼쪽에 있는지 검사
	
	if (9000 != fCondition)
	{
		if (90.f < D3DXToDegree(acosf(fConditionAngle)))
		{
			m_pTransCom->Rotation(Engine::CTransform::ROT_Y, D3DXToRadian(-acosf(fAngle) * 5.f));
		}
		else
		{
			m_pTransCom->Rotation(Engine::CTransform::ROT_Y, D3DXToRadian(acosf(fAngle) * 5.f));
		}
	}

	D3DXVec3Normalize(&vecTem, &vecTem);

	return vecTem;
}

void CMonster00::Calc_Render()
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

	if (fDist < 250.f)
	{
		m_bIsDraw = m_pOptimizationCom->Is_Frustum_ForObject(&vPosition, 15.f);
	}
	else
	{
		m_bIsDraw = false;
	}
}

void CMonster00::Check_Nan()
{
	if (isnan(m_pTransCom->m_matWorld.m[0][0]))
	{
		memcpy(&m_pTransCom->m_matWorld.m[0][0], &OriginRight, sizeof(_vec3));
		memcpy(&m_pTransCom->m_matWorld.m[1][0], &OriginvUp, sizeof(_vec3));
		memcpy(&m_pTransCom->m_matWorld.m[2][0], &_vec3(0.f, 1.f, 0.f), sizeof(_vec3));

		m_pTransCom->m_vAngle = OriginAngle;
	}
}

CMonster00 * CMonster00::Create(LPDIRECT3DDEVICE9 pGraphicDev,
								wstring tagMeshName,
								Engine::_vec3 * temp_Right,
								Engine::_vec3 * temp_Up,
								Engine::_vec3 * temp_Look,
								Engine::_vec3 * temp_Pos,
								Engine::_vec3 * temp_Scale,
								Engine::_vec3 * temp_Angle,
								int iObjIndex)
{
	CMonster00*		pInstance = new CMonster00(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(tagMeshName, temp_Right, temp_Up, temp_Look, temp_Pos, temp_Scale, temp_Angle, iObjIndex)))
	{
		Engine::Safe_Release(pInstance);
		ERR_BOX(L"DynamicMesh, Create Load Failed");
	}

	return pInstance;
}

void CMonster00::Free()
{

	for_each(m_vecCollider.begin(), m_vecCollider.end(), Engine::CDeleteObj());
	
	m_vecCollider.clear();
	m_vecBoreName.clear();

	Engine::CGameObject::Free();
}

