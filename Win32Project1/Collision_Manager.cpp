#include "stdafx.h"
#include "Collision_Manager.h"
#include "GameObject.h"
#include "Terrain.h"
#include "Player.h"
#include "Particle.h"
#include "Portal.h"
#include "Item.h"
#include "GameObject_Manager.h"
#include "Scroll_Manager.h"
#include "Inventory.h"


CCollision_Manager::CCollision_Manager()
{
}


CCollision_Manager::~CCollision_Manager()
{
}

//앞쪽이 타일
void CCollision_Manager::Collision_RectTerrain(list<CGameObject*> _Dst, list<CGameObject*> _Src)
{
	float fX = 0.f, fY = 0.f;
	vector<TILE*> vTerrain;
	for (auto& Dst : _Dst)
	{
		vTerrain = static_cast<CTerrain*>(Dst)->Get_Terrain();
		for (auto& Src : _Src)
		{
			for (int iSize = 0; iSize < (int)vTerrain.size(); ++iSize)
			{
				if (TERRAIN_COLLISON == vTerrain[iSize]->byOption)
				{
					if (Check_RectTerrain(vTerrain[iSize], Src, &fX, &fY))
					{
						if (fX < fY)
						{
							if (vTerrain[iSize]->vPos.x < Src->Get_Info().vPos.x)
								Src->Set_PosX(fX);
							else
							{
								Src->Set_PosX(-fX);

							}
						}
						else
						{
							if (vTerrain[iSize]->vPos.y < Src->Get_Info().vPos.y)
							{
								Src->Set_PosY(fY);
								int k = 0; 
							}
							else
							{
								Src->Set_PosY(-fY);

							}
						}
					}
				}
			}
		}
	}

}

void CCollision_Manager::Collision_BulletTerrain(list<CGameObject*> _Dst, list<CGameObject*> _Src)
{
	float fX = 0.f, fY = 0.f;
	vector<TILE*> vTerrain;
	for (auto& Dst : _Dst)
	{
		vTerrain = static_cast<CTerrain*>(Dst)->Get_Terrain();
		for (auto& Src : _Src)
		{
			for (int iSize = 0; iSize < (int)vTerrain.size(); ++iSize)
			{
				if (TERRAIN_COLLISON == vTerrain[iSize]->byOption)
				{
					if (Check_RectTerrain(vTerrain[iSize], Src, &fX, &fY))
					{
						Src->Set_Dead();
					}
				}
			}
		}
	}
}

void CCollision_Manager::Collision_Rect(list<CGameObject*> _Dst, list<CGameObject*> _Src)
{
	float fX = 0.f, fY = 0.f;
	for (auto& Dst : _Dst)
	{
		for (auto& Src : _Src)
		{
			if (!(Src->Get_Dead()) && Dst->Get_Info().vCSize.x > 5.f && Dst->Get_Info().vCSize.y > 5.f)
			{
				if (Check_Rect(Dst, Src, &fX, &fY))
				{
					if (fX < fY)
					{
						if (Dst->Get_Info().vPos.x < Src->Get_Info().vPos.x)
							Src->Set_PosX(fX);
						else
							Src->Set_PosX(-fX);
					}
					else
					{
						if (Dst->Get_Info().vPos.y < Src->Get_Info().vPos.y)
							Src->Set_PosY(fY);
						else
							Src->Set_PosY(-fY);
					}
				}
			}
		}
	}	
}
void CCollision_Manager::Collision_Sphere(list<CGameObject*> _Dst, list<CGameObject*> _Src)
{
	float fX = 0.f, fY = 0.f;
	for (auto& Dst : _Dst)
	{
		for (auto& Src : _Src)
		{
			if (Check_SphereRect(Dst, Src))
			{
				if (fX < fY)
				{
					if (Dst->Get_Info().vPos.x < Src->Get_Info().vPos.x)
						Src->Set_PosX(fX);
					else
						Src->Set_PosX(-fX);
				}
				else
				{
					if (Dst->Get_Info().vPos.y < Src->Get_Info().vPos.y)
						Src->Set_PosY(fY);
					else
						Src->Set_PosY(-fY);
				}
			}
		}
	}
}
//앞쪽이 플레이어공격범위
void CCollision_Manager::Collision_MonsterBeAttack(list<CGameObject*> _Dst, list<CGameObject*> _Src)
{
	float fX = 0.f, fY = 0.f;
	for (auto& Dst : _Dst)
	{
		for (auto& Src : _Src)
		{
			if (!(Src->Get_Dead()))
			{
				if (Check_Rect(Dst, Src, &fX, &fY))
				{
					CParticle* particle = new CParticle;
					D3DXVECTOR3 vPos = { fX, fY, 0.f };
					particle->Set_Pos(Src->Get_Info().vPos);
					if (BIGSWORD == Dst->Get_UnitInfo().byCurrentWeapon)
						static_cast<CParticle*>(particle)->Set_Type(CParticle::HIT_BIG);
					else if(SHORTSWORD == Dst->Get_UnitInfo().byCurrentWeapon)
						static_cast<CParticle*>(particle)->Set_Type(CParticle::HIT_SHORT);
					else if (BOW == Dst->Get_UnitInfo().byCurrentWeapon)
						static_cast<CParticle*>(particle)->Set_Type(CParticle::HIT_BOW);
					particle->Ready_GameObject();
					CGameObject_Manager::Get_Instance()->Add_GameObject(CGameObject_Manager::OBJ_EFFECT, particle);

					Src->Set_RGB(0);
					Src->Set_OnHit();
					CScroll_Manager::Scroll_Quake();
					Src->Set_Hp(-(Dst->Get_UnitInfo().iAtt + (rand() % 10) - (rand() % 10)));
					Dst->Set_Dead();
				}
			}
		}
	}
}
//앞쪽이 몬스터 공격범위
void CCollision_Manager::Collision_PlayerBeAttack(list<CGameObject*> _Dst, list<CGameObject*> _Src)
{
	float fX = 0.f, fY = 0.f;
	for (auto& Dst : _Dst)
	{
		for (auto& Src : _Src)
		{
			if (!(Src->Get_Dead()) && CPlayer::BEATTACK != static_cast<CPlayer*>(Src)->Get_CurState()
				&& CPlayer::ROLL != static_cast<CPlayer*>(Src)->Get_CurState())
			{
				if (Check_Rect(Dst, Src, &fX, &fY))
				{
					static_cast<CPlayer*>(Src)->Set_BeAttack();
					CScroll_Manager::Scroll_Quake();
				
					Src->Set_Hp(-(Dst->Get_UnitInfo().iAtt + (rand() % 10) - (rand() % 10)));
					Dst->Set_Dead();
				}
			}
		}
	}
}
//앞이 포탈
void CCollision_Manager::Collision_Portal(list<CGameObject*> _Dst, list<CGameObject*> _Src)
{
	float fX = 0.f, fY = 0.f;
	for (auto& Dst : _Dst)
	{
		for (auto& Src : _Src)
		{
				if (Check_Rect(Dst, Src, &fX, &fY))
				{
					static_cast<CPortal*>(Dst)->Set_Collision(true);
				}
				else
				{
					if(static_cast<CPortal*>(Dst)->Get_Collision())
						static_cast<CPortal*>(Dst)->Set_Collision(false);
				}
		}
	}
}
//앞이 아이템
void CCollision_Manager::Collision_Item(list<CGameObject*> _Dst, list<CGameObject*> _Src)
{
	float fX = 0.f, fY = 0.f;
	for (auto& Dst : _Dst)
	{
		for (auto& Src : _Src)
		{
			if (CItem::GROUND == static_cast<CItem*>(Dst)->Get_State())
			{
				if (Check_Rect(Dst, Src, &fX, &fY))
				{
					static_cast<CItem*>(Dst)->Set_State(CItem::INVEN);
					static_cast<CItem*>(Dst)->Set_Alpha();
					static_cast<CInventory*>(CGameObject_Manager::Get_Instance()->Get_Inven())->Insert_Item(Dst);
				}
			}
		}
	}
}


bool CCollision_Manager::Check_RectTerrain(TILE* _Dst, CGameObject * _Src, float* _x, float* _y)
{
	float	fCX = abs(_Dst->vPos.x - _Src->Get_Info().vPos.x);
	float	fCY = abs(_Dst->vPos.y - _Src->Get_Info().vPos.y);

	float	fDisX = (_Src->Get_Info().vCSize.x + TILECX)/2.f;
	float	fDisY = (_Src->Get_Info().vCSize.y + TILECY)/2.f;

	if (fCX < fDisX && fCY < fDisY)
	{
		*_x = fDisX - fCX;
		*_y = fDisY - fCY;

		return true;
	}

	return false;
}

bool CCollision_Manager::Check_Rect(CGameObject * _Dst, CGameObject * _Src, float * _x, float * _y)
{
	float	fCX = abs(_Dst->Get_Info().vPos.x - _Src->Get_Info().vPos.x);
	float	fCY = abs(_Dst->Get_Info().vPos.y - _Src->Get_Info().vPos.y);

	float	fDisX = (_Src->Get_Info().vCSize.x + _Dst->Get_Info().vCSize.x) / 2.f;
	float	fDisY = (_Src->Get_Info().vCSize.y + _Dst->Get_Info().vCSize.y) / 2.f;

	if (fCX < fDisX && fCY < fDisY)
	{
		*_x = fDisX - fCX;
		*_y = fDisY - fCY;

		return true;
	}

	return false;
}
//원과 사각형
bool CCollision_Manager::Check_SphereRect(CGameObject * _Dst, CGameObject * _Src)
{
	float fCircleX = _Dst->Get_Info().vPos.x;
	float fCircleY = _Dst->Get_Info().vPos.y;
	float fRadius = (float)_Dst->Get_Info().vCSize.x;

	float Left = (_Src->Get_Info().vPos.x - (_Src->Get_Info().vCSize.x / 2.f));
	float Top = (_Src->Get_Info().vPos.x + (_Src->Get_Info().vCSize.x / 2.f));
	float Right = (_Src->Get_Info().vPos.y - (_Src->Get_Info().vCSize.y / 2.f));
	float Bottom = (_Src->Get_Info().vPos.y + (_Src->Get_Info().vCSize.y / 2.f));

	if ((Left <= fCircleX && fCircleX <= Top) || (Right <= fCircleY && fCircleY <= Bottom))
	{
		RECT rcEx = {
			LONG(Left - fRadius),
			LONG(Top - fRadius),
			LONG(Right - fRadius),
			LONG(Bottom - fRadius)
		};

		if ((rcEx.left < fCircleX && fCircleX < rcEx.right) &&
			(rcEx.top < fCircleY && fCircleY < rcEx.bottom)) {
			return true;
		}
	}

	else
	{
		if (IsPointInCircle(fCircleX, fCircleY, fRadius, Left, Top))
			return true;
		if (IsPointInCircle(fCircleX, fCircleY, fRadius, Left, Bottom))
			return true;
		if (IsPointInCircle(fCircleX, fCircleY, fRadius, Right, Top))
			return true;
		if (IsPointInCircle(fCircleX, fCircleY, fRadius, Right, Bottom))
			return true;
	}

	return false;
}

bool CCollision_Manager::IsPointInCircle(float cx, float cy, float cr, float px, float py)
{
	float deltaX = cx = px;
	float deltaY = cy - py;
	float length = sqrt(deltaX * deltaX + deltaY * deltaY);

	if (length > cr)
		return false;

	return true;
}
