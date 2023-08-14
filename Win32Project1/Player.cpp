#include "stdafx.h"
#include "Player.h"
#include "Time_Manager.h"
#include "KeyManager.h"
#include "Texture_Manager.h"
#include "Graphic_Device.h"
#include "Scroll_Manager.h"
#include "Bullet.h"
#include "GameObject_Manager.h"
#include "UI.h"
#include "Inventory.h"
#include "Item.h"
#include "SoundMgr.h"

CPlayer::CPlayer()
	:m_eCurState(STATE::END), m_ePreState(STATE::END), m_fSpeed(0.f), m_bAttack(false), m_fWeaponSetX(0.f), m_fWeaponSetY(0.f)
	, m_bFirstAttack(true), m_bSecondAttack(true), m_bThirdAttack(true), m_bRoll(false), m_bBeAttack(false), m_bInven(false)
	, m_iCurrentEquipNum(0), m_iAlpha(0), m_bStart(false), m_bBossStart(false), m_bFirstSound(false), m_bSecondSound(false)
	,m_bThirdSound(false), m_bSetAtt(false), m_bShop(false)
{
	m_EquipWeapon[0] = nullptr;
	m_EquipWeapon[1] = nullptr;
}

CPlayer::~CPlayer()
{
}

HRESULT CPlayer::Ready_GameObject()
{
	m_eCurState = STATE::IDLE;
	m_eDirect = DIRECT::DOWN;
	m_strObjectKey = L"IDLE";
	m_strStateKey = L"DOWN";
	m_strWeaponObjectKey = L"W_BIG";

	m_tInfo.vPos = { 700.f, 680.f, 0.f };
	m_tInfo.vDir = { 0.f, 1.f, 0.f };
	m_tInfo.vSize = { 1.2f, 1.2f, 0.f };
	m_tInfo.vCSize = { 27.f, 40.f, 0.f };
	m_tUnitInfo.wstrName = L"";
	m_tUnitInfo.byCurrentItem = 0;
	m_tUnitInfo.byCurrentWeapon = BIGSWORD;
	m_tUnitInfo.iAtt = 0;
	m_tUnitInfo.iGold = 0;
	m_tUnitInfo.iHp = 250;
	m_tUnitInfo.iMaxHp = 250;

	m_fSpeed = 150.f;
	m_fSpeed *= CTime_Manager::Get_Instance()->Get_DeltaTime();

	m_tFrame = { 0.f, 9.f };

	CGameObject* pObj = new CInventory;
	static_cast<CInventory*>(pObj)->Set_OpenInven(&m_bInven);
	static_cast<CInventory*>(pObj)->Set_OpenShop(&m_bShop);
	//static_cast<CInventory*>(pObj)->Set_EquipWeapon(m_EquipWeapon);
	pObj->Ready_GameObject();
	CGameObject_Manager::Get_Instance()->Add_GameObject(CGameObject_Manager::OBJ_INVEN, pObj);
	
	pObj = new CUI;
	D3DXVECTOR3 vPos = { 800.f, 110.f, 0.f };
	static_cast<CUI*>(pObj)->Set_Type(CUI::TYPE::BAG);
	pObj->Set_Pos(vPos);
	pObj->Ready_GameObject();
	CGameObject_Manager::Get_Instance()->Add_GameObject(CGameObject_Manager::OBJ_UI, pObj);

	pObj = new CUI;
	vPos = { 800.f, 50.f, 0.f };
	static_cast<CUI*>(pObj)->Set_Type(CUI::TYPE::WEAPON);
	pObj->Set_Pos(vPos);
	pObj->Ready_GameObject();
	CGameObject_Manager::Get_Instance()->Add_GameObject(CGameObject_Manager::OBJ_UI, pObj);

	pObj = new CUI;
	vPos = { 50.f, 50.f, 0.f };
	static_cast<CUI*>(pObj)->Set_Type(CUI::TYPE::GOLD);
	pObj->Set_Pos(vPos);
	pObj->Ready_GameObject();
	CGameObject_Manager::Get_Instance()->Add_GameObject(CGameObject_Manager::OBJ_UI, pObj);

	pObj = new CUI;
	vPos = { 90.f, 40.f, 0.f };
	static_cast<CUI*>(pObj)->Set_Type(CUI::TYPE::HP_BAR);
	static_cast<CUI*>(pObj)->Set_Hp(&m_tUnitInfo.iHp);
	pObj->Set_Pos(vPos);
	pObj->Ready_GameObject();
	CGameObject_Manager::Get_Instance()->Add_GameObject(CGameObject_Manager::OBJ_UI, pObj);

	return S_OK;
}

int CPlayer::Update_GameObject()
{
	if (m_bDead)
		return OBJ_DEAD;
	if (m_bStart)
	{
		if (!m_bInven && !m_bShop)
		{
			Key_Check();
			if (!m_bAttack && m_eCurState != STATE::IDLE && m_eCurState != STATE::BEATTACK)
				m_tInfo.vPos += m_tInfo.vDir * m_fSpeed;
			else if (m_eCurState == STATE::BEATTACK)
			{
				D3DXVECTOR3 vDir;
				if (DIRECT::UP == m_eDirect)
					vDir = { 0.f, 1.f, 0.f };
				else if (DIRECT::DOWN == m_eDirect)
					vDir = { 0.f,-1.f, 0.f };
				else if (DIRECT::LEFT == m_eDirect)
					vDir = { 1.f,0.f, 0.f };
				else if (DIRECT::RIGHT == m_eDirect)
					vDir = { -1.f,0.f, 0.f };
				m_tInfo.vPos += vDir * (m_fSpeed / 5);
			}
			if (!m_bAttack)
			{
				m_bFirstAttack = true;
				m_bSecondAttack = true;
				m_bThirdAttack = true;

				m_bFirstSound = false;
				m_bSecondSound = false;
				m_bThirdSound = false;
			}
			Scene_Change();
			m_tKeyManager.KeyCheck();
		}
		else
		{
			if (static_cast<CItem*>(m_EquipWeapon[m_iCurrentEquipNum])->Get_Item() == CItem::WEAPON_BIG)
			{
				m_tUnitInfo.byCurrentWeapon = BIGSWORD;
				m_strWeaponObjectKey = L"W_BIG";
			}
			else if (static_cast<CItem*>(m_EquipWeapon[m_iCurrentEquipNum])->Get_Item() == CItem::WEAPON_SHORT)
			{
				m_tUnitInfo.byCurrentWeapon = SHORTSWORD;
				m_strWeaponObjectKey = L"W_SHORT";
			}
			else if (static_cast<CItem*>(m_EquipWeapon[m_iCurrentEquipNum])->Get_Item() == CItem::WEAPON_BOW)
			{
				m_tUnitInfo.byCurrentWeapon = BOW;
				m_strWeaponObjectKey = L"W_BOW";
			}
		}
	}
	return OBJ_NOEVENT;
}

void CPlayer::Late_Update_GameObject()
{
	Offset();
	if (m_bStart)
	{
		if (m_bAttack) {
			if (BIGSWORD == m_tUnitInfo.byCurrentWeapon)
				FrameMove(0.3f);
			else if (SHORTSWORD == m_tUnitInfo.byCurrentWeapon)
				FrameMove(0.8f);
			else if (BOW == m_tUnitInfo.byCurrentWeapon)
				FrameMove(0.6f);
		}
		else if (m_bRoll)
		{
			FrameMove(1.5f);
		}
		else if (BEATTACK == m_eCurState)
			FrameMove(1.f);
		else
			FrameMove(1.f);
	}
}

void CPlayer::Render_GameObject()
{
	if (m_bStart)
	{
		const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(m_strObjectKey, m_strStateKey, int(m_tFrame.fFrameStart));
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

		if (m_bAttack)
		{
			pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(m_strWeaponObjectKey, m_strStateKey, int(m_tFrame.fFrameStart));
			if (nullptr == pTexInfo)
				return;
			D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
			D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + iScrollX + m_fWeaponSetX, m_tInfo.vPos.y + iScrollY + m_fWeaponSetY, 0.f);
			fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
			fCenterY = float(pTexInfo->tImageInfo.Height >> 1);
			matWorld = matScale * matTrans;
			CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
			CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, m_iRGB, m_iRGB));

			Attack();
		}
	}
}

void CPlayer::Release_GameObject()
{
}

void CPlayer::FrameMove(float fMoveSpeed)
{
	m_tFrame.fFrameStart += m_tFrame.fFrameEnd *CTime_Manager::Get_Instance()->Get_DeltaTime() * fMoveSpeed;
	if (m_tFrame.fFrameStart > m_tFrame.fFrameEnd)
	{
		if (m_bAttack)
			m_bAttack = !m_bAttack;
		if (m_bRoll)
			m_bRoll = !m_bRoll;
		if (BEATTACK == m_eCurState)
		{
			m_bBeAttack = false;
			m_eCurState = IDLE;
			m_strObjectKey = L"IDLE";
		}
		m_tFrame.fFrameStart = 0.f;
	}
}

void CPlayer::Key_Check()
{
	if (!m_bRoll && !m_bBeAttack)
	{
		if (m_tKeyManager.KeyPressing(KEY_SPACE))
		{
			m_bAttack = false;
			m_bRoll = true;
			m_eCurState = STATE::ROLL;
			m_strObjectKey = L"ROLL";
			CSoundMgr::Get_Instance()->PlaySound(L"ROLL.wav", CSoundMgr::ROLL);
		}
		else if (m_tKeyManager.KeyPressing(KEY_UP))
		{
			m_bAttack = false;
			m_eCurState = STATE::WALK;
			m_eDirect = DIRECT::UP;
			m_strObjectKey = L"WALK";
			m_strStateKey = L"UP";
			if (m_tKeyManager.KeyPressing(KEY_LEFT))
			{
				m_tInfo.vDir = { -1.f, -1.f, 0.f };
				D3DXVec3Normalize(&m_tInfo.vDir, &m_tInfo.vDir);
			}
			else if (m_tKeyManager.KeyPressing(KEY_RIGHT))
			{
				m_tInfo.vDir = { 1.f, -1.f, 0.f };
				D3DXVec3Normalize(&m_tInfo.vDir, &m_tInfo.vDir);
			}
			else
			{
				m_tInfo.vDir = { 0.f, -1.f, 0.f };
			}
		}
		else if (m_tKeyManager.KeyPressing(KEY_DOWN))
		{
			m_bAttack = false;
			m_eCurState = STATE::WALK;
			m_eDirect = DIRECT::DOWN;
			m_strObjectKey = L"WALK";
			m_strStateKey = L"DOWN";
			if (m_tKeyManager.KeyPressing(KEY_LEFT))
			{
				m_tInfo.vDir = { -1.f, 1.f, 0.f };
				D3DXVec3Normalize(&m_tInfo.vDir, &m_tInfo.vDir);
			}
			else if (m_tKeyManager.KeyPressing(KEY_RIGHT))
			{
				m_tInfo.vDir = { 1.f, 1.f, 0.f };
				D3DXVec3Normalize(&m_tInfo.vDir, &m_tInfo.vDir);
			}
			else
			{
				m_tInfo.vDir = { 0.f, 1.f, 0.f };
			}
		}
		else if (m_tKeyManager.KeyPressing(KEY_LEFT))
		{
			m_bAttack = false;
			m_eCurState = STATE::WALK;
			m_eDirect = DIRECT::LEFT;
			m_strObjectKey = L"WALK";
			m_strStateKey = L"LEFT";
			if (m_tKeyManager.KeyPressing(KEY_UP))
			{
				m_tInfo.vDir = { -1.f, -1.f, 0.f };
				D3DXVec3Normalize(&m_tInfo.vDir, &m_tInfo.vDir);
			}
			else if (m_tKeyManager.KeyPressing(KEY_DOWN))
			{
				m_tInfo.vDir = { -1.f, 1.f, 0.f };
				D3DXVec3Normalize(&m_tInfo.vDir, &m_tInfo.vDir);
			}
			else
			{
				m_tInfo.vDir = { -1.f, 0.f, 0.f };
			}
		}
		else if (m_tKeyManager.KeyPressing(KEY_RIGHT))
		{
			m_bAttack = false;
			m_eCurState = STATE::WALK;
			m_eDirect = DIRECT::RIGHT;
			m_strObjectKey = L"WALK";
			m_strStateKey = L"RIGHT";
			if (m_tKeyManager.KeyPressing(KEY_UP))
			{
				m_tInfo.vDir = { 1.f, -1.f, 0.f };
				D3DXVec3Normalize(&m_tInfo.vDir, &m_tInfo.vDir);
			}
			else if (m_tKeyManager.KeyPressing(KEY_DOWN))
			{
				m_tInfo.vDir = { 1.f, 1.f, 0.f };
				D3DXVec3Normalize(&m_tInfo.vDir, &m_tInfo.vDir);
			}
			else
			{
				m_tInfo.vDir = { 1.f, 0.f, 0.f };
			}
		}
		else if (m_tKeyManager.KeyDown(KEY_A))
		{
			m_eCurState = STATE::ATTACK;
			m_bAttack = true;
			if (BIGSWORD == m_tUnitInfo.byCurrentWeapon)
			{
				m_strObjectKey = L"BIGSWORD";
			}
			else if (SHORTSWORD == m_tUnitInfo.byCurrentWeapon)
			{
				m_strObjectKey = L"SHORTSWORD";
			}
			else if (BOW == m_tUnitInfo.byCurrentWeapon)
			{
				m_strObjectKey = L"BOW";
			}
		}
		else
		{
			if (!m_bAttack)
			{
				m_eCurState = STATE::IDLE;
				m_strObjectKey = L"IDLE";
			}
		}
	}
	if (m_tKeyManager.KeyDown(KEY_R))
	{
		if (!m_bAttack) {
			if (0 == m_iCurrentEquipNum && nullptr != m_EquipWeapon[1])
				m_iCurrentEquipNum = 1;
			else if (1 == m_iCurrentEquipNum && nullptr != m_EquipWeapon[0])
				m_iCurrentEquipNum = 0;

			if (static_cast<CItem*>(m_EquipWeapon[m_iCurrentEquipNum])->Get_Item() == CItem::WEAPON_BIG)
			{
				m_tUnitInfo.byCurrentWeapon = BIGSWORD;
				m_strWeaponObjectKey = L"W_BIG";
			}
			else if (static_cast<CItem*>(m_EquipWeapon[m_iCurrentEquipNum])->Get_Item() == CItem::WEAPON_SHORT)
			{
				m_tUnitInfo.byCurrentWeapon = SHORTSWORD;
				m_strWeaponObjectKey = L"W_SHORT";
			}
			else if (static_cast<CItem*>(m_EquipWeapon[m_iCurrentEquipNum])->Get_Item() == CItem::WEAPON_BOW)
			{
				m_tUnitInfo.byCurrentWeapon = BOW;
				m_strWeaponObjectKey = L"W_BOW";
			}

			/*if (BIGSWORD == m_tUnitInfo.byCurrentWeapon)
			{
				m_tUnitInfo.byCurrentWeapon = SHORTSWORD;
				m_strWeaponObjectKey = L"W_SHORT";
			}
			else if (SHORTSWORD == m_tUnitInfo.byCurrentWeapon)
			{
				m_tUnitInfo.byCurrentWeapon = BOW;
				m_strWeaponObjectKey = L"W_BOW";
			}
			else if (BOW == m_tUnitInfo.byCurrentWeapon)
			{
				m_tUnitInfo.byCurrentWeapon = BIGSWORD;
				m_strWeaponObjectKey = L"W_BIG";
			}*/
		}
	}
	if (m_tKeyManager.KeyDown(KEY_O))
	{
		m_bSetAtt = !m_bSetAtt;
	}
}

void CPlayer::Scene_Change()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case STATE::IDLE:
			m_tFrame.fFrameStart = 0.f;
			m_tFrame.fFrameEnd = 9.f;
			break;
		case STATE::ROLL:
			m_tFrame.fFrameStart = 0.f;
			m_tFrame.fFrameEnd = 7.f;
			break;
		case STATE::WALK:
			m_tFrame.fFrameStart = 0.f;
			m_tFrame.fFrameEnd = 7.f;
			break;
		case STATE::BEATTACK:
			m_tFrame.fFrameStart = 0.f;
			m_tFrame.fFrameEnd = 8.f;
			break;
		case STATE::ATTACK:
			if (BIGSWORD == m_tUnitInfo.byCurrentWeapon)
			{
				switch (m_eDirect)
				{
				case DIRECT::DOWN:
					m_fWeaponSetX = 0.f;
					m_fWeaponSetY = -6.f;
					break;
				case DIRECT::UP:
					m_fWeaponSetX = 0.f;
					m_fWeaponSetY = 0.f;
					break;
				case DIRECT::RIGHT:
					m_fWeaponSetX = 0.f;
					m_fWeaponSetY = 0.f;
					break;
				case DIRECT::LEFT:
					m_fWeaponSetX = 0.f;
					m_fWeaponSetY = 0.f;
					break;
				default:
					break;
				}
				m_tFrame.fFrameStart = 0.f;
				m_tFrame.fFrameEnd = 39.f;
			}
			else if (SHORTSWORD == m_tUnitInfo.byCurrentWeapon)
			{
				switch (m_eDirect)
				{
				case DIRECT::DOWN:
					m_fWeaponSetX = 0.f;
					m_fWeaponSetY = -10.f;
					break;
				case DIRECT::UP:
					m_fWeaponSetX = 0.f;
					m_fWeaponSetY = 0.f;
					break;
				case DIRECT::RIGHT:
					m_fWeaponSetX = -8.f;
					m_fWeaponSetY = 0.f;
					break;
				case DIRECT::LEFT:
					m_fWeaponSetX = 8.f;
					m_fWeaponSetY = 0.f;
					break;
				default:
					break;
				}
				m_tFrame.fFrameStart = 0.f;
				m_tFrame.fFrameEnd = 17.f;
			}
			else if (BOW == m_tUnitInfo.byCurrentWeapon)
			{
				switch (m_eDirect)
				{
				case DIRECT::DOWN:
					m_fWeaponSetX = 0.f;
					m_fWeaponSetY = 13.f;
					break;
				case DIRECT::UP:
					m_fWeaponSetX = 0.f;
					m_fWeaponSetY = -17.f;
					break;
				case DIRECT::RIGHT:
					m_fWeaponSetX = 13.f;
					m_fWeaponSetY = 0.f;
					break;
				case DIRECT::LEFT:
					m_fWeaponSetX = -13.f;
					m_fWeaponSetY = 0.f;
					break;
				default:
					break;
				}
				m_tFrame.fFrameStart = 0.f;
				m_tFrame.fFrameEnd = 10.f;
			}
			break;
		}
		m_ePreState = m_eCurState;
	}
}

void CPlayer::Offset()
{
	float fSetOffSet = 70.f;
	float fOffSetX = WINCX / 2.f;
	float fOffSetY = WINCY / 2.f;
	float fScrollX = CScroll_Manager::Get_Scroll(CScroll_Manager::SCROLLX);
	float fScrollY = CScroll_Manager::Get_Scroll(CScroll_Manager::SCROLLY);

	if (m_bBossStart)
	{
		D3DXVECTOR3 vPos = CGameObject_Manager::Get_Instance()->Get_Monster()->Get_Info().vPos;
		if (fOffSetX + fSetOffSet < vPos.x + fScrollX)
			CScroll_Manager::Set_Scroll(CScroll_Manager::SCROLLX, fOffSetX + fSetOffSet - (vPos.x + fScrollX));
		if (fOffSetX - fSetOffSet > vPos.x + fScrollX)
			CScroll_Manager::Set_Scroll(CScroll_Manager::SCROLLX, fOffSetX - fSetOffSet - (vPos.x + fScrollX));
		if (fOffSetY + fSetOffSet < vPos.y + fScrollY)
			CScroll_Manager::Set_Scroll(CScroll_Manager::SCROLLY, fOffSetY + fSetOffSet - (vPos.y + fScrollY));
		if (fOffSetY - fSetOffSet > vPos.y + fScrollY)
			CScroll_Manager::Set_Scroll(CScroll_Manager::SCROLLY, fOffSetY - fSetOffSet - (vPos.y + fScrollY));
	}
	else
	{
		if (fOffSetX  < m_tInfo.vPos.x + fScrollX)
			CScroll_Manager::Set_Scroll(CScroll_Manager::SCROLLX, fOffSetX  - (m_tInfo.vPos.x + fScrollX));
		if (fOffSetX  > m_tInfo.vPos.x + fScrollX)
			CScroll_Manager::Set_Scroll(CScroll_Manager::SCROLLX, fOffSetX - (m_tInfo.vPos.x + fScrollX));
		if (fOffSetY  < m_tInfo.vPos.y + fScrollY)
			CScroll_Manager::Set_Scroll(CScroll_Manager::SCROLLY, fOffSetY  - (m_tInfo.vPos.y + fScrollY));
		if (fOffSetY  > m_tInfo.vPos.y + fScrollY)
			CScroll_Manager::Set_Scroll(CScroll_Manager::SCROLLY, fOffSetY  - (m_tInfo.vPos.y + fScrollY));
	}
}

void CPlayer::Create_Bullet(D3DXVECTOR3 vRange, D3DXVECTOR3 vDir, BYTE Weapon)
{
	m_bullet = new CBullet;
	m_bullet->Set_Pos(m_tInfo.vPos + (vRange));
	m_bullet->Set_Dir(vDir);
	static_cast<CBullet*>(m_bullet)->Set_StateKey(m_strStateKey);
	static_cast<CBullet*>(m_bullet)->Set_Weapon(Weapon);
	m_bullet->Ready_GameObject();
	if (m_bSetAtt)
		m_bullet->Set_Att(200);
	CGameObject_Manager::Get_Instance()->Add_GameObject(CGameObject_Manager::OBJ_BULLET, m_bullet);

	if (true == m_bFirstAttack && true == m_bSecondAttack && true == m_bThirdAttack)
	{
		m_bFirstAttack = false;
	}
	else if (false == m_bFirstAttack && true == m_bSecondAttack && true == m_bThirdAttack)
	{
		m_bSecondAttack = false;
	}
	else if (false == m_bFirstAttack && false == m_bSecondAttack && true == m_bThirdAttack)
	{
		m_bThirdAttack = false;
	}

	cout << "Create Bullet" << endl;
}

void CPlayer::Attack()
{
	if (BIGSWORD == m_tUnitInfo.byCurrentWeapon)
	{
		if ((5.f <= m_tFrame.fFrameStart && m_bFirstAttack)
			|| (15.5f <= m_tFrame.fFrameStart && m_bSecondAttack)
			|| (27.5f <= m_tFrame.fFrameStart && m_bThirdAttack))
		{
			m_vRange = 32.f * m_tInfo.vDir;
			m_vDir = { 0.f, 0.f, 0.f };
			Create_Bullet(m_vRange, m_vDir, BIGSWORD);
		}
		if (0.f <= m_tFrame.fFrameStart && !m_bFirstSound)
		{
			CSoundMgr::Get_Instance()->PlaySound(L"BIG_1.wav", CSoundMgr::EFFECT1);
			m_bFirstSound = true;
		}
		if (12.f <= m_tFrame.fFrameStart && !m_bSecondSound)
		{
			CSoundMgr::Get_Instance()->PlaySound(L"BIG_2.wav", CSoundMgr::EFFECT2);
			m_bSecondSound = true;
		}
		if (22.f <= m_tFrame.fFrameStart && !m_bThirdSound)
		{
			CSoundMgr::Get_Instance()->PlaySound(L"BIG_3.wav", CSoundMgr::EFFECT3);
			m_bThirdSound = true;
		}
	}
	else if (SHORTSWORD == m_tUnitInfo.byCurrentWeapon)
	{
		if ((1.f <= m_tFrame.fFrameStart && m_bFirstAttack)
			|| (4.f <= m_tFrame.fFrameStart && m_bSecondAttack)
			|| (8.f <= m_tFrame.fFrameStart && m_bThirdAttack))
		{
			m_vRange = 15.f * m_tInfo.vDir;
			m_vDir = { 0.f, 0.f, 0.f };
			Create_Bullet(m_vRange, m_vDir, SHORTSWORD);
		}
		if (0.f <= m_tFrame.fFrameStart && !m_bFirstSound)
		{
			CSoundMgr::Get_Instance()->PlaySound(L"SHORT_1.wav", CSoundMgr::EFFECT1);
			m_bFirstSound = true;
		}
		if (3.f <= m_tFrame.fFrameStart && !m_bSecondSound)
		{
			CSoundMgr::Get_Instance()->PlaySound(L"SHORT_2.wav", CSoundMgr::EFFECT2);
			m_bSecondSound = true;
		}
		if (7.f <= m_tFrame.fFrameStart && !m_bThirdSound)
		{
			CSoundMgr::Get_Instance()->PlaySound(L"SHORT_3.wav", CSoundMgr::EFFECT3);
			m_bThirdSound = true;
		}
	}
	else if (BOW == m_tUnitInfo.byCurrentWeapon)
	{
		if (5 <= m_tFrame.fFrameStart && m_bFirstAttack)
		{
			m_vRange = 15.f * m_tInfo.vDir;
			CSoundMgr::Get_Instance()->PlaySound(L"BOW.wav", CSoundMgr::EFFECT2);
			Create_Bullet(m_vRange, m_tInfo.vDir, BOW);
		}
		if (0.f <= m_tFrame.fFrameStart && !m_bFirstSound)
		{
			CSoundMgr::Get_Instance()->PlaySound(L"BOW_C.wav", CSoundMgr::EFFECT1);
			m_bFirstSound = true;
		}
	}
}

void CPlayer::Set_BeAttack()
{
	m_bAttack = false;
	m_eCurState = BEATTACK;
	m_strObjectKey = L"BEATTACK";
	m_bBeAttack = true;
	Scene_Change();
}
