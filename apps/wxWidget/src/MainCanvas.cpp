#include "MainCanvas.h"

//#pragma comment(lib, "glew32.lib")

BEGIN_EVENT_TABLE(MainCanvas, wxGLCanvas)
EVT_ENTER_WINDOW(MainCanvas::OnMouseEnter)
EVT_KEY_DOWN(MainCanvas::onKeyDown)
END_EVENT_TABLE()

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
	customShader = new Shader("../shaders/shader.vert", "../shaders/shader.frag");
	cam = new camera(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	m_value = 0.0;
	
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
	/*float points[] = {
		0.0f, 0.5f,
		0.5f, -0.5f,
		-0.5f, -0.5f
	};*/

	float points[] = {
		-0.25f,  0.25f, -0.25f,
		-0.25f, -0.25f, -0.25f,
		0.25f, -0.25f, -0.25f,

		0.25f, -0.25f, -0.25f,
		0.25f,  0.25f, -0.25f,
		-0.25f,  0.25f, -0.25f,

		0.25f, -0.25f, -0.25f,
		0.25f, -0.25f,  0.25f,
		0.25f,  0.25f, -0.25f,

		0.25f, -0.25f,  0.25f,
		0.25f,  0.25f,  0.25f,
		0.25f,  0.25f, -0.25f,

		0.25f, -0.25f,  0.25f,
		-0.25f, -0.25f,  0.25f,
		0.25f,  0.25f,  0.25f,

		-0.25f, -0.25f,  0.25f,
		-0.25f,  0.25f,  0.25f,
		0.25f,  0.25f,  0.25f,

		-0.25f, -0.25f,  0.25f,
		-0.25f, -0.25f, -0.25f,
		-0.25f,  0.25f,  0.25f,

		-0.25f, -0.25f, -0.25f,
		-0.25f,  0.25f, -0.25f,
		-0.25f,  0.25f,  0.25f,

		-0.25f, -0.25f,  0.25f,
		0.25f, -0.25f,  0.25f,
		0.25f, -0.25f, -0.25f,

		0.25f, -0.25f, -0.25f,
		-0.25f, -0.25f, -0.25f,
		-0.25f, -0.25f,  0.25f,

		-0.25f,  0.25f, -0.25f,
		0.25f,  0.25f, -0.25f,
		0.25f,  0.25f,  0.25f,

		0.25f,  0.25f,  0.25f,
		-0.25f,  0.25f,  0.25f,
		-0.25f,  0.25f, -0.25f
	};


	// upload vertex data
	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
	// setup vertex array objects
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	//GLint posAttrib = glGetAttribLocation(customShader->Program, "position");

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
}

void MainCanvas::OnPaint(wxPaintEvent& event)
{
	SetCurrent(*m_context);
	m_value += 0.1;
	// set background to black
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	customShader->Use();

	// Transformation matrices
	glm::mat4 projection = glm::perspective(cam->getZoom(), (float)800 / (float)800, 0.1f, 100.0f);
	glm::mat4 view = cam->getViewMatrix();
	glUniformMatrix4fv(glGetUniformLocation(customShader->Program, "projection_matrix"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(customShader->Program, "view_matrix"), 1, GL_FALSE, glm::value_ptr(view));

	auto r = ((double)rand() / (RAND_MAX)) + 1;

	glm::mat4 model;

	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); 
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f)); 
	model = glm::rotate(model, (float)m_value * 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(glGetUniformLocation(customShader->Program, "model_matrix"), 1, GL_FALSE, glm::value_ptr(model));

	// draw the graphics
	glDrawArrays(GL_TRIANGLES, 0, 36);
	// and display
	glFlush();
	SwapBuffers();
}

void MainCanvas::OnMouseEnter(wxMouseEvent& WXUNUSED(ev))
{
	SetFocus();
}

void MainCanvas::onKeyDown(wxKeyEvent& event)
{
	int code = event.GetKeyCode();
	
	if (code == 'z' || code == 'Z')
	{
		wxMessageBox(
			wxT("Key Z pressed. "),
			wxT("Test message box"),
			wxOK | wxICON_INFORMATION);

		cam->processKeyboard(FORWARD, 0.1);
	}

	if (code == 's' || code == 'S')
	{
		cam->processKeyboard(BACKWARD, 0.1);
	}

}
