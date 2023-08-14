#pragma once

#ifndef __LOADING_H__
#define __LOADING_H__

#include "Scene.h"
class CLoading :
	public CScene
{
public:
	CLoading();
	virtual ~CLoading();

	// CScene을(를) 통해 상속됨
	virtual HRESULT Ready_Scene() override;
	virtual void Update_Scene() override;
	virtual void LateUpdate_Scene() override;
	virtual void Render_Scene() override;
	virtual void Release_Scene() override;

public:
	static unsigned __stdcall ReadImageFunc(LPVOID pvoid);
	void FrameMove(float fSpeed);
private:
	HANDLE m_hThread;
	CRITICAL_SECTION m_CriticalSection;
	FRAME m_tFrame;
};

#endif // !__LOADING_H__
