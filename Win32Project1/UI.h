#pragma once

#ifndef __UI_H__
#define __UI_H__

#include "GameObject.h"
class CUI :
	public CGameObject
{
public:
	explicit CUI();
	virtual ~CUI();

	enum TYPE { HP_BAR, WEAPON, BAG, GOLD, MONSTER_HP, INVENTORY, EQUIPMENT, STAND, WAKEUP, END};
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;

public:
	void Set_Type(TYPE eType) { m_eCurType = eType; }
	void Set_Hp(int* iHp) { m_iHp = iHp; }
protected:
	TYPE	m_eCurType;
	int		m_iCurWeapon;
	int*	m_iHp;
	RECT	m_tHp;
	float	m_fHpPur;

	DWORD	m_dwHp;
	DWORD	m_dwDead;
};

#endif // !__UI_H__
