#include "stdafx.h"
#include "Plant.h"
#include "Scroll_Manager.h"
#include "Graphic_Device.h"
#include "Texture_Manager.h"
#include "Time_Manager.h"
#include "GameObject_Manager.h"
#include "MBullet.h"

CPlant::CPlant()
	:m_eCurState(END), m_ePreState(END), m_bStart(false), m_dwAttack(GetTickCount()), m_bAttack(false), m_bPots(true)
	, m_fAngle(0.f), m_bShot(false), m_fPosinDis(0.f)
{
}


CPlant::~CPlant()
{
}

HRESULT CPlant::Ready_GameObject()
{
	m_tInfo.vSize = { 3.f, 3.f, 0.f };
	m_tInfo.vDir = { 0.f, 0.f, 0.f };
	m_tInfo.vCSize = { 0.f, 0.f, 0.f };

	m_tUnitInfo.iAtt = 15;
	m_tUnitInfo.iGold = 0;
	m_tUnitInfo.iHp = 20;
	m_tUnitInfo.wstrName = L"BOSS_B";

	m_fSpeed = 150.f;
	m_fSpeed *= CTime_Manager::Get_Instance()->Get_DeltaTime();

	m_fPosinDis = 10.f;

	m_eCurState = STATE_WAKEUP;
	m_strStateKey = L"WAKEUP";
	m_tFrame = { 0.f, 15.f };

	return S_OK;
}

int CPlant::Update_GameObject()
{
	if (m_bDead)
		return OBJ_DEAD;
	if (!m_bPots)
	{
		if (m_tUnitInfo.iHp <= 0)
		{
			m_bAttack = false;
			m_bStart = false;
			m_eCurState = STATE_DEAD;
			m_strStateKey = L"BOSS_B_PARTICLE_DEATH";
			Scene_Change();
		}

		if (STATE_SHOT == m_eCurState)
			FrameMove(0.4f, 0.8f);
		else
			FrameMove(0.8f, 0.8f);
		Scene_Change();
		if (m_bStart)
		{
			float fCX = m_tTargetInfo->vPos.x - m_tInfo.vPos.x;
			float fCY = m_tTargetInfo->vPos.y - m_tInfo.vPos.y;
			float fDia = sqrtf(fCX*fCX + fCY*fCY);

			m_fAngle = acosf((fCX) / fDia) * 180.f / PI;
			if (m_tInfo.vPos.y > m_tTargetInfo->vPos.y)
				m_fAngle *= -1.f;
			
			Attack();
		}
	}
	else
	{
		float	fCX = m_tPotTarget.x - m_tInfo.vPos.x;
		float	fCY = m_tPotTarget.y - m_tInfo.vPos.y;
		float	fDia = sqrtf(fCX * fCX + fCY * fCY);

		float	fRad = acosf(fCX / fDia);

		m_fAngle = fRad * 180.f / PI;

		if (m_tInfo.vPos.y < m_tPotTarget.y)
			m_fAngle *= -1.f;

		m_tInfo.vPos.x += cosf(m_fAngle * PI / 180.f) * m_fSpeed;
		m_tInfo.vPos.y -= sinf(m_fAngle * PI / 180.f) * m_fSpeed;

		if (m_tPotTarget.x + 5.f > m_tInfo.vPos.x && m_tPotTarget.x - 5 < m_tInfo.vPos.x
			&& m_tPotTarget.y + 5.f > m_tInfo.vPos.y && m_tPotTarget.y - 5 < m_tInfo.vPos.y)
		{
			m_tInfo.vSize = { 1.5f, 1.5f, 0.f };
			m_bPots = false;
		}
	}
	OnHit();
	return OBJ_NOEVENT;
}

void CPlant::Late_Update_GameObject()
{
	m_vecPosin.x = (m_tInfo.vPos.x + cosf(m_fAngle * PI / 180.f) * m_fPosinDis);
	m_vecPosin.y = ((m_tInfo.vPos.y - 10.f) + sinf(m_fAngle * PI / 180.f) * m_fPosinDis);
	m_vecPosin.z = 0.f;
}

void CPlant::Render_GameObject()
{
	if (m_bStart)
	{
		const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(L"BOSS_B_PARTICLE_BODY", m_strBodyStateKey, int(m_tBodyFrame.fFrameStart));
		if (nullptr == pTexInfo)
			return;
		float iScrollX = CScroll_Manager::Get_Scroll(CScroll_Manager::SCROLLX);
		float iScrollY = CScroll_Manager::Get_Scroll(CScroll_Manager::SCROLLY);
		D3DXMATRIX matWorld, matRotZ, matTrans, matScale;
		D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
		D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + iScrollX, m_tInfo.vPos.y + iScrollY, 0.f);
		float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
		float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);
		matWorld = matScale * matTrans;
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
		CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, m_iRGB, m_iRGB));

		pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(L"BOSS_B_PARTICLE_HEAD", m_strHeadStateKey, int(m_tFrame.fFrameStart));
		if (nullptr == pTexInfo)
			return;
		iScrollX = CScroll_Manager::Get_Scroll(CScroll_Manager::SCROLLX);
		iScrollY = CScroll_Manager::Get_Scroll(CScroll_Manager::SCROLLY);
		D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
		D3DXMatrixRotationZ(&matRotZ, m_fAngle * PI / 180.f);
		D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + iScrollX, m_tInfo.vPos.y + iScrollY - 10.f, 0.f);
		fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
		fCenterY = float(pTexInfo->tImageInfo.Height >> 1);
		matWorld = matScale * matRotZ * matTrans;
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
		CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, m_iRGB, m_iRGB));
	}
	else
	{
		const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(L"BOSS_B_PARTICLE", m_strStateKey, int(m_tFrame.fFrameStart));
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
}

void CPlant::Release_GameObject()
{
}

void CPlant::FrameMove(float fMoveSpeed, float fMoveBodySpeed)
{
	m_tFrame.fFrameStart += m_tFrame.fFrameEnd *CTime_Manager::Get_Instance()->Get_DeltaTime() * fMoveSpeed;
	m_tBodyFrame.fFrameStart += m_tBodyFrame.fFrameEnd *CTime_Manager::Get_Instance()->Get_DeltaTime() * fMoveBodySpeed;
	if (m_tFrame.fFrameStart > m_tFrame.fFrameEnd)
	{
		if (STATE_WAKEUP == m_eCurState)
		{
			m_eCurState = STATE_IDLE;
			m_strBodyStateKey = L"IDLE";
			m_strHeadStateKey = L"IDLE";
			m_tInfo.vCSize = { 30.f, 30.f, 0.f };
			m_dwAttack = GetTickCount();
			m_bStart = true;
		}
		else if (STATE_SHOT == m_eCurState)
		{
			m_eCurState = STATE_IDLE;
			m_strBodyStateKey = L"IDLE";
			m_strHeadStateKey = L"IDLE";
			m_bShot = false;
			m_dwAttack = GetTickCount();
			m_bAttack = false;
		}
		else if (STATE_DEAD == m_eCurState)
		{
			m_tFrame.fFrameStart = m_tFrame.fFrameEnd;
			Set_Dead();
		}

		if (STATE_DEAD != m_eCurState)
			m_tFrame.fFrameStart = 0.f;
	}
	if (m_tBodyFrame.fFrameStart > m_tBodyFrame.fFrameEnd)
	{
		m_tBodyFrame.fFrameStart = 0.f;
	}
}

void CPlant::Scene_Change()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case STATE::STATE_WAKEUP:
			m_tFrame.fFrameStart = 0.f;
			m_tFrame.fFrameEnd = 15.f;
			break;
		case STATE::STATE_IDLE:
			m_tFrame.fFrameStart = 0.f;
			m_tFrame.fFrameEnd = 5.f;
			m_tBodyFrame.fFrameStart = 0.f;
			m_tBodyFrame.fFrameEnd = 5.f;
			break;
		case STATE::STATE_SHOT:
			m_tFrame.fFrameStart = 0.f;
			m_tFrame.fFrameEnd = 14.f;
			m_tBodyFrame.fFrameStart = 0.f;
			m_tBodyFrame.fFrameEnd = 5.f;
			break;
		case STATE::STATE_DEAD:
			m_tFrame.fFrameStart = 0.f;
			m_tFrame.fFrameEnd = 15.f;
			break;
		}
		m_ePreState = m_eCurState;
	}
}

void CPlant::Attack()
{
	if (!m_bAttack)
	{
		if (m_dwAttack + 2000 < GetTickCount())
		{
			m_eCurState = STATE_SHOT;
			m_strHeadStateKey = L"SHOT";
			m_strBodyStateKey = L"IDLE";
			m_bAttack = true;
		}
	}
	else
	{
		if (10.f <= m_tFrame.fFrameStart && !m_bShot)
		{
			CMBullet* bullet = new CMBullet;
			bullet->Set_Pos(m_vecPosin);
			D3DXVECTOR3 vSetPos = { 0.f, -10.f, 0.f };
			D3DXVECTOR3 vDir = m_vecPosin - (m_tInfo.vPos + vSetPos);
			D3DXVec3Normalize(&vDir, &vDir);
			bullet->Set_Dir(vDir);
			static_cast<CMBullet*>(bullet)->Set_Weapon(BOSSB_BULLET);
			bullet->Ready_GameObject();
			CGameObject_Manager::Get_Instance()->Add_GameObject(CGameObject_Manager::OBJ_MBULLET, bullet);

			m_bShot = true;
		}
	}
}
