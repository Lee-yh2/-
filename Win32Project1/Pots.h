#pragma once

#ifndef __POTS_H__
#define __POTS_H__

#include "GameObject.h"
class CPots :
	public CGameObject
{
public:
	explicit CPots();
	virtual ~CPots();

	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;

protected:
	INFO		m_tTargetInfo;
};

#endif // !__POTS_H__
