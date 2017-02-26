#include "CLogWindow.h"

CLogWindow::CLogWindow(wxWindow* parent)
{
	// Create console window
	mLogWindow = new wxFrame(parent, wxID_ANY, "Logger", { parent->GetPosition().x - 600, parent->GetPosition().y }, { 600, 300 }, wxMINIMIZE_BOX | wxCAPTION | wxCLIP_CHILDREN);
	logMainPanel = new wxPanel(mLogWindow, wxID_ANY, { 0,0 }, { 600, 300 });
	mLogPages = new wxAuiNotebook(logMainPanel, wxID_ANY, { 0,0 }, { 585, 260 }, wxAUI_NB_TAB_EXTERNAL_MOVE | wxAUI_NB_TAB_MOVE | wxAUI_NB_TAB_SPLIT);

	// Output tab
	mLogPagePanel = new wxPanel(mLogPages, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
	wxBoxSizer* sizer1;
	sizer1 = new wxBoxSizer(wxVERTICAL);
	mLog = new wxTextCtrl(mLogPagePanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_DONTWRAP | wxTE_MULTILINE | wxTE_READONLY | wxTE_RICH);
	mLog->SetFont(wxFont(wxNORMAL_FONT->GetPointSize(), 75, 90, 90, false, wxEmptyString));
	mLog->SetForegroundColour(wxColour(255, 255, 255));
	mLog->SetBackgroundColour(wxColour(0, 0, 0));
	sizer1->Add(mLog, 1, wxEXPAND, 0);
	mLogPagePanel->SetSizer(sizer1);
	mLogPagePanel->Layout();
	sizer1->Fit(mLogPagePanel);
	mLogPages->AddPage(mLogPagePanel, wxT("Output"), false, wxNullBitmap);

	// Notifications tab
	mNotificationLogPage = new wxPanel(mLogPages, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
	wxBoxSizer* sizer2;
	sizer2 = new wxBoxSizer(wxVERTICAL);
	mNotificationLog = new wxTextCtrl(mNotificationLogPage, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_DONTWRAP | wxTE_MULTILINE | wxTE_READONLY | wxTE_RICH);
	mNotificationLog->SetFont(wxFont(wxNORMAL_FONT->GetPointSize(), 75, 90, 90, false, wxEmptyString));
	mNotificationLog->SetForegroundColour(wxColour(255, 255, 255));
	mNotificationLog->SetBackgroundColour(wxColour(0, 0, 0));
	sizer2->Add(mNotificationLog, 1, wxEXPAND, 0);
	mNotificationLogPage->SetSizer(sizer2);
	mNotificationLogPage->Layout();
	sizer2->Fit(mNotificationLogPage);
	mLogPages->AddPage(mNotificationLogPage, wxT("Notifications"), false, wxNullBitmap);

	mLogWindow->Show(true);
}

CLogWindow::~CLogWindow()
{
	
}

wxFrame* 
CLogWindow::getWindow()
{
	return mLogWindow;
}

wxTextCtrl* 
CLogWindow::getOutputLogPanel()
{
	return mLog;
}