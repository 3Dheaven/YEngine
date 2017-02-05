#pragma once
#include <wx/wx.h>
#include <wx/timer.h>
#include "CGLCanvas.h"

class CRenderTimer : public wxTimer
{
	private:
		CGLCanvas* m_CGLCanvas;
	public:
		CRenderTimer(CGLCanvas* CGLCanvas);
		void notify();
		void start();
};

