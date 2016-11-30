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

bool Application::OnInit()
{
	//wxFrame* mainFrame = new wxFrame(nullptr, wxID_ANY, L"YEngine");

	MainWindow* mainFrame = new MainWindow(nullptr, L"YEngine!");
	mainFrame->Show(true);


	return true;
}

wxIMPLEMENT_APP(Application);