#pragma once

#ifndef __WAVE_H__
#define __WAVE_H__

#include "GameObject.h"
class CWave :
	public CGameObject
{
public:
	CWave();
	virtual ~CWave();

	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;

public:
	void FrameMove(float fMoveSpeed = 1.0f);
protected:
	float	m_fCSize;
	bool	m_bQuake;
};

#endif // !__WAVE_H__
