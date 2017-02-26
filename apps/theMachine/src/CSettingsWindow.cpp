#include "CSettingsWindow.h"

CSettingsWindow::CSettingsWindow(wxWindow* parent)
{
	mSettingsWindow = new wxFrame(parent, wxID_ANY, "Settings", { parent->GetPosition().x + parent->GetSize().x,  parent->GetPosition().y }, { 300, 800 }, wxMINIMIZE_BOX | wxCAPTION | wxCLIP_CHILDREN);
	mDynamicPanel = new wxPanel(mSettingsWindow, wxID_ANY, { 0,0 }, { 300, 800 });
	mDynamicPanel->SetBackgroundColour(wxT("#ededed"));
	mSettingsWindow->Show(true);
}

CSettingsWindow::~CSettingsWindow()
{
	
}

wxFrame*
CSettingsWindow::getWindow()
{
	return mSettingsWindow;
}

wxPanel* 
CSettingsWindow::getDynamicPanel()
{
	return mDynamicPanel;
}