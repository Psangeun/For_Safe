#pragma once
#include "Scene.h"
#include "BackGround.h"
#include "Define.h"

class CEndingScene :
    public CScene
{
private:
	explicit CEndingScene(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CEndingScene();

public:
	static CEndingScene* Create(LPDIRECT3DDEVICE9 _pGraphicDev);

public:
	virtual HRESULT Ready_Scene();
	virtual _int Update_Scene(const _float& _fTimeDelta);
	virtual void LateUpdate_Scene();
	virtual void Render_Scene();

private:

	//��ȯǥ �������
	void PlayVideo(HWND _hWnd, const wstring& _strFilePath);

private:
	HRESULT Ready_Layer_Environment(const _tchar* _pLayerTag);
	HRESULT Ready_Layer_GameLogic(const _tchar* _pLayerTag) { return S_OK; }
	HRESULT Ready_Layer_UI(const _tchar* _pLayerTag) { return S_OK; }

private:
	virtual void Free();

private:
	HWND m_hVideoHandle;
	_bool m_bVideoPlaying;
};

