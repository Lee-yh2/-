#include "stdafx.h"
#include "Graphic_Device.h"


CGraphic_Device* CGraphic_Device::m_pInstance = nullptr;
CGraphic_Device::CGraphic_Device()
{
}


CGraphic_Device::~CGraphic_Device()
{
	Release_Graphic_Device();
}

HRESULT CGraphic_Device::Ready_Graphic_Device(MODE eMode)
{
	// ��ġ �ʱ�ȭ �ϴ� ����. 
	//1. ��ġ�� ������ �İ�ü ����!. 
	m_pSDK = Direct3DCreate9(D3D_SDK_VERSION);

	D3DCAPS9 DeviceCaps;
	ZeroMemory(&DeviceCaps, sizeof(D3DCAPS9));
	//2. ��ġ�� ���� ������ ����. 
	//HAL - Hardware Abstraction Layer �ٷ� ���� ����?? 
	// ���Ұ� 
	if (FAILED(m_pSDK->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &DeviceCaps)))
	{
		ERR_MSG(L"SDK Creating Failed");
		return E_FAIL;
	}
	DWORD vp = 0;
	// ���ؽ� ���μ��� - ������ȯ + ������ 
	if (DeviceCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp |= D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED;
	else
		vp |= D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED;
	// else �� ź�ٶ�°�. �� ��ǻ�ʹ� ������ �ٷ� �����̶� ��. 

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));

	d3dpp.BackBufferWidth = WINCX;
	d3dpp.BackBufferHeight = WINCY;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8; /*A = 0~255,*/
	d3dpp.BackBufferCount = 1; //default + 1; �� 2���� ���۸� ����ϰڴ�. 

	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = 0;

	// D3DSWAPEFFECT_DISCARD - ���� ü�� ����� �̿��Ͽ� �׸��� �׸���. ������۸��� ����ȣȯ�̶� �����ϸ� �ȴ�. 

	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = g_hWND;
	d3dpp.Windowed = eMode; // true -â��� , false ��üȭ�� 
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;

	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	//3. SDK�� �̿��ؼ� ��ġ�� ������ �İ�ü ����!!! 
	if (FAILED(m_pSDK->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, g_hWND, vp, &d3dpp, &m_pDevice)))
	{
		ERR_MSG(L"Device Creating Failed");
		return E_FAIL;
	}
	if (FAILED(D3DXCreateSprite(m_pDevice, &m_pSprite)))
	{
		ERR_MSG(L"m_pSprite Creating Failed");
		return E_FAIL;
	}
	D3DXFONT_DESCW tFontInfo;
	ZeroMemory(&tFontInfo, sizeof(D3DXFONT_DESCW));

	tFontInfo.Height = 20;
	tFontInfo.Width = 10;
	tFontInfo.Weight = FW_BOLD;
	tFontInfo.CharSet = HANGEUL_CHARSET;
	lstrcpy(tFontInfo.FaceName, L"����ü");

	if (FAILED(D3DXCreateFontIndirect(m_pDevice, &tFontInfo, &m_pFont)))
	{
		ERR_MSG(L"m_pFont Creating Failed");
		return E_FAIL;
	}

	return S_OK;
}

void CGraphic_Device::Release_Graphic_Device()
{
	if (m_pFont)
		m_pFont->Release();
	if (m_pSprite)
		m_pSprite->Release();
	if (m_pDevice)
		m_pDevice->Release();
	if (m_pSDK)
		m_pSDK->Release();
	// ����� ������ �߿��ϴ�.
	//���۷��� ī��Ʈ��� ����� ����ϱ� ����. 
	// ���۷��� ī��Ʈ�� ���۸� �����͸� �����ϱ� ���� ���. 
}

void CGraphic_Device::Render_Begin()
{
	m_pDevice->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(255, 0, 0, 255), 0.f, 0);
	//2.�ĸ���ۿ� �׸��� �����Ѵ�. 
	m_pDevice->BeginScene();
	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
}

void CGraphic_Device::Render_End(HWND hWnd /*= nullptr*/)
{
	m_pSprite->End();
	m_pDevice->EndScene();
	// 3�� ���ڴ� ����ϰ��� �ϴ� ������â�� �������� �� �ִ�!!! 
	m_pDevice->Present(nullptr, nullptr, hWnd, nullptr);
}
