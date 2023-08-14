#pragma once

#ifndef __INVENTORY_H__
#define __INVENTORY_H__
#include "KeyManager.h"
#include "GameObject.h"
class CInventory :
	public CGameObject
{
public:
	explicit CInventory();
	virtual ~CInventory();

	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;

public:
	void Set_Alpha();
	void Set_OpenInven(bool* bInven) { m_bOpenInven = bInven; }
	void Set_OpenShop(bool* bOpenShop) { m_bOpenShop = bOpenShop; }
	//void Set_bOpenShop(bool bOpenShop) { *m_bOpenShop = bOpenShop; }
	//void Set_EquipWeapon(CGameObject* EquipWeapon[]) { m_EquipWeapon[0] = EquipWeapon[0]; m_EquipWeapon[1] = EquipWeapon[1];}
public: 
	const list<CGameObject*> Get_Inven() const { return m_listInven; }
	//const CGameObject* Get_StandItem() const { return m_ShopStandItem; }
	CGameObject** Get_ShopStandItem() { return m_ShopStandItem; }
	int* Get_Price() { return m_iPrice; }
	//CGameObject* Get_StandItem(int iNum) { return m_ShopStandItem[iNum]; }
	void Insert_Item(CGameObject* pItem);
	void OpenShop();
	void InvenKey();
	void ShopKey();
	void PriceKey();
	int Pow(int iA, int iB);

protected:
	D3DXVECTOR3			m_vItemPos[26]; // 20,21 은 장비창 pos 22,23,24,26 은 상점
	D3DXVECTOR3			m_vCheckPos;
	D3DXVECTOR3			m_vPricePos[4][6];
	list<CGameObject*>	m_listInven;
	CGameObject*		m_CurPickItem;
	CGameObject*		m_EquipWeapon[2];
	CGameObject*		m_ShopStandItem[4];
	int					m_iPrice[4];
	int					m_iSellPrice;

	int					m_iAlpha;
	int					m_iCurCheckIndex;
	int					m_iPreCheckIndex;
	int					m_iPreItemIndex;
	int					m_iCurPriceIndex;

	bool*				m_bOpenInven;
	bool*				m_bOpenShop;
	
	bool				m_bPriceOn;
	bool				m_bNoItem;
	bool				m_bCollisionStand;
	bool				m_bSetFirst;
	bool				m_bPick;
	bool				m_bHaveItem;
	wstring				m_strObjectKey;
	wstring				m_strStateKey;
	CKeyManager			m_tKeyManager;
};

#endif // !__INVENTORY_H__
