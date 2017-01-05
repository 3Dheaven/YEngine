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

using namespace glm;

class MainCanvas : public wxGLCanvas
{
public:
	MainCanvas(wxWindow* parent, wxWindowID id = wxID_ANY,
		const int* attribList = 0, const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize, long style = 0L,
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

	std::unique_ptr<wxGLContext> m_context;
	GLuint m_vbo; // vertex buffer pointer
	GLuint m_vao; // vertex array pointer
	Shader *customShader;
	camera *cam;


	float m_value;
	DECLARE_EVENT_TABLE()
	
};