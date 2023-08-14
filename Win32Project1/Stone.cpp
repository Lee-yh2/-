#include "stdafx.h"
#include "Stone.h"
#include "Time_Manager.h"
#include "Texture_Manager.h"
#include "Scroll_Manager.h"
#include "Graphic_Device.h"
#include "MBullet.h"
#include "GameObject_Manager.h"
#include "Particle.h"
#include "SoundMgr.h"


CStone::CStone()
	:dw_Fall(GetTickCount()), m_bShadow(true)
{
}


CStone::~CStone()
{
}

HRESULT CStone::Ready_GameObject()
{
	m_tInfo.vSize = { 1.f, 1.f, 0.f };
	m_tInfo.vCSize = { 0.f, 0.f, 0.f };
	m_vSetSize = { 30.f, 30.f, 0.f };

	m_tRockTexture.vPos = { m_tInfo.vPos.x, m_tInfo.vPos.y - WINCY, 0.f };
	m_tRockTexture.vSize = { 1.f, 1.f, 1.f };
	m_tRockTexture.vDir = { 0.f, 1.f, 0.f };

	m_fSpeed = 900.f;
	m_fSpeed *= CTime_Manager::Get_Instance()->Get_DeltaTime();

	m_iRockNum = rand() % 6;
	return S_OK;
}

int CStone::Update_GameObject()
{
	if (dw_Fall + 8000 < GetTickCount())
	{
		CParticle* particle = new CParticle;
		particle->Set_Pos(m_tInfo.vPos);
		static_cast<CParticle*>(particle)->Set_Type(CParticle::BOSSA_ROCKBREAK);
		particle->Ready_GameObject();
		CGameObject_Manager::Get_Instance()->Add_GameObject(CGameObject_Manager::OBJ_EFFECT, particle);
		return OBJ_DEAD;
	}

	if (dw_Fall + 1500 < GetTickCount())
	{
		if (m_tRockTexture.vPos.y >= m_tInfo.vPos.y)
		{
			if (m_bShadow)
			{
				CMBullet* bullet = new CMBullet;
				bullet->Set_Pos(m_tInfo.vPos);
				D3DXVECTOR3 vDir = { 0.f, 0.f, 0.f };
				bullet->Set_Dir(vDir);
				static_cast<CMBullet*>(bullet)->Set_Weapon(BOSSA_ROCK);
				bullet->Ready_GameObject();
				CGameObject_Manager::Get_Instance()->Add_GameObject(CGameObject_Manager::OBJ_MBULLET, bullet);

				CScroll_Manager::Scroll_Quake();
				CSoundMgr::Get_Instance()->PlaySound(L"BOSS_A_ROCK.wav", CSoundMgr::MONSTER);
				m_tInfo.vCSize = m_vSetSize;
				m_bShadow = false;
				m_tRockTexture.vPos.y = m_tInfo.vPos.y;
			}
		}
		else
			m_tRockTexture.vPos += m_tRockTexture.vDir * m_fSpeed;
	}
	return OBJ_NOEVENT;
}

void CStone::Late_Update_GameObject()
{
}

void CStone::Render_GameObject()
{
	const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(L"BOSS_A", L"ROCK", m_iRockNum);
	if (nullptr == pTexInfo)
		return;
	float iScrollX = CScroll_Manager::Get_Scroll(CScroll_Manager::SCROLLX);
	float iScrollY = CScroll_Manager::Get_Scroll(CScroll_Manager::SCROLLY);
	D3DXMATRIX matWorld, matTrans, matScale;
	D3DXMatrixScaling(&matScale, m_tRockTexture.vSize.x, m_tRockTexture.vSize.y, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tRockTexture.vPos.x + iScrollX, m_tRockTexture.vPos.y + iScrollY, 0.f);
	float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);
	matWorld = matScale * matTrans;
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	
	if (m_bShadow)
	{
		pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(L"BOSS_A", L"ROCK_SHADOW", 0);
		if (nullptr == pTexInfo)
			return;
		iScrollX = CScroll_Manager::Get_Scroll(CScroll_Manager::SCROLLX);
		iScrollY = CScroll_Manager::Get_Scroll(CScroll_Manager::SCROLLY);
		D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
		D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + iScrollX, m_tInfo.vPos.y + iScrollY, 0.f);
		fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
		fCenterY = float(pTexInfo->tImageInfo.Height >> 1);
		matWorld = matScale * matTrans;
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
		CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
}

void CStone::Release_GameObject()
{
}
