#pragma once
#include "wx/wxprec.h"
#include "VulkanCanvas.h"

class VulkanWindow :
    public wxFrame
{
private:
	enum
	{
		ID_Quit = wxID_HIGHEST + 1,
		ID_Settings,
		ID_ColorPicker
	};

public:
    VulkanWindow(wxWindow* parent, const wxString &title, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize);
    virtual ~VulkanWindow();

	void OnQuit(wxCommandEvent& event);
	void OnSettings(wxCommandEvent& WXUNUSED(event));
	void onClose(wxCloseEvent& evt);

	wxTextCtrl *getDebugConsole();
	bool colorHasChanged;
	wxColour color;
	wxPanel *m_parent;
	wxPanel * bottomPanel;
	wxPanel * rightPanel;

private:
    void OnResize(wxSizeEvent& event);
    VulkanCanvas* m_canvas;
	wxTextCtrl* m_console;
	void OnColourChanged(wxColourPickerEvent& evt);
	DECLARE_EVENT_TABLE()
};

