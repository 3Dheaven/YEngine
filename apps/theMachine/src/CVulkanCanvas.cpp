#include "CVulkanCanvas.h"


CVulkanCanvas::CVulkanCanvas(wxWindow *pParent,
							 wxWindowID id,
							 const wxPoint& pos,
							 const wxSize& size,
							 long style,
							 const wxString& name) : wxWindow(pParent, id, pos, size, style, name)
{
	Bind(wxEVT_PAINT, &CVulkanCanvas::OnPaint, this);
	Bind(wxEVT_SIZE, &CVulkanCanvas::OnResize, this);
	Bind(wxEVT_TIMER, &CVulkanCanvas::onTimer, this);

	m_timer = std::make_unique<wxTimer>(this, 1000 / 60);
	m_timer->Start(3);
	m_startTime = std::chrono::high_resolution_clock::now();

	auto pw = GetHwnd();
	mVulkan.prepare(&pw, size);
}

CVulkanCanvas::~CVulkanCanvas() noexcept
{
	m_timer->Stop();
}

void CVulkanCanvas::onTimer(wxTimerEvent& event)
{
	ProcessEvent(wxPaintEvent());
}

void CVulkanCanvas::OnPaint(wxPaintEvent& event)
{
	auto t_now = std::chrono::high_resolution_clock::now();
	auto time = std::chrono::duration_cast<std::chrono::microseconds>(t_now - m_startTime).count();

	mVulkan.render();
}

void CVulkanCanvas::OnResize(wxSizeEvent& event)
{
	wxSize size = GetSize();
	mVulkan.recreateSwapchain();
	wxRect refreshRect(size);
	RefreshRect(refreshRect, false);
}

void CVulkanCanvas::OnPaintException(const std::string& msg)
{
	wxMessageBox(msg, "Vulkan Error");
	wxTheApp->ExitMainLoop();
}


