#pragma once

#ifndef __BULLET_H__
#define __BULLET_H__

#include "GameObject.h"
class CBullet :
	public CGameObject
{
public:
	explicit CBullet();
	virtual ~CBullet();

	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;

public:
	void Set_Weapon(BYTE CurrentWeapon) { m_tUnitInfo.byCurrentWeapon = CurrentWeapon; }
	void Set_StateKey(wstring stateKey) { m_strStateKey = stateKey; }
protected:
	float		m_fSpeed;
	wstring		m_strStateKey;

	DWORD		dw_Bullet;
};

#endif // !__BULLET_H__
