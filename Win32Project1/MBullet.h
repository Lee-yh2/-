#pragma once
#ifndef __MBULLET_H__
#define __MBULLET_H__

#include "GameObject.h"
class CMBullet :
	public CGameObject
{
public:
	explicit CMBullet();
	virtual ~CMBullet();

	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;

public:
	void Set_Weapon(BYTE CurrentMonster) { m_CurrentMonster = CurrentMonster; }
protected:
	BYTE		m_CurrentMonster;
	float		m_fSpeed;
	wstring		m_strObjectKey;
	wstring		m_strStateKey;

	DWORD		dw_Bullet;
};

#endif // !__MBULLET_H__
