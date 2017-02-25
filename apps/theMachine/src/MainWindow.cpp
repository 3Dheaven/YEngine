#include "MainWindow.h"

const int mainCanvasID = 2000;			// TriangleCanvas widget ID

BEGIN_EVENT_TABLE(MainWindow, wxFrame)
EVT_CLOSE(MainWindow::onClose)
EVT_COLOURPICKER_CHANGED(ID_ColorPicker, MainWindow::OnColourChanged)
END_EVENT_TABLE()


MainWindow::MainWindow(wxWindow* parent, const std::wstring& title, const wxPoint& pos, const wxSize& size)
	: wxFrame(parent, wxID_ANY, title, pos, size, wxMINIMIZE_BOX | wxMAXIMIZE_BOX| wxCLOSE_BOX | wxSYSTEM_MENU | wxCAPTION | wxCLIP_CHILDREN | wxRESIZE_BORDER)
{
	// Display MainWindow on screen center
	Centre();

	mMainPanel = new wxPanel(this, wxID_ANY);
	moduleChoices = new wxComboBox(mMainPanel, ID_MODULES_COMBOBOX, "", { GetSize().GetWidth() - 215,0 }, { 200,20 });
	moduleChoices->AppendString("OBJ_LOADER");
	moduleChoices->AppendString("TERRAIN_CDLOD");
	moduleChoices->SetStringSelection("TERRAIN_CDLOD");
	
	// Create console window
	mConsoleWindow = new wxFrame(this, wxID_ANY, "Console", { GetPosition().x - 600, GetPosition().y }, { 600, 300 }, wxMINIMIZE_BOX |  wxCAPTION | wxCLIP_CHILDREN);
	consoleMainPanel = new wxPanel(mConsoleWindow, wxID_ANY, { 0,0 }, { 600, 300 });
	consoleMainPanel->SetBackgroundColour(wxT("#000000"));
	mConsoleWindow->Show(true);

	// Create custom settings window
	mSettingsWindow = new wxFrame(this, wxID_ANY, "Settings", { GetPosition().x + size.x,  GetPosition().y }, { 300, 800 }, wxMINIMIZE_BOX | wxCAPTION | wxCLIP_CHILDREN);
	settingsMainPanel = new wxPanel(mSettingsWindow, wxID_ANY, { 0,0 }, { 300, 800 });
	settingsMainPanel->SetBackgroundColour(wxT("#ededed"));
	//m_console = new wxTextCtrl(consoleMainPanel, wxID_ANY, wxEmptyString, { 0, 0 }, { 600, 300 }, wxTE_MULTILINE | wxTE_READONLY);
	//wxLog::SetActiveTarget(new wxLogTextCtrl(m_console));
	// All output to cout goes into the text control until the exit from current scope
	// wxStreamToTextRedirector redirect(m_console);
	mSettingsWindow->Show(true);

	createMenuBar();

	mGDriver = NULL;
	mRenderer = NULL;

	gApi = API_OPENGL;
	E_MODULES_EXAMPLES ex = TERRAIN;//OBJ_LOADER;
	switch (gApi)
	{
		case API_VULKAN: 
			{
				vcanvas = new CVulkanCanvas(mMainPanel, wxID_ANY, { 0, 0 }, GetSize());
				/*mGDriver = dynamic_cast<CGraphicDriver *>(new CVKDriver());
				mRenderer = new CRenderer(mGDriver);
				glcanvas->setGModule(mRenderer);*/
			}
			break;

		case API_OPENGL: 
			{
				glcanvas = new CGLCanvas(mMainPanel, mainCanvasID, nullptr, { 0, 0 }, GetSize(), wxFULL_REPAINT_ON_RESIZE);
				mGDriver = new CGLDriver();
				mRenderer = new CRenderer(mGDriver, ex, settingsMainPanel);
				glcanvas->setGModule(mRenderer);
			}
			break;
	}



	
	// Create a wxColourPickerCtrl control
	colorHasChanged = false;
	wxColourPickerCtrl* colourPickerCtrl = new wxColourPickerCtrl(settingsMainPanel, ID_ColorPicker, wxStockGDI::COLOUR_RED, { 20, 20 });
	colourPickerCtrl->Hide();
	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	
	Bind(wxEVT_SIZE, &MainWindow::OnResize, this);
	Connect(ID_MODULES_COMBOBOX, wxEVT_COMBOBOX, wxCommandEventHandler(MainWindow::OnModulesCombobox));
}

MainWindow::~MainWindow()
{
	if (mRenderer != NULL)
	{
		delete mRenderer;
		mRenderer = nullptr;
	}

	if (mGDriver != NULL)
	{
		delete mGDriver;
		mGDriver = nullptr;
	}
}

void 
MainWindow::OnResize(wxSizeEvent& event)
{
	moduleChoices->SetPosition({GetSize().GetWidth() - 215,0 });

	if (gApi == API_OPENGL && glcanvas != NULL)
	{
		mMainPanel->SetSize(GetSize());
		glcanvas->SetSize(GetSize());
		glcanvas->resize();
	}
}

wxTextCtrl* MainWindow::getDebugConsole()
{
	return m_console;
}

void 
MainWindow::OnQuit(wxCommandEvent& WXUNUSED(event))
{
	Close(true);
}

void
MainWindow::OnDisplayConsoleCheckbox(wxCommandEvent& event)
{
	wxMenuItem* menuItem = menubar->FindItem(ID_DISPLAY_CONSOLE);

	if (menuItem->IsChecked())
	{
		mConsoleWindow->Show(true);
	}
	else
	{
		mConsoleWindow->Show(false);
	}
}

void
MainWindow::OnDisplaySettingsCheckbox(wxCommandEvent& event)
{
	wxMenuItem* menuItem = menubar->FindItem(ID_DISPLAY_SETTINGS);

	if (menuItem->IsChecked())
	{
		mSettingsWindow->Show(true);
	}
	else
	{
		mSettingsWindow->Show(false);
	}
}

void
MainWindow::OnModulesCombobox(wxCommandEvent& event)
{
	//int s = event.GetSelection();
	auto s = moduleChoices->GetStringSelection();

	if (gApi == API_OPENGL && glcanvas != NULL)
	{
		delete mRenderer;
		delete mGDriver;

		mGDriver = new CGLDriver();
		if (s == "TERRAIN_CDLOD")
		{
			mRenderer = new CRenderer(mGDriver, TERRAIN, settingsMainPanel);
		}
		else if (s == "OBJ_LOADER")
		{
			mRenderer = new CRenderer(mGDriver, OBJ_LOADER, settingsMainPanel);
		}

		glcanvas->setGModule(mRenderer);
	}
}

void MainWindow::OnColourChanged(wxColourPickerEvent& evt)
{
	// Use the wxColourPickerEvent::GetColour() function to get the selected
	// color and set the color of the text control accordingly.
	//m_textCtrl->SetForegroundColour(evt.GetColour());
	//m_textCtrl->Refresh();
	colorHasChanged = true;
	color = evt.GetColour();
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

	viewItems = viewMenu->AppendCheckItem(ID_DISPLAY_CONSOLE, "Display console");
	if (mConsoleWindow->IsVisible())
	{
		viewItems->Check();
	}
	viewItems = viewMenu->AppendCheckItem(ID_DISPLAY_SETTINGS, "Display settings");
	if (mSettingsWindow->IsVisible())
	{
		viewItems->Check();
	}

	SetMenuBar(menubar);
	SetFocus();
	
	Connect(wxID_EXIT, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MainWindow::OnQuit));
	Connect(ID_QUIT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainWindow::OnQuit));
	Connect(ID_DISPLAY_CONSOLE,  wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainWindow::OnDisplayConsoleCheckbox));
	Connect(ID_DISPLAY_SETTINGS, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainWindow::OnDisplaySettingsCheckbox));
}
