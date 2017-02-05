#include "CRenderTimer.h"

CRenderTimer::CRenderTimer(CGLCanvas* CGLCanvas) : wxTimer()
{
	CRenderTimer::m_CGLCanvas = CGLCanvas;
}

void 
CRenderTimer::notify()
{
	m_CGLCanvas->Refresh(false);
}

void 
CRenderTimer::start()
{
	wxTimer::Start(30);
}