#include "MainCanvas.h"

BEGIN_EVENT_TABLE(MainCanvas, wxGLCanvas)
EVT_ENTER_WINDOW(MainCanvas::OnMouseEnter)
END_EVENT_TABLE()

MainCanvas::MainCanvas(wxWindow* parent, wxWindowID id,
	const int* attribList, const wxPoint& pos, const wxSize& size,
	long style, const wxString& name, const wxPalette& palette)
	: wxGLCanvas(parent, id, attribList, pos, size, style, name, palette)
{
	m_context = std::make_unique<wxGLContext>(this);
	Bind(wxEVT_PAINT, &MainCanvas::OnPaint, this);
	Bind(wxEVT_TIMER, &MainCanvas::OnTimer, this);

	SetCurrent(*m_context);
	InitializeGLEW();
	SetupGraphics();
	customShader = new Shader("../shaders/shader.vert", "../shaders/shader.frag");
	cam = new camera(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	m_timer = std::make_unique<wxTimer>(this, TIMERNUMBER);
	m_timer->Start(INTERVAL);
	m_startTime = std::chrono::high_resolution_clock::now();

	Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(MainCanvas::OnMouseEvent), NULL, this);
	Connect(wxEVT_LEFT_UP, wxMouseEventHandler(MainCanvas::OnMouseEvent), NULL, this);
	Connect(wxEVT_MOTION, wxMouseEventHandler(MainCanvas::OnMouseEvent), NULL, this);
	Connect(wxEVT_MOUSEWHEEL, wxMouseEventHandler(MainCanvas::OnMouseWheel), NULL, this);
	Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(MainCanvas::onKeyDown), NULL, this);

	lastX = 300;
	lastY = 300;
	firstMouse = true;
}

MainCanvas::~MainCanvas()
{
	SetCurrent(*m_context);
	m_timer->Stop();

	Disconnect(wxEVT_LEFT_DOWN, wxMouseEventHandler(MainCanvas::OnMouseEvent), NULL, this);
	Disconnect(wxEVT_LEFT_UP, wxMouseEventHandler(MainCanvas::OnMouseEvent), NULL, this);
	Disconnect(wxEVT_MOTION, wxMouseEventHandler(MainCanvas::OnMouseEvent), NULL, this);
	Disconnect(wxEVT_MOUSEWHEEL, wxMouseEventHandler(MainCanvas::OnMouseWheel), NULL, this);
	Disconnect(wxEVT_KEY_DOWN, wxKeyEventHandler(MainCanvas::onKeyDown), NULL, this);
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

void MainCanvas::OnTimer(wxTimerEvent& event)
{
	ProcessEvent(wxPaintEvent());
}

void MainCanvas::SetupGraphics()
{
	obj = new object("..//..//..//media//teapot.obj");
	
	obj->loadOBJ();
	obj->initBuffer();

}

void MainCanvas::OnPaint(wxPaintEvent& event)
{
	SetCurrent(*m_context);
	glClearColor(0.5, 0.5, 0.5, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	customShader->Use();

	// Transformation matrices
	glm::mat4 projection = glm::perspective(cam->getZoom(), (float)800 / (float)800, 0.1f, 100.0f);
	glm::mat4 view = cam->getViewMatrix();
	glUniformMatrix4fv(glGetUniformLocation(customShader->Program, "projection_matrix"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(customShader->Program, "view_matrix"), 1, GL_FALSE, glm::value_ptr(view));

	auto t_now = std::chrono::high_resolution_clock::now();
	auto time = std::chrono::duration_cast<std::chrono::microseconds>(t_now - m_startTime).count();
	glUniform3f(glGetUniformLocation(customShader->Program, "custom_color"), (sin(time * 1.0f) + 1.0f) / 2.0f, (sin(time * 0.5f) + 1.0f) / 2.0f,
		(cos(time * 0.25f) + 1.0f) / 2.0f);

	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); 
	model = glm::scale(model, glm::vec3(0.001f, .001f, .001f)); 
	//model = glm::rotate(model, (float)time * 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(glGetUniformLocation(customShader->Program, "model_matrix"), 1, GL_FALSE, glm::value_ptr(model));

	// draw the graphics
	obj->render();
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
		cam->processKeyboard(FORWARD, 0.001);
	}

	if (code == 's' || code == 'S')
	{
		cam->processKeyboard(BACKWARD, 0.001);
	}

	if (code == 'd' || code == 'D')
	{
		cam->processKeyboard(RIGHT, 0.001);
	}

	if (code == 'q' || code == 'Q')
	{
		cam->processKeyboard(LEFT, 0.001);
	}
}

void MainCanvas::OnMouseWheel(wxMouseEvent& event)
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

	cam->processMouseWheel(value);
}

void MainCanvas::OnMouseEvent(wxMouseEvent& event)
{
	if (event.Dragging() && event.ButtonIsDown(wxMOUSE_BTN_LEFT)) 
	{

		if (firstMouse)
		{
			lastX = event.GetX();
			lastY = event.GetY();
			firstMouse = false;
		}

		GLfloat xoffset = (float)(event.GetX() - lastX) ;
		GLfloat	yoffset = (float)(lastY - event.GetY());
		lastX = event.GetX();
		lastY = event.GetY();

		cam->processMouseMovement(xoffset, yoffset);
	}
}