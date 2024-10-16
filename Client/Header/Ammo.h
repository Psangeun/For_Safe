#pragma once
#include "Bullet.h"

BEGIN(Engine)
class CRcTex;
class CTexture;
class CTransform;
class CCollider;
END

class CAmmo :
    public Engine::CBullet
{
private:
    explicit CAmmo(LPDIRECT3DDEVICE9 _pGraphicDev);
    virtual ~CAmmo();

public:
    virtual HRESULT Ready_GameObject();
    virtual _int Update_GameObject(const _float& _fTimeDelta);
    virtual void LateUpdate_GameObject();
    virtual void Render_GameObject();
    virtual void Fire_Bullet(LPDIRECT3DDEVICE9 _pGraphicDev, const _vec3& _vStartPos, const _vec3& _vDir, const _float& _fAttackDamage, const _bool& _bIsBoss);

    static CAmmo* Create(LPDIRECT3DDEVICE9 _pGraphicDev);

private:
    HRESULT Add_Component();

private:
    Engine::CRcTex* m_pBufferCom;
    Engine::CTexture* m_pTextureCom;
    Engine::CTexture* m_pTextureCom_Boss;
    Engine::CTransform* m_pTransformCom;
    Engine::CCollider* m_pColliderCom;

    _float m_fTimer;
    _float m_fFrame;
    _float m_fMaxFrame;
    _vec3 m_fStartPos;
    _float m_fLinear;
    _bool m_bIsBoss;

private:
    virtual void Free();
};

