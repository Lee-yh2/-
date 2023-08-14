#pragma once
class CScene;
class CScene_Manager
{
	DECLARE_SINGLETON(CScene_Manager)
public:
	enum SCENEID { SCENE_MAINMENU ,SCENE_LOADING, SCENE_SHOP, SCENE_TOWN, SCENE_BOSSENTER, SCENE_BOSSROOM1, SCENE_BOSSROOM2, SCENE_END };
private:
	CScene_Manager();
	~CScene_Manager();

public:
	HRESULT Scene_Change_SceneManager(SCENEID eID);
	void Update_SceneManager();
	void LateUpdate_SceneManager();
	void Render_SceneManager();
	void Release_SceneManager();
private:
	CScene* m_pScene;
	SCENEID m_eCurScene;
	SCENEID m_eNextScene;
};

