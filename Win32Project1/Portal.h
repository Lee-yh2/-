#pragma once

#ifndef __PORTAL_H__
#define __PORTAL_H__
#include "KeyManager.h"
#include "GameObject.h"
class CPortal :
	public CGameObject
{
public:
	explicit CPortal();
	virtual ~CPortal();

	enum SCENE{ GO_SHOP, GO_SHOPTOTOWN, GO_ENTERTOTOWN, GO_BOSSENTER, GO_BOSSROOMA, GO_BOSSROOMB, OPEN_STAND, END };

	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;

public:
	void Set_GoScene(SCENE eGOScene) { m_eGoScene = eGOScene; }
	void Set_Collision(bool bCollision) { m_bCollision = bCollision; }

	bool Get_Collision() { return m_bCollision; }
	SCENE Get_Scene() { return m_eGoScene; }
protected:
	SCENE	m_eGoScene;
	bool	m_bCollision;
	DWORD	m_dwCoolTime;

	int		m_iAlpha;
	CKeyManager m_tKeyManager;
};

#endif // !__PORTAL_H__
