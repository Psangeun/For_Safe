#pragma once
#include "../Header/WallTB.h"
BEGIN(Engine)

class CWallTBTex;
class CTransform;
class CTexture;
class CCollider;

END

class CDoor :
    public CTileContorl
{
private:
    explicit CDoor(LPDIRECT3DDEVICE9 _pGraphicDev);
    virtual ~CDoor();

public:
    static CDoor* Create_InfoSave(LPDIRECT3DDEVICE9 _pGraphicDev, _vec3 _vecPos, _vec3 _vecRot,_vec3 _vecScale, const _int& _iNumber, Engine::TILE_DIRECTION _eTileDirection, const _int& _iTrigger);
    static CDoor* Create_Infoload(LPDIRECT3DDEVICE9 _pGraphicDev, _vec3 _vecPos, _vec3 _vecRot,_vec3 _vecScale, const _int& _iNumber,_vec3 _vecTileDirection, const _int& _iTrigger);

    virtual _vec3 Get_VecPos() { return m_vecPos; }
    virtual _vec3 Get_VecRot() { return m_vecRot; }
    virtual _vec3 Get_VecScale() { return m_vecScale; }
    virtual _int Get_Number() { return m_iNumber; }
    virtual _int Get_Number_Type() { return m_iNumber_Type; }
    _int Get_Trigger() { return m_iTriggerNumber; }//10.06
    _vec3 Get_TileDirection() { return m_vecWallDirection; }
    void Set_TileDirection(const _vec3& _vecDir);
    void Set_Number(const _int& _iNumber) { m_iNumber = _iNumber; }
    void Set_Trigger(const _int& _iTrigger) { m_iTriggerNumber = _iTrigger; }//10.06

public:
    virtual HRESULT	Ready_GameObject();
    virtual _int Update_GameObject(const _float& _fTimeDelta);
    virtual void LateUpdate_GameObject();
    virtual void Render_GameObject();
    void Set_TileDirection(Engine::TILE_DIRECTION _eTileDirection);
    virtual void Set_IsRender(const _bool& _bool);
private:
    virtual HRESULT Add_Component();
    virtual HRESULT Setup_Material();
    void Setup_Position(_vec3 _vecPos);
    void Setup_Angle(_vec3 _vecRot);
    void Setup_Scale(_vec3 _vecScale);

    void Moving_Open();

private:
    Engine::CWallTBTex* m_pBufferCom;
    Engine::CCollider* m_pColliderCom;
    _vec3 m_vecWallDirection;
    _float m_fMovingSpeed;
    _bool m_bIsOpen;
    _bool m_bEleOpen;

public:
    void OnCollisionEnter(CCollider& _pOther);
    void OnCollisionExit(CCollider& _pOther);

private:
    virtual void Free();
};

