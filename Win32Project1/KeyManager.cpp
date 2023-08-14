#include "stdafx.h"
#include "KeyManager.h"


CKeyManager::CKeyManager()
	:m_dwKey(0), m_dwKeyUp(0), m_dwKeyDown(0)
{
}


CKeyManager::~CKeyManager()
{
}

void CKeyManager::KeyCheck()
{
	m_dwKey = 0;
	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
		m_dwKey |= KEY_LBUTTON;
	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
		m_dwKey |= KEY_RBUTTON;
	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
		m_dwKey |= KEY_LEFT;
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
		m_dwKey |= KEY_RIGHT;
	if (GetAsyncKeyState(VK_UP) & 0x8000)
		m_dwKey |= KEY_UP;
	if (GetAsyncKeyState(VK_DOWN) & 0x8000)
		m_dwKey |= KEY_DOWN;
	if (GetAsyncKeyState(VK_RETURN) & 0x8000)
		m_dwKey |= KEY_ENTER;
	if (GetAsyncKeyState('R') & 0x8000)
		m_dwKey |= KEY_R;
	if (GetAsyncKeyState('A') & 0x8000)
		m_dwKey |= KEY_A;
	if (GetAsyncKeyState('P') & 0x8000)
		m_dwKey |= KEY_P;
	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
		m_dwKey |= KEY_SPACE;
	if (GetAsyncKeyState('I') & 0x8000)
		m_dwKey |= KEY_I;
	if (GetAsyncKeyState('Q') & 0x8000)
		m_dwKey |= KEY_Q;
	if (GetAsyncKeyState('O') & 0x8000)
		m_dwKey |= KEY_O;
	if (GetAsyncKeyState('C') & 0x8000)
		m_dwKey |= KEY_C;
}

bool CKeyManager::KeyUp(DWORD dwKey)
{
	if (m_dwKey & dwKey)
	{
		m_dwKeyUp |= dwKey;
		return false;
	}
	else if (m_dwKeyUp & dwKey)
	{
		m_dwKeyUp ^= dwKey;
		return true;
	}
	return false;
}

bool CKeyManager::KeyDown(DWORD dwKey)
{
	if ((m_dwKey & dwKey) && !(m_dwKeyDown & dwKey))
	{
		m_dwKeyDown |= dwKey;
		return true;
	}
	else if (!(m_dwKey & dwKey) && (m_dwKeyDown & dwKey))
	{
		m_dwKeyDown ^= dwKey;
		return false;
	}
	return false;
}

bool CKeyManager::KeyPressing(DWORD dwKey)
{
	if (m_dwKey & dwKey)
		return true;
	return false;
}
