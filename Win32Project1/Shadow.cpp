#include "stdafx.h"
#include "Shadow.h"
#include "GameObject_Manager.h"
#include "Graphic_Device.h"
#include "Scroll_Manager.h"
#include "Texture_Manager.h"


CShadow::CShadow()
{
}


CShadow::~CShadow()
{
}

HRESULT CShadow::Ready_GameObject()
{
	m_tInfo.vSize = { 0.f, 0.f, 0.f };
	m_tInfo.vPos = { m_tArmInfo->vPos.x, WINCY+(m_tArmInfo->vPos.y), 0.f };
	return S_OK;
}

int CShadow::Update_GameObject()
{
	if (*m_bDead)
		return OBJ_DEAD;

	if (!*m_bAttack)
	{
		m_tInfo.vPos.x = m_tArmInfo->vPos.x;
		m_tTargetInfo = CGameObject_Manager::Get_Instance()->Get_Player()->Get_Info();

		if (m_tInfo.vPos.y + (m_tTargetInfo.vCSize.y / 2.f) - 24.5f <= m_tTargetInfo.vPos.y && m_tInfo.vPos.y + (m_tTargetInfo.vCSize.y / 2.f) + 4.5f >= m_tTargetInfo.vPos.y)
			m_tInfo.vPos.y = m_tTargetInfo.vPos.y + (m_tTargetInfo.vCSize.y / 2.f) - 14.5f;
		else
		{
			float fDirY;
			if (m_tTargetInfo.vPos.y + (m_tTargetInfo.vCSize.y / 2.f) >= m_tInfo.vPos.y)
				fDirY = 1.f;
			else if (m_tTargetInfo.vPos.y + (m_tTargetInfo.vCSize.y / 2.f) < m_tInfo.vPos.y)
				fDirY = -1.f;
			m_tInfo.vPos.y += fDirY * m_fSpeed;
		}
	}
	float fSize;
	if (m_tInfo.vPos.y - 150.f <= m_tArmInfo->vPos.y)
		fSize = 1.f;
	else
		fSize = (1.f / (m_tInfo.vPos.y -150.f - m_tArmInfo->vPos.y));
	if (fSize > 0.4)
	{
		cout << "0.4aaaaaaaaaaaaa" << endl;
	}
 	if (fSize <= 0.2f)
		fSize = 0.2f;
	cout << fSize << endl;
	m_tInfo.vSize = { fSize, fSize, 0.f };
	return OBJ_NOEVENT;
}

void CShadow::Late_Update_GameObject()
{
}

void CShadow::Render_GameObject()
{
	const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(L"BOSS_A", L"PUNCH_SHADOW", 0);
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

void CShadow::Release_GameObject()
{
}
