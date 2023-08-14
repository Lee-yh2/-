#pragma once

#ifndef __MONSTER_H__
#define __MONSTER_H__

#include "GameObject.h"
class CMonster :
	public CGameObject
{
public:
	explicit CMonster();
	virtual ~CMonster();

	enum STATE { STATE_WAKEUP, STATE_PUNCH, STATE_PUNCHRECOVER, STATE_SPAWN, STATE_IDLE, STATE_NOARMIDLE, STATE_DEAD, STATE_END };

	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;

public:
	void Scene_Change();
	void FrameMove(float fMoveSpeed = 1.f);
	void Boss_Attack();
	void Create_Stone();
	void Create_Wave();
	void Create_Bullet(D3DXVECTOR3 vRange, D3DXVECTOR3 vDir, BYTE Monster);
	void Set_Start() { m_bStart = true; }
protected:
	wstring		m_strStateKey;

	BYTE		m_byCurrentMonster;
	STATE		m_eCurState;
	STATE		m_ePreState;
	bool		m_bStart;
	bool		m_bLaunch;
	bool		m_bStone;
	bool		m_bWave;
	bool		m_bAttack;
	bool		m_bItem;
	bool		m_bSetDead;
	bool		m_bDeadSound;

	int			m_iAttackTurn;
	DWORD		m_dwAttack;
	DWORD		m_dwRecoveryTime;
};

#endif // !__MONSTER_H__
