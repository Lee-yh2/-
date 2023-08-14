#pragma once

#ifndef __MONSTERB_H__
#define __MONSTERB_H__

#include "GameObject.h"
class CMonsterB :
	public CGameObject
{
public:
	explicit CMonsterB();
	virtual ~CMonsterB();
	
	enum STATE { STATE_WAKEUP, STATE_IDLE, STATE_POTS, STATE_READYSHOT, STATE_SHOT, STATE_ENDSHOT, STATE_JUMP, STATE_ROOT, STATE_DEAD, STATE_END };

	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;

public:
	void Scene_Change();
	void FrameMove(float fMoveSpeed = 1.f , float fMoveBodySpeed = 1.f);
	void Boss_Attack();
	void Move();
	void Pots();
	void Shot();
	void Root();
	void CreatePot();
	void CreateRoot();
	void CreateBullet(D3DXVECTOR3 vPos,D3DXVECTOR3 vDir, BYTE Monster);
	void Set_Start() { m_bStart = true; }
	void Set_Target(const INFO* tTarget) { m_tTargetInfo = tTarget; }

protected:
	wstring		m_strHeadObjectKey;
	wstring		m_strHeadStateKey;
	wstring		m_strBodyObjectKey;
	wstring		m_strBodyStateKey;

	STATE		m_eCurState;
	STATE		m_ePreState;
	bool		m_bStart;
	bool		m_bAttack;
	bool		m_bWakeUp;
	bool		m_bJump;
	bool		m_bJumpSound;
	bool		m_bPotsSound;
	bool		m_bDeadSound;

	bool		m_bFirstPot;
	bool		m_bSecondPot;
	bool		m_bThirdPot;

	bool		m_bFirstRoot;
	bool		m_bSecondRoot;
	bool		m_bThirdRoot;

	bool		m_bItem;

	int			m_iAttackTurn;
	float		m_fSpeed;
	float		m_fAngle;
	float		m_fPosinDis;
	DWORD		m_dwAttack;
	DWORD		m_dwJump;
	DWORD		m_dwShot;

	D3DXVECTOR3	m_vecPosin[4];
	const INFO*	m_tTargetInfo;
	FRAME		m_tBodyFrame;
};

#endif // !__MONSTERB_H__
