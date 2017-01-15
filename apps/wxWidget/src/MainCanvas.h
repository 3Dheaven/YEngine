#pragma once
#include "../../../yengine/Shader.h"
#include "../../../yengine/Camera.h"
#include <memory>
#include "wx/glcanvas.h"
#include "wx/wxprec.h"

#include <GL/glew.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <chrono>
#include "../../../yengine/object.h"

using namespace glm;

static const int INTERVAL = 1000 / 60;
static const int TIMERNUMBER = 3;

class MainCanvas : public wxGLCanvas
{
public:
	MainCanvas(wxWindow* parent, wxWindowID id = wxID_ANY,
		const int* attribList = 0, const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxSize(400,400), long style = 0L,
		const wxString& name = L"GLCanvas",
		const wxPalette& palette = wxNullPalette);

	virtual ~MainCanvas();
	MainCanvas(const MainCanvas& tc) = delete;
	MainCanvas(MainCanvas&& tc) = delete;
	MainCanvas& operator=(const MainCanvas& tc) = delete;
	MainCanvas& operator=(MainCanvas&& tc) = delete;

private:
	void InitializeGLEW();
	void SetupGraphics();
	void OnPaint(wxPaintEvent& event);
	void onKeyDown(wxKeyEvent& event);
	void OnMouseEnter(wxMouseEvent& WXUNUSED(ev));
	void OnMouseWheel(wxMouseEvent& WXUNUSED(ev));
	void OnMouseEvent(wxMouseEvent& WXUNUSED(ev));
	void OnTimer(wxTimerEvent& event);

	std::unique_ptr<wxGLContext> m_context;
	
	Shader *customShader;
	camera *cam;
	std::unique_ptr<wxTimer> m_timer;
	std::chrono::time_point<std::chrono::high_resolution_clock> m_startTime;
	//Model *model;
	object *obj;

	GLfloat lastX;
	GLfloat lastY;
	bool firstMouse;

	DECLARE_EVENT_TABLE()
	
};