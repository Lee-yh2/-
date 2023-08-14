#include "stdafx.h"
#include "Texture_Manager.h"
#include "Single_Texture.h"
#include "Multi_Texture.h"

IMPLEMENT_SINGLETON(CTexture_Manager)
CTexture_Manager::CTexture_Manager()
{
}


CTexture_Manager::~CTexture_Manager()
{
	Release_Texture_Manager();
}

const TEXINFO * CTexture_Manager::Get_TexInfo(const wstring & wstrObjectKey, const wstring & StateKey, const int & iIndex) const
{
	auto& iter_find = m_mapTexture.find(wstrObjectKey);
	if (iter_find == m_mapTexture.end())
		return nullptr;

	return iter_find->second->Get_Texture(StateKey, iIndex);
}

HRESULT CTexture_Manager::Insert_Texture(TEX_ID eTexID, const wstring & wstrFilePath, const wstring & wstrObjectKey, const wstring & wstrStateKey, const int & iCount)
{
	auto& iter_find = m_mapTexture.find(wstrObjectKey);
	if (iter_find == m_mapTexture.end())
	{
		CTexture* pTexture = nullptr;
		switch (eTexID)
		{
		case CTexture_Manager::TEX_SINGLE:
			pTexture = new CSingle_Texture;
			break;
		case CTexture_Manager::TEX_MULTI:
			pTexture = new CMulti_Texture;
			break;
		default:
			break;
		}
		if (FAILED(pTexture->Insert_Texture(wstrFilePath, wstrStateKey, iCount)))
		{
			ERR_MSG(L"Texture CreateFialed - Texture_Manager");
			return E_FAIL;
		}
		m_mapTexture.emplace(wstrObjectKey, pTexture);
	}
	else if (eTexID == TEX_MULTI)
	{
		if (FAILED(m_mapTexture[wstrObjectKey]->Insert_Texture(wstrFilePath, wstrStateKey, iCount)))
		{
			ERR_MSG(L"Texture CreateFialed - Texture_Manager");
			return E_FAIL;
		}
	}

	return S_OK;
}

void CTexture_Manager::Release_Texture_Manager()
{
	for (auto& rPair : m_mapTexture)
	{
		if (rPair.second)
		{
			delete rPair.second;
			rPair.second = nullptr;
		}
	}

	m_mapTexture.clear();
}

HRESULT CTexture_Manager::ReadImagePath(string strPath)
{
	wifstream fin;
	fin.open(strPath);

	if (fin.fail())
		return E_FAIL;

	TCHAR szPath[MAX_PATH] = L"";
	TCHAR szObjectKey[MAX_PATH] = L"";
	TCHAR szStateKey[MAX_PATH] = L"";
	TCHAR szCount[MAX_PATH] = L"";

	while (true)
	{
		fin.getline(szPath, MAX_PATH, '|');
		fin.getline(szObjectKey, MAX_PATH, '|');
		fin.getline(szStateKey, MAX_PATH, '|');
		fin.getline(szCount, MAX_PATH);
		if (fin.eof())
			break;
		int iCount = _ttoi(szCount);

		if (FAILED(CTexture_Manager::Insert_Texture(TEX_MULTI, szPath, szObjectKey, szStateKey, iCount)))
			return E_FAIL;
	}
	fin.close();
	return S_OK;
}
