#pragma once

#include "../../../yengine/scene/CScene.h"
#include "../../../yengine/scene/CCamera.h"
#include "../../../yengine/core/CGraphicDriver.h"
#include "CRenderer.h"
//#include "./workshop/TerrainCDLOD.h"
#include <memory>
#include "wx/glcanvas.h"
#include "wx/wxprec.h"

#include <GL/glew.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <chrono>

using namespace glm;

static const int INTERVAL = 1000 / 60;
static const int TIMERNUMBER = 3;

class CGLCanvas : public wxGLCanvas 
{
public:
	CGLCanvas(wxWindow* parent, wxWindowID id = wxID_ANY,
		const int* attribList = 0, const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxSize(400,400), long style = 0L,
		const wxString& name = L"CGLCanvas",
		const wxPalette& palette = wxNullPalette);

	virtual ~CGLCanvas();
	CGLCanvas(const CGLCanvas& tc) = delete;
	CGLCanvas(CGLCanvas&& tc) = delete;
	CGLCanvas& operator=(const CGLCanvas& tc) = delete;
	CGLCanvas& operator=(CGLCanvas&& tc) = delete;

	void setGModule(CRenderer/*TerrainCDLOD*/ *renderer);

private:
	void initializeGLEW();
	void setupGraphics();
	void setupControls();
	void onPaint(wxPaintEvent& event);
	void onKeyDown(wxKeyEvent& event);
	void onMouseEnter(wxMouseEvent& WXUNUSED(ev));
	void onMouseWheel(wxMouseEvent& WXUNUSED(ev));
	void onMouseEvent(wxMouseEvent& WXUNUSED(ev));
	void onTimer(wxTimerEvent& event);

private:
	std::unique_ptr<wxGLContext> m_context;
	std::unique_ptr<wxTimer> mTimer;

	typedef std::chrono::time_point<std::chrono::high_resolution_clock> sclock;
	sclock mStartTime;

	CRenderer* mGModule;

	GLfloat mLastMouseX;
	GLfloat mLastMouseY;
	bool mFirstMouse;

	DECLARE_EVENT_TABLE()
};