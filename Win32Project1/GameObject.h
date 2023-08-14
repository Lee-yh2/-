#pragma once
#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

class CGameObject abstract
{
public:
	explicit CGameObject();
	virtual ~CGameObject();
public:
	virtual HRESULT Ready_GameObject()PURE;
	virtual int		Update_GameObject()PURE;
	virtual void	Late_Update_GameObject()PURE;
	virtual void	Render_GameObject()PURE;
	virtual void	Release_GameObject()PURE;

public:
	inline void Set_PosX(float _x) { m_tInfo.vPos.x += _x; }
	inline void Set_PosY(float _y) { m_tInfo.vPos.y += _y; }
	inline void Set_Pos(D3DXVECTOR3 vPos) { m_tInfo.vPos = vPos; }
	inline void Set_Dir(D3DXVECTOR3 vDir) { m_tInfo.vDir = vDir; }
	inline void Set_Dead() { m_bDead = true; }
	inline void Set_Att(int iAtt) { m_tUnitInfo.iAtt = iAtt; }
	inline void Set_Hp(int iAtt) { m_tUnitInfo.iHp += iAtt; }
	inline void Set_Gold(int iGold) { m_tUnitInfo.iGold += iGold; }
	inline void Set_CSize(D3DXVECTOR3 vCSize) { m_tInfo.vCSize = vCSize; }

public:
	inline const INFO& Get_Info() const { return m_tInfo; }
	inline bool Get_Dead() { return m_bDead; }
	inline const UNITINFO& Get_UnitInfo() const { return m_tUnitInfo; }
public:
	void Set_RGB(int RGB) { m_iRGB = RGB; }
	void Set_OnHit() { m_bOnHit = true; }

protected:
	void FrameMove(float fMoveSpeed = 1.f);
	void OnHit();
protected:
	INFO		m_tInfo;
	UNITINFO	m_tUnitInfo;
	FRAME		m_tFrame;
	DWORD		m_dwOnHitTime;
	int			m_iRGB;
	bool		m_bOnHit;
	bool		m_bDead;
};

#endif // !__GAMEOBJECT_H__
