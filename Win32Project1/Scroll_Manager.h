#pragma once

#ifndef __SCROLL_MANAGER_H__
#define __SCROLL_MANAGER_H__

class CScroll_Manager
{
public:
	enum SCROLLID { SCROLLX, SCROLLY };
public:
	CScroll_Manager();
	~CScroll_Manager();
public:
	static const D3DXVECTOR3& Get_Scroll();
	static const float& Get_Scroll(const SCROLLID& eID);
	static int Get_Scene() { return m_iScene; }
	static void Set_Scroll(const D3DXVECTOR3& vScroll);
	static void Set_Scroll(const SCROLLID& eID, const float& fScroll);
	static void Set_Scene(int iScene) { m_iScene = iScene; }
	static void Scroll_Lock();
	static void Scroll_Quake();

private:
	static D3DXVECTOR3  m_vScroll;
	static int			m_iScene;
	static bool			m_bQuake;
	static bool			m_bDirection;
	static float		m_fQuakeTime;
};

#endif // !__SCROLL_MANAGER_H__
