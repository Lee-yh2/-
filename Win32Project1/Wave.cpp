#include "stdafx.h"
#include "Wave.h"
#include "Time_Manager.h"
#include "Texture_Manager.h"
#include "Scroll_Manager.h"
#include "Graphic_Device.h"
#include "SoundMgr.h"


CWave::CWave()
	:m_fCSize(1.f), m_bQuake(false)
{
}


CWave::~CWave()
{
}

HRESULT CWave::Ready_GameObject()
{
	m_tInfo.vSize = { 1.f, 1.f, 0.f };
	m_tInfo.vCSize = { 2.f, 2.f, 0.f };
	CSoundMgr::Get_Instance()->PlaySound(L"BOSS_A_WAVE.wav", CSoundMgr::MONSTER2);
	m_tFrame = { 0.f, 15.f };
	return S_OK;
}

int CWave::Update_GameObject()
{
	if (m_bDead)
		return OBJ_DEAD;
	m_fCSize += 0.0011f;
	m_tInfo.vCSize *= m_fCSize;

	if (m_tFrame.fFrameStart > 11.f && !m_bQuake)
	{
		CScroll_Manager::Scroll_Quake();
		m_bQuake = true;
	}

	FrameMove(0.6f);
	return OBJ_NOEVENT;
}

void CWave::Late_Update_GameObject()
{
}

void CWave::Render_GameObject()
{
	const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(L"BOSS_A", L"WAVE", m_tFrame.fFrameStart);
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

void CWave::Release_GameObject()
{
}

void CWave::FrameMove(float fMoveSpeed)
{
	m_tFrame.fFrameStart += m_tFrame.fFrameEnd *CTime_Manager::Get_Instance()->Get_DeltaTime() * fMoveSpeed;
	if (m_tFrame.fFrameStart > m_tFrame.fFrameEnd)
	{
		Set_Dead();
		m_tFrame.fFrameStart = 0.f;
	}
}