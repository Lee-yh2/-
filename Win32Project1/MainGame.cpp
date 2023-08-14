#include "stdafx.h"
#include "MainGame.h"
#include "Graphic_Device.h"
#include "Texture_Manager.h"
#include "Time_Manager.h"
#include "Scene_Manager.h"
#include "Frame_Manager.h"
#include "GameObject_Manager.h"
#include "SoundMgr.h"



CMainGame::CMainGame()
{
}


CMainGame::~CMainGame()
{
	Release_MainGame();
}

HRESULT CMainGame::Ready_MainGame()
{
	if (FAILED(CGraphic_Device::Get_Instance()->Ready_Graphic_Device(CGraphic_Device::MODE_WIN)))
	{
		ERR_MSG(L"Creating Failed Graphic_Device");
		return E_FAIL;
	}

	if (FAILED(CTexture_Manager::Get_Instance()->ReadImagePath("../Data/MenuPathInfo.txt")))
	{
		ERR_MSG(L"Creating Failed InsertTexture ");
		return E_FAIL;
	}

	if (FAILED(CTexture_Manager::Get_Instance()->Insert_Texture(CTexture_Manager::TEX_SINGLE, L"../Texture/RESOURCE/Test/PunchTest.png", L"PunchTest")))
	{
		ERR_MSG(L"Creating Failed InsertTexture - Player-Attack");
		return E_FAIL;
	}
	if (FAILED(CTexture_Manager::Get_Instance()->Insert_Texture(CTexture_Manager::TEX_SINGLE, L"../Texture/RESOURCE/Test/StampTest.png", L"StampTest")))
	{
		ERR_MSG(L"Creating Failed InsertTexture - Player-Attack");
		return E_FAIL;
	}
	if (FAILED(CTexture_Manager::Get_Instance()->Insert_Texture(CTexture_Manager::TEX_SINGLE, L"../Texture/RESOURCE/Test/ShortTest.png", L"ShortTest")))
	{
		ERR_MSG(L"Creating Failed InsertTexture - Player-Attack");
		return E_FAIL;
	}

	if (FAILED(CScene_Manager::Get_Instance()->Scene_Change_SceneManager(CScene_Manager::SCENE_MAINMENU)))
	{
		ERR_MSG(L"Creating Failed Scene_Change_SceneManager _ MAINMENU");
		return E_FAIL;
	}
	CSoundMgr::Get_Instance()->Initialize();
	srand(unsigned(time(NULL)));
	return S_OK;
}

int CMainGame::Update_MainGame()
{
	CTime_Manager::Get_Instance()->Update_Time_Manager();
	CScene_Manager::Get_Instance()->Update_SceneManager();
	return 0;
}

void CMainGame::LateUpdate_MainGame()
{
	CScene_Manager::Get_Instance()->LateUpdate_SceneManager();
}

void CMainGame::Render_MainGame(CFrame_Manager& rFrameManager)
{
	CGraphic_Device::Get_Instance()->Render_Begin();
	CScene_Manager::Get_Instance()->Render_SceneManager();
	rFrameManager.Render_Frame_Manager();
	CGraphic_Device::Get_Instance()->Render_End();
}

void CMainGame::Release_MainGame()
{
	CGameObject_Manager::Destroy_Instance();
	CScene_Manager::Destroy_Instance();
	CTexture_Manager::Destroy_Instance();
	CGraphic_Device::Destroy_Instance();
	CTime_Manager::Destroy_Instance();
	CSoundMgr::Destroy_Instance();
}
