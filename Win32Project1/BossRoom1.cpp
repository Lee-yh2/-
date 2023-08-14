#include "stdafx.h"
#include "BossRoom1.h"
#include "GameObject_Manager.h"
#include "Scroll_Manager.h"
#include "SoundMgr.h"
#include "GameObject.h"
#include "Terrain.h"
#include "BackGround.h"
#include "Player.h"
#include "Monster.h"
#include "Portal.h"

CBossRoom1::CBossRoom1()
	:m_bSummon(false), m_bOffset(false)
{
}


CBossRoom1::~CBossRoom1()
{
	Release_Scene();
}

HRESULT CBossRoom1::Ready_Scene()
{
	CGameObject* pObj = new CTerrain;
	static_cast<CTerrain*>(pObj)->Set_PathFile(L"../Data/Boss1.dat");
	pObj->Ready_GameObject();
	CGameObject_Manager::Get_Instance()->Add_GameObject(CGameObject_Manager::OBJ_TERRAIN, pObj);

	pObj = new CBackGround;
	static_cast<CBackGround*>(pObj)->Set_PathFile(L"../Data/Boss1BackGround.dat");
	pObj->Ready_GameObject();
	CGameObject_Manager::Get_Instance()->Add_GameObject(CGameObject_Manager::OBJ_BACKGROUND, pObj);

	D3DXVECTOR3 vPos;
	int iScene = CScroll_Manager::Get_Scene();
	if (BOSSENTER == iScene)
	{
		vPos = { 590.f, 680.f, 0.f};
		CGameObject_Manager::Get_Instance()->Get_Player()->Set_Pos(vPos);
		static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Offset();
	}

	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::BGM);
	CSoundMgr::Get_Instance()->PlayBGM(L"BOSS_A_BGM.wav");
	CScroll_Manager::Set_Scene(BOSSROOMA);
	return S_OK;
}

void CBossRoom1::Update_Scene()
{
	CGameObject_Manager::Get_Instance()->Update_GameObject_Manager();
	if (!m_bSummon)
	{
		CGameObject* pObj = new CMonster;
		pObj->Ready_GameObject();
		CGameObject_Manager::Get_Instance()->Add_GameObject(CGameObject_Manager::OBJ_MONSTER, pObj);
		m_bSummon = true;
	}
	if (CGameObject_Manager::Get_Instance()->Get_Monster())
	{
		if (CGameObject_Manager::Get_Instance()->Get_Player()->Get_Info().vPos.y < 650.f)
		{
			static_cast<CMonster*>(CGameObject_Manager::Get_Instance()->Get_Monster())->Set_Start();
			if (!m_bOffset)
			{
				static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Set_BossStart();
				CSoundMgr::Get_Instance()->PlaySound(L"BOSS_A_WAKEUP.wav", CSoundMgr::MONSTER);
				m_bOffset = true;
			}
		}
	}
}

void CBossRoom1::LateUpdate_Scene()
{
	CGameObject_Manager::Get_Instance()->Late_Update_GameObject_Manager();
	CScroll_Manager::Scroll_Lock();
}

void CBossRoom1::Render_Scene()
{
	CGameObject_Manager::Get_Instance()->Render_GameObject_Manager();
}

void CBossRoom1::Release_Scene()
{
	CSoundMgr::Get_Instance()->StopAll();
}
