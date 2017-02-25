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
		ID_QUIT = wxID_HIGHEST + 1,
		ID_DISPLAY_SETTINGS,
		ID_ColorPicker,
		ID_DISPLAY_CONSOLE
	};

	enum E_API3D
	{
		API_VULKAN,
		API_OPENGL
	};

	CRenderer* mRenderer;
	CGraphicDriver* mGDriver;
	CVulkanCanvas* vcanvas;
	CGLCanvas* glcanvas;
	E_API3D gApi;
	wxPanel *mMainPanel;
	bool mInitDone;

public:

	bool colorHasChanged;
	wxColour color;
	wxTextCtrl* m_console;

	MainWindow(wxWindow* parent, const std::wstring& title, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize);
	virtual ~MainWindow();
	MainWindow(const MainWindow& tw) = delete;
	MainWindow(MainWindow&& tw) = delete;
	MainWindow& operator=(const MainWindow& tw) = delete;
	MainWindow& operator=(MainWindow&&) = delete;

	

	wxTextCtrl *getDebugConsole();
	void OnColourChanged(wxColourPickerEvent& evt);
	
private:

	wxFrame *mConsoleWindow;
	wxFrame *mSettingsWindow;

	wxMenuBar *menubar;
	wxMenu *fileMenu;
	wxMenu *viewMenu;
	wxMenuItem* viewItems;

	void createMenuBar();
	void OnDisplayConsoleCheckbox(wxCommandEvent& event);
	void OnDisplaySettingsCheckbox(wxCommandEvent& event);
	void OnQuit(wxCommandEvent& event);
	void onClose(wxCloseEvent& evt);
	void OnResize(wxSizeEvent& event);
	
	DECLARE_EVENT_TABLE()
};

