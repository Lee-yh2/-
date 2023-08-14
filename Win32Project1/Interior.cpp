#include "stdafx.h"
#include "Interior.h"
#include "Texture_Manager.h"
#include "Scroll_Manager.h"
#include "Graphic_Device.h"
#include "Item.h"


CInterior::CInterior()
	:m_eFurniture(END), m_fFrameKey(0.f), m_bStandA(false), m_bStandB(false), m_bStandC(false), m_bStandD(false)
{
}


CInterior::~CInterior()
{
}

HRESULT CInterior::Ready_GameObject()
{
	if (STAND == m_eFurniture)
	{
		m_tInfo.vCSize = { 86.f, 103.f, 0.f };
		m_tInfo.vSize = { 1.f, 1.f, 0.f };
		m_strObjectKey = L"GAME_UI";
		m_strStateKey = L"SHOP_STAND";
		m_fFrameKey = 0.f;
	}
	else if (BED == m_eFurniture)
	{
		m_tInfo.vCSize = { 52.f, 13.f, 0.f };
		m_tInfo.vSize = { 1.4f, 1.4f, 0.f };
		m_strObjectKey = L"GAME_UI";
		m_strStateKey = L"Bed";
		m_fFrameKey = 0.f;
	}
	else if(CARPET == m_eFurniture)
	{
		m_tInfo.vCSize = { 0.f, 0.f, 0.f };
		m_tInfo.vSize = { 1.4f, 1.4f, 0.f };
		m_strObjectKey = L"GAME_UI";
		m_strStateKey = L"Bed";
		m_fFrameKey = 1.f;
	}
	else if (DOOR == m_eFurniture)
	{
		m_tInfo.vCSize = { 60.f, 46.f, 0.f };
		m_tInfo.vSize = { 1.0f, 1.0f, 0.f };
		m_strObjectKey = L"GAME_UI";
		m_strStateKey = L"SHOP_STAND";
		m_fFrameKey = 1.f;
	}
	else if (PORTALA == m_eFurniture)
	{
		m_tInfo.vCSize = { 60.f, 46.f, 0.f };
		m_tInfo.vSize = { 1.5f, 1.5f, 0.f };
		m_strObjectKey = L"STAGE_A";
		m_strStateKey = L"IDLE";
		m_tFrame = { 0.f, 5.f }; 
	}
	else if (PORTALB == m_eFurniture)
	{
		m_tInfo.vCSize = { 60.f, 46.f, 0.f };
		m_tInfo.vSize = { 1.5f, 1.5f, 0.f };
		m_strObjectKey = L"STAGE_B";
		m_strStateKey = L"IDLE";
		m_tFrame = { 0.f, 5.f };
	}
	return S_OK;
}

int CInterior::Update_GameObject()
{
	if (m_bDead)
		return OBJ_DEAD;
	if (PORTALB == m_eFurniture || PORTALA == m_eFurniture)
		FrameMove(0.6f);

	if (STAND == m_eFurniture)
	{
		if (m_ShopStandItem[0])
		{
			m_bStandA = true;
		}
		else
		{
			m_bStandA = false;
		}
		if (m_ShopStandItem[1])
		{
			m_bStandB = true;
		}
		else
		{
			m_bStandB = false;
		}
		if (m_ShopStandItem[2])
		{
			m_bStandC = true;
		}
		else
		{
			m_bStandC = false;
		}
		if (m_ShopStandItem[3])
		{
			m_bStandD = true;
		}
		else
		{
			m_bStandD = false;
		}
	}

	return OBJ_NOEVENT;
}

void CInterior::Late_Update_GameObject()
{
}

void CInterior::Render_GameObject()
{
	const TEXINFO* pTexInfo;
	if (PORTALB == m_eFurniture || PORTALA == m_eFurniture)
		pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(m_strObjectKey, m_strStateKey, m_tFrame.fFrameStart);
	else
		pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(m_strObjectKey, m_strStateKey, m_fFrameKey);
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

	if (STAND == m_eFurniture)
	{
		if (m_bStandA)
		{
			wstring strObjectKey = static_cast<CItem*>(m_ShopStandItem[0])->Get_ObjectKey();
			wstring strStateKey = static_cast<CItem*>(m_ShopStandItem[0])->Get_StateKey();
			int iKeyNum = static_cast<CItem*>(m_ShopStandItem[0])->Get_KeyNum();

			pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(strObjectKey, strStateKey, iKeyNum);
			if (nullptr == pTexInfo)
				return;
			D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
			D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + iScrollX - 21.f, m_tInfo.vPos.y + iScrollY - 26.f, 0.f);
			fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
			fCenterY = float(pTexInfo->tImageInfo.Height >> 1);
			matWorld = matScale * matTrans;
			CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
			CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
		}

		if (m_bStandB)
		{
			wstring strObjectKey = static_cast<CItem*>(m_ShopStandItem[1])->Get_ObjectKey();
			wstring strStateKey = static_cast<CItem*>(m_ShopStandItem[1])->Get_StateKey();
			int iKeyNum = static_cast<CItem*>(m_ShopStandItem[1])->Get_KeyNum();

			pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(strObjectKey, strStateKey, iKeyNum);
			if (nullptr == pTexInfo)
				return;
			D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
			D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + iScrollX + 21.f, m_tInfo.vPos.y + iScrollY - 26.f, 0.f);
			fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
			fCenterY = float(pTexInfo->tImageInfo.Height >> 1);
			matWorld = matScale * matTrans;
			CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
			CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

		}

		if (m_bStandC)
		{
			wstring strObjectKey = static_cast<CItem*>(m_ShopStandItem[2])->Get_ObjectKey();
			wstring strStateKey = static_cast<CItem*>(m_ShopStandItem[2])->Get_StateKey();
			int iKeyNum = static_cast<CItem*>(m_ShopStandItem[2])->Get_KeyNum();

			pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(strObjectKey, strStateKey, iKeyNum);
			if (nullptr == pTexInfo)
				return;
			D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
			D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + iScrollX - 21.f, m_tInfo.vPos.y + iScrollY + 26.f, 0.f);
			fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
			fCenterY = float(pTexInfo->tImageInfo.Height >> 1);
			matWorld = matScale * matTrans;
			CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
			CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

		}

		if (m_bStandD)
		{
			wstring strObjectKey = static_cast<CItem*>(m_ShopStandItem[3])->Get_ObjectKey();
			wstring strStateKey = static_cast<CItem*>(m_ShopStandItem[3])->Get_StateKey();
			int iKeyNum = static_cast<CItem*>(m_ShopStandItem[3])->Get_KeyNum();

			pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(strObjectKey, strStateKey, iKeyNum);
			if (nullptr == pTexInfo)
				return;
			D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
			D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + iScrollX + 21.f, m_tInfo.vPos.y + iScrollY + 26.f, 0.f);
			fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
			fCenterY = float(pTexInfo->tImageInfo.Height >> 1);
			matWorld = matScale * matTrans;
			CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
			CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
		}
	}
}

void CInterior::Release_GameObject()
{
}

void CInterior::Set_Stand(CGameObject ** Stand)
{
	for (int i = 0; i < 4; ++i)
		m_ShopStandItem[i] = Stand[i];
}

void CInterior::Set_Price(int * iPrice)
{
	for (int i = 0; i < 4; ++i)
	{
		m_iPrice[i] = &iPrice[i];
	}
}
