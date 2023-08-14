#include "stdafx.h"
#include "BossA_Arm.h"
#include "GameObject_Manager.h"
#include "Texture_Manager.h"
#include "Graphic_Device.h"
#include "Scroll_Manager.h"
#include "Time_Manager.h"
#include "Shadow.h"
#include "MBullet.h"
#include "SoundMgr.h"


CBossA_Arm::CBossA_Arm()
	:m_iCount(0), m_bAttack(false), m_fSpeed(0.f), m_eCurrentState(STATE_PUNCHDOWN), dw_AttackTime(GetTickCount())
	, m_bDelay(true)
{
}


CBossA_Arm::~CBossA_Arm()
{
}

HRESULT CBossA_Arm::Ready_GameObject()
{
	m_tInfo.vPos = { m_tTargetInfo.vPos.x, m_tTargetInfo.vPos.y - (float)WINCY, 0.f };
	m_tInfo.vSize = { 1.f, 1.f, 0.f };
	m_strStateKey = L"PUNCH_DOWN";

	m_fSpeed = 1100.f;
	m_fSpeed *= CTime_Manager::Get_Instance()->Get_DeltaTime();

	m_tFrame = { 0.f, 8.f };

	CGameObject* pObj = new CShadow;
	static_cast<CShadow*>(pObj)->Set_ArmInfo(&m_tInfo);
	static_cast<CShadow*>(pObj)->Set_Attack(&m_bAttack);
	static_cast<CShadow*>(pObj)->Set_Speed(m_fSpeed/8.f);
	static_cast<CShadow*>(pObj)->Set_Dead(&m_bDead);
	pObj->Ready_GameObject();
	CGameObject_Manager::Get_Instance()->Add_GameObject(CGameObject_Manager::OBJ_BACKEFFECT, pObj);
	return S_OK;
}

int CBossA_Arm::Update_GameObject()
{
	if (5 == m_iCount)
	{
		m_bDead = true;
		return OBJ_DEAD;
	}

	if (dw_AttackTime + 2000 < GetTickCount())
		m_bAttack = true;

	if (!m_bAttack)
	{
		m_tTargetInfo = CGameObject_Manager::Get_Instance()->Get_Player()->Get_Info();
		float fDirX = (m_tTargetInfo.vPos.x - m_tInfo.vPos.x)/ abs(m_tTargetInfo.vPos.x - m_tInfo.vPos.x);
		if (m_tInfo.vPos.x -20.f <= m_tTargetInfo.vPos.x && m_tInfo.vPos.x + 20.f >= m_tTargetInfo.vPos.x)
			m_tInfo.vPos.x = m_tTargetInfo.vPos.x;
		else
			m_tInfo.vPos.x += fDirX * (m_fSpeed / 8.f);
	}
	else
	{
		FrameMove(1.f);
		if (STATE_PUNCHDOWN == m_eCurrentState)
		{
			if (m_tInfo.vPos.y >= m_tTargetInfo.vPos.y)
			{
				m_tInfo.vPos.y = m_tTargetInfo.vPos.y;
				if (m_bDelay)
				{
					D3DXVECTOR3 vRange = { 0.f, 20.f, 0.f };
					D3DXVECTOR3 vDir = { 0.f, 0.f, 0.f };
					Create_Bullet(vRange, vDir, BOSSA_PUNCH);
				
					CScroll_Manager::Scroll_Quake();
					CSoundMgr::Get_Instance()->PlaySound(L"BOSS_A_ROCKET.wav", CSoundMgr::MONSTER2);
					dw_DelayTime = GetTickCount();
					m_bDelay = false;
				}
				if (dw_DelayTime + 1000 < GetTickCount())
				{
					m_eCurrentState = STATE_PUNCHUP;
					m_strStateKey = L"PUNCH_UP";
					m_tFrame.fFrameStart = 0.f;
				}
			}
			else
			{
				float fDirY = (m_tTargetInfo.vPos.y - m_tInfo.vPos.y) / abs(m_tTargetInfo.vPos.y - m_tInfo.vPos.y);
				m_tInfo.vPos.y += fDirY * m_fSpeed;
			}
		}
		else if (STATE_PUNCHUP == m_eCurrentState)
		{
			if (m_tInfo.vPos.y <= (m_tTargetInfo.vPos.y - WINCY))
			{
				m_eCurrentState = STATE_PUNCHDOWN;
				m_strStateKey = L"PUNCH_DOWN";
				m_tFrame.fFrameStart = 0.f;
				++m_iCount;
				dw_AttackTime = GetTickCount();
				m_bDelay = true;
				m_bAttack = false;
			}
			else
			{
				float fDirY = ((m_tTargetInfo.vPos.y - WINCY) - m_tInfo.vPos.y) / abs((m_tTargetInfo.vPos.y - WINCY) - m_tInfo.vPos.y);
				m_tInfo.vPos.y += fDirY * m_fSpeed;
			}
		}
	}
	return OBJ_NOEVENT;
}

void CBossA_Arm::Late_Update_GameObject()
{
}

void CBossA_Arm::Render_GameObject()
{
	const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(L"BOSS_A", m_strStateKey, int(m_tFrame.fFrameStart));
	if (nullptr == pTexInfo)
		return;

	float iScrollX = CScroll_Manager::Get_Scroll(CScroll_Manager::SCROLLX);
	float iScrollY = CScroll_Manager::Get_Scroll(CScroll_Manager::SCROLLY);
	D3DXMATRIX matWorld, matTrans, matScale;
	D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + iScrollX, m_tInfo.vPos.y + iScrollY, 0.f);
	float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tImageInfo.Height >> 1) + 9.f;
	matWorld = matScale * matTrans;
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

}

void CBossA_Arm::Release_GameObject()
{
}

void CBossA_Arm::FrameMove(float fMoveSpeed)
{
	m_tFrame.fFrameStart += m_tFrame.fFrameEnd *CTime_Manager::Get_Instance()->Get_DeltaTime() * fMoveSpeed;
	if (m_tFrame.fFrameStart > m_tFrame.fFrameEnd)
	{
			m_tFrame.fFrameStart = 8.f;
	}
}

void CBossA_Arm::Create_Bullet(D3DXVECTOR3 vRange, D3DXVECTOR3 vDir, BYTE Monster)
{
	CMBullet* bullet = new CMBullet;
	bullet->Set_Pos(m_tInfo.vPos + (vRange));
	bullet->Set_Dir(vDir);
	static_cast<CMBullet*>(bullet)->Set_Weapon(Monster);
	bullet->Ready_GameObject();
	CGameObject_Manager::Get_Instance()->Add_GameObject(CGameObject_Manager::OBJ_MBULLET, bullet);
}
