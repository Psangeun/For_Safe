#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CTransform;
class CTexture;

END
class CTileContorl : public CGameObject
{
protected:
	explicit CTileContorl(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CTileContorl();


public:
	virtual HRESULT	Ready_GameObject() PURE;
	virtual _int Update_GameObject(const _float& _fTimeDelta)PURE;
	virtual void LateUpdate_GameObject()PURE;
	virtual void Render_GameObject()PURE;

	virtual _vec3 Get_VecPos() { return m_vecPos; }
	virtual _vec3 Get_VecRot() { return m_vecRot; }
	virtual _vec3 Get_VecScale() { return m_vecScale; }
	virtual _int Get_Number() { return m_iNumber; }
	virtual _int Get_Number_Type() { return m_iNumber_Type; }
	virtual _int Get_Trigger() { return m_iTriggerNumber; }

protected:
	virtual HRESULT Add_Component()PURE;
	virtual HRESULT Setup_Material()PURE;

protected:
	virtual void Free();

protected:
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;

	_vec3 m_vecPos;
	_vec3 m_vecRot;
	_vec3 m_vecScale;

	_int m_iNumber; //�̰� ���° WallTB�̳Ĵ� ����
	_int m_iNumber_Type; //�̰� WallTB�̶�� ����

	_int m_iTriggerNumber; //���° Ʈ���ſ� �����ų� 10.06
	_bool m_bFirstDoor;//���������� ù��° ��
	_bool m_bLastDoor;//���������� ������ ��

	

};

