#include "MainWindow.h"

MainWindow::MainWindow(wxWindow* parent, const std::wstring& title, const wxPoint& pos, const wxSize& size)
	: wxFrame(parent, wxID_ANY, title, pos, size, wxMINIMIZE_BOX | wxMAXIMIZE_BOX| wxCLOSE_BOX | wxSYSTEM_MENU | wxCAPTION | wxCLIP_CHILDREN | wxRESIZE_BORDER)
{
	// Display current window on screen center
	Centre();

	// This panel contains graphics canvas (GL or VK)
	mMainPanel = new wxPanel(this, wxID_ANY);
	
	// All output to cout goes into this window
	mLogWindow = new CLogWindow(this);
	wxLog::SetActiveTarget(new wxLogTextCtrl(mLogWindow->getOutputLogPanel()));
	wxStreamToTextRedirector redirect(mLogWindow->getOutputLogPanel());

	// Create module example settings window
	mSettingsWindow = new CSettingsWindow(this);
	
	// Create a menu for this main window
	createMenuBar();
	
	// The renderer manager handles creation of module example by using vk or gl.
	mRendererManager = new CRendererManager(this);

	Bind(wxEVT_SIZE, &MainWindow::OnResize, this);
	Bind(wxEVT_CLOSE_WINDOW, &MainWindow::onClose, this);
}

MainWindow::~MainWindow()
{
	if (mLogWindow != NULL)
	{
		delete mLogWindow;
	}

	if (mLogWindow != NULL)
	{
		delete mSettingsWindow;
	}

	if (mLogWindow != NULL)
	{
		delete mRendererManager;
	}
}

CSettingsWindow*
MainWindow::getSettingsWindow()
{
	return mSettingsWindow;
}

void 
MainWindow::OnResize(wxSizeEvent& event)
{
	mMainPanel->SetSize(GetSize());

	// Redirect resize event
	mRendererManager->onResize();
}

void 
MainWindow::OnQuit(wxCommandEvent& WXUNUSED(event))
{
	Close(true);
}

void 
MainWindow::onClose(wxCloseEvent& evt)
{
	evt.Skip();
}

void MainWindow::createMenuBar()
{
	menubar = new wxMenuBar;

	// "File" in menu bar
	fileMenu = new wxMenu();
	fileMenu->Append(ID_QUIT, wxT("&Quit"));
	menubar->Append(fileMenu, wxT("&File"));

	// "View" in menu bar
	viewMenu = new wxMenu();
	menubar->Append(viewMenu, wxT("&View"));

	viewItems = viewMenu->AppendCheckItem(ID_DISPLAY_LOG, "Display console");
	if (mLogWindow->getWindow()->IsVisible())
	{
		viewItems->Check();
	}
	viewItems = viewMenu->AppendCheckItem(ID_DISPLAY_SETTINGS, "Display settings");
	if (mSettingsWindow->getWindow()->IsVisible())
	{
		viewItems->Check();
	}

	SetMenuBar(menubar);
	SetFocus();
	
	Connect(wxID_EXIT, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MainWindow::OnQuit));
	Connect(ID_QUIT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainWindow::OnQuit));
	Connect(ID_DISPLAY_LOG,  wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainWindow::OnDisplayLogWindowCheckbox));
	Connect(ID_DISPLAY_SETTINGS, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainWindow::OnDisplaySettingsWindowCheckbox));
}

void
MainWindow::OnDisplayLogWindowCheckbox(wxCommandEvent& event)
{
	wxMenuItem* menuItem = menubar->FindItem(ID_DISPLAY_LOG);

	if (menuItem->IsChecked())
	{
		mLogWindow->getWindow()->Show(true);
	}
	else
	{
		mLogWindow->getWindow()->Show(false);
	}
}

void
MainWindow::OnDisplaySettingsWindowCheckbox(wxCommandEvent& event)
{
	wxMenuItem* menuItem = menubar->FindItem(ID_DISPLAY_SETTINGS);

	if (menuItem->IsChecked())
	{
		mSettingsWindow->getWindow()->Show(true);
	}
	else
	{
		mSettingsWindow->getWindow()->Show(false);
	}
}
