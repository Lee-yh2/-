#pragma once

#ifndef __STONE_H__
#define __STONE_H__

#include "GameObject.h"
class CStone :
	public CGameObject
{
public:
	explicit CStone();
	virtual ~CStone();

	// CGameObject��(��) ���� ��ӵ�
	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;
	
protected:
	INFO	m_tRockTexture;
	DWORD	dw_Fall;
	float	m_fSpeed;
	int		m_iRockNum;

	bool	m_bShadow;
	D3DXVECTOR3 m_vSetSize;
};

#endif // !__STONE_H__
