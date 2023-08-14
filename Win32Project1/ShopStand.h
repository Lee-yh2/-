#pragma once

#ifndef __SHOPSTAND_H__
#define __SHOPSTAND_H__

#include "GameObject.h"
class CShopStand :
	public CGameObject
{
public:
	explicit CShopStand();
	virtual ~CShopStand();

	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;

protected:
	bool			m_bPutItem[4];
	D3DXVECTOR3		m_vPutItemPos[4];

};

#endif // !__SHOPSTAND_H__
