#include "CRenderTimer.h"

CRenderTimer::CRenderTimer(GLCanvas* glcanvas) : wxTimer()
{
	CRenderTimer::m_glcanvas = glcanvas;
}

void 
CRenderTimer::notify()
{
	m_glcanvas->Refresh(false);
}

void 
CRenderTimer::start()
{
	wxTimer::Start(30);
}