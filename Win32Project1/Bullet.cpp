#include "stdafx.h"
#include "Bullet.h"
#include "Time_Manager.h"
#include "Texture_Manager.h"
#include "Scroll_Manager.h"
#include "Graphic_Device.h"


CBullet::CBullet()
	:m_fSpeed(0.f), dw_Bullet(GetTickCount())
{
}


CBullet::~CBullet()
{
}

HRESULT CBullet::Ready_GameObject()
{
	m_tInfo.vSize = { 1.f, 1.f, 0.f };

	if (BIGSWORD == m_tUnitInfo.byCurrentWeapon)
	{
		m_tInfo.vCSize = { 30.f, 40.f, 0.f };
		m_tUnitInfo.iAtt = 15;
		m_fSpeed = 0.f;
	}
	else if (SHORTSWORD == m_tUnitInfo.byCurrentWeapon)
	{
		m_tInfo.vCSize = { 30.f, 30.f, 0.f };
		m_tUnitInfo.iAtt = 8;
		m_fSpeed = 0.f;
	}
	else if (BOW == m_tUnitInfo.byCurrentWeapon)
	{
		m_tFrame = { 0.f, 5.f };
		m_tInfo.vCSize = { 27.f, 10.f, 0.f };
		m_tUnitInfo.iAtt = 20;
		m_fSpeed = 300.f;
	}

	m_fSpeed *= CTime_Manager::Get_Instance()->Get_DeltaTime();
	return S_OK;
}

int CBullet::Update_GameObject()
{
	if (m_bDead)
		return OBJ_DEAD;
	m_tInfo.vPos += m_fSpeed * m_tInfo.vDir;

	if (BOW != m_tUnitInfo.byCurrentWeapon)
	{
		if (GetTickCount() > dw_Bullet + 200)
			m_bDead = true;
	}
	else
	{
		if (GetTickCount() > dw_Bullet + 1500)
			m_bDead = true;
	}
	FrameMove();
	return OBJ_NOEVENT;
}

void CBullet::Late_Update_GameObject()
{
}

void CBullet::Render_GameObject()
{
	if (BOW == m_tUnitInfo.byCurrentWeapon)
	{
		const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(L"ARROW", m_strStateKey ,m_tFrame.fFrameStart);
		if (nullptr == pTexInfo)
			return;

		float iScrollX = CScroll_Manager::Get_Scroll(CScroll_Manager::SCROLLX);
		float iScrollY = CScroll_Manager::Get_Scroll(CScroll_Manager::SCROLLY);
		D3DXMATRIX matWorld, matTrans, matScale;
		D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
		D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + iScrollX, m_tInfo.vPos.y + iScrollY, 0.f);
		float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
		float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);
		matWorld = matScale * matTrans;
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
		CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

}

void CBullet::Release_GameObject()
{
}
