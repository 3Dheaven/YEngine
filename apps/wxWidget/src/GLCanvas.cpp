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

	setupControls();

	mTimer = std::make_unique<wxTimer>(this, TIMERNUMBER);
	mTimer->Start(INTERVAL);
	mStartTime = std::chrono::high_resolution_clock::now();
}

GLCanvas::~GLCanvas()
{
	SetCurrent(*m_context);
	mTimer->Stop();

	Disconnect(wxEVT_LEFT_DOWN, wxMouseEventHandler(GLCanvas::onMouseEvent), NULL, this);
	Disconnect(wxEVT_LEFT_UP, wxMouseEventHandler(GLCanvas::onMouseEvent), NULL, this);
	Disconnect(wxEVT_MOTION, wxMouseEventHandler(GLCanvas::onMouseEvent), NULL, this);
	Disconnect(wxEVT_MOUSEWHEEL, wxMouseEventHandler(GLCanvas::onMouseWheel), NULL, this);
	Disconnect(wxEVT_KEY_DOWN, wxKeyEventHandler(GLCanvas::onKeyDown), NULL, this);

}

void 
GLCanvas::setGModule(CRenderer *renderer)
{
	mGModule = renderer;
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
GLCanvas::setupControls()
{
	mLastMouseX = 300;
	mLastMouseY = 300;
	mFirstMouse = true;

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
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	//glCullFace(GL_BACK);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	auto t_now = std::chrono::high_resolution_clock::now();
	auto time = std::chrono::duration_cast<std::chrono::microseconds>(t_now - mStartTime).count();

	mGModule->render();

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
	auto cam = mGModule->getCam();

	if (cam != NULL)
	{
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

	auto cam = mGModule->getCam();
	if (cam != NULL)
	{
		cam->processMouseWheel(value);
	}
}

void 
GLCanvas::onMouseEvent(wxMouseEvent& event)
{
	if (event.Dragging() && event.ButtonIsDown(wxMOUSE_BTN_LEFT)) 
	{
		if (mFirstMouse)
		{
			mLastMouseX = event.GetX();
			mLastMouseY = event.GetY();
			mFirstMouse = false;
		}

		GLfloat xoffset = (float)(event.GetX() - mLastMouseX) ;
		GLfloat	yoffset = (float)(mLastMouseY - event.GetY());

		mLastMouseX = event.GetX();
		mLastMouseY = event.GetY();

		auto cam = mGModule->getCam();

		if (cam != NULL)
		{
			cam->processMouseMovement(xoffset, yoffset);
		}
	}
	else
	{
		mFirstMouse = true;
	}
}