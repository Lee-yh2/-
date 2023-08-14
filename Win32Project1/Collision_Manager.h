#pragma once

#ifndef __COLLISIONMANAGER_H__
#define __COLLISIONMANAGER_H__
class CGameObject;
class CCollision_Manager
{
public:
	CCollision_Manager();
	~CCollision_Manager();

public:
	static void Collision_RectTerrain(list<CGameObject*> _Dst, list<CGameObject*> _Src);
	static void Collision_BulletTerrain(list<CGameObject*> _Dst, list<CGameObject*> _Src);
	static void Collision_Rect(list<CGameObject*> _Dst, list<CGameObject*> _Src);
	static void Collision_Sphere(list<CGameObject*> _Dst, list<CGameObject*> _Src);
	static void Collision_MonsterBeAttack(list<CGameObject*> _Dst, list<CGameObject*> _Src);
	static void Collision_PlayerBeAttack(list<CGameObject*> _Dst, list<CGameObject*> _Src);
	static void Collision_Portal(list<CGameObject*> _Dst, list<CGameObject*> _Src);
	static void Collision_Item(list<CGameObject*> _Dst, list<CGameObject*> _Src);


	static bool Check_RectTerrain(TILE* _Dst, CGameObject * _Src, float* _x, float* _y);
	static bool Check_Rect(CGameObject * _Dst, CGameObject * _Src, float* _x, float* _y);
	static bool Check_SphereRect(CGameObject * _Dst, CGameObject * _Src);
	static bool IsPointInCircle(float cx, float cy, float cr, float px, float py);
};

#endif // !__COLLISIONMANAGER_H__
