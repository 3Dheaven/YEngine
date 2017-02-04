#pragma once

#include "../../../yengine/scene/CScene.h"
#include "../../../yengine/CShaderFactory.h"
#include "../../../yengine/scene/CCamera.h"
#include "../../../yengine/core/CGraphicDriver.h"

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

class GLCanvas : public wxGLCanvas
{
public:
	GLCanvas(wxWindow* parent, wxWindowID id = wxID_ANY,
		const int* attribList = 0, const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxSize(400,400), long style = 0L,
		const wxString& name = L"GLCanvas",
		const wxPalette& palette = wxNullPalette);

	virtual ~GLCanvas();
	GLCanvas(const GLCanvas& tc) = delete;
	GLCanvas(GLCanvas&& tc) = delete;
	GLCanvas& operator=(const GLCanvas& tc) = delete;
	GLCanvas& operator=(GLCanvas&& tc) = delete;

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
	
	CGraphicDriver *mGraphicDriver;
	CShaderFactory *m_customShader;
	CCamera *m_cam;
	std::unique_ptr<wxTimer> m_timer;

	typedef std::chrono::time_point<std::chrono::high_resolution_clock> sclock;
	sclock m_startTime;

	CScene *mScene;

	GLfloat m_lastMouseX;
	GLfloat m_lastMouseY;
	bool m_firstMouse;

	DECLARE_EVENT_TABLE()
};