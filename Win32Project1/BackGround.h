#pragma once

#ifndef __BACKGROUND_H__
#define __BACKGROUND_H__

#include "GameObject.h"
class CBackGround final
	: public CGameObject
{
public:
	CBackGround();
	~CBackGround();

public:
	void MiniRender_BackGround();
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;
public:
	void Set_BackGround(TILE* backTile) { m_backTile = backTile; }
	void Set_Size(float size) { m_backTile->vSize.x = size; m_backTile->vSize.y = size;}
	void Set_DrawID(int drawID) { m_backTile->byDrawID = drawID; }
	void Set_PathFile(wstring terrain) { m_StrBackPathFile = terrain; }
	TILE* Get_BackGround() { return m_backTile; }
	HRESULT LoadBackTileData(const wstring& wstrFilePath);

private:
	TILE*			m_backTile;
	wstring			m_StrBackPathFile;

};

#endif // !__BACKGROUND_H__
