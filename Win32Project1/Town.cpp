#include "stdafx.h"
#include "Town.h"
#include "Terrain.h"
#include "BackGround.h"
#include "GameObject_Manager.h"
#include "Scroll_Manager.h"
#include "SoundMgr.h"
#include "Player.h"
#include "Portal.h"

CTown::CTown()
{
}


CTown::~CTown()
{
	Release_Scene();
}

HRESULT CTown::Ready_Scene()
{
	CGameObject* pObj = new CTerrain;
	static_cast<CTerrain*>(pObj)->Set_PathFile(L"../Data/Town.dat");
	pObj->Ready_GameObject();
	CGameObject_Manager::Get_Instance()->Add_GameObject(CGameObject_Manager::OBJ_TERRAIN, pObj);

	pObj = new CBackGround;
	static_cast<CBackGround*>(pObj)->Set_PathFile(L"../Data/TownBackGround.dat");
	pObj->Ready_GameObject();
	CGameObject_Manager::Get_Instance()->Add_GameObject(CGameObject_Manager::OBJ_BACKGROUND, pObj);
	
	/*pObj = new CPlayer;
	pObj->Ready_GameObject();
	D3DXVECTOR3 vPos = { 1000.f, 190.f, 0.f };
	pObj->Set_Pos(vPos);
	CGameObject_Manager::Get_Instance()->Add_GameObject(CGameObject_Manager::OBJ_PLAYER, pObj);*/

	D3DXVECTOR3 vPos;
	int iScene = CScroll_Manager::Get_Scene();
	if (BOSSROOMA == iScene || BOSSROOMB == iScene)
	{
		vPos = { 650.f, 300.f, 0.f };
		CGameObject_Manager::Get_Instance()->Get_Player()->Set_Pos(vPos);
	}
	else if (SHOP == iScene)
	{
		vPos = { 1000.f, 190.f, 0.f };
		CGameObject_Manager::Get_Instance()->Get_Player()->Set_Pos(vPos);
	}
	else if (BOSSENTER == iScene)
	{
		vPos = { 165.f, 80.f, 0.f };
		CGameObject_Manager::Get_Instance()->Get_Player()->Set_Pos(vPos);
	}
	static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Offset();

	pObj = new CPortal;
	static_cast<CPortal*>(pObj)->Set_GoScene(CPortal::GO_SHOP);
	vPos = { 1000.f, 170.f, 0.f };
	pObj->Set_Pos(vPos);
	pObj->Ready_GameObject();
	CGameObject_Manager::Get_Instance()->Add_GameObject(CGameObject_Manager::OBJ_PORTAL, pObj);

	pObj = new CPortal;
	static_cast<CPortal*>(pObj)->Set_GoScene(CPortal::GO_BOSSENTER);
	vPos = { 165.f, 10.f, 0.f };
	pObj->Set_Pos(vPos);
	pObj->Ready_GameObject();
	CGameObject_Manager::Get_Instance()->Add_GameObject(CGameObject_Manager::OBJ_PORTAL, pObj);

	CSoundMgr::Get_Instance()->PlayBGM(L"LODING.wav");
	CScroll_Manager::Set_Scene(TOWN);
	return S_OK;
}

void CTown::Update_Scene()
{
	CGameObject_Manager::Get_Instance()->Update_GameObject_Manager();
}

void CTown::LateUpdate_Scene()
{
	CGameObject_Manager::Get_Instance()->Late_Update_GameObject_Manager();
	CScroll_Manager::Scroll_Lock();
}

void CTown::Render_Scene()
{
	CGameObject_Manager::Get_Instance()->Render_GameObject_Manager();
}

void CTown::Release_Scene()
{
}
