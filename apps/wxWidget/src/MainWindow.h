#pragma once
#include <wx/timer.h>
#include "RenderTimer.h"

class MainWindow : public wxFrame
{
private:
	enum 
	{ 
		ID_Quit = wxID_HIGHEST + 1, 
		ID_Settings
	};

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

private:
	wxTextCtrl* m_console;
	DECLARE_EVENT_TABLE()
};

