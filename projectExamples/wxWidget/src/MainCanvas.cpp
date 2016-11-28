#include "wx/wxprec.h"
#include <GL/glew.h>
#include "MainCanvas.h"

//#pragma comment(lib, "glew32.lib")

MainCanvas::MainCanvas(wxWindow* parent, wxWindowID id,
	const int* attribList, const wxPoint& pos, const wxSize& size,
	long style, const wxString& name, const wxPalette& palette)
	: wxGLCanvas(parent, id, attribList, pos, size, style, name, palette),
	m_vbo(0), m_vao(0)
{
	m_context = std::make_unique<wxGLContext>(this);
	Bind(wxEVT_PAINT, &MainCanvas::OnPaint, this);

	SetCurrent(*m_context);
	InitializeGLEW();
	SetupGraphics();
}

MainCanvas::~MainCanvas()
{
	SetCurrent(*m_context);
	glDeleteVertexArrays(1, &m_vao);
	glDeleteBuffers(1, &m_vbo);
}

void MainCanvas::InitializeGLEW()
{
	glewExperimental = true;
	GLenum err = glewInit();
	if (err != GLEW_OK) {
		const GLubyte* msg = glewGetErrorString(err);
		throw std::exception(reinterpret_cast<const char*>(msg));
	}
}

void MainCanvas::SetupGraphics()
{
	// define vertices
	float points[] = {
		0.0f, 0.5f,
		0.5f, -0.5f,
		-0.5f, -0.5f
	};
	// upload vertex data
	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
	// setup vertex array objects
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
}

void MainCanvas::OnPaint(wxPaintEvent& event)
{
	SetCurrent(*m_context);

	// set background to black
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// draw the graphics
	glDrawArrays(GL_TRIANGLES, 0, 3);
	// and display
	glFlush();
	SwapBuffers();
}