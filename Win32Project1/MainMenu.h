#pragma once

#ifndef __MAINMENU_H__
#define __MAINMENU_H__
#include "Scene.h"
class CMainMenu final
	:public CScene
{
public:
	CMainMenu();
	~CMainMenu();

	// CScene을(를) 통해 상속됨
	virtual HRESULT Ready_Scene() override;
	virtual void Update_Scene() override;
	virtual void LateUpdate_Scene() override;
	virtual void Render_Scene() override;
	virtual void Release_Scene() override;

public:
	void FrameMove(float fSpeed);
	void Draw_Back();
	void Key_Set();

private:
	FRAME				m_tFrame;
	int					m_iAlpha;
	D3DXVECTOR3			m_tLeftLogo;
	D3DXVECTOR3			m_tRightLogo;
	vector<MENUBUTTON>	m_vecMenu;
	float				m_fMenuIcon[2];
	int					m_iCurrentSelMenu;
	bool				m_bStart;
};

#endif // !__MAINMENU_H__
