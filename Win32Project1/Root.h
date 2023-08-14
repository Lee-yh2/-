#pragma once

#ifndef __ROOT_H__
#define __ROOT_H__

#include "GameObject.h"
class CRoot :
	public CGameObject
{
public:
	explicit CRoot();
	virtual ~CRoot();

	enum TYPE { GUARD, ATTACK };
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;

public:
	void FrameMove(float fMoveSpeed = 1.f);
	void Set_Type(TYPE eType) { m_eType = eType; }

protected:
	TYPE	m_eType;
	wstring m_strStateKey;
	bool	m_bAttack;
	bool    m_bRootSound;
};

#endif // !__ROOT_H__
