#include "stdafx.h"
#include "Portal.h"
#include "Scene_Manager.h"
#include "KeyManager.h"
#include "GameObject_Manager.h"
#include "Graphic_Device.h"
#include "Scroll_Manager.h"
#include "Texture_Manager.h"
#include "Inventory.h"


CPortal::CPortal()
	:m_eGoScene(END), m_bCollision(false), m_dwCoolTime(GetTickCount()), m_iAlpha(255)
{
}


CPortal::~CPortal()
{
}

HRESULT CPortal::Ready_GameObject()
{
	m_tInfo.vDir = { 0.f, 0.f, 0.f };
	m_tInfo.vCSize = { 70.f , 40.f, 0.f };
	m_tInfo.vSize = { 0.7f, 0.7f, 1.f };
	return S_OK;
}

int CPortal::Update_GameObject()
{
	if (m_bDead)
		return OBJ_DEAD;

	if (m_bCollision)
	{
		if (m_dwCoolTime + 1500 < GetTickCount())
		{
			if (GO_BOSSENTER == m_eGoScene || GO_ENTERTOTOWN == m_eGoScene)
			{
				for (size_t i = 0; i < CGameObject_Manager::OBJ_END; ++i)
				{
					if (CGameObject_Manager::OBJ_PLAYER != i && CGameObject_Manager::OBJ_UI != i && CGameObject_Manager::OBJ_ITEM != i)
					{
						for (auto& pObject : CGameObject_Manager::Get_Instance()->Get_ListObject()[i])
						{
							pObject->Set_Dead();
						}
					}
				}
				CGameObject_Manager::Get_Instance()->Clear();
				if (GO_BOSSENTER == m_eGoScene)
					CScene_Manager::Get_Instance()->Scene_Change_SceneManager(CScene_Manager::SCENE_BOSSENTER);
				else if (GO_ENTERTOTOWN == m_eGoScene)
					CScene_Manager::Get_Instance()->Scene_Change_SceneManager(CScene_Manager::SCENE_TOWN);
			}
			else if (OPEN_STAND == m_eGoScene)
			{
				if (m_tKeyManager.KeyDown(KEY_C))
				{
					static_cast<CInventory*>(CGameObject_Manager::Get_Instance()->Get_Inven())->OpenShop();
					m_iAlpha = 255 == m_iAlpha ? 0 : 255;
				}
				m_tKeyManager.KeyCheck();
			}
			else
			{
				if(GO_SHOPTOTOWN == m_eGoScene && m_tKeyManager.KeyDown(KEY_C))
				{
				}
				if (m_tKeyManager.KeyDown(KEY_Q))
				{
					for (size_t i = 0; i < CGameObject_Manager::OBJ_END; ++i)
					{
						if (CGameObject_Manager::OBJ_PLAYER != i && CGameObject_Manager::OBJ_UI != i && CGameObject_Manager::OBJ_ITEM != i)
						{
							for (auto& pObject : CGameObject_Manager::Get_Instance()->Get_ListObject()[i])
							{
								pObject->Set_Dead();
							}
						}
					}

					if (GO_SHOP == m_eGoScene)
					{
						CScene_Manager::Get_Instance()->Scene_Change_SceneManager(CScene_Manager::SCENE_SHOP);
						return OBJ_NOEVENT;
					}
					else if (GO_SHOPTOTOWN == m_eGoScene)
					{
						CScene_Manager::Get_Instance()->Scene_Change_SceneManager(CScene_Manager::SCENE_TOWN);
						return OBJ_NOEVENT;
					}
					else if (GO_BOSSROOMA == m_eGoScene)
					{
						CScene_Manager::Get_Instance()->Scene_Change_SceneManager(CScene_Manager::SCENE_BOSSROOM1);
						return OBJ_NOEVENT;
					}
					else if (GO_BOSSROOMB == m_eGoScene)
					{
						CScene_Manager::Get_Instance()->Scene_Change_SceneManager(CScene_Manager::SCENE_BOSSROOM2);
						return OBJ_NOEVENT;
					}
				}
				m_tKeyManager.KeyCheck();
			}
		}
	}
	return OBJ_NOEVENT;
}

void CPortal::Late_Update_GameObject()
{
}

void CPortal::Render_GameObject()
{
	if (m_bCollision && GO_SHOPTOTOWN == m_eGoScene)
	{
		const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(L"GAME_UI", L"UI", 0);
		if (nullptr == pTexInfo)
			return;
		float iScrollX = CScroll_Manager::Get_Scroll(CScroll_Manager::SCROLLX);
		float iScrollY = CScroll_Manager::Get_Scroll(CScroll_Manager::SCROLLY);
		D3DXMATRIX matWorld, matTrans, matScale;
		D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
		D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + 95.f + iScrollX, m_tInfo.vPos.y + 55.f + iScrollY, 0.f);
		float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
		float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);
		matWorld = matScale * matTrans;
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
		CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

		pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(L"GAME_UI", L"UI", 1);
		if (nullptr == pTexInfo)
			return;
		D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
		D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + 95.f + iScrollX, m_tInfo.vPos.y + 20.f + iScrollY, 0.f);
		fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
		fCenterY = float(pTexInfo->tImageInfo.Height >> 1);
		matWorld = matScale * matTrans;
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
		CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	if (m_bCollision && GO_SHOP == m_eGoScene)
	{
		const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(L"GAME_UI", L"UI", 3);
		if (nullptr == pTexInfo)
			return;
		float iScrollX = CScroll_Manager::Get_Scroll(CScroll_Manager::SCROLLX);
		float iScrollY = CScroll_Manager::Get_Scroll(CScroll_Manager::SCROLLY);
		D3DXMATRIX matWorld, matTrans, matScale;
		D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
		D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + 55.f + iScrollX, m_tInfo.vPos.y + 5.f + iScrollY, 0.f);
		float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
		float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);
		matWorld = matScale * matTrans;
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
		CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	if (m_bCollision && OPEN_STAND == m_eGoScene)
	{
		const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(L"GAME_UI", L"UI", 2);
		if (nullptr == pTexInfo)
			return;
		float iScrollX = CScroll_Manager::Get_Scroll(CScroll_Manager::SCROLLX);
		float iScrollY = CScroll_Manager::Get_Scroll(CScroll_Manager::SCROLLY);
		D3DXMATRIX matWorld, matTrans, matScale;
		D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
		D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + 90.f + iScrollX, m_tInfo.vPos.y - 60.f + iScrollY, 0.f);
		float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
		float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);
		matWorld = matScale * matTrans;
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
		CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(m_iAlpha, 255, 255, 255));
	}
}

void CPortal::Release_GameObject()
{
}
