#pragma once
class CTime_Manager
{
	DECLARE_SINGLETON(CTime_Manager)
private:
	CTime_Manager();
	~CTime_Manager();
public:
	float Get_DeltaTime() { return m_fDeltaTime; }
	void Update_Time_Manager();

private:
	LARGE_INTEGER m_CpuTick;
	LARGE_INTEGER m_BeginTime;
	LARGE_INTEGER m_EndTime;
	float m_fDeltaTime;
};

