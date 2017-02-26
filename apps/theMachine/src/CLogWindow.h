#pragma once

#include <memory>
#include "wx/wxprec.h"
#include <wx/combobox.h>
#include <wx/aui/auibook.h>

class CLogWindow
{
public:
	CLogWindow(wxWindow* parent);
	~CLogWindow();
	
	wxFrame* getWindow();
	wxTextCtrl* getOutputLogPanel();

private:

	wxFrame *mLogWindow;
	wxPanel *logMainPanel;
	wxPanel *settingsMainPanel;
	wxAuiNotebook *mLogPages;
	wxPanel *mLogPagePanel;
	wxTextCtrl *mLog;
	wxPanel *mNotificationLogPage;
	wxTextCtrl *mNotificationLog;
	

};