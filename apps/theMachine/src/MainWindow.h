#pragma once
#include "wx/wxprec.h"
#include "CLogWindow.h"
#include "CSettingsWindow.h"
#include "CRendererManager.h"

class CRendererManager;

class MainWindow : public wxFrame
{
private:
	enum 
	{ 
		ID_QUIT = wxID_HIGHEST + 1,
		ID_DISPLAY_SETTINGS,
		ID_DISPLAY_LOG
	};

	CLogWindow *mLogWindow;
	CSettingsWindow *mSettingsWindow;
	CRendererManager *mRendererManager;

	// Menu bar
	wxMenuBar *menubar;
	wxMenu *fileMenu;
	wxMenu *viewMenu;
	wxMenuItem* viewItems;

	void createMenuBar();
	void OnDisplayLogWindowCheckbox(wxCommandEvent& event);
	void OnDisplaySettingsWindowCheckbox(wxCommandEvent& event);

	// Current (main) window events
	void OnQuit(wxCommandEvent& event);
	void onClose(wxCloseEvent& evt);
	void OnResize(wxSizeEvent& event);

public:
	wxPanel* mMainPanel;
	
	CSettingsWindow* getSettingsWindow();

	MainWindow(wxWindow* parent, const std::wstring& title, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize);
	virtual ~MainWindow();
	MainWindow(const MainWindow& tw) = delete;
	MainWindow(MainWindow&& tw) = delete;
	MainWindow& operator=(const MainWindow& tw) = delete;
	MainWindow& operator=(MainWindow&&) = delete;
};

