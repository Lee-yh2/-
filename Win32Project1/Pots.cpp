#include "stdafx.h"
#include "Pots.h"


CPots::CPots()
{
}


CPots::~CPots()
{
}

HRESULT CPots::Ready_GameObject()
{
	m_tInfo.vCSize = { 0.f, 0.f, 0.f };
	m_tInfo.vSize = { 1.f, 1.f, 0.f };

	return S_OK;
}

int CPots::Update_GameObject()
{
	
	return OBJ_NOEVENT;
}

void CPots::Late_Update_GameObject()
{
}

void CPots::Render_GameObject()
{
}

void CPots::Release_GameObject()
{
}
