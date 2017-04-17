#include <wx/wxprec.h>
#include "Application.h"
#include "MainWindow.h"

#ifdef _UNICODE
#ifdef _DEBUG
#pragma comment(lib, "wxbase31ud.lib")
#else
#pragma comment(lib, "wxbase31u.lib")
#endif
#else
#ifdef _DEBUG
#pragma comment(lib, "wxbase31d.lib")
#else
#pragma comment(lib, "wxbase31.lib")
#endif
#endif

Application::Application()
{
}

Application::~Application()
{
}

bool 
Application::OnInit()
{
	MainWindow* mainFrame = new MainWindow(	nullptr, 
											L"TheMachine v1.0", 
											wxDefaultPosition, 
											wxSize(1000,800));

	mainFrame->SetIcon(wxIcon(wxT("iconapp.jpg")));
	mainFrame->Show(true);
	return true;
}

wxIMPLEMENT_APP(Application); 
