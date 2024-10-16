#include "pch.h"
#include "../Header/EffectExplosion.h"
#include "Export_Utility.h"
#include "Export_System.h"

CEffectExplosion::CEffectExplosion(LPDIRECT3DDEVICE9 _pGrphicDev)
	: CGameObject(_pGrphicDev)
{
}

CEffectExplosion::~CEffectExplosion()
{
}

HRESULT CEffectExplosion::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->Set_Pos(1.f, 1.f, 1.f);
	//m_pTransformCom->m_vScale = { 0.5f, 0.5f, 0.5f };

	m_pEffectCom->Set_LifeTime(1.1f);
	m_pEffectCom->Set_Visibility(FALSE);
	m_pEffectCom->Set_Repeatable(FALSE);
	m_pEffectCom->Set_CallBack(OnOperate);
	m_pEffectCom->Set_CallBackParam(this);

	Set_FlareParticle();
	Set_SmokeParticle();
	Set_FragmentParticle();

	return S_OK;
}

_int CEffectExplosion::Update_GameObject(const _float& _fTimeDelta)
{
	if (!m_pEffectCom->Get_Visibility())
		return 0;

	Engine::Add_RenderGroup(RENDERID::RENDER_ALPHA, this);


	_int iExit = Engine::CGameObject::Update_GameObject(_fTimeDelta);
	return iExit;
}

void CEffectExplosion::LateUpdate_GameObject()
{
	if (!m_pEffectCom->Get_Visibility())
		return;

	_vec3	vTemp;
	m_pTransformCom->Get_Info(Engine::INFO::INFO_POS, &vTemp);
	CGameObject::Compute_ViewZ(&vTemp);

	Engine::CGameObject::LateUpdate_GameObject();
}

void CEffectExplosion::Render_GameObject()
{
	if (!m_pEffectCom->Get_Visibility())
		return;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureSmokeCom->Set_Texture();
	m_pParticleSystemSmoke->Render_Parcitle();

	m_pTextureCom->Set_Texture();
	m_pParticleSystemFragment->Render_Parcitle();
	m_pParticleSystemFlare->Render_Parcitle();
}

HRESULT CEffectExplosion::Add_Component()
{
	CComponent* pComponent(nullptr);

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(*this);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

	pComponent = m_pTextureSmokeCom = static_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Glow_2Texture"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(*this);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_SmokeTexture", pComponent });

	pComponent = m_pTextureCom = static_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Glow_White"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(*this);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture", pComponent });

	pComponent = m_pParticleSystemFlare = static_cast<CParticleSystem*>(Engine::Clone_Proto(L"Proto_ParticleSystem"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(*this);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_ParticleSystem_Flare", pComponent });

	pComponent = m_pParticleSystemFragment = static_cast<CParticleSystem*>(Engine::Clone_Proto(L"Proto_ParticleSystem"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(*this);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_ParticleSystem_Fragment", pComponent });

	pComponent = m_pParticleSystemSmoke = static_cast<CParticleSystem*>(Engine::Clone_Proto(L"Proto_ParticleSystem"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(*this);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_ParticleSystem_Smoke", pComponent });

	pComponent = m_pEffectCom = static_cast<CEffect*>(Engine::Clone_Proto(L"Proto_Effect"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(*this);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Effect", pComponent });

}

CEffectExplosion* CEffectExplosion::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CEffectExplosion* pExplosion = new CEffectExplosion(pGraphicDev);

	if (FAILED(pExplosion->Ready_GameObject()))
	{
		Safe_Release(pExplosion);
		MSG_BOX("pExplosion Create Failed");
		return nullptr;
	}

	return pExplosion;
}

void CEffectExplosion::Free()
{
	Engine::CGameObject::Free();
}

void CEffectExplosion::OnOperate(void* _pParam)
{
	CEffectExplosion* pThis = (CEffectExplosion*)_pParam;

	pThis->m_pParticleSystemSmoke->Reset();
	pThis->m_pParticleSystemFlare->Reset();
	pThis->m_pParticleSystemFragment->Reset();

	pThis->m_pEffectCom->Stop_Effect();

	std::wstring strSoundKey = L"explosions-00";
	_uint iIdx = rand() % 4 + 1;
	strSoundKey += std::to_wstring(iIdx) + L".wav";
	Engine::Play_Sound(strSoundKey.c_str(), CHANNELID::SOUND_EFFECT, 0.7f);

}

//void CEffectExplosion::Set_FlareParticle()
//{
//	CParticleSystem::PARAM tParam;
//	tParam.tInit.tHexahedron.tStartBoundary.vMin = { 0.f, 0.f, 0.f };
//	tParam.tInit.tHexahedron.tStartBoundary.vMax = { 0.f, 0.f, 0.f };
//	tParam.tInit.tHexahedron.vInitVelocity = { 0.f, 0.0f, 0.f };
//	tParam.vVelocityNoise = { 1.f, 1.f, 1.f };
//	tParam.vColor = _vec4(1.0f, 50.f / 255.f, 0.f, 1.f);
//	tParam.vColorFade = _vec4(0.1f, 0.1f, 0.1f, 0.0f);
//	tParam.iTotalCnt = 100;
//
//	tParam.fSize = 1.0f;
//	tParam.fSizeFade = 0.2f;
//	tParam.fLifeTime = 0.5f;
//
//	tParam.fEmitRate = 25.;
//	tParam.iEmitCnt = 60.f;
//
//	m_pParticleSystemFlare->Set_Parameter(tParam);
//	//m_pParticleSystemFlare->Set_Option(CParticleSystem::OPTION::REPEAT, TRUE);
//	m_pParticleSystemFlare->Set_Option(CParticleSystem::OPTION::DEATH_OVER_TIME, TRUE);
//	m_pParticleSystemFlare->Set_Option(CParticleSystem::OPTION::ZWRITE_DISABLE, TRUE);
//	m_pParticleSystemFlare->Set_Option(CParticleSystem::OPTION::COLOR_FADE, TRUE);
//	m_pParticleSystemFlare->Set_Option(CParticleSystem::OPTION::ALPHAOP_ADD, TRUE);
//	m_pParticleSystemFlare->Set_Option(CParticleSystem::OPTION::EMISSION_CONTROL, TRUE);
//	m_pParticleSystemFlare->Set_Option(CParticleSystem::OPTION::SIZE_OVER_TIME, TRUE);
//
//	m_pParticleSystemFlare->SetUp_Particle();	
//}
//
//void CEffectExplosion::Set_SmokeParticle()
//{
//	CParticleSystem::PARAM tParam;
//	tParam.tInit.tHexahedron.tStartBoundary.vMin = { 0.f, 0.f, 0.f };
//	tParam.tInit.tHexahedron.tStartBoundary.vMax = { 0.f, 0.f, 0.f };
//	tParam.tInit.tHexahedron.vInitVelocity = { 0.f, 0.f, 0.f };
//	tParam.vVelocityNoise = { 1.f, 1.f, 1.f };
//	tParam.vColor = _vec4(0.5f, 0.5f, 0.5f, 1.f);
//	tParam.vColorFade = _vec4(-0.f, -0.f, -0.f, 0.0f);
//	tParam.iTotalCnt = 150;
//
//	tParam.fSize = 1.0f;
//	tParam.fSizeFade = 2.f;
//	tParam.fLifeTime = 1.75f;
//
//	tParam.fEmitRate = 50.f;
//	tParam.iEmitCnt = 50;
//
//	m_pParticleSystemSmoke->Set_Parameter(tParam);
//	//m_pParticleSystemSmoke->Set_Option(CParticleSystem::OPTION::REPEAT, TRUE);
//	m_pParticleSystemSmoke->Set_Option(CParticleSystem::OPTION::DEATH_OVER_TIME, TRUE);
//	m_pParticleSystemSmoke->Set_Option(CParticleSystem::OPTION::ZWRITE_DISABLE, TRUE);
//	m_pParticleSystemSmoke->Set_Option(CParticleSystem::OPTION::COLOR_FADE, TRUE);
//	m_pParticleSystemSmoke->Set_Option(CParticleSystem::OPTION::EMISSION_CONTROL, TRUE);
//	m_pParticleSystemSmoke->Set_Option(CParticleSystem::OPTION::SIZE_OVER_TIME, TRUE);
//
//	m_pParticleSystemSmoke->SetUp_Particle();
//}
//
//
//void CEffectExplosion::Set_FragmentParticle()
//{
//	CParticleSystem::PARAM tParam;
//	tParam.eShape = CParticleSystem::SHAPE::SPHERE;
//
//	tParam.tInit.tSphere.fRadius = 7.8;
//	tParam.tInit.tSphere.fTheta = D3DX_PI;
//	tParam.vVelocityNoise = { 0.f, 0.5f, 0.f };
//	tParam.vColor = _vec4(1.f, 225.f / 255.f, 134.f / 255.f, 1.f);
//	//tParam.vColor = _vec4(0.2f, 0.1f, 0.1f, 1.f);
//	//tParam.vColorFade = _vec4(85.f / 255.f, 32.f / 255.f, 0.f, 1.0f);
//	tParam.vColorFade = _vec4(35.f / 255.f, 14.f / 255.f, 0.f, 1.0f);
//	tParam.iTotalCnt = 25;
//
//	tParam.fSize = 0.3f;
//	tParam.fSizeFade = 0.f;
//	tParam.fLifeTime = 0.25f;
//
//	tParam.fEmitRate = 50.f;
//	tParam.iEmitCnt = 50;
//
//	tParam.fGravity = 8.5f;
//
//	m_pParticleSystemFragment->Set_Parameter(tParam);
//	//m_pParticleSystemFragment->Set_Option(CParticleSystem::OPTION::REPEAT, TRUE);
//	m_pParticleSystemFragment->Set_Option(CParticleSystem::OPTION::DEATH_OVER_TIME, TRUE);
//	m_pParticleSystemFragment->Set_Option(CParticleSystem::OPTION::ZWRITE_DISABLE, TRUE);
//	m_pParticleSystemFragment->Set_Option(CParticleSystem::OPTION::COLOR_FADE, TRUE);
//	//m_pParticleSystemFragment->Set_Option(CParticleSystem::OPTION::EMISSION_CONTROL, TRUE);
//	m_pParticleSystemFragment->Set_Option(CParticleSystem::OPTION::SIZE_OVER_TIME, TRUE);
//	m_pParticleSystemFragment->Set_Option(CParticleSystem::OPTION::ALPHAOP_ADD, TRUE);
//	//m_pParticleSystemFragment->Set_Option(CParticleSystem::OPTION::GRAVITY, TRUE);
//
//	m_pParticleSystemFragment->SetUp_Particle();
//}

#pragma region TestParticleSetting

void CEffectExplosion::Set_FlareParticle()
{
	CParticleSystem::PARAM tParam;
	tParam.tInit.tHexahedron.tStartBoundary.vMin = { -0.3f, -0.3f, -0.3f };
	tParam.tInit.tHexahedron.tStartBoundary.vMax = { 0.3f, 0.3f, 0.3f };
	tParam.tInit.tHexahedron.vInitVelocity = { 0.f, 0.0f, 0.f };
	tParam.vVelocityNoise = { 1.f, 1.f, 1.f };
	tParam.vColor = _vec4(1.0f, 50.f / 255.f, 0.f, 1.f);
	tParam.vColorFade = _vec4(0.1f, 0.1f, 0.1f, 0.0f);
	tParam.iTotalCnt = 100;

	tParam.fSize = 1.f;
	tParam.fSizeFade = 0.f;
	tParam.fLifeTime = 0.35f;

	tParam.fEmitRate = 25.;
	tParam.iEmitCnt = 60.f;

	m_pParticleSystemFlare->Set_Parameter(tParam);
	//m_pParticleSystemFlare->Set_Option(CParticleSystem::OPTION::REPEAT, TRUE);
	m_pParticleSystemFlare->Set_Option(CParticleSystem::OPTION::DEATH_OVER_TIME, TRUE);
	m_pParticleSystemFlare->Set_Option(CParticleSystem::OPTION::ZWRITE_DISABLE, TRUE);
	m_pParticleSystemFlare->Set_Option(CParticleSystem::OPTION::COLOR_FADE, TRUE);
	m_pParticleSystemFlare->Set_Option(CParticleSystem::OPTION::ALPHAOP_ADD, TRUE);
	m_pParticleSystemFlare->Set_Option(CParticleSystem::OPTION::EMISSION_CONTROL, TRUE);
	m_pParticleSystemFlare->Set_Option(CParticleSystem::OPTION::SIZE_OVER_TIME, TRUE);

	m_pParticleSystemFlare->SetUp_Particle();
}

void CEffectExplosion::Set_SmokeParticle()
{
	CParticleSystem::PARAM tParam;
	tParam.tInit.tHexahedron.tStartBoundary.vMin = { 0.f, 0.f, 0.f };
	tParam.tInit.tHexahedron.tStartBoundary.vMax = { 0.f, 0.f, 0.f };
	tParam.tInit.tHexahedron.vInitVelocity = { 0.f, 0.f, 0.f };
	tParam.vVelocityNoise = { 1.f, 1.f, 1.f };
	tParam.vColor = _vec4(1.f, 1.f, 1.f, 1.f);
	tParam.vColorFade = _vec4(0.0f, 0.f, 0.f, 0.0f);
	tParam.iTotalCnt = 200;

	tParam.fSize = 0.5f;
	tParam.fSizeFade = 1.f;
	//tParam.fLifeTime = 0.75f;
	tParam.fLifeTime = 1.00f;

	tParam.fEmitRate = 50.f;
	tParam.iEmitCnt = 50;

	m_pParticleSystemSmoke->Set_Parameter(tParam);
	//m_pParticleSystemSmoke->Set_Option(CParticleSystem::OPTION::REPEAT, TRUE);
	m_pParticleSystemSmoke->Set_Option(CParticleSystem::OPTION::DEATH_OVER_TIME, TRUE);
	m_pParticleSystemSmoke->Set_Option(CParticleSystem::OPTION::ZWRITE_DISABLE, TRUE);
	m_pParticleSystemSmoke->Set_Option(CParticleSystem::OPTION::COLOR_FADE, TRUE);
	m_pParticleSystemSmoke->Set_Option(CParticleSystem::OPTION::EMISSION_CONTROL, TRUE);
	m_pParticleSystemSmoke->Set_Option(CParticleSystem::OPTION::SIZE_OVER_TIME, TRUE);

	m_pParticleSystemSmoke->SetUp_Particle();
}

void CEffectExplosion::Set_FragmentParticle()
{
	CParticleSystem::PARAM tParam;
	tParam.eShape = CParticleSystem::SHAPE::SPHERE;

	tParam.tInit.tSphere.fRadius = 5.8;
	tParam.tInit.tSphere.fTheta = D3DX_PI;
	tParam.vVelocityNoise = { 0.f, 0.5f, 0.f };
	tParam.vColor = _vec4(1.f, 225.f / 255.f, 134.f / 255.f, 1.f);
	//tParam.vColor = _vec4(0.2f, 0.1f, 0.1f, 1.f);
	//tParam.vColorFade = _vec4(85.f / 255.f, 32.f / 255.f, 0.f, 1.0f);
	tParam.vColorFade = _vec4(35.f / 255.f, 14.f / 255.f, 0.f, 1.0f);
	tParam.iTotalCnt = 25;

	tParam.fSize = 0.1f;
	tParam.fSizeFade = 0.f;
	tParam.fLifeTime = 0.75f;

	tParam.fEmitRate = 50.f;
	tParam.iEmitCnt = 50;

	tParam.fGravity = 8.5f;

	m_pParticleSystemFragment->Set_Parameter(tParam);
	//m_pParticleSystemFragment->Set_Option(CParticleSystem::OPTION::REPEAT, TRUE);
	m_pParticleSystemFragment->Set_Option(CParticleSystem::OPTION::DEATH_OVER_TIME, TRUE);
	m_pParticleSystemFragment->Set_Option(CParticleSystem::OPTION::ZWRITE_DISABLE, TRUE);
	m_pParticleSystemFragment->Set_Option(CParticleSystem::OPTION::COLOR_FADE, TRUE);
	//m_pParticleSystemFragment->Set_Option(CParticleSystem::OPTION::EMISSION_CONTROL, TRUE);
	m_pParticleSystemFragment->Set_Option(CParticleSystem::OPTION::SIZE_OVER_TIME, TRUE);
	m_pParticleSystemFragment->Set_Option(CParticleSystem::OPTION::ALPHAOP_ADD, TRUE);
	m_pParticleSystemFragment->Set_Option(CParticleSystem::OPTION::GRAVITY, TRUE);

	m_pParticleSystemFragment->SetUp_Particle();
}

#pragma endregion
