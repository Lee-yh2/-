#pragma once

#ifndef __SHOP_H__
#define __SHOP_H__

#include "Scene.h"
class CShop :
	public CScene
{
public:
	explicit CShop();
	virtual ~CShop();

	// CScene을(를) 통해 상속됨
	virtual HRESULT Ready_Scene() override;
	virtual void Update_Scene() override;
	virtual void LateUpdate_Scene() override;
	virtual void Render_Scene() override;
	virtual void Release_Scene() override;

protected:
	DWORD	m_Time;
	bool	m_bItem;
};

#endif // !__SHOP_H__
