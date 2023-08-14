#include "stdafx.h"
#include "Particle.h"
#include "Time_Manager.h"
#include "Texture_Manager.h"
#include "Scroll_Manager.h"
#include "Graphic_Device.h"


CParticle::CParticle()
{
}


CParticle::~CParticle()
{
}

HRESULT CParticle::Ready_GameObject()
{
	m_tInfo.vSize = { 1.f, 1.f, 0.f };
	Check_Type();
	return S_OK;
}

int CParticle::Update_GameObject()
{
	if (m_bDead)
		return OBJ_DEAD;
	if (TYPE::BOSSB_BULLETBREAK == m_eType)
		FrameMove(1.f);
	else
		FrameMove(1.4f);
	return OBJ_NOEVENT;
}

void CParticle::Late_Update_GameObject()
{
}

void CParticle::Render_GameObject()
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

void CParticle::Release_GameObject()
{
}

void CParticle::FrameMove(float fMoveSpeed)
{
	m_tFrame.fFrameStart += m_tFrame.fFrameEnd *CTime_Manager::Get_Instance()->Get_DeltaTime() * fMoveSpeed;
	if (m_tFrame.fFrameStart > m_tFrame.fFrameEnd)
	{
		m_tFrame.fFrameStart = m_tFrame.fFrameEnd;
		m_bDead = true;
	}
}

void CParticle::Check_Type()
{
	if (TYPE::BOSSA_ROCKBREAK == m_eType)
	{
		m_strObjectKey = L"BOSS_A";
		m_strStateKey = L"ROCK_DISTROY";
		m_tFrame = { 0.f, 9.f };
	}
	else if (TYPE::HIT_BIG == m_eType)
	{
		m_strObjectKey = L"PARTICLE";
		m_strStateKey = L"HIT_BIG";
		m_tFrame = { 0.f, 2.f };
	}
	else if (TYPE::HIT_BOW == m_eType)
	{
		m_strObjectKey = L"PARTICLE";
		m_strStateKey = L"HIT_BOW";
		m_tFrame = { 0.f, 2.f };
	}
	else if (TYPE::HIT_PLAYER == m_eType)
	{
		m_strObjectKey = L"PARTICLE";
		m_strStateKey = L"HIT_PLAYER";
		m_tFrame = { 0.f, 2.f };
	}
	else if (TYPE::HIT_SHORT == m_eType)
	{
		m_strObjectKey = L"PARTICLE";
		m_strStateKey = L"HIT_SHORT";
		m_tFrame = { 0.f, 2.f };
	}
	else if (TYPE::BOSSB_BULLETBREAK == m_eType)
	{
		m_strObjectKey = L"BOSS_B_PARTICLE";
		m_strStateKey = L"BOSS_B_PARTICLE_BULLET";
		m_tInfo.vSize = { 1.5f, 1.5f, 0.f };
		m_tFrame = { 0.f, 8.f };
	}
}
