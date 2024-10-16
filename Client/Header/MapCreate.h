#pragma once
#include "Scene.h"
#include "Export_Utility.h"
#include "Export_System.h"
#include "Engine_Enum.h"

class CMapCreate : public CScene
{
private:
	explicit CMapCreate(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CMapCreate();

public:
	static CMapCreate* Create(LPDIRECT3DDEVICE9 _pGraphicDev);

public:
	virtual HRESULT	Ready_Scene();
	virtual _int Update_Scene(const _float& _fTimeDelta);
	virtual void LateUpdate_Scene();
	virtual void Render_Scene();

private:
	//���� �⺻ ���̾��
	HRESULT Ready_LightInfo();
	HRESULT Ready_Layer_ToolEnvironment(const _tchar* _pLayerTag);
	HRESULT Ready_Layer_Terrain(const _tchar* _pLayerTag);

	//Ÿ�� ������ ���̾�
	HRESULT Ready_Layer_PickingTile(const _tchar* _pLayerTag);
	//Ÿ�� ������ ���̾� ã�Ƽ� ��������
	CLayer* Find_Layer(const _tchar* _pLayerTag);

	//�ٴ� ������Ʈ ����
	HRESULT Create_Layer_PickingFloor(CLayer* _pLayer);
	//�� ������Ʈ ����
	HRESULT Create_Layer_PickingWall(CLayer* _pLayer, Engine::TILE_DIRECTION _eTileDirection);
	//�� ������Ʈ ����
	HRESULT Create_Layer_PickingWallTB(CLayer* _pLayer, Engine::TILE_DIRECTION _eTileDirection);
	//Monster ������Ʈ ����
	HRESULT Create_Layer_PickingMonster(CLayer* _pLayer);
	//Door ������Ʈ ����
	HRESULT Create_Layer_PickingDoor(CLayer* _pLayer, Engine::TILE_DIRECTION _eTileDirection);
	//item ������Ʈ ����
	HRESULT Create_Layer_PickingItem(CLayer* _pLayer);

	// ���̵� ���� ���� ��������
	_vec3 TilePiking_OnTerrain(int _iTile);
	// ���̵� ������ ���ۿ� ���콺 ��ǥ�� ���� [�ٴ�] Ÿ�� ����
	_vec3 FloorCreate_OnTerrain(HWND _hWnd, CGuideTex* _pGuideBufferCom);


	// ���̵� ������ ���ۿ� ���콺 ��ǥ�� ���� [��] Ÿ�� ����
	//left,right
	_vec3 WallCreate_OnTerrain1(HWND _hWnd, CGuideTex* _pGuideBufferCom);
	//top,bottom
	_vec3 WallCreate_OnTerrain2(HWND _hWnd, CGuideTex* _pGuideBufferCom);

	// ���̵� ������ ���ۿ� ���콺 ��ǥ�� ���� [Monster] Ÿ�� ����
	_vec3 MonsterCreate_OnTerrain(HWND _hWnd, CGuideTex* _pGuideBufferCom);

	// �ٴ� ���ۿ� ���콺 ��ǥ�� ���� ����
	HRESULT PickingTile_PosDelete(CLayer* _pLayer, const _tchar* _TileTag);
	// �̹� Ÿ���� ������ ���� X
	void PickingTile_PosCheck(CLayer* _pLayer, const _tchar* _TileTag);


	void MapSave2(CLayer* _pLayer);
	void MapLoad2(CLayer* _pLayer);


	//imgui �Լ�
	void ShowGUI();
	void SetMenu();
	void SetFloor();//imgui�ٴ� 
	void SetWall();//imgui��
	void SetMonster();//imgui����
	void SetDoor();//imgui Door
	void SetItem();//imgui ������

	HRESULT Ready_Texture_FloorInsert(const _tchar* _pPath, const _tchar* _pComponentTag, TEXTUREID _eType, const int& _iCnt); // imgui ����� �ٴ� ����
	HRESULT Ready_Texture_WallInsert(const _tchar* _pPath, const _tchar* _pComponentTag, TEXTUREID _eType, const int& _iCnt); // imgui ����� �� ����
	HRESULT Ready_Texture_MonsterInsert(const _tchar* _pPath, const _tchar* _pComponentTag, TEXTUREID _eType, const int& _iCnt); // imgui ����� Monster ����
	HRESULT Ready_Texture_DoorInsert(const _tchar* _pPath, const _tchar* _pComponentTag, TEXTUREID _eType, const int& _iCnt); // imgui ����� Door ����
	HRESULT Ready_Texture_ItemInsert(const _tchar* _pPath, const _tchar* _pComponentTag, TEXTUREID _eType, const int& _iCnt); // imgui ����� Door ����


private:
	virtual void Free();


private:
	bool m_bCreateCheck;//Ÿ�� ���� ���� üũ
	float m_fHeight;// ���̵� ���� ����

	_int m_iTriggerNumber; // �̰� ���� ���° Ʈ���ſ��� �ߵ���ų���̳� �� ���� int ��//10.06

	_int m_iNumber; // ����Ǵ� �̹��� ��ȣ

	_vec3 m_vecPosition; // imgui �� ���� ��ǥ ���� ��
	_vec3 m_vecRot;//ȸ�� �� ����
	_vec3 m_vecScale; // ũ�� �� ���� 

	map<const _tchar*, IDirect3DBaseTexture9*> m_mapImageFloor;
	map<const _tchar*, IDirect3DBaseTexture9*> m_mapImageWall;
	map<const _tchar*, IDirect3DBaseTexture9*> m_mapImageMonster;
	map<const _tchar*, IDirect3DBaseTexture9*> m_mapImageDoor;
	map<const _tchar*, IDirect3DBaseTexture9*> m_mapImageItem;

};
