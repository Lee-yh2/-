#pragma once

#ifndef __ITEM_H__
#define __ITEM_H__

#include "GameObject.h"
class CItem :
	public CGameObject
{
public:
	explicit CItem();
	virtual ~CItem();

	enum ITEM {BOSSB_CRISTAL, BOSSB_SEED, BOSSA_KINGCORE, BOSSA_GOLEMPAPER, BOSSA_GOLEMBOOK, BOSSA_IRON , WEAPON_BIG, WEAPON_SHORT, WEAPON_BOW, ITEM_END};
	enum STATE { INVEN, GROUND , EQUIP, SHOPSTAND};
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;

public:
	void Set_Item(ITEM eItem) { m_eItem = eItem; }
	void Set_State(STATE  eState) { m_eState = eState; }
	void Set_Num(int iNum) { m_iNum += iNum; }
	void Set_Alpha();

public:
	STATE Get_State() { return m_eState; }
	ITEM Get_Item() { return m_eItem; }
	bool Get_IsWeapon() { return m_bWeapon; }
	int	Get_Num() { return m_iNum; }
	const wstring Get_ObjectKey() const { return m_strObjectKey; }
	const wstring Get_StateKey() const { return m_strStateKey; }
	int Get_KeyNum() { return m_iKeyNum; }
	D3DXVECTOR3 Parabola();

protected:
	ITEM	m_eItem;
	STATE	m_eState;
	int		m_iKeyNum;
	int		m_iAlpha;
	int		m_iNum;
	wstring m_strObjectKey;
	wstring m_strStateKey;

	bool	m_bWeapon;
	PARABOLAINFO m_tParabola;
};

#endif // !__ITEM_H__
