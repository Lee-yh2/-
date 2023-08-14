#include "stdafx.h"
#include "Multi_Texture.h"
#include "Graphic_Device.h"


CMulti_Texture::CMulti_Texture()
{
}


CMulti_Texture::~CMulti_Texture()
{
	Release_Texture();
}

HRESULT CMulti_Texture::Insert_Texture(const wstring & wstrFilePaht, const wstring & wstrStateKey, const int & iCount)
{
	auto& iter_find = m_mapMulti.find(wstrStateKey);

	if (iter_find != m_mapMulti.end())
		return E_FAIL;

	TCHAR szBuf[MAX_PATH] = L"";
	for (int i = 0; i < iCount; i++)
	{
		swprintf_s(szBuf, wstrFilePaht.c_str(), i);
		TEXINFO* pTexInfo = new TEXINFO;
		ZeroMemory(pTexInfo, sizeof(TEXINFO));

		if (FAILED(D3DXGetImageInfoFromFile(szBuf, &pTexInfo->tImageInfo)))
		{
			wstring wstrErr = wstrFilePaht + L"LoadFailed";
			ERR_MSG(wstrErr.c_str());
			return E_FAIL;
		}
		if (FAILED(D3DXCreateTextureFromFileEx(CGraphic_Device::Get_Instance()->Get_Device(),
			szBuf,
			pTexInfo->tImageInfo.Width,
			pTexInfo->tImageInfo.Height,
			pTexInfo->tImageInfo.MipLevels,
			0,
			pTexInfo->tImageInfo.Format,
			D3DPOOL_MANAGED,
			D3DX_DEFAULT,
			D3DX_DEFAULT,
			0,
			nullptr, nullptr,
			&pTexInfo->pTexture)))
		{
			wstring wstrErr = wstrFilePaht + L"LoadFailed";
			ERR_MSG(wstrErr.c_str());
			return E_FAIL;
		}
		m_mapMulti[wstrStateKey].emplace_back(pTexInfo);
	}
	return S_OK;
}

const TEXINFO * CMulti_Texture::Get_Texture(const wstring & wstrStateKey, const int & iCount)
{
	auto& iter_find = m_mapMulti.find(wstrStateKey);
	if (iter_find == m_mapMulti.end())
		return nullptr;

	return iter_find->second[iCount];
	//return m_mapMulti[wstrStateKey][iCount];
}

void CMulti_Texture::Release_Texture()
{
	for (auto& rPair : m_mapMulti)
	{
		for (auto& pTexInfo : rPair.second)
		{
			if (pTexInfo->pTexture)
				pTexInfo->pTexture->Release();
			if (pTexInfo)
			{
				delete pTexInfo;
				pTexInfo = nullptr;
			}
		}
		rPair.second.clear();
		rPair.second.shrink_to_fit();
	}
}
