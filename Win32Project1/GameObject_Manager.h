#pragma once
class CGameObject;
class CGameObject_Manager
{
public:
	enum OBJID { OBJ_BACKGROUND, OBJ_BACKEFFECT,OBJ_WAVE, OBJ_TERRAIN, OBJ_INTERIOR, OBJ_PORTAL, OBJ_MONSTER, OBJ_PLAYER, OBJ_EFFECT , OBJ_MBULLET, OBJ_BULLET, OBJ_INVEN, OBJ_ITEM, OBJ_UI, OBJ_END };

	DECLARE_SINGLETON(CGameObject_Manager)
private:
	CGameObject_Manager();
	~CGameObject_Manager();
public:
	HRESULT Add_GameObject(OBJID eID, CGameObject* pGameObject);
public:
	int Update_GameObject_Manager();
	void Late_Update_GameObject_Manager();
	void Render_GameObject_Manager();
	void Release_GameObject_Manager();

public:
	CGameObject* Get_Player() const { return m_listObject[OBJID::OBJ_PLAYER].front(); }
	CGameObject* Get_Monster() const { if (m_listObject[OBJ_MONSTER].size()) return m_listObject[OBJ_MONSTER].front(); else return false; }
	CGameObject* Get_Inven() const {return m_listObject[OBJID::OBJ_INVEN].front();}

	void Clear();
	list<CGameObject*>* Get_ListObject() { return m_listObject; }
private:
	list<CGameObject*>	m_listObject[OBJ_END];
};

