#pragma once
#include <wx/timer.h>
#include "../../../yengine/core/CGraphicDriver.h"

class CRenderer;

class MainWindow : public wxFrame
{
private:
	enum 
	{ 
		ID_Quit = wxID_HIGHEST + 1, 
		ID_Settings
	};

	CRenderer* mRenderer;
	CGraphicDriver *mGDriver;

public:
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

private:
	wxTextCtrl* m_console;
	DECLARE_EVENT_TABLE()
};

