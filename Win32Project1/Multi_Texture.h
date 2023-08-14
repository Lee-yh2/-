#pragma once

#ifndef __MULTITEXTURE_H__
#define __MULTITEXTURE_H__


#include "Texture.h"
class CMulti_Texture :
	public CTexture
{
public:
	explicit CMulti_Texture();
	virtual ~CMulti_Texture();

	// CTexture을(를) 통해 상속됨
	virtual HRESULT Insert_Texture(const wstring & wstrFilePaht, const wstring & wstrStateKey = L"", const int & iCount = 0) override;
	virtual const TEXINFO * Get_Texture(const wstring & wstrStateKey = L"", const int & iCount = 0) override;
	virtual void Release_Texture() override;

private:
	map<wstring, vector<TEXINFO*>> m_mapMulti;
};

#endif // !__MULTITEXTURE_H__
