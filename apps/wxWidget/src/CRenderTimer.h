#pragma once
#include <wx/wx.h>
#include <wx/timer.h>
#include "GLCanvas.h"

class CRenderTimer : public wxTimer
{
	private:
		GLCanvas* m_glcanvas;
	public:
		CRenderTimer(GLCanvas* glcanvas);
		void notify();
		void start();
};

