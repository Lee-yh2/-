#pragma once

#ifndef __PLAYER_H__
#define __PLAYER_H__
#include "KeyManager.h"
#include "GameObject.h"
class CPlayer final:
	public CGameObject
{
public:
	explicit CPlayer();
	virtual ~CPlayer();

public:
	enum STATE {IDLE, WALK, ATTACK, ROLL, BEATTACK, DEAD, END};
	enum DIRECT {DOWN, UP, RIGHT, LEFT};

	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;

public:
	void FrameMove(float fMoveSpeed = 1.f);
	void Key_Check();
	void Scene_Change();
	void Offset();
	void Create_Bullet(D3DXVECTOR3 vRange, D3DXVECTOR3 vDir, BYTE Weapon);
	void Attack();

public:
	void Set_BeAttack();
	void Set_Inven() { m_bInven = !m_bInven; }
	void Set_EquipWeapon(int iNum, CGameObject* EquipWeapon) { m_EquipWeapon[iNum] = EquipWeapon; }
	void Set_Start() { m_bStart = true; }
	void Set_BossStart() { m_bBossStart = !m_bBossStart; }

	STATE Get_CurState() { return m_eCurState; }
	int	Get_CurEquipNum() { return m_iCurrentEquipNum; }
private:
	STATE			m_eCurState;
	STATE			m_ePreState;
	DIRECT			m_eDirect;

	CGameObject*	m_bullet;
	CGameObject*	m_EquipWeapon[2];
	int				m_iCurrentEquipNum;

	CKeyManager		m_tKeyManager;
	wstring			m_strObjectKey;
	wstring			m_strWeaponObjectKey;
	wstring			m_strStateKey;
	float			m_fSpeed;
	bool			m_bInven;
	bool			m_bShop;

	bool			m_bAttack;
	bool			m_bRoll;
	bool			m_bBeAttack;
	bool			m_bStart;
	bool			m_bBossStart;

	bool			m_bFirstAttack;
	bool			m_bSecondAttack;
	bool			m_bThirdAttack;

	bool			m_bFirstSound;
	bool			m_bSecondSound;
	bool			m_bThirdSound;
	float			m_fWeaponSetX;
	float			m_fWeaponSetY;
	int				m_iAlpha;

	bool			m_bSetAtt;
	D3DXVECTOR3		m_vRange;
	D3DXVECTOR3		m_vDir;
};

#endif // !__PLAYER_H__
