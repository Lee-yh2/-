#include "stdafx.h"
#include "MainMenu.h"
#include "Texture_Manager.h"
#include "GameObject_Manager.h"
#include "Time_Manager.h"
#include "Graphic_Device.h"
#include "KeyManager.h"
#include "Scene_Manager.h"
#include "SoundMgr.h"


CMainMenu::CMainMenu()
	:m_iAlpha(0), m_iCurrentSelMenu(0), m_bStart(false)
{
}


CMainMenu::~CMainMenu()
{
	Release_Scene();
}

HRESULT CMainMenu::Ready_Scene()
{
	CSoundMgr::Get_Instance()->PlayBGM(L"MAIN.wav");
	m_tFrame = { 0.f, 15.f };
	m_tLeftLogo = { WINCX / 4.f, WINCY / 2.f , 0.f };
	m_tRightLogo = { (WINCX / 4.f)*3.f + 10.f, WINCY / 2.f, 0.f };

	MENUBUTTON tButton;
	tButton.wstrName = L"New Game";
	tButton.vPos = { (WINCX / 2.f) - 38.f, (WINCY / 2.f)+50.f, 0.f };
	m_vecMenu.emplace_back(tButton);
	tButton.wstrName = L"Exit";
	tButton.vPos = { (WINCX / 2.f) -18.f, (WINCY / 2.f) + 80.f, 0.f };
	m_vecMenu.emplace_back(tButton);

	m_fMenuIcon[0] = 100.f;
	m_fMenuIcon[1] = 55.f;
	return S_OK;
}

void CMainMenu::Update_Scene()
{
	if (255 > m_iAlpha)
		++m_iAlpha;
	else
	{
		if (WINCX / 4.f - 70.f < m_tLeftLogo.x)
			m_tLeftLogo.x -= 10.f;
		else if(WINCX / 4.f - 330.f < m_tLeftLogo.x)
			m_tLeftLogo.x -= 2.f;
		if ((WINCX / 4.f)*3.f + 70.f > m_tRightLogo.x)
			m_tRightLogo.x += 10.f;
		else if((WINCX / 4.f)*3.f + 330.f > m_tRightLogo.x)
			m_tRightLogo.x += 2.f;

		if ((WINCX / 4.f)*3.f + 330.f <= m_tRightLogo.x)
		{
			m_bStart = true;
		}
	}
	CGameObject_Manager::Get_Instance()->Update_GameObject_Manager();
}

void CMainMenu::LateUpdate_Scene()
{
	Key_Set();
	FrameMove(1.f);
	CGameObject_Manager::Get_Instance()->Late_Update_GameObject_Manager();
}

void CMainMenu::Render_Scene()
{
	Draw_Back();
	CGameObject_Manager::Get_Instance()->Render_GameObject_Manager();
}

void CMainMenu::Release_Scene()
{
	CSoundMgr::Get_Instance()->StopAll();
	m_vecMenu.clear();
}

void CMainMenu::FrameMove(float fSpeed)
{
	m_tFrame.fFrameStart += m_tFrame.fFrameEnd *CTime_Manager::Get_Instance()->Get_DeltaTime() * fSpeed;
	if (m_tFrame.fFrameStart > m_tFrame.fFrameEnd)
	{
		m_tFrame.fFrameStart = 0.f;
	}
}

void CMainMenu::Draw_Back()
{
	const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(L"RESOURCE", L"LOGOSCREEN", int(m_tFrame.fFrameStart));
	if (nullptr == pTexInfo)
		return;

	D3DXMATRIX matWorld, matTrans, matScale;
	D3DXMatrixScaling(&matScale, 1.1f, 1.6f, 0.f);
	D3DXMatrixTranslation(&matTrans, float(WINCX >> 1), float(WINCY >> 1), 0.f);
	float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);
	matWorld = matScale * matTrans;
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(L"RESOURCE", L"LOGOSCREEN2", int(m_tFrame.fFrameStart));
	D3DXMatrixScaling(&matScale, 1.1f, 1.6f, 0.f);
	D3DXMatrixTranslation(&matTrans, float(WINCX >> 1), float(WINCY >> 1), 0.f);
	fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	fCenterY = float(pTexInfo->tImageInfo.Height >> 1);
	matWorld = matScale * matTrans;
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	for (int i = 0; i < (int)m_vecMenu.size(); ++i)
	{
		D3DXMatrixTranslation(&matTrans, m_vecMenu[i].vPos.x, m_vecMenu[i].vPos.y, 0.f);
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
		CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(), m_vecMenu[i].wstrName.c_str(), m_vecMenu[i].wstrName.length(), nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(L"RESOURCE", L"LOGO_ICON", 0);
	D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
	D3DXMatrixTranslation(&matTrans, m_vecMenu[m_iCurrentSelMenu].vPos.x - 14.f, m_vecMenu[m_iCurrentSelMenu].vPos.y + 10.f, 0.f);
	fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	fCenterY = float(pTexInfo->tImageInfo.Height >> 1);
	matWorld = matScale * matTrans;
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(L"RESOURCE", L"LOGO_ICON", 0);
	D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
	D3DXMatrixTranslation(&matTrans, m_vecMenu[m_iCurrentSelMenu].vPos.x + m_fMenuIcon[m_iCurrentSelMenu], m_vecMenu[m_iCurrentSelMenu].vPos.y + 10.f, 0.f);
	fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	fCenterY = float(pTexInfo->tImageInfo.Height >> 1);
	matWorld = matScale * matTrans;
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));


	pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(L"RESOURCE", L"LOGO", 1);
	D3DXMatrixScaling(&matScale, 1.4f, 1.6f, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tLeftLogo.x, m_tLeftLogo.y, 0.f);
	fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	fCenterY = float(pTexInfo->tImageInfo.Height >> 1);
	matWorld = matScale * matTrans;
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(L"RESOURCE", L"LOGO", 2);
	D3DXMatrixScaling(&matScale, 1.4f, 1.6f, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tRightLogo.x, m_tRightLogo.y, 0.f);
	fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	fCenterY = float(pTexInfo->tImageInfo.Height >> 1);
	matWorld = matScale * matTrans;
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(L"RESOURCE", L"LOGO", 0);
	D3DXMatrixScaling(&matScale, 1.3f, 1.3f, 0.f);
	D3DXMatrixTranslation(&matTrans, float(WINCX >> 1), float(WINCY >> 1) - 100, 0.f);
	fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	fCenterY = float(pTexInfo->tImageInfo.Height >> 1);
	matWorld = matScale * matTrans;
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(m_iAlpha, 255, 255, 255));
}

void CMainMenu::Key_Set()
{
	if (m_bStart)
	{
		CKeyManager Key;
		Key.KeyCheck();
		if (Key.KeyDown(KEY_UP))
		{
			if (0 < m_iCurrentSelMenu)
			{
				--m_iCurrentSelMenu;
			}
		}
		if (Key.KeyDown(KEY_DOWN))
		{
			if (1 > m_iCurrentSelMenu)
			{
				++m_iCurrentSelMenu;
			}
		}
		if (Key.KeyDown(KEY_ENTER))
		{
			if (0 == m_iCurrentSelMenu)
			{
				CSoundMgr::Get_Instance()->PlaySound(L"START.wav", CSoundMgr::EFFECT1);
				if (FAILED(CScene_Manager::Get_Instance()->Scene_Change_SceneManager(CScene_Manager::SCENE_LOADING)))
				{
					ERR_MSG(L"Creating Failed Scene_Change_SceneManager _ LOADING");
					return;
				}
			}
			else if (1 == m_iCurrentSelMenu)
			{
				return;
			}
		}
	}
}
