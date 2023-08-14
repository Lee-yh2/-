#include "stdafx.h"
#include "MBullet.h"
#include "Time_Manager.h"
#include "Graphic_Device.h"
#include "Scroll_Manager.h"
#include "Texture_Manager.h"
#include "Particle.h"
#include "GameObject_Manager.h"

CMBullet::CMBullet()
	:m_fSpeed(0.f), dw_Bullet(GetTickCount())
{
}


CMBullet::~CMBullet()
{
}

HRESULT CMBullet::Ready_GameObject()
{
	m_tInfo.vSize = { 1.f, 1.f, 0.f };

	if (BOSSA_PUNCH == m_CurrentMonster)
	{
		m_tInfo.vCSize = { 50.f, 40.f, 0.f };
		m_tUnitInfo.iAtt = 40;
		m_fSpeed = 0.f;
	}
	else if (BOSSA_STAMP == m_CurrentMonster)
	{
		m_tInfo.vCSize = { 100.f, 50.f, 0.f };
		m_tUnitInfo.iAtt = 30;
		m_fSpeed = 0.f;
	}
	else if (BOSSB_JUMP == m_CurrentMonster)
	{
		m_tInfo.vCSize = { 250.f, 150.f, 0.f };
		m_tUnitInfo.iAtt = 30;
		m_fSpeed = 0.f;
	}
	else if (BOSSB_BULLET == m_CurrentMonster)
	{
		m_tFrame = { 0.f, 8.f };
		m_tInfo.vCSize = { 10.f, 10.f, 0.f };
		m_tInfo.vSize = { 1.5f, 1.5f, 0.f };
		m_strObjectKey = L"BOSS_B_PARTICLE";
		m_strStateKey = L"BOSS_B_PARTICLE_BULLET";
		m_tUnitInfo.iAtt = 20;
		m_fSpeed = 160.f;
	}
	else if (BOSSB_ROOT == m_CurrentMonster)
	{
		m_tInfo.vCSize = { 10.f, 10.f, 0.f };
		m_tUnitInfo.iAtt = 20;
		m_fSpeed = 0.f;
	}
	else if (BOSSA_ROCK == m_CurrentMonster)
	{
		m_tInfo.vCSize = { 25.f, 25.f, 0.f };
		m_tUnitInfo.iAtt = 30;
		m_fSpeed = 0.f;
	}
	m_fSpeed *= CTime_Manager::Get_Instance()->Get_DeltaTime();

	return S_OK;
}

int CMBullet::Update_GameObject()
{
	if (m_bDead)
	{
		if (m_CurrentMonster == BOSSB_BULLET)
		{
			CParticle* particle = new CParticle;
			particle->Set_Pos(m_tInfo.vPos);
			static_cast<CParticle*>(particle)->Set_Type(CParticle::BOSSB_BULLETBREAK);
			particle->Ready_GameObject();
			CGameObject_Manager::Get_Instance()->Add_GameObject(CGameObject_Manager::OBJ_EFFECT, particle);
		}
		return OBJ_DEAD;
	}
	m_tInfo.vPos += m_fSpeed * m_tInfo.vDir;

	if(BOSSA_ROCK == m_CurrentMonster)
	{
		if (GetTickCount() > dw_Bullet + 200)
			m_bDead = true;
	}
	else if (BOSSB_ROOT == m_CurrentMonster)
	{
		if (GetTickCount() > dw_Bullet + 800)
			m_bDead = true;
	}
	else if(BOSSB_BULLET != m_CurrentMonster)
	{
		if (GetTickCount() > dw_Bullet + 400)
			m_bDead = true;
	}

	if (BOSSB_BULLET != m_CurrentMonster)
	{
		FrameMove();
	}
	return OBJ_NOEVENT;
}

void CMBullet::Late_Update_GameObject()
{

}

void CMBullet::Render_GameObject()
{
	if (BOSSB_BULLET == m_CurrentMonster)
	{
		const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(m_strObjectKey, m_strStateKey, m_tFrame.fFrameStart);
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

void CMBullet::Release_GameObject()
{
}
