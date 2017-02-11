#include "MainWindow.h"

const int mainCanvasID = 2000;			// TriangleCanvas widget ID

BEGIN_EVENT_TABLE(MainWindow, wxFrame)
EVT_CLOSE(MainWindow::onClose)
EVT_COLOURPICKER_CHANGED(ID_ColorPicker, MainWindow::OnColourChanged)
END_EVENT_TABLE()

enum E_API3D 
{ 
	API_VULKAN, 
	API_OPENGL
};

MainWindow::MainWindow(wxWindow* parent, const std::wstring& title, const wxPoint& pos, const wxSize& size)
	: wxFrame(parent, wxID_ANY, title, pos, size, wxMINIMIZE_BOX | wxCLOSE_BOX | wxSYSTEM_MENU | wxCAPTION | wxCLIP_CHILDREN)
{
	mGDriver = NULL;
	mRenderer = NULL;

	mMainPanel = new wxPanel(this, wxID_ANY);

	// Display MainWindow on screen center
	Centre();

	E_API3D gApi = API_OPENGL;
	switch (gApi)
	{
		case API_VULKAN: 
			{
				std::cout << "API_VULKAN\n";
				CVulkanCanvas* vcanvas = new CVulkanCanvas(mMainPanel, wxID_ANY, { 0, 0 }, { 600, 600 });
				/*mGDriver = dynamic_cast<CGraphicDriver *>(new CVKDriver());
				mRenderer = new CRenderer(mGDriver);
				glcanvas->setGModule(mRenderer);*/
			}
			break;

		case API_OPENGL: 
			{
				CGLCanvas* glcanvas = new CGLCanvas(mMainPanel, mainCanvasID, nullptr, { 0, 0 }, { 600, 600 });
				mGDriver = dynamic_cast<CGraphicDriver *>(new CGLDriver());
				mRenderer = new CRenderer(mGDriver);
				glcanvas->setGModule(mRenderer);
			}
			break;
	}

	
	wxPanel * bottomPanel = new wxPanel(mMainPanel, wxID_ANY, { 0, 600 }, { 600, 200 });
	wxPanel * rightPanel = new wxPanel(mMainPanel, wxID_ANY, { 600, 0 }, { 400, 800 });

	wxButton *button = new wxButton(rightPanel, wxID_EXIT, wxT("Quit"), wxPoint(0, 600), { 400, 200 });
		
	// Create a wxColourPickerCtrl control
	colorHasChanged = false;
	wxColourPickerCtrl* colourPickerCtrl = new wxColourPickerCtrl(rightPanel, ID_ColorPicker, wxStockGDI::COLOUR_RED, { 20, 20 });

	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	m_console = new wxTextCtrl(bottomPanel, wxID_ANY, wxEmptyString, { 0, 0 }, { 600, 140 }, wxTE_MULTILINE | wxTE_READONLY);

	wxLog::SetActiveTarget(new wxLogTextCtrl(m_console));

	// All output to cout goes into the text control until the exit from current scope
	// wxStreamToTextRedirector redirect(m_console);

	wxMenuBar *menubar = new wxMenuBar;
	wxMenu *fileMenu = new wxMenu();
	fileMenu->Append(ID_Quit, wxT("&Quit"));
	menubar->Append(fileMenu, wxT("&File"));
	wxMenu *settingsMenu = new wxMenu();
	settingsMenu->Append(ID_Settings, wxT("&Rendering options"));
	menubar->Append(settingsMenu, wxT("&Settings"));

	wxCheckBox *useOpenglCheckbox = new wxCheckBox();
	wxCheckBox *useVulkanCheckbox = new wxCheckBox();
	settingsMenu->AppendCheckItem(useOpenglCheckbox->GetId(), "Use OpenGL");
	settingsMenu->AppendCheckItem(useVulkanCheckbox->GetId(), "Use Vulkan");

	SetMenuBar(menubar);
	SetFocus();

	Connect(wxID_EXIT, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MainWindow::OnQuit));
	Connect(ID_Quit, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainWindow::OnQuit));
	Connect(ID_Settings, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainWindow::OnSettings));
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
MainWindow::OnSettings(wxCommandEvent& WXUNUSED(event))
{
	wxMessageBox(	
		wxT("A settings window will be used instead of this messagebox."), 			
		wxT("Test message box"),			
		wxOK | wxICON_INFORMATION);
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
