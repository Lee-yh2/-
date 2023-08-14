#pragma once

#ifndef __BOSSROOM2_H__
#define __BOSSROOM2_H__

#include "Scene.h"
class CBossRoom2 :
	public CScene
{
public:
	explicit CBossRoom2();
	virtual ~CBossRoom2();

	// CScene을(를) 통해 상속됨
	virtual HRESULT Ready_Scene() override;
	virtual void Update_Scene() override;
	virtual void LateUpdate_Scene() override;
	virtual void Render_Scene() override;
	virtual void Release_Scene() override;

protected:
	bool m_bSummon;
	bool m_bOffset;
};

#endif // !__BOSSROOM2_H__
