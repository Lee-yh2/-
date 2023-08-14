#include "stdafx.h"
#include "MonsterB.h"
#include "Graphic_Device.h"
#include "Texture_Manager.h"
#include "Scroll_Manager.h"
#include "SoundMgr.h"
#include "Time_Manager.h"
#include "GameObject_Manager.h"
#include "UI.h"
#include "MBullet.h"
#include "Plant.h"
#include "Root.h"
#include "Item.h"
#include "Player.h"


CMonsterB::CMonsterB()
	:m_eCurState(STATE_END), m_ePreState(STATE_END), m_bStart(false), m_bWakeUp(false)
	, m_dwAttack(GetTickCount()), m_bAttack(false), m_dwJump(GetTickCount()), m_bJump(false)
	, m_bFirstPot(false), m_bSecondPot(false), m_bThirdPot(false), m_fAngle(0.f), m_fPosinDis(0.f), m_dwShot(GetTickCount())
	, m_bFirstRoot(false), m_bSecondRoot(false), m_bThirdRoot(false), m_iAttackTurn(0), m_bItem(false), m_bJumpSound(false)
	, m_bPotsSound(false), m_bDeadSound(false)
{
}


CMonsterB::~CMonsterB()
{
}

HRESULT CMonsterB::Ready_GameObject()
{
	m_tInfo.vPos = { 650.f, 350.f, 0.f };
	m_tInfo.vSize = { 1.3f, 1.3f, 0.f };
	m_tInfo.vDir = { 0.f, 0.f, 0.f };
	m_tInfo.vCSize = { 250.f, 150.f, 0.f };

	m_tUnitInfo.iAtt = 30;
	m_tUnitInfo.iGold = 0;
	m_tUnitInfo.iHp = 504;
	m_tUnitInfo.wstrName = L"BOSS_B";

	m_eCurState = STATE_WAKEUP;
	m_tFrame = { 0.f, 168.f };

	m_strHeadObjectKey = L"BOSS_B_HEAD";
	m_strBodyObjectKey = L"BOSS_B_BODY";

	m_fPosinDis = 70.f;
	m_fSpeed = 3.f;
	m_fSpeed *= CTime_Manager::Get_Instance()->Get_DeltaTime();

	CGameObject* pObj = new CUI;
	D3DXVECTOR3 vPos = { 425.f, 450.f, 0.f };
	static_cast<CUI*>(pObj)->Set_Type(CUI::TYPE::MONSTER_HP);
	static_cast<CUI*>(pObj)->Set_Hp(&m_tUnitInfo.iHp);
	pObj->Set_Pos(vPos);
	pObj->Ready_GameObject();
	CGameObject_Manager::Get_Instance()->Add_GameObject(CGameObject_Manager::OBJ_UI, pObj);

	cout << m_tInfo.vPos.y << endl;
	return S_OK;
}

int CMonsterB::Update_GameObject()
{
	if (m_tUnitInfo.iHp <= 0)
		Set_Dead();

	if (m_bDead)
	{
		m_bAttack = false;
		m_eCurState = STATE_DEAD;
		m_strHeadStateKey = L"DEATH";
		m_strBodyStateKey = L"DEATH";
		if (!m_bDeadSound)
		{
			CSoundMgr::Get_Instance()->PlaySound(L"BOSS_B_DEAD.wav", CSoundMgr::MONSTERDEAD);
			m_bDeadSound = true;
		}

		Scene_Change();
	}

	if (m_bStart)
	{
		if (m_eCurState == STATE_WAKEUP)
			FrameMove(0.05f, 0.05f);
		else if (m_eCurState == STATE_POTS || m_eCurState == STATE_SHOT || m_eCurState == STATE_READYSHOT || m_eCurState == STATE_ENDSHOT)
			FrameMove(0.2f, 0.9f);
		else if (m_eCurState == STATE_JUMP)
			FrameMove(0.4f, 0.4f);
		else if (m_eCurState == STATE_ROOT)
			FrameMove(0.9f, 0.2f);
		else if (m_eCurState == STATE_DEAD)
			FrameMove(0.1f, 0.1f);
		else
			FrameMove(0.9f, 0.9f);

		if (m_bWakeUp && !m_bDead)
		{
			Boss_Attack();

			if (m_eCurState != STATE_ROOT && m_eCurState != STATE_JUMP)
				Move();

			if (m_eCurState == STATE_JUMP && m_dwJump + 1000 < GetTickCount() && m_dwJump + 1500 > GetTickCount())
			{
				if (!m_bJumpSound)
				{
					CSoundMgr::Get_Instance()->PlaySound(L"BOSS_B_JUMP.wav", CSoundMgr::MONSTER2);
					m_bJumpSound = true;
				}
				m_tInfo.vCSize = { 0.f, 0.f, 0.f };
				m_tInfo.vPos += m_tInfo.vDir * (m_fSpeed * 20.f);
			}
			else if (m_eCurState == STATE_JUMP && m_dwJump + 1500 < GetTickCount() && m_bJump)
			{
				D3DXVECTOR3 vDir = { 0.f, 0.f, 0.f };
				CreateBullet(m_tInfo.vPos, vDir, BOSSB_JUMP);
				CScroll_Manager::Scroll_Quake();
				m_tInfo.vCSize = { 250.f, 150.f, 0.f };
				m_bJumpSound = false;
				m_bJump = false;
			}
			Root();
			Shot();
			Pots();
		}
	}
	OnHit();
	Scene_Change();
	return OBJ_NOEVENT;
}

void CMonsterB::Late_Update_GameObject()
{
	for (int i = 0; i < 4; ++i)
	{
		m_vecPosin[i].x = (m_tInfo.vPos.x + cosf((m_fAngle + (i * 90.f)) * PI / 180.f) * m_fPosinDis);
		m_vecPosin[i].y = (m_tInfo.vPos.y - sinf((m_fAngle + (i * 90.f)) * PI / 180.f) * m_fPosinDis);
	}
}

void CMonsterB::Render_GameObject()
{
	if (m_bWakeUp)
	{
		const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(m_strBodyObjectKey, m_strBodyStateKey, int(m_tBodyFrame.fFrameStart));
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

		pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(m_strHeadObjectKey, m_strHeadStateKey, int(m_tFrame.fFrameStart));
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
	}
	else
	{
		const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(L"BOSS_B", L"WAKEUP", int(m_tFrame.fFrameStart));
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
	cout << m_tInfo.vPos.y << endl;
}

void CMonsterB::Release_GameObject()
{
}

void CMonsterB::Scene_Change()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case STATE::STATE_WAKEUP:
			m_tFrame.fFrameStart = 0.f;
			m_tFrame.fFrameEnd = 168.f;
			break;
		case STATE::STATE_IDLE:
			CSoundMgr::Get_Instance()->PlaySound(L"BOSS_B_IDLE.wav", CSoundMgr::MONSTERIDLE);
			m_tFrame.fFrameStart = 0.f;
			m_tFrame.fFrameEnd = 7.f;
			m_tBodyFrame.fFrameStart = 0.f;
			m_tBodyFrame.fFrameEnd = 7.f;
			break;
		case STATE::STATE_POTS:
			m_tFrame.fFrameStart = 0.f;
			m_tFrame.fFrameEnd = 39.f;
			m_tBodyFrame.fFrameStart = 0.f;
			m_tBodyFrame.fFrameEnd = 7.f;
			break;
		case STATE::STATE_READYSHOT:
			m_tFrame.fFrameStart = 0.f;
			m_tFrame.fFrameEnd = 27.f;
			m_tBodyFrame.fFrameStart = 0.f;
			m_tBodyFrame.fFrameEnd = 7.f;
			break;
		case STATE::STATE_SHOT:
			m_tFrame.fFrameStart = 0.f;
			m_tFrame.fFrameEnd = 15.f;
			m_tBodyFrame.fFrameStart = 0.f;
			m_tBodyFrame.fFrameEnd = 7.f;
			break;
		case STATE::STATE_ENDSHOT:
			m_tFrame.fFrameStart = 0.f;
			m_tFrame.fFrameEnd = 11.f;
			m_tBodyFrame.fFrameStart = 0.f;
			m_tBodyFrame.fFrameEnd = 7.f;
			break;
		case STATE::STATE_JUMP:
			m_tFrame.fFrameStart = 0.f;
			m_tFrame.fFrameEnd = 29.f;
			m_tBodyFrame.fFrameStart = 0.f;
			m_tBodyFrame.fFrameEnd = 29.f;
			break;
		case STATE::STATE_ROOT:
			m_tFrame.fFrameStart = 0.f;
			m_tFrame.fFrameEnd = 7.f;
			m_tBodyFrame.fFrameStart = 0.f;
			m_tBodyFrame.fFrameEnd = 47.f;
			break;
		case STATE::STATE_DEAD:
			m_tFrame.fFrameStart = 0.f;
			m_tFrame.fFrameEnd = 90.f;
			m_tBodyFrame.fFrameStart = 0.f;
			m_tBodyFrame.fFrameEnd = 61.f;
			break;
		}
		m_ePreState = m_eCurState;
	}
}

void CMonsterB::FrameMove(float fMoveSpeed, float fMoveBodySpeed)
{
	m_tFrame.fFrameStart += m_tFrame.fFrameEnd *CTime_Manager::Get_Instance()->Get_DeltaTime() * fMoveSpeed;
	m_tBodyFrame.fFrameStart += m_tBodyFrame.fFrameEnd *CTime_Manager::Get_Instance()->Get_DeltaTime() * fMoveBodySpeed;
	if (m_tFrame.fFrameStart > m_tFrame.fFrameEnd)
	{
		if (m_eCurState == STATE_WAKEUP)
		{
			m_eCurState = STATE_IDLE;
			m_strHeadStateKey = L"IDLE";
			m_strBodyStateKey = L"IDLE";
			m_dwAttack = GetTickCount();
			static_cast<CPlayer*>(CGameObject_Manager::Get_Instance()->Get_Player())->Set_BossStart();
			m_bWakeUp = true;
		}
		else if (m_eCurState == STATE_READYSHOT)
		{
			m_eCurState = STATE_SHOT;
			CSoundMgr::Get_Instance()->PlaySound(L"BOSS_B_SHOT.wav", CSoundMgr::MONSTER2);
			m_strHeadStateKey = L"CYCLESHAKE";
		}
		else if (m_eCurState == STATE_SHOT)
		{
			m_eCurState = STATE_ENDSHOT;
			m_strHeadStateKey = L"ENDSHAKE";
		}
		else if (m_eCurState == STATE_ENDSHOT)
		{
			m_eCurState = STATE_IDLE;
			m_strHeadStateKey = L"IDLE";
			m_dwAttack = GetTickCount();
			m_fAngle = 0.f;
			m_bAttack = false;
		}
		else if (m_eCurState == STATE_JUMP)
		{
			m_eCurState = STATE_IDLE;
			m_strHeadStateKey = L"IDLE";
			m_strBodyStateKey = L"IDLE";
			m_dwAttack = GetTickCount();
			m_bAttack = false;
		}
		else if (m_eCurState == STATE_POTS)
		{
			m_eCurState = STATE_IDLE;
			m_strHeadStateKey = L"IDLE";
			m_strBodyStateKey = L"IDLE";
			m_dwAttack = GetTickCount();
			m_bAttack = false;
		}

		if (m_eCurState == STATE_DEAD && m_bStart)
		{
			m_tFrame.fFrameStart = m_tFrame.fFrameEnd;
			if (!m_bItem)
			{
				CGameObject* pObj = new CItem;
				pObj->Set_Pos(m_tInfo.vPos);
				static_cast<CItem*>(pObj)->Set_Item(CItem::BOSSB_CRISTAL);
				static_cast<CItem*>(pObj)->Set_State(CItem::GROUND);
				static_cast<CItem*>(pObj)->Set_Num(rand() % 7 + 1);
				pObj->Ready_GameObject();
				CGameObject_Manager::Get_Instance()->Add_GameObject(CGameObject_Manager::OBJ_ITEM, pObj);

				pObj = new CItem;
				pObj->Set_Pos(m_tInfo.vPos);
				static_cast<CItem*>(pObj)->Set_Item(CItem::BOSSB_SEED);
				static_cast<CItem*>(pObj)->Set_State(CItem::GROUND);
				static_cast<CItem*>(pObj)->Set_Num(rand() % 7 + 1);
				pObj->Ready_GameObject();
				CGameObject_Manager::Get_Instance()->Add_GameObject(CGameObject_Manager::OBJ_ITEM, pObj);

				m_bItem = true;
			}
			m_bStart = false;
		}
		else
			m_tFrame.fFrameStart = 0.f;
	}
	if (m_tBodyFrame.fFrameStart > m_tBodyFrame.fFrameEnd)
	{
		if (m_eCurState == STATE_ROOT)
		{
			m_eCurState = STATE_IDLE;
			m_strHeadStateKey = L"IDLE";
			m_strBodyStateKey = L"IDLE";
			m_dwAttack = GetTickCount();
			m_bAttack = false;
		}

		if (m_eCurState == STATE_DEAD)
		{
			m_tBodyFrame.fFrameStart = m_tBodyFrame.fFrameEnd;
		}
		else
			m_tBodyFrame.fFrameStart = 0.f;
	}
}

void CMonsterB::Boss_Attack()
{
	if (!m_bAttack)
	{
		if (m_dwAttack + 6000 < GetTickCount())
		{
			//int Pattern = POTS/*rand() % 4*/;
			switch (m_iAttackTurn)
			{
			case SHOT:
				m_eCurState = STATE_READYSHOT;
				m_strHeadStateKey = L"STARTSHAKE";
				m_strBodyStateKey = L"IDLE";
				m_bAttack = true;
				++m_iAttackTurn;
				break;
			case POTS:
				m_eCurState = STATE_POTS;
				m_strHeadStateKey = L"POTS";
				m_strBodyStateKey = L"IDLE";
				m_bAttack = true;
				++m_iAttackTurn;
				break;
			case JUMP:
				m_eCurState = STATE_JUMP;
				m_strHeadStateKey = L"JUMP";
				m_strBodyStateKey = L"JUMP";
				m_dwJump = GetTickCount();
				m_bJump = true;
				m_bAttack = true;
				++m_iAttackTurn;
				break;
			case ROOT:
				m_eCurState = STATE_ROOT;
				m_strHeadStateKey = L"IDLE";
				m_strBodyStateKey = L"STEM";
				m_bAttack = true;
				m_iAttackTurn = 0;
				break;
			default:
				break;
			}
		}
	}
}

void CMonsterB::Move()
{
	m_tInfo.vDir = m_tTargetInfo->vPos - m_tInfo.vPos;
	D3DXVec3Normalize(&m_tInfo.vDir, &m_tInfo.vDir);

	m_tInfo.vPos += m_tInfo.vDir * m_fSpeed;
}

void CMonsterB::Pots()
{
	if (!m_bPotsSound && STATE_POTS == m_eCurState && 1.f < m_tFrame.fFrameStart)
	{
		CSoundMgr::Get_Instance()->PlaySound(L"BOSS_B_SHOT.wav", CSoundMgr::MONSTER);
		m_bPotsSound = true;
	}

	if (STATE_POTS == m_eCurState && !m_bFirstPot && 16.f < m_tFrame.fFrameStart)
	{
		CreatePot();
		m_bFirstPot = true;
	}
	else if(STATE_POTS == m_eCurState && !m_bSecondPot && 22.f < m_tFrame.fFrameStart)
	{
		CreatePot();
		m_bSecondPot = true;
	}
	else if (STATE_POTS == m_eCurState && !m_bThirdPot && 28.f < m_tFrame.fFrameStart)
	{
		CreatePot();
		m_bThirdPot = true;
	}

	if (STATE_POTS != m_eCurState && m_bFirstPot && m_bSecondPot && m_bThirdPot)
	{
		m_bFirstPot = false;
		m_bSecondPot = false;
		m_bThirdPot = false;
	}
}

void CMonsterB::Shot()
{
	if (STATE_SHOT == m_eCurState || (STATE_READYSHOT == m_eCurState && 20.f < m_tFrame.fFrameStart))
	{
		if (m_dwShot + 200 < GetTickCount())
		{
			for (int i = 0; i < 4; ++i)
			{
				D3DXVECTOR3 vDir = m_vecPosin[i] - m_tInfo.vPos;
				D3DXVec3Normalize(&vDir, &vDir);
				CreateBullet(m_vecPosin[i], vDir, BOSSB_BULLET);
			}
			m_dwShot = GetTickCount();
		}
		m_fAngle += 1.f;
	}
}

void CMonsterB::Root()
{
	if (STATE_ROOT == m_eCurState && !m_bFirstRoot && 24.f < m_tBodyFrame.fFrameStart)
	{
		CreateRoot();
		m_bFirstRoot = true;
	}
	else if (STATE_ROOT == m_eCurState && !m_bSecondRoot && 30.f < m_tBodyFrame.fFrameStart)
	{
		CreateRoot();
		m_bSecondRoot = true;
	}
	else if (STATE_ROOT == m_eCurState && !m_bThirdRoot && 36.f < m_tBodyFrame.fFrameStart)
	{
		CreateRoot();
		m_bThirdRoot = true;
	}

	if (STATE_ROOT != m_eCurState && m_bFirstRoot && m_bSecondRoot && m_bThirdRoot)
	{
		m_bFirstRoot = false;
		m_bSecondRoot = false;
		m_bThirdRoot = false;
	}
}

void CMonsterB::CreatePot()
{
	CPlant* pot = new CPlant;
	D3DXVECTOR3 vPos = { 0.f, -140.f, 0.f };
	pot->Set_Pos(m_tInfo.vPos + vPos);
	D3DXVECTOR3 vDir = { 0.f, 0.f, 0.f };
	pot->Set_Dir(vDir);

	D3DXVECTOR3 vTarget = { float((rand() % 950) + 95), float((rand() % 550) + 85), 0.f };
	static_cast<CPlant*>(pot)->Set_PotTarget(vTarget);
	static_cast<CPlant*>(pot)->Set_Target(m_tTargetInfo);
	pot->Ready_GameObject();
	CGameObject_Manager::Get_Instance()->Add_GameObject(CGameObject_Manager::OBJ_MONSTER, pot);
}

void CMonsterB::CreateRoot()
{
	for (int i = 0; i < 4; ++i)
	{
		CRoot* Root = new CRoot;
		D3DXVECTOR3	vPosSet = { -50.f, -50.f, 0.f };
		D3DXVECTOR3 vPos = { float(rand() % 101), float(rand() % 101), 0.f };
		Root->Set_Pos(m_tTargetInfo->vPos + vPos + vPosSet);
		static_cast<CRoot*>(Root)->Set_Type(CRoot::ATTACK);
		Root->Ready_GameObject();
		CGameObject_Manager::Get_Instance()->Add_GameObject(CGameObject_Manager::OBJ_MONSTER, Root);
	}
}

void CMonsterB::CreateBullet(D3DXVECTOR3 vPos, D3DXVECTOR3 vDir, BYTE Monster)
{
	CMBullet* bullet = new CMBullet;
	bullet->Set_Pos(vPos);
	bullet->Set_Dir(vDir);
	static_cast<CMBullet*>(bullet)->Set_Weapon(Monster);
	bullet->Ready_GameObject();
	CGameObject_Manager::Get_Instance()->Add_GameObject(CGameObject_Manager::OBJ_MBULLET, bullet);
}
