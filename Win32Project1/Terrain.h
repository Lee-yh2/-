#pragma once

#ifndef __TERRAIN_H__
#define __TERRAIN_H__
#include "KeyManager.h"
#include "GameObject.h"
class CTerrain final
	:public CGameObject
{
public:
	CTerrain();
	~CTerrain();

public:
	void Set_Terrain(vector<TILE*>& vecTile) { m_vecTile = vecTile; }
	vector<TILE*>& Get_Terrain() { return m_vecTile; }
	void Set_PathFile(wstring terrain) { m_StrTerrianPathFile = terrain; }
public:
	void Set_Alpha();
public:
	// GameObject을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject() override;
	virtual int Update_GameObject() override;
	virtual void Late_Update_GameObject() override;
	virtual void Render_GameObject() override;
	virtual void Release_GameObject() override;
public:
	HRESULT LoadTileData(const wstring& wstrFilePath);
protected: 
	vector<TILE*>	m_vecTile;
	TILE*			m_backTile;
	wstring			m_StrTerrianPathFile;
	int				m_iAlpha;
	bool			m_bAlpha;
	CKeyManager		m_tKeyManager;
};

#endif // !__TERRAIN_H__
