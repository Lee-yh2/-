#pragma once

#ifndef __MYBUTTON_H__
#define __MYBUTTON_H__

#include "GameObject.h"
class CMyButton :
	public CGameObject
{
public:
	explicit CMyButton();
	virtual ~CMyButton();

	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;

private:

};

#endif // !__MYBUTTON_H__
