#pragma once
#include "GameObject.h"
class CTile :
	public CGameObject
{
public:
	explicit CTile();
	virtual ~CTile();

	// CGameObject��(��) ���� ��ӵ�
	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;

protected:
	TILE		m_backTile;
};

