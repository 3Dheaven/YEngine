#pragma once

#include "wx/wxprec.h"
#include <wx/timer.h>
#include <string>
#include <chrono>
#include "glm.hpp"
#include "CRenderer.h"
#include "CVulkan.h"
#include "MainWindow.h"

class CVulkanCanvas :
    public wxWindow
{
public:
	
	CVulkanCanvas(CVulkan* vk, wxWindow *pParent,
        wxWindowID id = wxID_ANY,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = 0,
        const wxString& name = "VulkanCanvasName");

    virtual ~CVulkanCanvas() noexcept;
	void setRenderer(CRenderer *renderer);

	HWND* getCanvasHandling();
	wxWindow *m_pParent;

private:

	CVulkan* mVulkan;
	CRenderer* mRenderer;

	std::unique_ptr<wxTimer> m_timer;
	typedef std::chrono::time_point<std::chrono::high_resolution_clock> sclock;
	sclock m_startTime;

	virtual void OnPaint(wxPaintEvent& event);
	virtual void OnResize(wxSizeEvent& event);
	virtual void onTimer(wxTimerEvent& event);
	void OnPaintException(const std::string& msg);
};

