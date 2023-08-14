#pragma once

#ifndef __BOSSA_ARM__
#define __BOSSA_ARM__

#include "GameObject.h"
class CBossA_Arm :
	public CGameObject
{
public:
	explicit CBossA_Arm();
	virtual ~CBossA_Arm();

	enum STATE { STATE_PUNCHDOWN, STATE_PUNCHUP };
public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;

public:
	void FrameMove(float fMoveSpeed = 1.f);
	void Set_Target(INFO Target) { m_tTargetInfo = Target; }
	void Create_Bullet(D3DXVECTOR3 vRange, D3DXVECTOR3 vDir, BYTE Monster);

protected:
	int		m_iCount;
	INFO	m_tTargetInfo;
	wstring m_strStateKey;
	STATE	m_eCurrentState;

	bool	m_bAttack;
	bool	m_bDelay;
	float	m_fSpeed;

	DWORD	dw_AttackTime;
	DWORD	dw_DelayTime;
};

#endif // !__BOSSA_ARM__
