#include "stdafx.h"
#include "Item.h"
#include "Texture_Manager.h"
#include "Scroll_Manager.h"
#include "Graphic_Device.h"
#include "Time_Manager.h"

CItem::CItem()
	:m_iKeyNum(0), m_eItem(ITEM_END), m_eState(GROUND), m_iAlpha(255), m_iNum(0), m_bWeapon(false)
{
}


CItem::~CItem()
{
}

HRESULT CItem::Ready_GameObject()
{


	switch (m_eItem)
	{
	case CItem::BOSSB_CRISTAL:
		m_tUnitInfo.wstrName = L"¼öÁ¤¼®";
		m_tUnitInfo.iGold = 90;
		m_strObjectKey = L"BOSS_A";
		m_iKeyNum = 0;
		break;
	case CItem::BOSSB_SEED:
		m_tUnitInfo.wstrName = L"¾¾¾Ñ";
		m_tUnitInfo.iGold = 100;
		m_strObjectKey = L"BOSS_A";
		m_iKeyNum = 7;
		break;
	case CItem::BOSSA_KINGCORE:
		m_tUnitInfo.wstrName = L"°ñ·½¿ÕÀÇ ÄÚ¾î";
		m_tUnitInfo.iGold = 560;
		m_strObjectKey = L"BOSS_A";
		m_iKeyNum = 1;
		m_iNum = 1;
		break;
	case CItem::BOSSA_GOLEMPAPER:
		m_tUnitInfo.wstrName = L"°ñ·½µµ¾È";
		m_tUnitInfo.iGold = 1200;
		m_strObjectKey = L"BOSS_A";
		m_iKeyNum = 2;
		m_iNum = 1;
		break;
	case CItem::BOSSA_GOLEMBOOK:
		m_tUnitInfo.wstrName = L"°ñ·½¿ª»ç";
		m_tUnitInfo.iGold = 2000;
		m_strObjectKey = L"BOSS_A";
		m_iKeyNum = 3;
		m_iNum = 1;
		break;
	case CItem::BOSSA_IRON:
		m_tUnitInfo.wstrName = L"°­Ã¶";
		m_tUnitInfo.iGold = 130;
		m_strObjectKey = L"BOSS_A";
		m_iKeyNum = 6;
		break;
	case CItem::WEAPON_BIG:
		m_tUnitInfo.wstrName = L"´ë°Ë";
		m_tUnitInfo.iGold = 10;
		m_strObjectKey = L"GAME_UI";
		m_bWeapon = true;
		m_iKeyNum = 2;
		m_iNum = 1;
		break;
	case CItem::WEAPON_SHORT:
		m_tUnitInfo.wstrName = L"´Ü°Ë";
		m_tUnitInfo.iGold = 10;
		m_strObjectKey = L"GAME_UI";
		m_bWeapon = true;
		m_iKeyNum = 1;
		m_iNum = 1;
		break;
	case CItem::WEAPON_BOW:
		m_tUnitInfo.wstrName = L"È°";
		m_tUnitInfo.iGold = 10;
		m_strObjectKey = L"GAME_UI";
		m_bWeapon = true;
		m_iKeyNum = 5;
		m_iNum = 1;
		break;
	default:
		break;
	}

		m_tInfo.vCSize = { 10.f, 10.f, 0.f };
		m_tInfo.vSize = { 1.5f, 1.5f, 0.f };
		m_strStateKey = L"ITEM";
	if (GROUND == m_eState)
	{
		/*m_tInfo.vDir = { (float)(rand() % 2), (float)(rand() % 2), 0.f };
		D3DXVec3Normalize(&m_tInfo.vDir, &m_tInfo.vDir);*/

		m_tParabola.vStart = m_tInfo.vPos;
		m_tParabola.vDestination = { m_tInfo.vPos.x - 100.f + float(rand() % 200), m_tInfo.vPos.y + 200.f, 0.f };
		m_tParabola.fTime = 0.f;
		m_tParabola.fMaxY = m_tInfo.vPos.y -25.f;
		m_tParabola.fMaxYTime = 1.3f;

		m_tParabola.fDHeight = m_tParabola.vDestination.y - m_tInfo.vPos.y;
		m_tParabola.fHeight = m_tParabola.fMaxY - m_tInfo.vPos.y;

		/*m_tParabola.fDHeight = m_tInfo.vPos.y - m_tParabola.vDestination.y;
		m_tParabola.fHeight = m_tParabola.fMaxY - m_tParabola.vDestination.y;*/

		m_tParabola.fGravity = (2 * m_tParabola.fHeight) / (m_tParabola.fMaxYTime * m_tParabola.fMaxYTime);

		m_tParabola.fSpeedY = sqrtf(2 * m_tParabola.fGravity*m_tParabola.fHeight);
		float a = m_tParabola.fGravity;
		float b = -2 * m_tParabola.fSpeedY;
		float c = 2 * m_tParabola.fDHeight;

		m_tParabola.fDTime = (-b + sqrtf(b*b - 4 * a*c)) / (2 * a);
		m_tParabola.fSpeedX = -(m_tInfo.vPos.x - m_tParabola.vDestination.x) / m_tParabola.fDTime;
		//m_tParabola.fSpeedX = -(m_tParabola.vDestination.x - m_tInfo.vPos.x) / m_tParabola.fDTime;
	}
	return S_OK;
}

int CItem::Update_GameObject()
{
	if (m_bDead)
		return OBJ_DEAD;

	if (GROUND == m_eState)
	{
		m_tInfo.vPos = Parabola();
	}
	return OBJ_NOEVENT;
}

void CItem::Late_Update_GameObject()
{
}

void CItem::Render_GameObject()
{
	const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(m_strObjectKey, m_strStateKey, m_iKeyNum);
	if (nullptr == pTexInfo)
		return;
	float iScrollX = CScroll_Manager::Get_Scroll(CScroll_Manager::SCROLLX);
	float iScrollY = CScroll_Manager::Get_Scroll(CScroll_Manager::SCROLLY);
	D3DXMATRIX matWorld, matTrans, matScale;
	D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
	if (GROUND == m_eState)
		D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + iScrollX, m_tInfo.vPos.y + iScrollY, 0.f);
	else
		D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);
	float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);
	matWorld = matScale * matTrans;
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(m_iAlpha, 255, 255, 255));

	if ((INVEN == m_eState && 1 < m_iNum) || (SHOPSTAND == m_eState && 1 < m_iNum))
	{
		TCHAR strNum[MAX_PATH];
		//wstring strNum = L""+m_iNum;
		wsprintf(strNum, TEXT("%d"), m_iNum);
		D3DXMatrixScaling(&matScale, 0.5f, 0.5f, 0.f);
		D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + 7.f, m_tInfo.vPos.y + 7.f, 0.f);
		matWorld = matScale * matTrans;
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
		CGraphic_Device::Get_Instance()->Get_Font()->DrawTextW(CGraphic_Device::Get_Instance()->Get_Sprite(), strNum, lstrlen(strNum) , nullptr, 0, D3DCOLOR_ARGB(m_iAlpha, 0, 0, 0));
	}
}

void CItem::Release_GameObject()
{
}

void CItem::Set_Alpha()
{
	if (255 == m_iAlpha)
		m_iAlpha = 0;
	else
		m_iAlpha = 255;
}

D3DXVECTOR3 CItem::Parabola()
{
	m_tParabola.fTime += CTime_Manager::Get_Instance()->Get_DeltaTime();

	if (m_tParabola.fTime >= m_tParabola.fMaxYTime)
		m_tParabola.fTime = m_tParabola.fMaxYTime;

	float x = m_tParabola.vStart.x + m_tParabola.fSpeedX *m_tParabola.fTime;
	float y = m_tParabola.vStart.y + m_tParabola.fSpeedY * m_tParabola.fTime - 0.5f*m_tParabola.fGravity * m_tParabola.fTime*m_tParabola.fTime;

	/*float x = m_tParabola.vDestination.x + m_tParabola.fSpeedX *m_tParabola.fTime;
	float y = m_tParabola.vDestination.y + m_tParabola.fSpeedY * m_tParabola.fTime - 0.5f*m_tParabola.fGravity * m_tParabola.fTime*m_tParabola.fTime;*/
	D3DXVECTOR3 vPos = { x,y,0.f };
	return vPos;
}
