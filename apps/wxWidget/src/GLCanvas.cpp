#include "GLCanvas.h"

BEGIN_EVENT_TABLE(GLCanvas, wxGLCanvas)
EVT_ENTER_WINDOW(GLCanvas::onMouseEnter)
END_EVENT_TABLE()

GLCanvas::GLCanvas(wxWindow* parent, wxWindowID id,
	const int* attribList, const wxPoint& pos, const wxSize& size,
	long style, const wxString& name, const wxPalette& palette)
	: wxGLCanvas(parent, id, attribList, pos, size, style, name, palette)
{
	m_context = std::make_unique<wxGLContext>(this);
	Bind(wxEVT_PAINT, &GLCanvas::onPaint, this);
	Bind(wxEVT_TIMER, &GLCanvas::onTimer, this);

	SetCurrent(*m_context);
	initializeGLEW();
	setupGraphics();
	setupControls();

	m_timer = std::make_unique<wxTimer>(this, TIMERNUMBER);
	m_timer->Start(INTERVAL);
	m_startTime = std::chrono::high_resolution_clock::now();
}

GLCanvas::~GLCanvas()
{
	SetCurrent(*m_context);
	m_timer->Stop();

	Disconnect(wxEVT_LEFT_DOWN, wxMouseEventHandler(GLCanvas::onMouseEvent), NULL, this);
	Disconnect(wxEVT_LEFT_UP, wxMouseEventHandler(GLCanvas::onMouseEvent), NULL, this);
	Disconnect(wxEVT_MOTION, wxMouseEventHandler(GLCanvas::onMouseEvent), NULL, this);
	Disconnect(wxEVT_MOUSEWHEEL, wxMouseEventHandler(GLCanvas::onMouseWheel), NULL, this);
	Disconnect(wxEVT_KEY_DOWN, wxKeyEventHandler(GLCanvas::onKeyDown), NULL, this);

	delete m_cam;
	delete m_customShader;
	delete m_model;
}

void 
GLCanvas::initializeGLEW()
{
	glewExperimental = true;
	GLenum err = glewInit();
	if (err != GLEW_OK) 
	{
		const GLubyte* msg = glewGetErrorString(err);
		throw std::exception(reinterpret_cast<const char*>(msg));
	}
}

void 
GLCanvas::onTimer(wxTimerEvent& event)
{
	ProcessEvent(wxPaintEvent());
}

void 
GLCanvas::setupGraphics()
{
	m_customShader = new CShaderFactory("../shaders/shader.vert",
										"../shaders/shader.frag");

	m_cam = new CCamera(glm::vec3(0.0f, 0.0f, 5.0f), 
						glm::vec3(0.0f, 0.0f, 0.0f), 
						glm::vec3(0.0f, 1.0f, 0.0f));

	m_model = new CModel("..//..//..//media//teapot.obj");
}

void 
GLCanvas::setupControls()
{
	m_lastMouseX = 300;
	m_lastMouseY = 300;
	m_firstMouse = true;

	Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(GLCanvas::onMouseEvent), NULL, this);
	Connect(wxEVT_LEFT_UP, wxMouseEventHandler(GLCanvas::onMouseEvent), NULL, this);
	Connect(wxEVT_MOTION, wxMouseEventHandler(GLCanvas::onMouseEvent), NULL, this);
	Connect(wxEVT_MOUSEWHEEL, wxMouseEventHandler(GLCanvas::onMouseWheel), NULL, this);
	Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(GLCanvas::onKeyDown), NULL, this);
}

void 
GLCanvas::onPaint(wxPaintEvent& event)
{
	SetCurrent(*m_context);
	glClearColor(0.5, 0.5, 0.5, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	auto t_now = std::chrono::high_resolution_clock::now();
	auto time = std::chrono::duration_cast<std::chrono::microseconds>(t_now - m_startTime).count();

	m_customShader->shader->use();

	m_cam->projMatrix = glm::perspective(m_cam->getZoom(), (float)600 / (float)600, 0.1f, 100.0f);
	m_customShader->shader->setUniform("projection_matrix", m_cam->projMatrix);
	m_customShader->shader->setUniform("view_matrix", m_cam->getViewMatrix());
	m_customShader->shader->setUniform("custom_color", glm::vec3((sin(time * 1.0f) + 1.0f) / 2.0f, (sin(time * 0.5f) + 1.0f) / 2.0f, (cos(time * 0.25f) + 1.0f) / 2.0f));

	glm::mat4 model;
	//glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); 
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f)); 
	//model = glm::rotate(model, (float)time * 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	m_customShader->shader->setUniform("model_matrix", model);

	// draw the graphics
	m_model->render(m_customShader);

	// and display
	glFlush();
	SwapBuffers();
}

void 
GLCanvas::onMouseEnter(wxMouseEvent& WXUNUSED(ev))
{
	SetFocus();
}

void 
GLCanvas::onKeyDown(wxKeyEvent& event)
{
	int code = event.GetKeyCode();
	
	if (code == 'z' || code == 'Z')
	{
		m_cam->processKeyboard(FORWARD, 0.001);
	}

	if (code == 's' || code == 'S')
	{
		m_cam->processKeyboard(BACKWARD, 0.001);
	}

	if (code == 'd' || code == 'D')
	{
		m_cam->processKeyboard(RIGHT, 0.001);
	}

	if (code == 'q' || code == 'Q')
	{
		m_cam->processKeyboard(LEFT, 0.001);
	}
}

void 
GLCanvas::onMouseWheel(wxMouseEvent& event)
{
	float value = 0;
	if(event.GetWheelRotation() > 0) 
	{
		value += (event.GetWheelDelta() / 240.0);
	}
	else if(event.GetWheelRotation() < 0) 
	{
		value -= (event.GetWheelDelta() / 240.0);
	}

	m_cam->processMouseWheel(value);
}

void 
GLCanvas::onMouseEvent(wxMouseEvent& event)
{
	if (event.Dragging() && event.ButtonIsDown(wxMOUSE_BTN_LEFT)) 
	{
		if (m_firstMouse)
		{
			m_lastMouseX = event.GetX();
			m_lastMouseY = event.GetY();
			m_firstMouse = false;
		}

		GLfloat xoffset = (float)(event.GetX() - m_lastMouseX) ;
		GLfloat	yoffset = (float)(m_lastMouseY - event.GetY());

		m_lastMouseX = event.GetX();
		m_lastMouseY = event.GetY();

		m_cam->processMouseMovement(xoffset, yoffset);
	}
	else
	{
		m_firstMouse = true;
	}
}