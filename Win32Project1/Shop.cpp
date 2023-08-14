#include "stdafx.h"
#include "Shop.h"
#include "GameObject_Manager.h"
#include "Terrain.h"
#include "BackGround.h"
#include "Scroll_Manager.h"
#include "SoundMgr.h"
#include "Player.h"
#include "Portal.h"
#include "Item.h"
#include "UI.h"
#include "Interior.h"
#include "Inventory.h"


CShop::CShop()
	:m_Time(GetTickCount()), m_bItem(false)
{
}


CShop::~CShop()
{
}

HRESULT CShop::Ready_Scene()
{
	CGameObject* pObj = new CTerrain;
	static_cast<CTerrain*>(pObj)->Set_PathFile(L"../Data/Shop.dat");
	pObj->Ready_GameObject();
	CGameObject_Manager::Get_Instance()->Add_GameObject(CGameObject_Manager::OBJ_TERRAIN, pObj);

	pObj = new CBackGround;
	static_cast<CBackGround*>(pObj)->Set_PathFile(L"../Data/Shop_BackGround.dat");
	pObj->Ready_GameObject();
	CGameObject_Manager::Get_Instance()->Add_GameObject(CGameObject_Manager::OBJ_BACKGROUND, pObj);

	pObj = new CBackGround;
	static_cast<CBackGround*>(pObj)->Set_PathFile(L"../Data/Shop_BackGround2.dat");
	pObj->Ready_GameObject();
	CGameObject_Manager::Get_Instance()->Add_GameObject(CGameObject_Manager::OBJ_EFFECT, pObj);

	D3DXVECTOR3 vPos;
	int iScene = CScroll_Manager::Get_Scene();
	if (LOADING == iScene)
	{
		pObj = new CPlayer;
		pObj->Ready_GameObject();
		vPos = { 694.f, 264.f, 0.f };
		pObj->Set_Pos(vPos);
		CGameObject_Manager::Get_Instance()->Add_GameObject(CGameObject_Manager::OBJ_PLAYER, pObj);

		pObj = new CUI;
		vPos = { 690.f, 260.f, 0.f };
		static_cast<CUI*>(pObj)->Set_Type(CUI::TYPE::WAKEUP);
		pObj->Set_Pos(vPos);
		pObj->Ready_GameObject();
		CGameObject_Manager::Get_Instance()->Add_GameObject(CGameObject_Manager::OBJ_UI, pObj);
	}
	else if (BOSSROOMA == iScene || BOSSROOMB == iScene)
	{
		vPos = { 650.f, 300.f, 0.f };
		CGameObject_Manager::Get_Instance()->Get_Player()->Set_Pos(vPos);
	}
	else if (TOWN == iScene)
	{
		vPos = { 545.f, 845.f, 0.f };
		CGameObject_Manager::Get_Instance()->Get_Player()->Set_Pos(vPos);
	}
	static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Offset();

	pObj = new CInterior;
	static_cast<CInterior*>(pObj)->Set_Furniture(CInterior::FURNITURE::BED);
	vPos = { 690.f, 260.f, 0.f };
	pObj->Set_Pos(vPos);
	pObj->Ready_GameObject();
	CGameObject_Manager::Get_Instance()->Add_GameObject(CGameObject_Manager::OBJ_INTERIOR, pObj);

	pObj = new CInterior;
	static_cast<CInterior*>(pObj)->Set_Furniture(CInterior::FURNITURE::CARPET);
	vPos = { 690.f, 260.f, 0.f };
	pObj->Set_Pos(vPos);
	pObj->Ready_GameObject();
	CGameObject_Manager::Get_Instance()->Add_GameObject(CGameObject_Manager::OBJ_INTERIOR, pObj);

	pObj = new CInterior;
	static_cast<CInterior*>(pObj)->Set_Furniture(CInterior::FURNITURE::STAND);
	static_cast<CInterior*>(pObj)->Set_Stand(static_cast<CInventory*>(CGameObject_Manager::Get_Instance()->Get_Inven())->Get_ShopStandItem());
	static_cast<CInterior*>(pObj)->Set_Price(static_cast<CInventory*>(CGameObject_Manager::Get_Instance()->Get_Inven())->Get_Price());
	vPos = { 359.f, 714.f, 0.f };
	pObj->Set_Pos(vPos);
	pObj->Ready_GameObject();
	CGameObject_Manager::Get_Instance()->Add_GameObject(CGameObject_Manager::OBJ_INTERIOR, pObj);

	pObj = new CInterior;
	static_cast<CInterior*>(pObj)->Set_Furniture(CInterior::FURNITURE::DOOR);
	vPos = { 550.f, 897.f, 0.f };
	pObj->Set_Pos(vPos);
	pObj->Ready_GameObject();
	CGameObject_Manager::Get_Instance()->Add_GameObject(CGameObject_Manager::OBJ_INTERIOR, pObj);

	pObj = new CPortal;
	static_cast<CPortal*>(pObj)->Set_GoScene(CPortal::GO_SHOPTOTOWN);
	vPos = { 545.f, 845.f, 0.f };
	pObj->Set_Pos(vPos);
	pObj->Ready_GameObject();
	CGameObject_Manager::Get_Instance()->Add_GameObject(CGameObject_Manager::OBJ_PORTAL, pObj);

	pObj = new CPortal;
	static_cast<CPortal*>(pObj)->Set_GoScene(CPortal::OPEN_STAND);
	vPos = { 359.f, 714.f, 0.f };
	pObj->Set_Pos(vPos);
	pObj->Ready_GameObject();
	vPos = { 96.f, 113.f, 0.f };
	pObj->Set_CSize(vPos);
	CGameObject_Manager::Get_Instance()->Add_GameObject(CGameObject_Manager::OBJ_PORTAL, pObj);

	CSoundMgr::Get_Instance()->PlayBGM(L"LODING.wav");
	CScroll_Manager::Set_Scene(SHOP);
	return S_OK;
}

void CShop::Update_Scene()
{
	/*if (m_Time + 1000 < GetTickCount() && !m_bItem)
	{
		CGameObject* pObj = new CItem;
		D3DXVECTOR3 vPos = { 650.f, 300.f, 0.f };
		pObj->Set_Pos(vPos);
		static_cast<CItem*>(pObj)->Set_Item(CItem::BOSSA_IRON);
		static_cast<CItem*>(pObj)->Set_State(CItem::GROUND);
		static_cast<CItem*>(pObj)->Set_Num(rand()%7+1);
		pObj->Ready_GameObject();
		CGameObject_Manager::Get_Instance()->Add_GameObject(CGameObject_Manager::OBJ_ITEM, pObj);

		pObj = new CItem;
		vPos = { 650.f, 300.f, 0.f };
		pObj->Set_Pos(vPos);
		static_cast<CItem*>(pObj)->Set_Item(CItem::BOSSA_IRON);
		static_cast<CItem*>(pObj)->Set_State(CItem::GROUND);
		static_cast<CItem*>(pObj)->Set_Num(rand() % 7 + 1);
		pObj->Ready_GameObject();
		CGameObject_Manager::Get_Instance()->Add_GameObject(CGameObject_Manager::OBJ_ITEM, pObj);

		pObj = new CItem;
		vPos = { 650.f, 300.f, 0.f };
		pObj->Set_Pos(vPos);
		static_cast<CItem*>(pObj)->Set_Item(CItem::BOSSA_IRON);
		static_cast<CItem*>(pObj)->Set_State(CItem::GROUND);
		static_cast<CItem*>(pObj)->Set_Num(rand() % 7 + 1);
		pObj->Ready_GameObject();
		CGameObject_Manager::Get_Instance()->Add_GameObject(CGameObject_Manager::OBJ_ITEM, pObj);

		m_bItem = true;
	}*/

	CGameObject_Manager::Get_Instance()->Update_GameObject_Manager();
}

void CShop::LateUpdate_Scene()
{
	CGameObject_Manager::Get_Instance()->Late_Update_GameObject_Manager();
	CScroll_Manager::Scroll_Lock();
}

void CShop::Render_Scene()
{
	CGameObject_Manager::Get_Instance()->Render_GameObject_Manager();
}

void CShop::Release_Scene()
{
}
