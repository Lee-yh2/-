#include "stdafx.h"
#include "GameObject.h"
#include "Time_Manager.h"


CGameObject::CGameObject()
	:m_bDead(false), m_iRGB(255), m_dwOnHitTime(GetTickCount()), m_bOnHit(false)
{
}


CGameObject::~CGameObject()
{
	//Release_GameObject();
}

void CGameObject::FrameMove(float fMoveSpeed)
{
	m_tFrame.fFrameStart += m_tFrame.fFrameEnd *CTime_Manager::Get_Instance()->Get_DeltaTime() * fMoveSpeed;
	if (m_tFrame.fFrameStart > m_tFrame.fFrameEnd)
	{
		m_tFrame.fFrameStart = 0.f;
	}
}

void CGameObject::OnHit()
{
	if (m_bOnHit)
	{
		m_dwOnHitTime = GetTickCount();
		m_bOnHit = false;
	}
	if (m_dwOnHitTime + 80 < GetTickCount())
	{
		Set_RGB(255);
	}
}
