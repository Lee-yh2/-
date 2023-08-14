#pragma once

#ifndef __PLANT_H__
#define __PLANT_H__

#include "GameObject.h"
class CPlant :
	public CGameObject
{
public:
	explicit CPlant();
	virtual ~CPlant();

	enum STATE { STATE_WAKEUP, STATE_IDLE, STATE_SHOT, STATE_DEAD, END };
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;

public:
	void FrameMove(float fMoveSpeed = 1.f, float fMoveBodySpeed = 1.f);
	void Scene_Change();
	void Attack();
	void Set_Target(const INFO* tTarget) { m_tTargetInfo = tTarget; }
	void Set_PotTarget(D3DXVECTOR3 tTarget) { m_tPotTarget = tTarget; }

protected:
	wstring		m_strHeadStateKey;
	wstring		m_strBodyStateKey;
	wstring		m_strStateKey;

	STATE		m_eCurState;
	STATE		m_ePreState;
	bool		m_bStart;
	bool		m_bAttack;
	bool		m_bPots;
	bool		m_bShot;

	DWORD		m_dwAttack;

	D3DXVECTOR3	m_tPotTarget;
	D3DXVECTOR3	m_vecPosin;
	const INFO*	m_tTargetInfo;
	FRAME		m_tBodyFrame;

	float		m_fAngle;
	float		m_fSpeed;
	float		m_fPosinDis;
};

#endif // !__PLANT_H__
