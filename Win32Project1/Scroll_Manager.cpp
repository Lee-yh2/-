#include "stdafx.h"
#include "Scroll_Manager.h"
#include "Time_Manager.h"


D3DXVECTOR3 CScroll_Manager::m_vScroll = {0.f, 0.f, 0.f};
int CScroll_Manager::m_iScene = LOADING;
bool CScroll_Manager::m_bQuake = false;
float CScroll_Manager::m_fQuakeTime = 0.f;
bool CScroll_Manager::m_bDirection = true;
CScroll_Manager::CScroll_Manager()
{
}


CScroll_Manager::~CScroll_Manager()
{
}

const D3DXVECTOR3 & CScroll_Manager::Get_Scroll()
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return m_vScroll;
}

const float & CScroll_Manager::Get_Scroll(const SCROLLID & eID)
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	switch (eID)
	{
	case SCROLLX:
		return m_vScroll.x;
	case SCROLLY:
		return m_vScroll.y;
	default:
		break;
	}
	return m_vScroll.x;
}

void CScroll_Manager::Set_Scroll(const D3DXVECTOR3 & vScroll)
{
	m_vScroll += vScroll;
}

void CScroll_Manager::Set_Scroll(const SCROLLID & eID, const float & fScroll)
{
	switch (eID)
	{
	case SCROLLX:
		m_vScroll.x += fScroll;
		break;
	case SCROLLY:
		m_vScroll.y += fScroll;
		break;
	default:
		break;
	}
}

void CScroll_Manager::Scroll_Lock()
{
	float fMinRockX;
	float fMinRockY;
	float fMaxRockX;
	float fMaxRockY;

	if (BOSSROOMB == m_iScene)
	{
		fMinRockX = -2.5f;
		fMinRockY = -2.f;
		fMaxRockX = -415.f;
		fMaxRockY = -290.f;
	}
	else if (BOSSROOMA == m_iScene)
	{
		fMinRockX = -2.5f;
		fMinRockY = -2.f;
		fMaxRockX = -304.f;
		fMaxRockY = -280.f;
	}
	else if (BOSSENTER == m_iScene)
	{
		fMinRockX = -2.5f;
		fMinRockY = -2.f;
		fMaxRockX = -304.f;
		fMaxRockY = -605.f;
	}
	else if (TOWN == m_iScene)
	{
		fMinRockX = -2.5f;
		fMinRockY = -2.f;
		fMaxRockX = -415.f;
		fMaxRockY = -350.f;
	}
	else if (SHOP == m_iScene)
	{
		fMinRockX = -70.f;
		fMinRockY = -2.f;
		fMaxRockX = -90.f;
		fMaxRockY = -500.f;
	}

	if (fMinRockX < m_vScroll.x)
		m_vScroll.x = fMinRockX;
	if (fMinRockY < m_vScroll.y)
		m_vScroll.y = fMinRockY;
	if(fMaxRockX > m_vScroll.x)
		m_vScroll.x = fMaxRockX;
	if (fMaxRockY > m_vScroll.y)
		m_vScroll.y = fMaxRockY;

	if (m_bQuake)
	{
		m_fQuakeTime += CTime_Manager::Get_Instance()->Get_DeltaTime() * 10.f;
		if (m_bDirection)
			m_vScroll.y += 10.f * CTime_Manager::Get_Instance()->Get_DeltaTime() * 10.f;
		else
			m_vScroll.y -= 10.f * CTime_Manager::Get_Instance()->Get_DeltaTime() * 10.f;

		m_bDirection = !m_bDirection;
		if (1.5f <= m_fQuakeTime)
		{
			m_bQuake = false;
		}
	}
}

void CScroll_Manager::Scroll_Quake()
{
	m_bQuake = true;
	m_bDirection = true;
	m_fQuakeTime = 0.f;
}
