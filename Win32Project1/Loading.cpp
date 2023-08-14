#include "stdafx.h"
#include "Loading.h"
#include "Scene_Manager.h"
#include "Texture_Manager.h"
#include "Time_Manager.h"
#include "Graphic_Device.h"
#include "GameObject_Manager.h"
#include "SoundMgr.h"


CLoading::CLoading()
{
}


CLoading::~CLoading()
{
	Release_Scene();
}

HRESULT CLoading::Ready_Scene()
{
	InitializeCriticalSection(&m_CriticalSection);
	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, ReadImageFunc, this, 0, nullptr);
	if (nullptr == m_hThread)
		return E_FAIL;

	//CSoundMgr::Get_Instance()->PlayBGM(L"LODING.wav");
	m_tFrame = { 0.f, 66.f };
	return S_OK;
}

void CLoading::Update_Scene()
{
}

void CLoading::LateUpdate_Scene()
{
	if (GetAsyncKeyState(VK_RETURN) & 0x0001)
	{
		WaitForSingleObject(m_hThread, INFINITE);
		CloseHandle(m_hThread);
		DeleteCriticalSection(&m_CriticalSection);
		CScene_Manager::Get_Instance()->Scene_Change_SceneManager(CScene_Manager::SCENE_SHOP);
	}
	FrameMove(0.3f);
}

void CLoading::Render_Scene()
{
	const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(L"RESOURCE", L"INTRO", 1);
	if (nullptr == pTexInfo)
		return;
	D3DXMATRIX matWorld, matTrans, matScale;
	D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
	D3DXMatrixTranslation(&matTrans, WINCX/2.f, WINCY/2.f, 0.f);
	float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);
	matWorld = matScale * matTrans;
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(L"RESOURCE", L"LOADING_ICON", int(m_tFrame.fFrameStart));
	if (nullptr == pTexInfo)
		return;
	D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
	D3DXMatrixTranslation(&matTrans, float(WINCX - 80.f), float(WINCY - 60.f), 0.f);
	fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	fCenterY = float(pTexInfo->tImageInfo.Height >> 1);
	matWorld = matScale * matTrans;
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	
}

void CLoading::Release_Scene()
{
	CSoundMgr::Get_Instance()->StopAll();
}

unsigned CLoading::ReadImageFunc(LPVOID pVoid)
{
	CLoading* pLoading = static_cast<CLoading*>(pVoid);
	EnterCriticalSection(&pLoading->m_CriticalSection);
	if (FAILED(CTexture_Manager::Get_Instance()->ReadImagePath("../Data/PathInfo.txt")))
	{
		ERR_MSG(L"Creating Failed InsertTexture ");
		return E_FAIL;
	}
	//CTexture_Manager::Get_Instance()->Set_String(L"Press Enter!");
	LeaveCriticalSection(&pLoading->m_CriticalSection);

	return 0;
}

void CLoading::FrameMove(float fSpeed)
{
	m_tFrame.fFrameStart += m_tFrame.fFrameEnd *CTime_Manager::Get_Instance()->Get_DeltaTime() * fSpeed;
	if (m_tFrame.fFrameStart > m_tFrame.fFrameEnd)
	{
		m_tFrame.fFrameStart = 0.f;
	}
}

