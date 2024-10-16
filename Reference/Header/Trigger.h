#pragma once
#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CTrigger :
    public CBase
{
	DECLARE_SINGLETON(CTrigger)
private:
	explicit CTrigger();
	virtual ~CTrigger();

public:
	void Set_Trigger(_int _iTrigger, CGameObject* _pGameObject); //ó���� ���� Setting�Ҷ� Tile�� Monster�� Ʈ���� ���ڸ� ������� Vector���ٰ� �־��ִ� �Լ�
	void Calculate_Trigger(); // Render Ȯ�����ִ� �༮
	void Collision_With_Trigger(const _int& _iTriggerNumber); // TriggerNumber�� �ǵ���ִ� �Լ�,

private:
	vector<CGameObject*> m_vecTrigger[TRIGGERCOUNT];
	_int m_iCurTriggerNumber;
	_int m_iPreTriggerNumber;
	_int m_iPrePreTriggerNumber;
public:
	virtual void Free();
};

END