#pragma once

#ifndef __GRAPHIC_DEVICE_H__
#define __GRAPHIC_DEVICE_H__

class CGraphic_Device
{
public:
	static CGraphic_Device* Get_Instance()
	{
		if (nullptr == m_pInstance)
			m_pInstance = new CGraphic_Device;

		return m_pInstance;
	}
	static void Destroy_Instance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}
private:
	static CGraphic_Device* m_pInstance;

public:
	enum MODE { MODE_FULL, MODE_WIN };

private:
	CGraphic_Device();
	~CGraphic_Device();
public:
	LPDIRECT3DDEVICE9 Get_Device() { return m_pDevice; }
	LPD3DXSPRITE Get_Sprite() { return m_pSprite; }
	LPD3DXFONT Get_Font() { return m_pFont; }
public:
	HRESULT Ready_Graphic_Device(MODE eMode);
	void Release_Graphic_Device();
public:
	void Render_Begin();
	void Render_End(HWND hWnd = nullptr);


	LPDIRECT3D9 m_pSDK;
	LPD3DXFONT m_pFont;
	LPDIRECT3DDEVICE9 m_pDevice;
	LPD3DXSPRITE  m_pSprite;

};

#endif // !__GRAPHIC_DEVICE_H__