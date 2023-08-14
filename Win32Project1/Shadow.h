#pragma once

#ifndef __SHADOW_H__
#define __SHADOW_H__

#include "GameObject.h"
class CShadow :
	public CGameObject
{
public:
	explicit CShadow();
	virtual ~CShadow();

	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;

public:
	void Set_ArmInfo(INFO* ArmInfo) { m_tArmInfo = ArmInfo; }
	void Set_Attack(bool* Attack) { m_bAttack = Attack; }
	void Set_Speed(float speed) { m_fSpeed = speed; }
	void Set_Dead(bool* Dead) { m_bDead = Dead; }

protected:
	INFO	m_tTargetInfo;
	INFO*	m_tArmInfo;
	bool*	m_bAttack;
	bool*	m_bDead;
	float	m_fSpeed;
};

#endif // !__SHADOW_H__
