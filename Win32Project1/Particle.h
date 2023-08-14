#pragma once

#ifndef __PARTICLE_H__
#define __PARTICLE_H__

#include "GameObject.h"
class CParticle :
	public CGameObject
{
public:
	explicit CParticle();
	virtual ~CParticle();

	enum TYPE { BOSSA_ROCKBREAK, HIT_BIG, HIT_BOW, HIT_SHORT, BOSSB_BULLETBREAK, HIT_PLAYER,END};

	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;

public:
	void Set_Type(TYPE eType) { m_eType = eType; }
private:
	void FrameMove(float fMoveSpeed = 1.f);
	void Check_Type();
protected:
	TYPE m_eType;
	wstring m_strObjectKey;
	wstring m_strStateKey;
};

#endif // !__PARTICLE_H__
