#pragma once

#ifndef __MYSHOP_H__
#define __MYSHOP_H__

#include "GameObject.h"
class CMyShop :
	public CGameObject
{
public:
	explicit CMyShop();
	virtual ~CMyShop();

	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;

protected:

};

#endif // !__MYSHOP_H__
