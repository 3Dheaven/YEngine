#pragma once
#include <wx/wx.h>
#include <wx/timer.h>
#include "MainCanvas.h"

class RenderTimer : public wxTimer
{
private:
	MainCanvas* m_glcanvas;
public:
	RenderTimer(MainCanvas* glcanvas);
	void Notify();
	void start();
};

