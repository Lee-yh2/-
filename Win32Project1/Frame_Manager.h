#pragma once
class CFrame_Manager
{
public:
	CFrame_Manager();
	~CFrame_Manager();
public:
	void Ready_Frame_Manager(float fFPS);
	bool FrameLock();
	void Render_Frame_Manager();
private:
	LARGE_INTEGER m_CpuTick;
	LARGE_INTEGER m_BeginTime;
	LARGE_INTEGER m_EndTime;
	float m_fDeltaTime;

	float m_fSPF;

	float m_fFPSTime;
	int m_iFPS;
	TCHAR m_szFPS[MAX_PATH];
};

