#include "stdafx.h"
#include "Scene_Manager.h"
#include "Scene.h"
#include "Town.h"
#include "MainMenu.h"
#include "Loading.h"
#include "BossRoom1.h"
#include "BossRoom2.h"
#include "Shop.h"
#include "BossEnter.h"

IMPLEMENT_SINGLETON(CScene_Manager)
CScene_Manager::CScene_Manager()
	:m_eCurScene(SCENE_END)
	, m_eNextScene(SCENE_END)
	, m_pScene(nullptr)
{
}


CScene_Manager::~CScene_Manager()
{
	Release_SceneManager();
}

HRESULT CScene_Manager::Scene_Change_SceneManager(SCENEID eID)
{
	m_eNextScene = eID;
	if (m_eNextScene != m_eCurScene)
	{
		Safe_Delete(m_pScene);
		switch (m_eNextScene)
		{
		case CScene_Manager::SCENE_MAINMENU:
			m_pScene = new CMainMenu;
			break;
		case CScene_Manager::SCENE_LOADING:
			m_pScene = new CLoading;
			break;
		case CScene_Manager::SCENE_SHOP:
			m_pScene = new CShop;
			break;
		case CScene_Manager::SCENE_TOWN:
			m_pScene = new CTown;
			break;
		case CScene_Manager::SCENE_BOSSENTER:
			m_pScene = new CBossEnter;
			break;
		case CScene_Manager::SCENE_BOSSROOM1:
			m_pScene = new CBossRoom1;
			break;
		case CScene_Manager::SCENE_BOSSROOM2:
			m_pScene = new CBossRoom2;
			break;
		case CScene_Manager::SCENE_END:
			break;
		default:
			break;
		}
		if (FAILED(m_pScene->Ready_Scene()))
		{
			Safe_Delete(m_pScene);
			ERR_MSG(L"Ready Faield - Scene_Manager()");
			return E_FAIL;
		}
		m_eCurScene = m_eNextScene;
	}
	return S_OK;
}

void CScene_Manager::Update_SceneManager()
{
	m_pScene->Update_Scene();
}

void CScene_Manager::LateUpdate_SceneManager()
{
	m_pScene->LateUpdate_Scene();
}

void CScene_Manager::Render_SceneManager()
{
	m_pScene->Render_Scene();
}

void CScene_Manager::Release_SceneManager()
{
	m_pScene->Release_Scene();
}
