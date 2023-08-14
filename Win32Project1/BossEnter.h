#pragma once

#ifndef __BOSSENTER_H__
#define __BOSSENTER_H__

#include "Scene.h"
class CBossEnter :
	public CScene
{
public:
	explicit CBossEnter();
	virtual ~CBossEnter();

	// CScene을(를) 통해 상속됨
	virtual HRESULT Ready_Scene() override;
	virtual void Update_Scene() override;
	virtual void LateUpdate_Scene() override;
	virtual void Render_Scene() override;
	virtual void Release_Scene() override;
};

#endif // !__BOSSENTER_H__
