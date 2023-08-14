#include "stdafx.h"
#include "BackGround.h"
#include "Graphic_Device.h"
#include "Texture_Manager.h"
#include "Scroll_Manager.h"

CBackGround::CBackGround()
{
}


CBackGround::~CBackGround()
{
	Release_GameObject();
}

void CBackGround::MiniRender_BackGround()
{
}

HRESULT CBackGround::LoadBackTileData(const wstring & wstrFilePath)
{
	HANDLE hFile = CreateFile(wstrFilePath.c_str(), GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	DWORD dwByte = 0;
	TILE* pTile = new TILE;
	ReadFile(hFile, pTile, sizeof(TILE), &dwByte, nullptr);

	m_backTile = pTile;
	CloseHandle(hFile);
	return S_OK;
}

HRESULT CBackGround::Ready_GameObject()
{
	if (FAILED(LoadBackTileData(m_StrBackPathFile)))
	{
		ERR_MSG(L"BackTileData Load FAILED");
		return E_FAIL;
	}
	return S_OK;
}

int CBackGround::Update_GameObject()
{
	if (m_bDead)
		return OBJ_DEAD;

	return OBJ_NOEVENT;
}

void CBackGround::Late_Update_GameObject()
{
}

void CBackGround::Render_GameObject()
{
	D3DXMATRIX matScale, matTrans, matWorld;

	const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(L"TEXTURE", L"BACKGROUND", m_backTile->byDrawID);
	if (!pTexInfo)
		return;
	float iScrollX = CScroll_Manager::Get_Scroll(CScroll_Manager::SCROLLX);
	float iScrollY = CScroll_Manager::Get_Scroll(CScroll_Manager::SCROLLY);
	D3DXMatrixScaling(&matScale, m_backTile->vSize.x, m_backTile->vSize.y, 0.f);
	D3DXMatrixTranslation(&matTrans, m_backTile->vPos.x + iScrollX, m_backTile->vPos.y + iScrollY, 0.f);
	matWorld = matScale * matTrans;
	float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CBackGround::Release_GameObject()
{
	Safe_Delete(m_backTile);
}
