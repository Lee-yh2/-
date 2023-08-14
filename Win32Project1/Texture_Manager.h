#pragma once

#ifndef __TEXTUREMANAGER_H__
#define __TEXTUREMANAGER_H__

class CTexture;
class CTexture_Manager
{
	DECLARE_SINGLETON(CTexture_Manager)
private:
	CTexture_Manager();
	~CTexture_Manager();
public:
	enum TEX_ID { TEX_SINGLE, TEX_MULTI };
public:
	const TEXINFO* Get_TexInfo(const wstring& wstrObjectKey, const wstring& StateKey = L"", const int& iIndex = 0) const;
	HRESULT Insert_Texture(TEX_ID eTexID, const wstring& wstrFilePath, const wstring& wstrObjectKey, const wstring& wstrStateKey = L"", const int& iCount = 0);
	void Release_Texture_Manager();
	HRESULT ReadImagePath(string strPath);
private:
	// ObjectKey 
	map<wstring, CTexture*> m_mapTexture;
};

#endif // !__TEXTUREMANAGER_H__
