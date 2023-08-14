#include "stdafx.h"
#include "Terrain.h"
#include "Graphic_Device.h"
#include "Texture_Manager.h"
#include "Scroll_Manager.h"
#include "Time_Manager.h"
#include "KeyManager.h"


CTerrain::CTerrain()
	:m_iAlpha(0), m_bAlpha(FALSE)
{
}


CTerrain::~CTerrain()
{
	Release_GameObject();
}

void CTerrain::Set_Alpha()
{
	m_bAlpha = !m_bAlpha;
	if (m_bAlpha)
		m_iAlpha = 255;
	else
		m_iAlpha = 0;
}

HRESULT CTerrain::LoadTileData(const wstring & wstrFilePath)
{
	HANDLE hFile = CreateFile(wstrFilePath.c_str(), GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	DWORD dwByte = 0;
	TILE* pTile = nullptr;

	while (true)
	{
		pTile = new TILE;
		ReadFile(hFile, pTile, sizeof(TILE), &dwByte, nullptr);

		if (0 == dwByte)
		{
			Safe_Delete(pTile);
			break;
		}
		m_vecTile.emplace_back(pTile);
	}
	CloseHandle(hFile);
	return S_OK;
}

HRESULT CTerrain::Ready_GameObject()
{
	if (FAILED(LoadTileData(m_StrTerrianPathFile)))
	{
		ERR_MSG(L"TileData Load FAILED");
		return E_FAIL;
	}
	return S_OK;
}

int CTerrain::Update_GameObject()
{
	if (m_bDead)
		return OBJ_DEAD;

	if (m_tKeyManager.KeyDown(KEY_P))
		Set_Alpha();
	m_tKeyManager.KeyCheck();
	return OBJ_NOEVENT;
}

void CTerrain::Late_Update_GameObject()
{
}

void CTerrain::Render_GameObject()
{
	D3DXMATRIX matScale, matTrans, matWorld;
	int iSize = m_vecTile.size();
	//TCHAR szBuf[MAX_PATH] = L"";
	for (size_t i = 0; i < iSize; ++i)
	{
		const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(L"TEXTURE", L"TILE", m_vecTile[i]->byDrawID);
		if (!pTexInfo)
			return;
		float iScrollX = CScroll_Manager::Get_Scroll(CScroll_Manager::SCROLLX);
		float iScrollY = CScroll_Manager::Get_Scroll(CScroll_Manager::SCROLLY);
		D3DXMatrixScaling(&matScale, m_vecTile[i]->vSize.x, m_vecTile[i]->vSize.y, 0.f);
		D3DXMatrixTranslation(&matTrans, m_vecTile[i]->vPos.x + iScrollX, m_vecTile[i]->vPos.y + iScrollY, 0.f);
		matWorld = matScale * matTrans;
		float fCenterX = (float)(pTexInfo->tImageInfo.Width >> 1);
		float fCenterY = (float)(pTexInfo->tImageInfo.Height >> 1);

		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
		CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(m_iAlpha, 255, 255, 255));
	}
}

void CTerrain::Release_GameObject()
{
	for (auto& pTile : m_vecTile)
		Safe_Delete(pTile);

	m_vecTile.clear();
	m_vecTile.shrink_to_fit();
}
