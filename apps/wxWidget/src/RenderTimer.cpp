#include "RenderTimer.h"

RenderTimer::RenderTimer(MainCanvas* glcanvas) : wxTimer()
{
	RenderTimer::m_glcanvas = glcanvas;
}

void RenderTimer::Notify()
{
	m_glcanvas->Refresh(false);
}

void RenderTimer::start()
{
	wxTimer::Start(30);
}