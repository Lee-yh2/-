#include "stdafx.h"
#include "BossEnter.h"
#include "GameObject_Manager.h"
#include "Terrain.h"
#include "BackGround.h"
#include "Player.h"
#include "Scroll_Manager.h"
#include "Portal.h"
#include "Interior.h"
#include "SoundMgr.h"


CBossEnter::CBossEnter()
{
}


CBossEnter::~CBossEnter()
{
	Release_Scene();
}

HRESULT CBossEnter::Ready_Scene()
{
	CGameObject* pObj = new CTerrain;
	static_cast<CTerrain*>(pObj)->Set_PathFile(L"../Data/Boss_Enter.dat");
	pObj->Ready_GameObject();
	CGameObject_Manager::Get_Instance()->Add_GameObject(CGameObject_Manager::OBJ_TERRAIN, pObj);

	pObj = new CBackGround;
	static_cast<CBackGround*>(pObj)->Set_PathFile(L"../Data/Boss_EnterBackGround.dat");
	pObj->Ready_GameObject();
	CGameObject_Manager::Get_Instance()->Add_GameObject(CGameObject_Manager::OBJ_BACKGROUND, pObj);

	pObj = new CBackGround;
	static_cast<CBackGround*>(pObj)->Set_PathFile(L"../Data/Boss_EnterBackGround2.dat");
	pObj->Ready_GameObject();
	CGameObject_Manager::Get_Instance()->Add_GameObject(CGameObject_Manager::OBJ_EFFECT, pObj);

	/*pObj = new CPlayer;
	pObj->Ready_GameObject();
	D3DXVECTOR3 vPos = { 925.f, 520.f, 0.f };
	pObj->Set_Pos(vPos);
	CGameObject_Manager::Get_Instance()->Add_GameObject(CGameObject_Manager::OBJ_PLAYER, pObj);*/

	D3DXVECTOR3 vPos;
	pObj = new CInterior;
	static_cast<CInterior*>(pObj)->Set_Furniture(CInterior::FURNITURE::PORTALA);
	vPos = { 378.f, 490.f, 0.f };
	pObj->Set_Pos(vPos);
	pObj->Ready_GameObject();
	CGameObject_Manager::Get_Instance()->Add_GameObject(CGameObject_Manager::OBJ_INTERIOR, pObj);

	pObj = new CInterior;
	static_cast<CInterior*>(pObj)->Set_Furniture(CInterior::FURNITURE::PORTALB);
	vPos = { 925.f, 490.f, 0.f };
	pObj->Set_Pos(vPos);
	pObj->Ready_GameObject();
	CGameObject_Manager::Get_Instance()->Add_GameObject(CGameObject_Manager::OBJ_INTERIOR, pObj);

	int iScene = CScroll_Manager::Get_Scene();
	if (BOSSROOMA == iScene)
	{
		vPos = { 378.f, 520.f, 0.f };
		CGameObject_Manager::Get_Instance()->Get_Player()->Set_Pos(vPos);
	}
	else if (BOSSROOMB == iScene)
	{
		vPos = { 925.f, 520.f, 0.f };
		CGameObject_Manager::Get_Instance()->Get_Player()->Set_Pos(vPos);
	}
	else if (TOWN == iScene)
	{
		vPos = { 658.f, 1040.f, 0.f };
		CGameObject_Manager::Get_Instance()->Get_Player()->Set_Pos(vPos);
	}

	static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Offset();

	pObj = new CPortal;
	static_cast<CPortal*>(pObj)->Set_GoScene(CPortal::GO_BOSSROOMA);
	vPos = { 378.f, 500.f, 0.f };
	pObj->Set_Pos(vPos);
	pObj->Ready_GameObject();
	CGameObject_Manager::Get_Instance()->Add_GameObject(CGameObject_Manager::OBJ_PORTAL, pObj);

	pObj = new CPortal;
	static_cast<CPortal*>(pObj)->Set_GoScene(CPortal::GO_BOSSROOMB);
	vPos = { 925.f, 520.f, 0.f  };
	pObj->Set_Pos(vPos);
	pObj->Ready_GameObject();
	CGameObject_Manager::Get_Instance()->Add_GameObject(CGameObject_Manager::OBJ_PORTAL, pObj);

	pObj = new CPortal;
	static_cast<CPortal*>(pObj)->Set_GoScene(CPortal::GO_ENTERTOTOWN);
	vPos = { 658.f, 1080.f, 0.f };
	pObj->Set_Pos(vPos);
	pObj->Ready_GameObject();
	CGameObject_Manager::Get_Instance()->Add_GameObject(CGameObject_Manager::OBJ_PORTAL, pObj);

	CSoundMgr::Get_Instance()->PlayBGM(L"LODING.wav");
	CScroll_Manager::Set_Scene(BOSSENTER);
	return S_OK;
}

void CBossEnter::Update_Scene()
{
	CGameObject_Manager::Get_Instance()->Update_GameObject_Manager();
}

void CBossEnter::LateUpdate_Scene()
{
	CGameObject_Manager::Get_Instance()->Late_Update_GameObject_Manager();
	CScroll_Manager::Scroll_Lock();
}

void CBossEnter::Render_Scene()
{
	CGameObject_Manager::Get_Instance()->Render_GameObject_Manager();
}

void CBossEnter::Release_Scene()
{
	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::BGM);
}
