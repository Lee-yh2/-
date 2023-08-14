#include "stdafx.h"
#include "GameObject_Manager.h"
#include "GameObject.h"
#include "Collision_Manager.h"

IMPLEMENT_SINGLETON(CGameObject_Manager)
CGameObject_Manager::CGameObject_Manager()
{
}


CGameObject_Manager::~CGameObject_Manager()
{
	Release_GameObject_Manager();
}

HRESULT CGameObject_Manager::Add_GameObject(OBJID eID, CGameObject * pGameObject)
{
	if (0 > eID || OBJ_END <= eID)
		return E_FAIL;

	m_listObject[eID].emplace_back(pGameObject);
	return S_OK;
}

int CGameObject_Manager::Update_GameObject_Manager()
{
	for (size_t i = 0; i < OBJ_END; ++i)
	{
		for (auto& iter = m_listObject[i].begin(); iter != m_listObject[i].end(); )
		{
			int iEvent = (*iter)->Update_GameObject();

			if (OBJ_DEAD == iEvent)
			{
				Safe_Delete(*iter);
				iter = m_listObject[i].erase(iter);
			}
			else
				++iter;
		}
	}

	CCollision_Manager::Collision_RectTerrain(m_listObject[OBJID::OBJ_TERRAIN], m_listObject[OBJID::OBJ_PLAYER]);
	CCollision_Manager::Collision_RectTerrain(m_listObject[OBJID::OBJ_TERRAIN], m_listObject[OBJID::OBJ_MONSTER]);
	CCollision_Manager::Collision_Portal(m_listObject[OBJID::OBJ_PORTAL], m_listObject[OBJID::OBJ_PLAYER]);
	CCollision_Manager::Collision_BulletTerrain(m_listObject[OBJID::OBJ_TERRAIN], m_listObject[OBJID::OBJ_MBULLET]);
	CCollision_Manager::Collision_MonsterBeAttack(m_listObject[OBJID::OBJ_BULLET], m_listObject[OBJID::OBJ_MONSTER]);
	CCollision_Manager::Collision_PlayerBeAttack(m_listObject[OBJID::OBJ_MBULLET], m_listObject[OBJID::OBJ_PLAYER]);
	CCollision_Manager::Collision_Rect(m_listObject[OBJID::OBJ_MONSTER], m_listObject[OBJID::OBJ_PLAYER]);
	CCollision_Manager::Collision_Rect(m_listObject[OBJID::OBJ_INTERIOR], m_listObject[OBJID::OBJ_PLAYER]);
	CCollision_Manager::Collision_Sphere(m_listObject[OBJID::OBJ_WAVE], m_listObject[OBJID::OBJ_PLAYER]);
	CCollision_Manager::Collision_Item(m_listObject[OBJID::OBJ_ITEM], m_listObject[OBJID::OBJ_PLAYER]);

	return 0;
}

void CGameObject_Manager::Late_Update_GameObject_Manager()
{
	for (size_t i = 0; i < OBJ_END; ++i)
	{
		for (auto& pObject : m_listObject[i])
			pObject->Late_Update_GameObject();
	}
}

void CGameObject_Manager::Render_GameObject_Manager()
{
	for (size_t i = 0; i < OBJ_END; ++i)
	{
		for (auto& pObject : m_listObject[i])
			pObject->Render_GameObject();
	}
}

void CGameObject_Manager::Release_GameObject_Manager()
{
	for (size_t i = 0; i < OBJ_END; ++i)
	{
		for (auto& pObject : m_listObject[i])
			Safe_Delete(pObject);

		m_listObject[i].clear();
	}
}

void CGameObject_Manager::Clear()
{
	if (m_listObject[OBJ_MONSTER].size())
	{
		Safe_Delete(m_listObject[OBJ_MONSTER].front());
		m_listObject[OBJ_MONSTER].clear();
	}
}
