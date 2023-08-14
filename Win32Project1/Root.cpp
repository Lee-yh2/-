#include "stdafx.h"
#include "Root.h"
#include "Texture_Manager.h"
#include "Scroll_Manager.h"
#include "SoundMgr.h"
#include "Graphic_Device.h"
#include "GameObject_Manager.h"
#include "Time_Manager.h"
#include "MBullet.h"


CRoot::CRoot()
	:m_bAttack(false), m_bRootSound(false)
{
}


CRoot::~CRoot()
{
}

HRESULT CRoot::Ready_GameObject()
{
	m_tInfo.vSize = { 1.2f, 1.2f, 0.f };
	m_tInfo.vDir = { 0.f, 0.f, 0.f };

	if (GUARD == m_eType)
		m_tInfo.vCSize = { 10.f, 10.f, 0.f };
	else
		m_tInfo.vCSize = { 0.f, 0.f, 0.f };

	int iRand = rand() % 2;
	if (0 == iRand)
		m_strStateKey = L"ROOT_A";
	else
		m_strStateKey = L"ROOT_B";

	m_tFrame = { 0.f, 60.f };
	return S_OK;
}

int CRoot::Update_GameObject()
{
	if (GUARD == m_eType && m_tFrame.fFrameStart >= 33.f && !m_bDead)
		m_tFrame.fFrameStart = 18.f;

	if (m_bDead)
		return OBJ_DEAD;

	if (!m_bRootSound)
	{
		CSoundMgr::Get_Instance()->PlaySound(L"BOSS_B_STEM_I.wav", CSoundMgr::MONSTERROOT);
		m_bRootSound = true;
	}

	if (ATTACK == m_eType && m_tFrame.fFrameStart >= 4.f && !m_bAttack)
	{
		CMBullet* bullet = new CMBullet;
		bullet->Set_Pos(m_tInfo.vPos);
		D3DXVECTOR3 vDir = { 0.f, 0.f, 0.f };
		bullet->Set_Dir(vDir);
		static_cast<CMBullet*>(bullet)->Set_Weapon(BOSSA_ROCK);
		bullet->Ready_GameObject();
		CGameObject_Manager::Get_Instance()->Add_GameObject(CGameObject_Manager::OBJ_MBULLET, bullet);

		m_bAttack = true;
	}

	FrameMove(0.2f);
	return OBJ_NOEVENT;
}

void CRoot::Late_Update_GameObject()
{
}

void CRoot::Render_GameObject()
{
	const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(L"BOSS_B", m_strStateKey, int(m_tFrame.fFrameStart));
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
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, m_iRGB, m_iRGB));
}

void CRoot::Release_GameObject()
{
}

void CRoot::FrameMove(float fMoveSpeed)
{
	m_tFrame.fFrameStart += m_tFrame.fFrameEnd *CTime_Manager::Get_Instance()->Get_DeltaTime() * fMoveSpeed;
	if (m_tFrame.fFrameStart > m_tFrame.fFrameEnd)
	{
		Set_Dead();
		m_tFrame.fFrameStart = m_tFrame.fFrameEnd;
	}
}
