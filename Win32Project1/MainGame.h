#pragma once
class CFrame_Manager;
class CMainGame
{
public:
	CMainGame();
	~CMainGame();

public:
	HRESULT Ready_MainGame();
	int Update_MainGame();
	void LateUpdate_MainGame();
	void Render_MainGame(CFrame_Manager& rFrameManager);
	void Release_MainGame();
};

