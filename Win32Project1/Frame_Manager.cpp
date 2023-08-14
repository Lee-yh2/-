#include "stdafx.h"
#include "Frame_Manager.h"
#include "Time_Manager.h"


CFrame_Manager::CFrame_Manager()
	: m_fDeltaTime(0.f)
	, m_fSPF(0.f)
	, m_fFPSTime(0.f)
	, m_iFPS(0)
	, m_szFPS()
{
	QueryPerformanceFrequency(&m_CpuTick);
	QueryPerformanceCounter(&m_EndTime);
	QueryPerformanceCounter(&m_BeginTime);

}


CFrame_Manager::~CFrame_Manager()
{
}

void CFrame_Manager::Ready_Frame_Manager(float fFPS)
{
	m_fSPF = 1.f / fFPS;
}

bool CFrame_Manager::FrameLock()
{
	QueryPerformanceFrequency(&m_CpuTick);
	QueryPerformanceCounter(&m_EndTime);
	m_fDeltaTime += float(m_EndTime.QuadPart - m_BeginTime.QuadPart) / m_CpuTick.QuadPart;
	m_BeginTime.QuadPart = m_EndTime.QuadPart;
	if (m_fSPF <= m_fDeltaTime)
	{
		m_fDeltaTime = 0.f;
		++m_iFPS;
		return true;
	}
	return false;
}

void CFrame_Manager::Render_Frame_Manager()
{
	m_fFPSTime += CTime_Manager::Get_Instance()->Get_DeltaTime();

	if (1.f <= m_fFPSTime)
	{
		swprintf_s(m_szFPS, L"FPS : %d", m_iFPS);
		m_iFPS = 0;
		m_fFPSTime = 0.f;
	}

	SetWindowText(g_hWND, m_szFPS);
}
