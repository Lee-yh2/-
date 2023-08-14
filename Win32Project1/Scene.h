#pragma once
class CScene abstract
{
public:
	CScene();
	~CScene();
public:
	virtual HRESULT Ready_Scene() PURE;
	virtual void Update_Scene() PURE;
	virtual void	LateUpdate_Scene()PURE;
	virtual void	Render_Scene()PURE;
	virtual void	Release_Scene()PURE;
};

