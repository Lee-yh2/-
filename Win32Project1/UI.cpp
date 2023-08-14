#include "stdafx.h"
#include "UI.h"
#include "Texture_Manager.h"
#include "Graphic_Device.h"
#include "GameObject_Manager.h"
#include "Scroll_Manager.h"
#include "Player.h"


CUI::CUI()
	:m_dwHp(GetTickCount()), m_dwDead(GetTickCount())
{
}


CUI::~CUI()
{
}

HRESULT CUI::Ready_GameObject()
{
	m_tInfo.vSize = { 1.2f, 1.2f, 0.f };
	m_iCurWeapon = 1;
	m_iRGB = 36;

	if (TYPE::MONSTER_HP == m_eCurType)
	{
		m_tHp = { 0, 0, 504, 7 };
	}
	if (TYPE::HP_BAR == m_eCurType)
	{
		m_tHp = { 0, 0, 76, 15 };
		m_fHpPur = 250 / (float)m_tHp.right;
	}
	if (TYPE::WAKEUP == m_eCurType)
	{
		m_tInfo.vSize = { 1.4f, 1.4f, 0.f };
		m_tFrame = { 0.f,57.f };
	}
	return S_OK;
}

int CUI::Update_GameObject()
{
	if (TYPE::HP_BAR == m_eCurType)
	{
		if ((int)(m_tHp.right * m_fHpPur) > *m_iHp)
		{
			--m_tHp.right;
			Set_RGB(255);
		}
		else
			Set_RGB(36);

		if ((int)m_tHp.right < 0)
			m_tHp.right = 0;
	}
	else if (TYPE::WEAPON == m_eCurType)
	{
		BYTE byCurWeapon = CGameObject_Manager::Get_Instance()->Get_Player()->Get_UnitInfo().byCurrentWeapon;
		if (SHORTSWORD == byCurWeapon)
			m_iCurWeapon = 1;
		else if (BIGSWORD == byCurWeapon)
			m_iCurWeapon = 2;
		else if (BOW == byCurWeapon)
			m_iCurWeapon = 5;
	}
	else if (TYPE::MONSTER_HP == m_eCurType)
	{
		if ((int)m_tHp.right > *m_iHp)
		{
			--m_tHp.right;
			Set_RGB(255);
		}
		else
			Set_RGB(36);

		if ((int)m_tHp.right < 0)
		{
			m_tHp.right = 0;
			m_dwDead = GetTickCount();
			Set_Dead();
		}

		if (m_bDead && m_dwDead + 1000 < GetTickCount())
			return OBJ_DEAD;
		
	}
	else if (TYPE::WAKEUP == m_eCurType)
	{
		FrameMove(0.1f);
		if (m_tFrame.fFrameStart > 56.f)
		{
			static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Set_Start();
			return OBJ_DEAD;
		}
	}
	return OBJ_NOEVENT;
}

void CUI::Late_Update_GameObject()
{
}

void CUI::Render_GameObject()
{
	
	if (TYPE::HP_BAR == m_eCurType)
	{
		const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(L"GAME_UI", L"HEAL_BAR", 2);
		if (nullptr == pTexInfo)
			return;
		D3DXMATRIX matWorld, matTrans, matScale;
		D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
		D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);
		float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
		float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);
		matWorld = matScale * matTrans;
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
		CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

		pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(L"GAME_UI", L"HEAL_BAR", 3);
		if (nullptr == pTexInfo)
			return;
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
		D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y + 1.f, 0.f);
		fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
		fCenterY = float(pTexInfo->tImageInfo.Height >> 1);
		matWorld = matScale * matTrans;
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
		CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, m_iRGB, m_iRGB));

		pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(L"GAME_UI", L"HEAL_BAR", 0);
		if (nullptr == pTexInfo)
			return;
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
		D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x+ 53.f, m_tInfo.vPos.y, 0.f);
		fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
		fCenterY = float(pTexInfo->tImageInfo.Height >> 1);
		matWorld = matScale * matTrans;
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
		CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

		pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(L"GAME_UI", L"HEAL_BAR", 1);
		if (nullptr == pTexInfo)
			return;
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
		D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + 55.8f, m_tInfo.vPos.y, 0.f);
		fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
		fCenterY = float(pTexInfo->tImageInfo.Height >> 1);
		matWorld = matScale * matTrans;
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
		CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, &m_tHp, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, m_iRGB, m_iRGB));

	}
	else if (TYPE::WEAPON == m_eCurType)
	{
		const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(L"GAME_UI", L"BASECIRCLE", 0);
		if (nullptr == pTexInfo)
			return;

		D3DXMATRIX matWorld, matTrans, matScale;
		D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
		D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x - 7.f, m_tInfo.vPos.y + 2.f, 0.f);
		float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
		float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);
		matWorld = matScale * matTrans;
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
		CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 100, 100, 100));

		pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(L"GAME_UI", L"BASECIRCLE", 0);
		if (nullptr == pTexInfo)
			return;
		D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
		D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);
		fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
		fCenterY = float(pTexInfo->tImageInfo.Height >> 1);
		matWorld = matScale * matTrans;
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
		CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

		pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(L"GAME_UI", L"ITEM", m_iCurWeapon);
		if (nullptr == pTexInfo)
			return;
		D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
		D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);
		fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
		fCenterY = float(pTexInfo->tImageInfo.Height >> 1);
		matWorld = matScale * matTrans;
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
		CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

		pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(L"GAME_UI", L"ITEM", 6);
		if (nullptr == pTexInfo)
			return;
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
		D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x - 20.f, m_tInfo.vPos.y + 15.f, 0.f);
		fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
		fCenterY = float(pTexInfo->tImageInfo.Height >> 1);
		matWorld = matScale * matTrans;
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
		CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
	else if (TYPE::BAG == m_eCurType)
	{
		const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(L"GAME_UI", L"BASECIRCLE", 0);
		if (nullptr == pTexInfo)
			return;
		D3DXMATRIX matWorld, matTrans, matScale;
		D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
		D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);
		float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
		float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);
		matWorld = matScale * matTrans;
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
		CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

		pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(L"GAME_UI", L"ITEM", 0);
		if (nullptr == pTexInfo)
			return;
		D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
		D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);
		fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
		fCenterY = float(pTexInfo->tImageInfo.Height >> 1);
		matWorld = matScale * matTrans;
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
		CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
	else if (TYPE::GOLD == m_eCurType)
	{
		const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(L"GAME_UI", L"BASECIRCLE", 0);
		if (nullptr == pTexInfo)
			return;
		D3DXMATRIX matWorld, matTrans, matScale;
		D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
		D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);
		float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
		float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);
		matWorld = matScale * matTrans;
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
		CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

		pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(L"GAME_UI", L"ITEM", 7);
		if (nullptr == pTexInfo)
			return;
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
		D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);
		fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
		fCenterY = float(pTexInfo->tImageInfo.Height >> 1);
		matWorld = matScale * matTrans;
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
		CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
	else if (TYPE::MONSTER_HP == m_eCurType)
	{

		const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(L"GAME_UI", L"HEAL_BAR", 5);
		if (nullptr == pTexInfo)
			return;
		D3DXMATRIX matWorld, matTrans, matScale;
		D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
		D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);
		float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
		float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);
		matWorld = matScale * matTrans;
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
		CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, &m_tHp, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, m_iRGB, m_iRGB));
		
		pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(L"GAME_UI", L"HEAL_BAR", 4);
		if (nullptr == pTexInfo)
			return;
		D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
		D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);
		fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
		fCenterY = float(pTexInfo->tImageInfo.Height >> 1);
		matWorld = matScale * matTrans;
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
		CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
	else if (TYPE::WAKEUP == m_eCurType)
	{
		const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(L"FEILD", L"WAKEUP", m_tFrame.fFrameStart);
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

void CUI::Release_GameObject()
{
}


