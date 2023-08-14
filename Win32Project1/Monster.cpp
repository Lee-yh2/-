#include "stdafx.h"
#include "Monster.h"
#include "Graphic_Device.h"
#include "Texture_Manager.h"
#include "Scroll_Manager.h"
#include "Time_Manager.h"
#include "GameObject_Manager.h"
#include "BossA_Arm.h"
#include "Stone.h"
#include "Wave.h"
#include "UI.h"
#include "MBullet.h"
#include "Item.h"
#include "Interior.h"
#include "Portal.h"
#include "Player.h"
#include "SoundMgr.h"


CMonster::CMonster()
	:m_byCurrentMonster(MONSTER_BOSS_A), m_eCurState(STATE_WAKEUP), m_ePreState(STATE_END), m_bStart(false)
	, m_dwAttack(GetTickCount()), m_bLaunch(false), m_bAttack(false), m_dwRecoveryTime(GetTickCount()), m_bStone(false)
	, m_bWave(false), m_iAttackTurn(0), m_bItem(false) , m_bSetDead(false), m_bDeadSound(false)
{
}


CMonster::~CMonster()
{
}

HRESULT CMonster::Ready_GameObject()
{
	m_tInfo.vPos = { 610.f, 340.f, 0.f };
	m_tInfo.vSize = { 1.5f, 1.5f, 0.f };
	m_tInfo.vDir = { 0.f, 0.f, 0.f };
	m_tInfo.vCSize = { 200.f, 130.f, 0.f };

	cout << m_tInfo.vPos.y << endl;

	m_tUnitInfo.iAtt = 30;
	m_tUnitInfo.iGold = 0;
	m_tUnitInfo.iHp = 504;
	m_tUnitInfo.wstrName = L"BOSS_A";

	m_tFrame = { 0.f, 31.f };

	m_strStateKey = L"WAKEUP";

	CGameObject* pObj = new CUI;
	D3DXVECTOR3 vPos = { 425.f, 450.f, 0.f };
	static_cast<CUI*>(pObj)->Set_Type(CUI::TYPE::MONSTER_HP);
	static_cast<CUI*>(pObj)->Set_Hp(&m_tUnitInfo.iHp);
	pObj->Set_Pos(vPos);
	pObj->Ready_GameObject();
	CGameObject_Manager::Get_Instance()->Add_GameObject(CGameObject_Manager::OBJ_UI, pObj);
	return S_OK;
}

int CMonster::Update_GameObject()
{ 
	/*if (m_bSendDead && m_dwDeadTime + 3000 < GetTickCount())
	{
		return OBJ_DEAD;
	}*/
	/*if (m_bDead)
		return OBJ_DEAD;*/

	if (m_tUnitInfo.iHp <= 0)
		m_bSetDead = true;

	if (m_bSetDead)
	{
		m_bAttack = false;
		m_bLaunch = false;
		m_eCurState = STATE_DEAD;
		m_strStateKey = L"DEATH";
		if (!m_bDeadSound)
		{
			CSoundMgr::Get_Instance()->PlaySound(L"BOSS_A_DEAD.wav", CSoundMgr::MONSTERDEAD);
			m_bDeadSound = true;
		}
		Scene_Change();
	}


	OnHit();
	if (m_bStart)
	{
		if (m_bAttack)
			Boss_Attack();

		if (STATE_WAKEUP == m_eCurState)
			FrameMove(0.3f);
		else if (STATE_PUNCH == m_eCurState || STATE_SPAWN == m_eCurState)
			FrameMove(0.4f);
		else if (STATE_DEAD == m_eCurState)
			FrameMove(0.1f);
		else
			FrameMove(0.6f);

		if (STATE_SPAWN == m_eCurState && m_tFrame.fFrameStart > 18.f && m_bStone)
		{
			D3DXVECTOR3 vRange = { 8.f, 110.f, 0.f };
			D3DXVECTOR3 vDir = { 0.f, 0.f, 0.f };
			Create_Bullet(vRange, vDir, BOSSA_STAMP);
			Create_Stone();
		}
		if (m_bWave)
			Create_Wave();
		Scene_Change();
	} 
	return OBJ_NOEVENT;
}

void CMonster::FrameMove(float fMoveSpeed)
{
	m_tFrame.fFrameStart += m_tFrame.fFrameEnd *CTime_Manager::Get_Instance()->Get_DeltaTime() * fMoveSpeed;
	if (m_tFrame.fFrameStart > m_tFrame.fFrameEnd)
	{
		if (STATE_WAKEUP == m_eCurState)
		{
			m_eCurState = STATE_IDLE;
			m_strStateKey = L"IDLE";
			m_dwAttack = GetTickCount();
			static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Set_BossStart();
			m_bAttack = true;
		}
		else if (STATE_PUNCH == m_eCurState)
		{
			CGameObject* pObj = new CBossA_Arm;
			static_cast<CBossA_Arm*>(pObj)->Set_Target(CGameObject_Manager::Get_Instance()->Get_Player()->Get_Info());
			pObj->Ready_GameObject();
			CGameObject_Manager::Get_Instance()->Add_GameObject(CGameObject_Manager::OBJ_EFFECT, pObj);

			m_dwRecoveryTime = GetTickCount();
			m_eCurState = STATE_NOARMIDLE;
			m_strStateKey = L"NOARM_IDLE";
		}
		else if (STATE_PUNCHRECOVER == m_eCurState)
		{
			m_eCurState = STATE_IDLE;
			m_strStateKey = L"IDLE";
			m_dwAttack = GetTickCount();
			m_bLaunch = false;
		}
		else if (STATE_SPAWN == m_eCurState)
		{
			m_eCurState = STATE_IDLE;
			m_strStateKey = L"IDLE";
			m_dwAttack = GetTickCount();
			m_bLaunch = false;
		}

		if (STATE_DEAD == m_eCurState && m_bStart)
		{
			m_tFrame.fFrameStart = m_tFrame.fFrameEnd;
			if (!m_bItem)
			{
				CGameObject* pObj = new CItem;
				pObj->Set_Pos(m_tInfo.vPos);
				static_cast<CItem*>(pObj)->Set_Item(CItem::BOSSA_GOLEMBOOK);
				static_cast<CItem*>(pObj)->Set_State(CItem::GROUND);
				pObj->Ready_GameObject();
				CGameObject_Manager::Get_Instance()->Add_GameObject(CGameObject_Manager::OBJ_ITEM, pObj);

				pObj = new CItem;
				pObj->Set_Pos(m_tInfo.vPos);
				static_cast<CItem*>(pObj)->Set_Item(CItem::BOSSA_KINGCORE);
				static_cast<CItem*>(pObj)->Set_State(CItem::GROUND);
				pObj->Ready_GameObject();
				CGameObject_Manager::Get_Instance()->Add_GameObject(CGameObject_Manager::OBJ_ITEM, pObj);

				pObj = new CItem;
				pObj->Set_Pos(m_tInfo.vPos);
				static_cast<CItem*>(pObj)->Set_Item(CItem::BOSSA_IRON);
				static_cast<CItem*>(pObj)->Set_State(CItem::GROUND);
				static_cast<CItem*>(pObj)->Set_Num(rand() % 7 + 1);
				pObj->Ready_GameObject();
				CGameObject_Manager::Get_Instance()->Add_GameObject(CGameObject_Manager::OBJ_ITEM, pObj);

				pObj = new CItem;
				pObj->Set_Pos(m_tInfo.vPos);
				static_cast<CItem*>(pObj)->Set_Item(CItem::BOSSA_IRON);
				static_cast<CItem*>(pObj)->Set_State(CItem::GROUND);
				static_cast<CItem*>(pObj)->Set_Num(rand() % 7 + 1);
				pObj->Ready_GameObject();
				CGameObject_Manager::Get_Instance()->Add_GameObject(CGameObject_Manager::OBJ_ITEM, pObj);

				pObj = new CInterior;
				static_cast<CInterior*>(pObj)->Set_Furniture(CInterior::FURNITURE::PORTALA);
				D3DXVECTOR3 vPos = { 580.f, 50.f, 0.f };
				pObj->Set_Pos(vPos);
				pObj->Ready_GameObject();
				CGameObject_Manager::Get_Instance()->Add_GameObject(CGameObject_Manager::OBJ_INTERIOR, pObj);

				pObj = new CPortal;
				static_cast<CPortal*>(pObj)->Set_GoScene(CPortal::GO_BOSSENTER);
				vPos = { 580.f, 60.f, 0.f };
				pObj->Set_Pos(vPos);
				pObj->Ready_GameObject();
				CGameObject_Manager::Get_Instance()->Add_GameObject(CGameObject_Manager::OBJ_PORTAL, pObj);

				/*m_bSendDead = true;
				m_dwDeadTime = GetTickCount();*/
				m_bItem = true;
			}

			m_bStart = false;
		}
		else
			m_tFrame.fFrameStart = 0.f;
	}
}

void CMonster::Boss_Attack()
{
	if (!m_bLaunch)
	{
		if (m_dwAttack + 5000 < GetTickCount())
		{
			/*int Pattern = rand() % 4;*/
			switch (m_iAttackTurn)
			{
			case PUNCH:
				m_eCurState = STATE_PUNCH;
				m_strStateKey = L"LAUNCH";
				m_bLaunch = true;
				++m_iAttackTurn;
				break;
			case WAVE:
				m_bWave = true;
				m_bLaunch = true;
				m_iAttackTurn = 0;
				break;
			case ROCK:
				m_eCurState = STATE_SPAWN;
				m_strStateKey = L"SPAWN";
				m_bStone = true;
				m_bLaunch = true;
				++m_iAttackTurn;
				break;
			default:
				break;
			}
		}
	}

	if (STATE_NOARMIDLE == m_eCurState && m_dwRecoveryTime + 24000 < GetTickCount())
	{
		m_eCurState = STATE_PUNCHRECOVER;
		m_strStateKey = L"RECOVER";
	}
}


void CMonster::Late_Update_GameObject()
{
}

void CMonster::Render_GameObject()
{
	if (L"BOSS_A" == m_tUnitInfo.wstrName)
	{
		const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(m_tUnitInfo.wstrName, L"CRACK", 0);
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

		pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(m_tUnitInfo.wstrName, m_strStateKey, int(m_tFrame.fFrameStart));
		if (nullptr == pTexInfo)
			return;
		iScrollX = CScroll_Manager::Get_Scroll(CScroll_Manager::SCROLLX);
		iScrollY = CScroll_Manager::Get_Scroll(CScroll_Manager::SCROLLY);
		D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
		D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + iScrollX, m_tInfo.vPos.y + iScrollY, 0.f);
		fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
		fCenterY = float(pTexInfo->tImageInfo.Height >> 1);
		matWorld = matScale * matTrans;
		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
		CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, m_iRGB, m_iRGB));

		cout << m_tInfo.vPos.y << endl;
	}
}

void CMonster::Release_GameObject()
{
}

void CMonster::Scene_Change()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case STATE::STATE_WAKEUP:
			m_tFrame.fFrameStart = 0.f;
			m_tFrame.fFrameEnd = 31.f;
			break;
		case STATE::STATE_IDLE:
			m_tFrame.fFrameStart = 0.f;
			m_tFrame.fFrameEnd = 15.f;
			break;
		case STATE::STATE_NOARMIDLE:
			m_tFrame.fFrameStart = 0.f;
			m_tFrame.fFrameEnd = 15.f;
			break;
		case STATE::STATE_SPAWN:
			m_tFrame.fFrameStart = 0.f;
			m_tFrame.fFrameEnd = 42.f;
			break;
		case STATE::STATE_PUNCH:
			m_tFrame.fFrameStart = 0.f;
			m_tFrame.fFrameEnd = 38.f;
			break;
		case STATE::STATE_PUNCHRECOVER:
			m_tFrame.fFrameStart = 0.f;
			m_tFrame.fFrameEnd = 23.f;
			break;
		case STATE::STATE_DEAD:
			m_tFrame.fFrameStart = 0.f;
			m_tFrame.fFrameEnd = 81.f;
			break;
		}
		m_ePreState = m_eCurState;
	}
}

void CMonster::Create_Stone()
{
	CGameObject* pObj;
	int iRock = rand() % 3;
	wstring strRock;
	if(0 == iRock)
		strRock = L"../Data/Boss1_Rock0.dat";
	else if(1 == iRock)
		strRock = L"../Data/Boss1_Rock1.dat";
	else if(2 == iRock)
		strRock = L"../Data/Boss1_Rock2.dat";
	HANDLE hFile = CreateFile(strRock.c_str(), GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	DWORD dwByte = 0;
	D3DXVECTOR3 Rock_Pos;
	while (true)
	{
		
		ReadFile(hFile, Rock_Pos, sizeof(D3DXVECTOR3), &dwByte, nullptr);

		if (0 == dwByte)
		{
			break;
		}
		pObj = new CStone;
		pObj->Set_Pos(Rock_Pos);
		pObj->Ready_GameObject();
		CGameObject_Manager::Get_Instance()->Add_GameObject(CGameObject_Manager::OBJ_MONSTER, pObj);
	}
	CloseHandle(hFile);
	
	m_bStone = false;
}

void CMonster::Create_Wave()
{
	CGameObject* pObj = new CWave;
	pObj->Set_Pos(m_tInfo.vPos);
	pObj->Ready_GameObject();
	CGameObject_Manager::Get_Instance()->Add_GameObject(CGameObject_Manager::OBJ_MONSTER, pObj);
	m_bWave = false;
	m_dwAttack = GetTickCount();
	m_bLaunch = false;
}

void CMonster::Create_Bullet(D3DXVECTOR3 vRange, D3DXVECTOR3 vDir, BYTE Monster)
{
	CMBullet* bullet = new CMBullet;
	bullet->Set_Pos(m_tInfo.vPos + (vRange));
	bullet->Set_Dir(vDir);
	static_cast<CMBullet*>(bullet)->Set_Weapon(Monster);
	bullet->Ready_GameObject();
	CGameObject_Manager::Get_Instance()->Add_GameObject(CGameObject_Manager::OBJ_MBULLET, bullet);
}
