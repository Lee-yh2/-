#pragma once

#ifndef __TOWN_H__
#define __TOWN_H__

#include "Scene.h"
class CTown final:
	public CScene
{
public:
	CTown();
	~CTown();

	// CScene��(��) ���� ��ӵ�
	virtual HRESULT Ready_Scene() override;
	virtual void Update_Scene() override;
	virtual void LateUpdate_Scene() override;
	virtual void Render_Scene() override;
	virtual void Release_Scene() override;
};

#endif // !__TOWN_H__
