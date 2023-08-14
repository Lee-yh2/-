#include "stdafx.h"
#include "Inventory.h"
#include "Texture_Manager.h"
#include "Graphic_Device.h"
#include "GameObject_Manager.h"
#include "Item.h"
#include "Player.h"


CInventory::CInventory()
	:m_iAlpha(0), m_bOpenInven(false), m_bHaveItem(false), m_iCurCheckIndex(0), m_iPreCheckIndex(0), m_iPreItemIndex(0)
	, m_bPick(false), m_bSetFirst(false), m_bOpenShop(false), m_bCollisionStand(false), m_bNoItem(false), m_bPriceOn(false)
	, m_iCurPriceIndex(0)
{
}


CInventory::~CInventory()
{
}

HRESULT CInventory::Ready_GameObject()
{
	for (int i = 0; i < 26; ++i)
	{
		if (i < 5)
		{
			m_vItemPos[i] = { 137.f + float(51 * i), 151.f, 0.f };
		}
		else if (i < 20)
		{
			m_vItemPos[i] = { 138.f + float(51 * (i % 5)), 212.f + float(52 * (i / 5 - 1)), 0.f };
		}
		else if(i < 22)
		{
			m_vItemPos[i] = { 514.f + (124.f * (i % 20)), 148.f, 0.f };
		}
		else
		{
			if (22 == i)
				m_vItemPos[i] = { 525.f, 151.5f, 0.f };
			else if (23 == i)
				m_vItemPos[i] = { 701.f, 151.5f, 0.f };
			else if (24 == i)
				m_vItemPos[i] = { 525.f, 304.f, 0.f };
			else if (25 == i)
				m_vItemPos[i] = { 701.f, 304.f, 0.f };
		}
	}

	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 6; ++j)
		{
			if (0 == i)
				m_vPricePos[i][j] = { 542.f - j * 6.5f, 201.f, 0.f };
			else if (1 == i)
				m_vPricePos[i][j] = { 718.f - j * 6.5f, 201.f, 0.f };
			else if (2 == i)
				m_vPricePos[i][j] = { 542.f - j * 6.5f, 353.5f, 0.f };
			else if (3 == i)
				m_vPricePos[i][j] = { 718.f - j * 6.5f, 353.5f, 0.f };

		}
	}
	m_strObjectKey = L"GAME_UI";
	m_strStateKey = L"INVENTORY";
	for (int i = 0; i < 4; ++i)
		m_iPrice[i] = 0;

	CGameObject* pObj = new CItem;
	static_cast<CItem*>(pObj)->Set_Item(CItem::WEAPON_BIG);
	static_cast<CItem*>(pObj)->Set_State(CItem::EQUIP);
	static_cast<CItem*>(pObj)->Set_Alpha();
	pObj->Ready_GameObject();
	CGameObject_Manager::Get_Instance()->Add_GameObject(CGameObject_Manager::OBJ_ITEM, pObj);
	Insert_Item(pObj);
	pObj->Set_Pos(m_vItemPos[20]);
	m_EquipWeapon[0] = pObj;

	pObj = new CItem;
	static_cast<CItem*>(pObj)->Set_Item(CItem::WEAPON_SHORT);
	static_cast<CItem*>(pObj)->Set_State(CItem::INVEN);
	static_cast<CItem*>(pObj)->Set_Alpha();
	pObj->Ready_GameObject();
	CGameObject_Manager::Get_Instance()->Add_GameObject(CGameObject_Manager::OBJ_ITEM, pObj);
	Insert_Item(pObj);

	pObj = new CItem;
	static_cast<CItem*>(pObj)->Set_Item(CItem::WEAPON_BOW);
	static_cast<CItem*>(pObj)->Set_State(CItem::INVEN);
	static_cast<CItem*>(pObj)->Set_Alpha();
	pObj->Ready_GameObject();
	CGameObject_Manager::Get_Instance()->Add_GameObject(CGameObject_Manager::OBJ_ITEM, pObj);
	Insert_Item(pObj);



	m_iCurCheckIndex = 0;
	m_vCheckPos = m_vItemPos[m_iCurCheckIndex];
	m_tInfo.vPos = { WINCX / 2.f, WINCY / 2.f, 0.f };
	m_tInfo.vSize = { 1.f, 1.f, 0.f };
	return S_OK;
}

int CInventory::Update_GameObject()
{
	if (!m_bSetFirst)
	{
		static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Set_EquipWeapon(0, m_EquipWeapon[0]);
		m_bSetFirst = true;
	}
	if (m_tKeyManager.KeyDown(KEY_I))
	{
		for (auto& pItem : m_listInven)
		{
			if (CItem::INVEN == static_cast<CItem*>(pItem)->Get_State() || CItem::EQUIP == static_cast<CItem*>(pItem)->Get_State())
				static_cast<CItem*>(pItem)->Set_Alpha();
		}
		Set_Alpha();

		if (*m_bOpenInven)
			m_iCurCheckIndex = 0;
		*m_bOpenInven = !*m_bOpenInven;
	}

	/*if (m_bCollisionStand)
	{
		if (m_tKeyManager.KeyDown(KEY_Q))
		{
			for (auto& pItem : m_listInven)
			{
				if (CItem::INVEN == static_cast<CItem*>(pItem)->Get_State() || CItem::SHOPSTAND == static_cast<CItem*>(pItem)->Get_State())
					static_cast<CItem*>(pItem)->Set_Alpha();
			}
			Set_Alpha();
			if (*m_bOpenShop)
				m_iCurCheckIndex = 0;
			*m_bOpenShop = !*m_bOpenShop;
		}
	}*/


	if (*m_bOpenInven)
	{
		InvenKey();
		if (m_bPick)
		{
			D3DXVECTOR3	vPos = { m_vCheckPos.x, m_vCheckPos.y - 50.f, 0.f };
			m_CurPickItem->Set_Pos(vPos);
		}
		
	}

	if (*m_bOpenShop)
	{
		if (22 <= m_iCurCheckIndex && 25 >= m_iCurCheckIndex)
		{
			if (m_tKeyManager.KeyDown(KEY_ENTER))
			{
				m_bPriceOn = !m_bPriceOn;
				m_iCurPriceIndex = 0;
			}
		}
		if (!m_bPriceOn)
		{
			ShopKey();
			if (m_bPick)
			{
				D3DXVECTOR3	vPos = { m_vCheckPos.x, m_vCheckPos.y - 50.f, 0.f };
				m_CurPickItem->Set_Pos(vPos);
			}
		}
		else
			PriceKey();

	}

	m_tKeyManager.KeyCheck();
	return OBJ_NOEVENT;
}

void CInventory::Late_Update_GameObject()
{
}

void CInventory::Render_GameObject()
{
	if (*m_bOpenInven)
	{
		const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(m_strObjectKey, m_strStateKey, 0);
		if (nullptr == pTexInfo)
			return;
		D3DXMATRIX matWorld, matTrans, matScale;
		D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
		D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);
		float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
		float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);
		matWorld = matScale * matTrans;
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
		CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(m_iAlpha, 255, 255, 255));
	}
	if (*m_bOpenShop)
	{
		const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(m_strObjectKey, m_strStateKey, 2);
		if (nullptr == pTexInfo)
			return;
		D3DXMATRIX matWorld, matTrans, matScale;
		D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
		D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);
		float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
		float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);
		matWorld = matScale * matTrans;
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
		CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(m_iAlpha, 255, 255, 255));
	}
	if (*m_bOpenInven || *m_bOpenShop)
	{
		const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(m_strObjectKey, m_strStateKey, 1);
		if (nullptr == pTexInfo)
			return;
		D3DXMATRIX matWorld, matTrans, matScale;
		D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
		D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);
		float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
		float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);
		matWorld = matScale * matTrans;
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
		CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(m_iAlpha, 255, 255, 255));

		pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(m_strObjectKey, m_strStateKey, 3);
		if (nullptr == pTexInfo)
			return;
		D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
		D3DXMatrixTranslation(&matTrans, m_vCheckPos.x, m_vCheckPos.y, 0.f);
		fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
		fCenterY = float(pTexInfo->tImageInfo.Height >> 1);
		matWorld = matScale * matTrans;
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
		CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(m_iAlpha, 255, 255, 255));

		if (m_bPick)
		{
			pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(m_strObjectKey, m_strStateKey, 4);
			if (nullptr == pTexInfo)
				return;
			D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
			D3DXMatrixTranslation(&matTrans, m_vCheckPos.x, m_vCheckPos.y - 50.f, 0.f);
			fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
			fCenterY = float(pTexInfo->tImageInfo.Height >> 1);
			matWorld = matScale * matTrans;
			CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
			CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(m_iAlpha, 255, 255, 255));
		}
	}

	if (*m_bOpenShop)
	{
		for (int i = 0; i < 4; ++i)
		{
			int iNum;
			if (nullptr == m_ShopStandItem[i])
				iNum = 0;
			else
				iNum = static_cast<CItem*>(m_ShopStandItem[i])->Get_Num();
			TCHAR strNum[MAX_PATH];
			//wstring strNum = L""+m_iNum;
			wsprintf(strNum, TEXT("x%d"), iNum);
			D3DXMATRIX matWorld, matTrans, matScale;
			D3DXMatrixScaling(&matScale, 0.6f, 0.6f, 0.f);
			D3DXMatrixTranslation(&matTrans, m_vItemPos[i+22].x + 33.f, m_vItemPos[i + 22].y + 59.f, 0.f);
			matWorld = matScale * matTrans;
			CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
			CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(), strNum, lstrlen(strNum), nullptr, DT_RIGHT, D3DCOLOR_ARGB(m_iAlpha, 20, 157, 114));
		
			if (nullptr == m_ShopStandItem[i])
				m_iPrice[i] = 0;
			//wstring strNum = L""+m_iNum;
			wsprintf(strNum, TEXT("%06d"), m_iPrice[i]);
			D3DXMatrixScaling(&matScale, 0.6f, 0.6f, 0.f);
			D3DXMatrixTranslation(&matTrans, m_vItemPos[i + 22].x - 20.f, m_vItemPos[i + 22].y + 46.f, 0.f);
			matWorld = matScale * matTrans;
			CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
			CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(), strNum, lstrlen(strNum), nullptr, DT_RIGHT, D3DCOLOR_ARGB(m_iAlpha, 0, 0, 0));

			if (nullptr == m_ShopStandItem[i])
				m_iPrice[i] = 0;
			//wstring strNum = L""+m_iNum;
			wsprintf(strNum, TEXT("%d"), m_iPrice[i] * iNum);
			D3DXMatrixScaling(&matScale, 0.6f, 0.6f, 0.f);
			D3DXMatrixTranslation(&matTrans, m_vItemPos[i + 22].x + 5.f, m_vItemPos[i + 22].y + 76.f, 0.f);
			matWorld = matScale * matTrans;
			RECT rect = { 0, 0, 72, 20 };
			CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
			CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(), strNum, lstrlen(strNum), &rect, DT_RIGHT, D3DCOLOR_ARGB(m_iAlpha, 255, 255, 255));

		}
		if (m_bPriceOn)
		{
			const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(m_strObjectKey, m_strStateKey, 5);
			if (nullptr == pTexInfo)
				return;
			D3DXMATRIX matWorld, matTrans, matScale;
			D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
			D3DXMatrixTranslation(&matTrans, m_vPricePos[m_iCurCheckIndex- 22][m_iCurPriceIndex].x, m_vPricePos[m_iCurCheckIndex - 22][m_iCurPriceIndex].y, 0.f);
			float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
			float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);
			matWorld = matScale * matTrans;
			CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
			CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
		}
	}
}

void CInventory::Release_GameObject()
{
}

void CInventory::Set_Alpha()
{
	if (255 == m_iAlpha)
		m_iAlpha = 0;
	else
		m_iAlpha = 255;
}

void CInventory::Insert_Item(CGameObject * pItem)
{
	for (int i = 0; i < 20; ++i)
	{
		m_bHaveItem = false;
		for (auto& pItem : m_listInven)
		{
			if (pItem->Get_Info().vPos == m_vItemPos[i])
			{
				m_bHaveItem = true;
				break;
			}
		}
		if (!m_bHaveItem)
		{
			pItem->Set_Pos(m_vItemPos[i]);
			break;
		}
	}
	m_listInven.emplace_back(pItem);
}

void CInventory::InvenKey()
{
	if (m_tKeyManager.KeyDown(KEY_LEFT))
	{
		if (0 == m_iCurCheckIndex)
			m_iCurCheckIndex = m_iCurCheckIndex;
		else if (20 == m_iCurCheckIndex)
		{
			m_iCurCheckIndex = m_iPreCheckIndex;
		}
		else
			m_iCurCheckIndex = --m_iCurCheckIndex;

		m_vCheckPos = m_vItemPos[m_iCurCheckIndex];
	}
	if (m_tKeyManager.KeyDown(KEY_RIGHT))
	{
		if (4 == m_iCurCheckIndex || 9 == m_iCurCheckIndex || 14 == m_iCurCheckIndex || 19 == m_iCurCheckIndex)
		{
			m_iPreCheckIndex = m_iCurCheckIndex;
			m_iCurCheckIndex = 20;
		}
		else if (21 == m_iCurCheckIndex)
			m_iCurCheckIndex = m_iCurCheckIndex;
		else
			m_iCurCheckIndex = ++m_iCurCheckIndex;

		m_vCheckPos = m_vItemPos[m_iCurCheckIndex];
	}
	if (m_tKeyManager.KeyDown(KEY_UP))
	{
		if (0 <= m_iCurCheckIndex && 4 >= m_iCurCheckIndex)
			m_iCurCheckIndex = m_iCurCheckIndex;
		else if (21 == m_iCurCheckIndex || 22 == m_iCurCheckIndex)
			m_iCurCheckIndex = m_iCurCheckIndex;
		else
			m_iCurCheckIndex = m_iCurCheckIndex - 5;

		m_vCheckPos = m_vItemPos[m_iCurCheckIndex];
	}
	if (m_tKeyManager.KeyDown(KEY_DOWN))
	{
		if (15 <= m_iCurCheckIndex && 19 >= m_iCurCheckIndex)
			m_iCurCheckIndex = m_iCurCheckIndex;
		else if (21 == m_iCurCheckIndex || 22 == m_iCurCheckIndex)
			m_iCurCheckIndex = m_iCurCheckIndex;
		else
			m_iCurCheckIndex = m_iCurCheckIndex + 5;

		m_vCheckPos = m_vItemPos[m_iCurCheckIndex];
	}

	if (m_tKeyManager.KeyDown(KEY_A))
	{
		if ((20 == m_iCurCheckIndex && 0 == static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Get_CurEquipNum())
			|| (21 == m_iCurCheckIndex && 1 == static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Get_CurEquipNum()))
		{
		}
		else
		{
			for (auto& pItem : m_listInven)
			{
				if (pItem->Get_Info().vPos == m_vItemPos[m_iCurCheckIndex])
				{
					m_bPick = true;
					m_iPreItemIndex = m_iCurCheckIndex;
					m_CurPickItem = pItem;
					break;
				}
			}
		}
		
	}
	if (m_tKeyManager.KeyUp(KEY_A))
	{
		if (m_bPick)
		{
			m_bPick = false;

			if (20 == m_iCurCheckIndex || 21 == m_iCurCheckIndex)			//장비창으로 템 옮길때
			{
				if (!static_cast<CItem*>(m_CurPickItem)->Get_IsWeapon())	//무기아니면 원래 자리로
					m_CurPickItem->Set_Pos(m_vItemPos[m_iPreItemIndex]);
				else                                                        //무기면
				{
					if (20 == m_iPreItemIndex || 21 == m_iPreItemIndex)		//끌어온 곳이 장비창이면 
					{
						for (auto& pItem : m_listInven)
						{
							if (pItem->Get_Info().vPos == m_vItemPos[m_iCurCheckIndex])
							{
								static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Set_EquipWeapon(m_iCurCheckIndex - 20, nullptr);
								static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Set_EquipWeapon(m_iCurCheckIndex - 20, m_CurPickItem);
								m_EquipWeapon[m_iCurCheckIndex - 20] = nullptr;
								m_EquipWeapon[m_iCurCheckIndex - 20] = m_CurPickItem;

								static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Set_EquipWeapon(m_iPreItemIndex - 20, nullptr);
								static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Set_EquipWeapon(m_iPreItemIndex - 20, pItem);
								m_EquipWeapon[m_iPreItemIndex - 20] = nullptr;
								m_EquipWeapon[m_iPreItemIndex - 20] = pItem;

								pItem->Set_Pos(m_vItemPos[m_iPreItemIndex]);
								break;
							}
						}
						m_CurPickItem->Set_Pos(m_vItemPos[m_iCurCheckIndex]);
					}
					else                                                    //인벤토리에서 가져올때
					{
						for (auto& pItem : m_listInven)
						{
							if (pItem->Get_Info().vPos == m_vItemPos[m_iCurCheckIndex])
							{
								pItem->Set_Pos(m_vItemPos[m_iPreItemIndex]);
								static_cast<CItem*>(pItem)->Set_State(CItem::INVEN);
								break;
							}
						}
						
						static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Set_EquipWeapon(m_iCurCheckIndex - 20, nullptr);
						static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Set_EquipWeapon(m_iCurCheckIndex - 20, m_CurPickItem);
						m_EquipWeapon[m_iCurCheckIndex - 20] = nullptr;
						m_EquipWeapon[m_iCurCheckIndex - 20] = m_CurPickItem;
	
						static_cast<CItem*>(m_CurPickItem)->Set_State(CItem::EQUIP);
						m_CurPickItem->Set_Pos(m_vItemPos[m_iCurCheckIndex]);
					}
				}
				m_CurPickItem = nullptr;
			}
			else                                                                               //일반 인벤토리로 템 옮길때
			{
				if (20 == m_iPreItemIndex || 21 == m_iPreItemIndex)								//장비창에서 무기 들고왔으면
				{
					for (auto& pItem : m_listInven)
					{
						m_bNoItem = false;
						if (pItem->Get_Info().vPos == m_vItemPos[m_iCurCheckIndex])
						{
								if (static_cast<CItem*>(pItem)->Get_IsWeapon())					//놓으려고 하는곳에 무기가 있으면 서로 바뀌도록
								{
									static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Set_EquipWeapon(m_iPreItemIndex - 20, nullptr);
									static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Set_EquipWeapon(m_iPreItemIndex - 20, pItem);
									m_EquipWeapon[m_iPreItemIndex - 20] = nullptr;
									m_EquipWeapon[m_iPreItemIndex - 20] = pItem;
									
									static_cast<CItem*>(pItem)->Set_State(CItem::EQUIP);
									pItem->Set_Pos(m_vItemPos[m_iPreItemIndex]);
									m_bNoItem = true;
									//m_CurPickItem->Set_Pos(m_vItemPos[m_iCurCheckIndex]);
									break;
								}
								else                                                             //아니면 원래자리로 돌아가도록
								{
									m_CurPickItem->Set_Pos(m_vItemPos[m_iPreItemIndex]);
									m_CurPickItem = nullptr;
									break;
								}
						}
					}
					if (m_CurPickItem)
					{
						if (!m_bNoItem)
						{
							static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Set_EquipWeapon(m_iPreItemIndex - 20, nullptr);
							m_EquipWeapon[m_iPreItemIndex - 20] = nullptr;
						}
						static_cast<CItem*>(m_CurPickItem)->Set_State(CItem::INVEN);
						m_CurPickItem->Set_Pos(m_vItemPos[m_iCurCheckIndex]);
					}
				}
				else                                                                //인벤토리에서 끼리 바꿀때
				{
					for (auto& pItem : m_listInven)
					{
						if (pItem->Get_Info().vPos == m_vItemPos[m_iCurCheckIndex])
						{
							if (static_cast<CItem*>(pItem)->Get_Item() == static_cast<CItem*>(m_CurPickItem)->Get_Item())
							{
								static_cast<CItem*>(pItem)->Set_Num(static_cast<CItem*>(m_CurPickItem)->Get_Num());
								m_CurPickItem->Set_Dead();
							}
							else
							{
								pItem->Set_Pos(m_vItemPos[m_iPreItemIndex]);
							}
							break;
						}
					}
					m_CurPickItem->Set_Pos(m_vItemPos[m_iCurCheckIndex]);
				}
				m_CurPickItem = nullptr;
			}
		}
	}
}

void CInventory::OpenShop()
{
	for (auto& pItem : m_listInven)
	{
		if (CItem::INVEN == static_cast<CItem*>(pItem)->Get_State() || CItem::SHOPSTAND == static_cast<CItem*>(pItem)->Get_State())
			static_cast<CItem*>(pItem)->Set_Alpha();
	}
	Set_Alpha();
	if (*m_bOpenShop)
		m_iCurCheckIndex = 0;
	*m_bOpenShop = !*m_bOpenShop;
}

void CInventory::ShopKey()
{
	if (m_tKeyManager.KeyDown(KEY_LEFT))
	{
		if (0 == m_iCurCheckIndex)
			m_iCurCheckIndex = m_iCurCheckIndex;
		else if (22 == m_iCurCheckIndex || 24 == m_iCurCheckIndex)
		{
			m_iCurCheckIndex = m_iPreCheckIndex;
		}
		else
			m_iCurCheckIndex = --m_iCurCheckIndex;

		m_vCheckPos = m_vItemPos[m_iCurCheckIndex];
	}
	if (m_tKeyManager.KeyDown(KEY_RIGHT))
	{
		if (4 == m_iCurCheckIndex || 9 == m_iCurCheckIndex || 14 == m_iCurCheckIndex || 19 == m_iCurCheckIndex)
		{
			m_iPreCheckIndex = m_iCurCheckIndex;
			m_iCurCheckIndex = 22;
		}
		else if (23 == m_iCurCheckIndex || 25 == m_iCurCheckIndex)
			m_iCurCheckIndex = m_iCurCheckIndex;
		else
			m_iCurCheckIndex = ++m_iCurCheckIndex;

		m_vCheckPos = m_vItemPos[m_iCurCheckIndex];
	}
	if (m_tKeyManager.KeyDown(KEY_UP))
	{
		if (0 <= m_iCurCheckIndex && 4 >= m_iCurCheckIndex)
			m_iCurCheckIndex = m_iCurCheckIndex;
		else if (22 == m_iCurCheckIndex || 23 == m_iCurCheckIndex)
			m_iCurCheckIndex = m_iCurCheckIndex;
		else if (24 == m_iCurCheckIndex || 25 == m_iCurCheckIndex)
			m_iCurCheckIndex = m_iCurCheckIndex - 2;
		else
			m_iCurCheckIndex = m_iCurCheckIndex - 5;

		m_vCheckPos = m_vItemPos[m_iCurCheckIndex];
	}
	if (m_tKeyManager.KeyDown(KEY_DOWN))
	{
		if (15 <= m_iCurCheckIndex && 19 >= m_iCurCheckIndex)
			m_iCurCheckIndex = m_iCurCheckIndex;
		else if (24 == m_iCurCheckIndex || 25 == m_iCurCheckIndex)
			m_iCurCheckIndex = m_iCurCheckIndex;
		else if (22 == m_iCurCheckIndex || 23 == m_iCurCheckIndex)
			m_iCurCheckIndex = m_iCurCheckIndex + 2;
		else
			m_iCurCheckIndex = m_iCurCheckIndex + 5;

		m_vCheckPos = m_vItemPos[m_iCurCheckIndex];
	}

	if (m_tKeyManager.KeyDown(KEY_A))
	{	
		for (auto& pItem : m_listInven)
		{
			if (pItem->Get_Info().vPos == m_vItemPos[m_iCurCheckIndex])
			{
				m_bPick = true;
				m_iPreItemIndex = m_iCurCheckIndex;
				m_CurPickItem = pItem;
				break;
			}
		}
	}
	if (m_tKeyManager.KeyUp(KEY_A))
	{
		if (m_bPick)
		{
			m_bPick = false;

			if (22 <= m_iCurCheckIndex && 25 >= m_iCurCheckIndex)			//상점창으로 템 옮길때
			{
				if (22 <= m_iPreItemIndex && 25 >= m_iPreItemIndex)		//끌어온 곳이 상점창이면 
				{
					for (auto& pItem : m_listInven)
					{
						m_bNoItem = false;
						if (pItem->Get_Info().vPos == m_vItemPos[m_iCurCheckIndex])
						{
							m_ShopStandItem[m_iPreItemIndex - 22] = nullptr;
							m_ShopStandItem[m_iPreItemIndex - 22] = pItem;
							pItem->Set_Pos(m_vItemPos[m_iPreItemIndex]);
							m_bNoItem = true;
							break;
						}
					}
					if(!m_bNoItem)
						m_ShopStandItem[m_iPreItemIndex - 22] = nullptr;
					m_ShopStandItem[m_iCurCheckIndex - 22] = nullptr;
					m_ShopStandItem[m_iCurCheckIndex - 22] = m_CurPickItem;
					m_CurPickItem->Set_Pos(m_vItemPos[m_iCurCheckIndex]);
				}
				else                                                    //인벤토리에서 가져올때
				{
					for (auto& pItem : m_listInven)
					{
						if (pItem->Get_Info().vPos == m_vItemPos[m_iCurCheckIndex])
						{
							static_cast<CItem*>(pItem)->Set_State(CItem::INVEN);
							pItem->Set_Pos(m_vItemPos[m_iPreItemIndex]);
							break;
						}
					}

					static_cast<CItem*>(m_CurPickItem)->Set_State(CItem::SHOPSTAND);
					m_CurPickItem->Set_Pos(m_vItemPos[m_iCurCheckIndex]);
					m_ShopStandItem[m_iCurCheckIndex - 22] = nullptr;
					m_ShopStandItem[m_iCurCheckIndex - 22] = m_CurPickItem;

				}
			m_CurPickItem = nullptr;
			}
			else                                                                               //일반 인벤토리로 템 옮길때
			{
				if (22 <= m_iPreItemIndex && 25 >= m_iPreItemIndex)								//끌고온 곳이 상점창이면
				{
					for (auto& pItem : m_listInven)
					{
						m_bNoItem = false;
						if (pItem->Get_Info().vPos == m_vItemPos[m_iCurCheckIndex])
						{
							pItem->Set_Pos(m_vItemPos[m_iPreItemIndex]);
							static_cast<CItem*>(pItem)->Set_State(CItem::SHOPSTAND);
							m_ShopStandItem[m_iPreItemIndex - 22] = nullptr;
							m_ShopStandItem[m_iPreItemIndex - 22] = pItem;
							m_bNoItem = true;
							break;
						}
					}	
					if (!m_bNoItem)
						m_ShopStandItem[m_iPreItemIndex - 22] = nullptr;
					static_cast<CItem*>(m_CurPickItem)->Set_State(CItem::INVEN);
					m_CurPickItem->Set_Pos(m_vItemPos[m_iCurCheckIndex]);
				}
				else                                                                //인벤토리에서 끼리 바꿀때
				{
					for (auto& pItem : m_listInven)
					{
						if (pItem->Get_Info().vPos == m_vItemPos[m_iCurCheckIndex])
						{
							if (static_cast<CItem*>(pItem)->Get_Item() == static_cast<CItem*>(m_CurPickItem)->Get_Item())
							{
								static_cast<CItem*>(pItem)->Set_Num(static_cast<CItem*>(m_CurPickItem)->Get_Num());
								m_CurPickItem->Set_Dead();
							}
							else
							{
								pItem->Set_Pos(m_vItemPos[m_iPreItemIndex]);
							}
							break;
						}
					}
					m_CurPickItem->Set_Pos(m_vItemPos[m_iCurCheckIndex]);
				}
				m_CurPickItem = nullptr;
			}
		}
	}
}

void CInventory::PriceKey()
{
	if (m_tKeyManager.KeyDown(KEY_LEFT))
	{
		if (5 > m_iCurPriceIndex)
			++m_iCurPriceIndex;
	}
	if (m_tKeyManager.KeyDown(KEY_RIGHT))
	{
		if (0 < m_iCurPriceIndex)
			--m_iCurPriceIndex;
	}
	if (m_tKeyManager.KeyDown(KEY_UP))
	{
		m_iPrice[m_iCurCheckIndex - 22] += Pow(10, m_iCurPriceIndex);
	}
	if (m_tKeyManager.KeyDown(KEY_DOWN))
	{
		m_iPrice[m_iCurCheckIndex - 22] -= Pow(10, m_iCurPriceIndex);

		if (0 > m_iPrice[m_iCurCheckIndex - 22])
			m_iPrice[m_iCurCheckIndex - 22] = 0;
	}
}

int CInventory::Pow(int iA, int iB)
{
	if (0 == iB)
		return 1;

	int Temp = 1;
	
	for (int i = 0; i < iB; ++i)
		Temp *= iA;

	return Temp;
}
