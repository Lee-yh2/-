#pragma once

#ifndef __SINGLETEXTURE_H__
#define __SINGLETEXTURE_H__


#include "Texture.h"
class CSingle_Texture :
	public CTexture
{
public:
	explicit CSingle_Texture();
	virtual ~CSingle_Texture();

	// CTexture을(를) 통해 상속됨
	virtual HRESULT Insert_Texture(const wstring & wstrFilePaht, const wstring & wstrStateKey = L"", const int & iCount = 0) override;
	virtual const TEXINFO * Get_Texture(const wstring & wstrStateKey = L"", const int & iCount = 0) override;
	virtual void Release_Texture() override;

private:
	TEXINFO m_tTexInfo;
};

#endif // !__SINGLETEXTURE_H__
