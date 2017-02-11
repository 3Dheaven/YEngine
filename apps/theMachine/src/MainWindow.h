#pragma once
#include <wx/timer.h>
#include "../../../yengine/core/gl/CGLdriver.h"
#include "../../../yengine/core/vk/CVKdriver.h"
#include <wx/clrpicker.h>
#include "wx/wxprec.h"
#include "CVulkanCanvas.h"
#include "CGLCanvas.h"
#include "CRenderer.h"
#include "ERenderingWorkshop.h"

class CRenderer;
class CVulkanCanvas;

class MainWindow : public wxFrame
{
private:
	enum 
	{ 
		ID_Quit = wxID_HIGHEST + 1, 
		ID_Settings,
		ID_ColorPicker
	};

	CRenderer* mRenderer;
	CGraphicDriver* mGDriver;

public:
	wxPanel* mMainPanel;
	wxPanel* mBottomPanel;
	wxPanel* mRightPanel;
	bool colorHasChanged;
	wxColour color;
	wxTextCtrl* m_console;

	MainWindow(wxWindow* parent, const std::wstring& title, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize);
	virtual ~MainWindow();
	MainWindow(const MainWindow& tw) = delete;
	MainWindow(MainWindow&& tw) = delete;
	MainWindow& operator=(const MainWindow& tw) = delete;
	MainWindow& operator=(MainWindow&&) = delete;

	void OnQuit(wxCommandEvent& event);
	void OnSettings(wxCommandEvent& WXUNUSED(event));
	void onClose(wxCloseEvent& evt);

	wxTextCtrl *getDebugConsole();
	void OnColourChanged(wxColourPickerEvent& evt);
	
private:
	
	DECLARE_EVENT_TABLE()
};

