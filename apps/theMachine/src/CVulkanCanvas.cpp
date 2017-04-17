#include "CVulkanCanvas.h"


CVulkanCanvas::CVulkanCanvas(CVulkan* vk, wxWindow *pParent,
							 wxWindowID id,
							 const wxPoint& pos,
							 const wxSize& size,
							 long style,
							 const wxString& name) : wxWindow(pParent, id, pos, size, style, name)
{
	Bind(wxEVT_PAINT, &CVulkanCanvas::OnPaint, this);
	Bind(wxEVT_SIZE, &CVulkanCanvas::OnResize, this);
	Bind(wxEVT_TIMER, &CVulkanCanvas::onTimer, this);

	setupControls();

	m_timer = std::make_unique<wxTimer>(this, 1000 / 60);
	m_timer->Start(3);
	m_startTime = std::chrono::high_resolution_clock::now();

	mVulkan = vk;
	auto pw = GetHwnd();
	mVulkan->prepare(&pw, size);
	
}

CVulkanCanvas::~CVulkanCanvas() noexcept
{
	m_timer->Stop();

	Disconnect(wxEVT_LEFT_DOWN, wxMouseEventHandler(CVulkanCanvas::onMouseEvent), NULL, this);
	Disconnect(wxEVT_LEFT_UP, wxMouseEventHandler(CVulkanCanvas::onMouseEvent), NULL, this);
	Disconnect(wxEVT_MOTION, wxMouseEventHandler(CVulkanCanvas::onMouseEvent), NULL, this);
	Disconnect(wxEVT_MOUSEWHEEL, wxMouseEventHandler(CVulkanCanvas::onMouseWheel), NULL, this);
	Disconnect(wxEVT_KEY_DOWN, wxKeyEventHandler(CVulkanCanvas::onKeyDown), NULL, this);
}

HWND* 
CVulkanCanvas::getCanvasHandling()
{
	HWND pw = GetHwnd();
	return &pw;
}

void
CVulkanCanvas::setRenderer(CRenderer *renderer)
{
	mRenderer = renderer;
}

void CVulkanCanvas::onTimer(wxTimerEvent& event)
{
	ProcessEvent(wxPaintEvent());
}

void
CVulkanCanvas::setupControls()
{
	mLastMouseX = 300;
	mLastMouseY = 300;
	mFirstMouse = true;

	Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(CVulkanCanvas::onMouseEvent), NULL, this);
	Connect(wxEVT_LEFT_UP, wxMouseEventHandler(CVulkanCanvas::onMouseEvent), NULL, this);
	Connect(wxEVT_MOTION, wxMouseEventHandler(CVulkanCanvas::onMouseEvent), NULL, this);
	Connect(wxEVT_MOUSEWHEEL, wxMouseEventHandler(CVulkanCanvas::onMouseWheel), NULL, this);
	Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(CVulkanCanvas::onKeyDown), NULL, this);
}

void
CVulkanCanvas::onMouseEnter(wxMouseEvent& WXUNUSED(ev))
{
	SetFocus();
}

void
CVulkanCanvas::onKeyDown(wxKeyEvent& event)
{
	int code = event.GetKeyCode();
	auto cam = mRenderer->getCam();

	if (cam)
	{
		if (code == 'z' || code == 'Z')
		{
			cam->processKeyboard(FORWARD, 0.005);
		}

		if (code == 's' || code == 'S')
		{
			cam->processKeyboard(BACKWARD, 0.005);
		}

		if (code == 'd' || code == 'D')
		{
			cam->processKeyboard(RIGHT, 0.005);
		}

		if (code == 'q' || code == 'Q')
		{
			cam->processKeyboard(LEFT, 0.005);
		}
	}
}

void
CVulkanCanvas::onMouseWheel(wxMouseEvent& event)
{
	float value = 0;
	if (event.GetWheelRotation() > 0)
	{
		value += (event.GetWheelDelta() / 240.0);
	}
	else if (event.GetWheelRotation() < 0)
	{
		value -= (event.GetWheelDelta() / 240.0);
	}

	auto cam = mRenderer->getCam();
	if (cam)
	{
		cam->processMouseWheel(value);
	}
}

void
CVulkanCanvas::onMouseEvent(wxMouseEvent& event)
{
	if (event.Dragging() && event.ButtonIsDown(wxMOUSE_BTN_LEFT))
	{
		if (mFirstMouse)
		{
			mLastMouseX = event.GetX();
			mLastMouseY = event.GetY();
			mFirstMouse = false;
		}

		float xoffset = (float)(event.GetX() - mLastMouseX);
		float	yoffset = (float)(mLastMouseY - event.GetY());

		mLastMouseX = event.GetX();
		mLastMouseY = event.GetY();
		auto cam = mRenderer->getCam();

		if (cam)
		{
			cam->processMouseMovement(xoffset, yoffset);
		}
	}
	else
	{
		mFirstMouse = true;
	}
}

void CVulkanCanvas::OnPaint(wxPaintEvent& event)
{
	try
	{
		auto t_now = std::chrono::high_resolution_clock::now();
		auto time = std::chrono::duration_cast<std::chrono::microseconds>(t_now - m_startTime).count();

		mRenderer->render();
	}
	catch (const CVulkanException& ve) 
	{
		std::string status = ve.GetStatus();
		std::stringstream ss;
		ss << ve.what() << "\n" << status;
		CallAfter(&CVulkanCanvas::OnPaintException, ss.str());			// http://wxwidgets.blogspot.com/2013/01/about-benefits-of-procrastination.html
	}
	catch (const std::exception& err) 
	{
		std::stringstream ss;
		ss << "Error encountered trying to create the Vulkan canvas:\n";
		ss << err.what();
		CallAfter(&CVulkanCanvas::OnPaintException, ss.str());
	}
}

void CVulkanCanvas::OnResize(wxSizeEvent& event)
{
	wxSize size = GetSize();
	mVulkan->recreateSwapchain();
	wxRect refreshRect(size);
	RefreshRect(refreshRect, false);
}

void CVulkanCanvas::OnPaintException(const std::string& msg)
{
	wxMessageBox(msg, "Vulkan Error");
	wxTheApp->ExitMainLoop();
}

void CVulkanCanvas::resize(wxSize size)
{
	mVulkan->recreateSwapchain();
	wxRect refreshRect(size);
	RefreshRect(refreshRect, false);
}



