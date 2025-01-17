#include "pch.h"
#include "../Header/BlackMan.h"
#include "Export_System.h"
#include "Export_Utility.h"
#include "../Header/Shield.h"
#include "../Header/Player.h"

CBlackMan::CBlackMan(LPDIRECT3DDEVICE9 _pGraphicDev) : 
    CHumanoid(_pGraphicDev)
	, m_pShield(nullptr)
	, m_bIsShield(true)
	, m_bIsFormChange(false)
{
	m_fFireDelayTime = 5.f;
	m_fAttackTimer = 6.f;
	for (_int i = 0; i < SHIELDSTATE::SHIELDSTATE_END; ++i)
		m_pShieldTextureCom[i] = nullptr;
}

CBlackMan::CBlackMan(LPDIRECT3DDEVICE9 _pGraphicDev, _vec3 _vecPos)
	: CHumanoid(_pGraphicDev)
	, m_pShield(nullptr)
	, m_bIsShield(true)
	, m_bIsFormChange(false)
{
	m_fFireDelayTime = 5.f;
	m_fAttackTimer = 6.f;
	for (_int i = 0; i < SHIELDSTATE::SHIELDSTATE_END; ++i)
		m_pShieldTextureCom[i] = nullptr;

	m_vStartPos = _vecPos;
}

CBlackMan::~CBlackMan()
{
}

CBlackMan* CBlackMan::Create(LPDIRECT3DDEVICE9 _pGraphicDev, CGameObject* _pShield)
{
	CBlackMan* pMonster = new CBlackMan(_pGraphicDev);

	if (FAILED(pMonster->Ready_GameObject()))
	{
		Safe_Release(pMonster);
		MSG_BOX("BlackMan Create Failed");
		return nullptr;
	}
	pMonster->Set_Shield(_pShield);
	return pMonster;
}

CBlackMan* CBlackMan::Create(LPDIRECT3DDEVICE9 _pGraphicDev, CGameObject* _pShield, _vec3 _vecPos)
{
	CBlackMan* pMonster = new CBlackMan(_pGraphicDev, _vecPos);

	if (FAILED(pMonster->Ready_GameObject()))
	{
		Safe_Release(pMonster);
		MSG_BOX("BlackMan Create Failed");
		return nullptr;
	}
	pMonster->Set_Shield(_pShield);
	return pMonster;
}

HRESULT CBlackMan::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_vStartPos.y += 1.f;
	//m_pTransformCom->Set_Pos(6.f, 1.f, 8.f);
	m_pTransformCom->Set_Pos(m_vStartPos);
	_vec3 vDir = { 0.5f, 0.5f, 0.5f };

	m_pColliderCom->SetTransform(m_pTransformCom);
	m_pColliderCom->SetRadius(1.f);
	m_pColliderCom->SetLookDir(vDir);
	m_pColliderCom->SetShow(true);
	m_pColliderCom->SetActive(true);

	_int iTemp = rand() % 20;
	m_fAttackTimer = (iTemp + 1) / 2.f;

	Set_Animation();
	m_pHitBufferCom->Set_Hit_Parts(CRcCol::HIT_BODY);
	m_pHeadHit->Set_Hit_Parts(CRcCol::HIT_HEAD);
	m_pCriticalHit->Set_Hit_Parts(CRcCol::HIT_CRITICAL);
	return S_OK;
}

_bool CBlackMan::Get_Execution(_vec3 _vLook, const _bool& _bIsDo)
{
	if (!m_bIsExecution && _bIsDo)
	{
		m_bIsExecution = !m_bIsExecution;
		m_vPlayerLook = _vLook;
		Changing_State(CHumanoid::HUMANOID_EXECUTION);
		m_pAnimatorCom->PlayAnimation(L"Execution", false);
		m_fViewZ = 11.f;
		m_pTransformCom->Set_Scale({ 375.f, 375.f, 0.5f });
		m_bIsShield = false;
	}

	return true;
}

HRESULT CBlackMan::Add_Component()
{
	CComponent* pComponent = NULL;

	pComponent = m_pHitBufferCom = dynamic_cast<CRcCol*>(Engine::Clone_Proto(L"Proto_HitBufferCom"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_HitBufferCom", pComponent });
	pComponent->SetOwner(*this);

	pComponent = m_pHeadHit = dynamic_cast<CRcCol*>(Engine::Clone_Proto(L"Proto_HitBufferCom"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_HeadHit", pComponent });
	pComponent->SetOwner(*this);

	pComponent = m_pCriticalHit = dynamic_cast<CRcCol*>(Engine::Clone_Proto(L"Proto_HitBufferCom"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_CriticalHit", pComponent });
	pComponent->SetOwner(*this);


	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Buffer", pComponent });

	pComponent = m_pShieldTextureCom[SHIELDSTATE::SHIELDSTATE_ATTACK] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_BlackManShieldAttackTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_ShieldAttackTexture", pComponent });

	pComponent = m_pShieldTextureCom[SHIELDSTATE::SHIELDSTATE_IDLE] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_BlackManShieldIdleTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_ShieldIdleTexture", pComponent });

	pComponent = m_pTextureCom[HUMANOIDSTATE::HUMANOID_IDLE] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_BlackManIdleTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_IdleTexture", pComponent });

	pComponent = m_pTextureCom[HUMANOIDSTATE::HUMANOID_ATTACK] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_BlackManAttackTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_AttackTexture", pComponent });

	pComponent = m_pTextureCom[HUMANOIDSTATE::HUMANOID_ATTACK_DELAY] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_BlackManAttackDelayTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_AttackDelayTexture", pComponent });

	pComponent = m_pTextureCom[HUMANOIDSTATE::HUMANOID_HEADSHOT] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_BlackManHeadShotTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_HeadShotTexture", pComponent });

	pComponent = m_pTextureCom[HUMANOIDSTATE::HUMANOID_BULLSHOT] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_BlackManBullShotTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_BullShotTexture", pComponent });

	pComponent = m_pTextureCom[HUMANOIDSTATE::HUMANOID_PUSH_ONE] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_BlackManPushOneTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_PushOneTexture", pComponent });

	pComponent = m_pTextureCom[HUMANOIDSTATE::HUMANOID_PUSH_TWO] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_BlackManPushTwoTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_PushTwoTexture", pComponent });

	pComponent = m_pTextureCom[HUMANOIDSTATE::HUMANOID_SHOT_ONE] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_BlackManShotOneTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_ShotOneTexture", pComponent });

	pComponent = m_pTextureCom[HUMANOIDSTATE::HUMANOID_SHOT_TWO] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_BlackManShotTwoTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_ShotTwoTexture", pComponent });

	pComponent = m_pTextureCom[HUMANOIDSTATE::HUMANOID_KATANA] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_BlackManKatanaDownTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_KatanaTexture", pComponent });

	pComponent = m_pTextureCom[HUMANOIDSTATE::HUMANOID_EXECUTION] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_BlackManExecutionTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_ExecutionTexture", pComponent });

	pComponent = m_pCalculatorCom = dynamic_cast<CCalculator*>(Engine::Clone_Proto(L"Proto_Calculator"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Calculator", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

	pComponent = m_pColliderCom = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Proto_Collider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Collider", pComponent });
	pComponent->SetOwner(*this);

	pComponent = m_pAnimatorCom = dynamic_cast<CAnimator*>(Engine::Clone_Proto(L"Proto_Animator"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Animator", pComponent });

	return S_OK;
}

void CBlackMan::State_Check()
{
	if (m_eCurState != m_ePreState || m_bIsFormChange)
	{
		switch (m_eCurState)
		{
		case CHumanoid::HUMANOID_IDLE:
			if (!m_bIsShield)
				m_pAnimatorCom->PlayAnimation(L"Idle", true);
			else
				m_pAnimatorCom->PlayAnimation(L"Idle_Shield", true);
			break;
		case CHumanoid::HUMANOID_ATTACK:
			if (!m_bIsShield)
				m_pAnimatorCom->PlayAnimation(L"Attack", false);
			else
				m_pAnimatorCom->PlayAnimation(L"Attack_Shield", false);
			m_bIsFire = false;
			m_fAttackTimer = 0.f;
			break;
		case CHumanoid::HUMANOID_ATTACK_DELAY:
			if (!m_bIsShield)
				m_pAnimatorCom->PlayAnimation(L"Attack_Delay", true);
			else
				m_pAnimatorCom->PlayAnimation(L"Attack_Delay_Shield", true);
			break;
		case CHumanoid::HUMANOID_HEADSHOT:
			m_pAnimatorCom->PlayAnimation(L"HeadShot", false);
			break;
		case CHumanoid::HUMANOID_PUSH_ONE:
			m_pAnimatorCom->PlayAnimation(L"Push_One", false);
			break;
		case CHumanoid::HUMANOID_PUSH_TWO:
			m_pAnimatorCom->PlayAnimation(L"Push_Two", false);
			break;
		case CHumanoid::HUMANOID_BULLSHOT:
			m_pAnimatorCom->PlayAnimation(L"BullShot", false);
			break;
		case CHumanoid::HUMANOID_SHOT_ONE:
			m_pAnimatorCom->PlayAnimation(L"Shot_One", false);
			break;
		case CHumanoid::HUMANOID_SHOT_TWO:
			m_pAnimatorCom->PlayAnimation(L"Shot_Two", false);
			break;
		case CHumanoid::HUMANOID_KATANA:
			m_pAnimatorCom->PlayAnimation(L"Katana", false);
			break;
		case CHumanoid::HUMANOID_EXECUTION:
			m_pAnimatorCom->PlayAnimation(L"Execution", false);
			break;
		}
		m_bIsFormChange = false;
		m_ePreState = m_eCurState;
	}
}

void CBlackMan::Attack(const _float& _fTimeDelta)
{
	if (m_bIsExecution && m_pAnimatorCom->GetCurrAnim()->GetFinish())
	{
		m_bIsExecution = false;
		Changing_State(CHumanoid::HUMANOID_IDLE);
		m_pTransformCom->Set_Scale({ 1.f, 1.f, 1.f });
		AddForce(16.5f, m_vPlayerLook, 15.f);
		CComponent* pComponent = Engine::Get_Component(COMPONENTID::ID_DYNAMIC, L"Layer_Effect", L"EffectHeal", L"Com_Effect");
		static_cast<CEffect*>(pComponent)->Set_Visibility(TRUE);

		pComponent = Engine::Get_Component(COMPONENTID::ID_DYNAMIC, L"Layer_Effect", L"EffectGreenFlash", L"Com_Effect");
		if (pComponent)
			static_cast<CEffect*>(pComponent)->Operate_Effect();

		m_fAttackTimer = 0.f;

		return;
	}
	else if (m_bIsExecution)
		return;

	_vec3 vPos, vPlayerPos, vDir, vUp, vRight;

	if (nullptr == m_pPlayerTransformCom)
	{
		m_pPlayerTransformCom = dynamic_cast<Engine::CTransform*>
			(Engine::Get_Component(COMPONENTID::ID_DYNAMIC, L"Layer_Player", L"Player", L"Com_Body_Transform"));
		NULL_CHECK(m_pPlayerTransformCom, -1);
	}

	m_pPlayerTransformCom->Get_Info(INFO::INFO_POS, &vPlayerPos);
	m_pTransformCom->Get_Info(INFO::INFO_POS, &vPos);
	if (m_bIsShield)
	{
		m_pTransformCom->Get_Info(INFO::INFO_UP, &vUp);
		vPos.y += 0.5f;
		vDir = vPlayerPos - vPos;
		D3DXVec3Cross(&vRight, &vUp, &vDir);
		D3DXVec3Normalize(&vRight, &vRight);
		vPos += (vRight * 0.4f);
		vDir = vPlayerPos - vPos;
	}
	else
	{
		vPos.y += 0.65f;
		vDir = vPlayerPos - vPos;
	}

	if (15.f < D3DXVec3Length(&vDir))
	{
		Changing_State(CHumanoid::HUMANOID_IDLE);
	}
	else
	{
		if (m_fAttackTimer > m_fFireDelayTime || m_eCurState == CHumanoid::HUMANOID_IDLE)
		{
			if (!m_bIsExecution)
			{
				Changing_State(CHumanoid::HUMANOID_ATTACK);
				D3DXVec3Normalize(&vDir, &vDir);
				Engine::Fire_Bullet(m_pGraphicDev, vPos, vDir, 5, CBulletManager::BULLET_PISTOL);
				Engine::Play_Sound(L"Pistol.wav", CHANNELID::SOUND_ENEMY_GUN, 0.8f);
				m_bIsFire = true;
			}
		}
		else if (m_pAnimatorCom->GetCurrAnim()->GetFinish())
		{
			Changing_State(CHumanoid::HUMANOID_ATTACK_DELAY);
		}
	}

	if (m_eCurState == CHumanoid::HUMANOID_ATTACK_DELAY)
	{
		m_fAttackTimer += _fTimeDelta; // check delay, Player is not far from it
	}
}

void CBlackMan::Set_Animation()
{
	m_pAnimatorCom->CreateAnimation(L"Idle", m_pTextureCom[HUMANOID_IDLE], 13.f);
	m_pAnimatorCom->CreateAnimation(L"Attack", m_pTextureCom[HUMANOID_ATTACK], 13.f);
	m_pAnimatorCom->CreateAnimation(L"Attack_Delay", m_pTextureCom[HUMANOID_ATTACK_DELAY], 13.f);
	m_pAnimatorCom->CreateAnimation(L"HeadShot", m_pTextureCom[HUMANOID_HEADSHOT], 13.f);
	m_pAnimatorCom->CreateAnimation(L"BullShot", m_pTextureCom[HUMANOID_BULLSHOT], 13.f);
	m_pAnimatorCom->CreateAnimation(L"Push_One", m_pTextureCom[HUMANOID_PUSH_ONE], 13.f);
	m_pAnimatorCom->CreateAnimation(L"Push_Two", m_pTextureCom[HUMANOID_PUSH_TWO], 13.f);
	m_pAnimatorCom->CreateAnimation(L"Shot_One", m_pTextureCom[HUMANOID_SHOT_ONE], 13.f);
	m_pAnimatorCom->CreateAnimation(L"Shot_Two", m_pTextureCom[HUMANOID_SHOT_TWO], 13.f);
	m_pAnimatorCom->CreateAnimation(L"Idle_Shield", m_pShieldTextureCom[SHIELDSTATE_IDLE], 13.f);
	m_pAnimatorCom->CreateAnimation(L"Attack_Shield", m_pShieldTextureCom[SHIELDSTATE_ATTACK], 13.f);
	m_pAnimatorCom->CreateAnimation(L"Attack_Delay_Shield", m_pShieldTextureCom[SHIELDSTATE_IDLE], 13.f);
	m_pAnimatorCom->CreateAnimation(L"Katana", m_pTextureCom[HUMANOID_KATANA], 13.f);
	m_pAnimatorCom->CreateAnimation(L"Execution", m_pTextureCom[HUMANOID_EXECUTION], 15.f);

	m_pAnimatorCom->PlayAnimation(L"Idle_Shield", true);
}

void CBlackMan::Damaged_By_Player(const DAMAGED_STATE& _eDamagedState, const _float& _fAttackDamage)
{
	_int iTemp(0);

	if (m_bIsShield)
	{
		m_bIsShield = false;
		_vec3 vPos, vPlayerPos, vDir;
		m_pTransformCom->Get_Info(INFO::INFO_POS, &vPos);
		m_pPlayerTransformCom->Get_Info(INFO::INFO_POS, &vPlayerPos);
		vDir = vPlayerPos - vPos;
		D3DXVec3Normalize(&vDir, &vDir);
		vDir.y = 0.f;
		vPos += vDir;
		dynamic_cast<CShield*>(m_pShield)->Spawn_Shield(vPos);

		m_bIsFormChange = true;

		if (m_eCurState == CHumanoid::HUMANOID_IDLE)
			m_pAnimatorCom->PlayAnimation(L"Idle", true);
	}
	else if (!m_bIsShield)
	{
		m_fHP -= _fAttackDamage;
		Engine::Play_Sound(L"Blood_01.wav", CHANNELID::SOUND_ENEMY, 0.4f);
		if (0.f >= m_fHP)
		{
			_vec3 vPos;
			m_pTransformCom->Get_Info(INFO::INFO_POS, &vPos);
			_int iIndex = rand() % 13;

			CUI* pUI = Engine::Activate_UI(UITYPE::UI_PLUS);
			Dead_Combo();

			switch (_eDamagedState)
			{
			case Engine::DAMAGED_STATE::DAMAGED_HEADSHOT:
				Changing_State(CHumanoid::HUMANOID_HEADSHOT);
				static_cast<CUIPlus*>(pUI)->Init(vPos, CUIPlus::UI_PLUS::PLUS_HEADSHOT);
				//static_cast<CUIPlus*>(pUI)->Init(vPos, (CUIPlus::UI_PLUS)iIndex);
				if (0 == iIndex % 4)
					Engine::Play_Sound(L"HeadShot_01.wav", CHANNELID::SOUND_ENEMY_DAMAGED, 0.7f);
				else if (1 == iIndex % 4)
					Engine::Play_Sound(L"HeadShot_02.wav", CHANNELID::SOUND_ENEMY_DAMAGED, 0.7f);
				else if (2 == iIndex % 4)
					Engine::Play_Sound(L"HeadShot_03.wav", CHANNELID::SOUND_ENEMY_DAMAGED, 0.7f);
				else
					Engine::Play_Sound(L"HeadShot_04.wav", CHANNELID::SOUND_ENEMY_DAMAGED, 0.7f);

				dynamic_cast<CPlayer*>(Engine::Get_CurrScene()->Get_GameObject(L"Layer_Player", L"Player"))->Set_PlayerHP_Plus(3.f);
				break;
			case Engine::DAMAGED_STATE::DAMAGED_BULLSHOT:
				Changing_State(CHumanoid::HUMANOID_BULLSHOT);
				static_cast<CUIPlus*>(pUI)->Init(vPos, CUIPlus::UI_PLUS::PLUS_NUTSHOT);
				if (0 == iIndex % 4)
					Engine::Play_Sound(L"Death_01.wav", CHANNELID::SOUND_ENEMY_DAMAGED, 0.7f);
				else if (1 == iIndex % 4)
					Engine::Play_Sound(L"Death_02.wav", CHANNELID::SOUND_ENEMY_DAMAGED, 0.7f);
				else if (2 == iIndex % 4)
					Engine::Play_Sound(L"Death_03.wav", CHANNELID::SOUND_ENEMY_DAMAGED, 0.7f);
				else
					Engine::Play_Sound(L"Death_04.wav", CHANNELID::SOUND_ENEMY_DAMAGED, 0.7f);

				dynamic_cast<CPlayer*>(Engine::Get_CurrScene()->Get_GameObject(L"Layer_Player", L"Player"))->Set_PlayerHP_Plus(3.f);
				break;
			case Engine::DAMAGED_STATE::DAMAGED_KATANA:
				Changing_State(CHumanoid::HUMANOID_KATANA);
				static_cast<CUIPlus*>(pUI)->Init(vPos, CUIPlus::UI_PLUS::PLUS_SAYONARA);
				if (0 == iIndex % 4)
					Engine::Play_Sound(L"Blood_01.wav", CHANNELID::SOUND_ENEMY_DAMAGED, 0.7f);
				else if (1 == iIndex % 4)
					Engine::Play_Sound(L"Blood_02.wav", CHANNELID::SOUND_ENEMY_DAMAGED, 0.7f);
				else if (2 == iIndex % 4)
					Engine::Play_Sound(L"Blood_03.wav", CHANNELID::SOUND_ENEMY_DAMAGED, 0.7f);
				else
					Engine::Play_Sound(L"Blood_04.wav", CHANNELID::SOUND_ENEMY_DAMAGED, 0.7f);

				dynamic_cast<CPlayer*>(Engine::Get_CurrScene()->Get_GameObject(L"Layer_Player", L"Player"))->Set_PlayerHP_Plus(3.f);
				break;
			case Engine::DAMAGED_STATE::DAMAGED_BODYSHOT:

				iTemp = _int(rand() % 64);

				if (0 == _int(iTemp % 4))
					Changing_State(CHumanoid::HUMANOID_SHOT_ONE);
				else if (1 == _int(iTemp % 4))
					Changing_State(CHumanoid::HUMANOID_SHOT_TWO);
				else if (2 == _int(iTemp % 4))
					Changing_State(CHumanoid::HUMANOID_PUSH_ONE);
				else
					Changing_State(CHumanoid::HUMANOID_PUSH_TWO);

				if (0 == iIndex % 4)
					Engine::Play_Sound(L"Death_01.wav", CHANNELID::SOUND_ENEMY_DAMAGED, 0.7f);
				else if (1 == iIndex % 4)
					Engine::Play_Sound(L"Death_02.wav", CHANNELID::SOUND_ENEMY_DAMAGED, 0.7f);
				else if (2 == iIndex % 4)
					Engine::Play_Sound(L"Death_03.wav", CHANNELID::SOUND_ENEMY_DAMAGED, 0.7f);
				else
					Engine::Play_Sound(L"Death_04.wav", CHANNELID::SOUND_ENEMY_DAMAGED, 0.7f);

				static_cast<CUIPlus*>(pUI)->Init(vPos, (CUIPlus::UI_PLUS)iIndex);
				dynamic_cast<CPlayer*>(Engine::Get_CurrScene()->Get_GameObject(L"Layer_Player", L"Player"))->Set_PlayerHP_Plus(2.f);
				break;
			case Engine::DAMAGED_STATE::DAMAGED_PUSHSHOT:

				iTemp = _int(rand() % 8);

				if (0 == _int(iTemp % 2))
					Changing_State(CHumanoid::HUMANOID_PUSH_ONE);
				else
					Changing_State(CHumanoid::HUMANOID_PUSH_TWO);

				static_cast<CUIPlus*>(pUI)->Init(vPos, (CUIPlus::UI_PLUS)iIndex);
				dynamic_cast<CPlayer*>(Engine::Get_CurrScene()->Get_GameObject(L"Layer_Player", L"Player"))->Set_PlayerHP_Plus(2.f);
				break;
			case Engine::DAMAGED_STATE::DAMAGED_EXECUTION:
				static_cast<CUIPlus*>(pUI)->Init(vPos, CUIPlus::UI_PLUS::PLUS_GOODBYE);
				dynamic_cast<CPlayer*>(Engine::Get_CurrScene()->Get_GameObject(L"Layer_Player", L"Player"))->Set_PlayerHP_Plus(20.f);
				break;
			}
			//m_pColliderCom->SetActive(false);
			m_bIsDead = true;
		}
	}
}

void CBlackMan::Free()
{
	CHumanoid::Free();
}
