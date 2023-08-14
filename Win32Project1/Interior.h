#pragma once

#ifndef __INTERIOR_H__
#define __INTERIOR_H__

#include "GameObject.h"
class CInterior :
	public CGameObject
{
public:
	explicit CInterior();
	virtual ~CInterior();

	enum FURNITURE{STAND, BED, CARPET, DOOR, PORTALA, PORTALB,END};

	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;

public:
	void Set_Furniture(FURNITURE eFurniture) { m_eFurniture = eFurniture; }
	void Set_Stand(CGameObject** Stand);
	void Set_Price(int* iPrice);
protected:
	FURNITURE	m_eFurniture;
	wstring		m_strObjectKey;
	wstring		m_strStateKey;
	float		m_fFrameKey;

	CGameObject*		m_ShopStandItem[4];
	int*				m_iPrice[4];

	bool				m_bStandA;
	bool				m_bStandB;
	bool				m_bStandC;
	bool				m_bStandD;
};

#endif // !__INTERIOR_H__
