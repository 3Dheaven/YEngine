#pragma once

#include <memory>
#include "wx/wxprec.h"
#include <wx/combobox.h>
#include <wx/aui/auibook.h>

class CSettingsWindow
{
	wxFrame *mSettingsWindow;

	// Panel which can be changed by examples modules
	wxPanel *mDynamicPanel;

public:
	CSettingsWindow(wxWindow* parent);
	~CSettingsWindow();

	wxFrame* getWindow();
	wxPanel* getDynamicPanel();
};