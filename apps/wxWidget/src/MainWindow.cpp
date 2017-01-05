#include "wx/wxprec.h"
#include "MainCanvas.h"
#include "MainWindow.h"

const int mainCanvasID = 2000;			// TriangleCanvas widget ID

BEGIN_EVENT_TABLE(MainWindow, wxFrame)
EVT_CLOSE(MainWindow::onClose)
END_EVENT_TABLE()


MainWindow::MainWindow(wxWindow* parent, const std::wstring& title, const wxPoint& pos,
	const wxSize& size)
	: wxFrame(parent, wxID_ANY, title, pos, size, wxMINIMIZE_BOX | wxCLOSE_BOX |
		wxSYSTEM_MENU | wxCAPTION | wxCLIP_CHILDREN)
{
	
	MainCanvas* graphicsCanvas = new MainCanvas(this, mainCanvasID, nullptr, { 0, 0 },
	{ 800, 800 });
	Fit();
	Centre();

	m_timer = new RenderTimer(graphicsCanvas);
	m_timer->start();

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
	Connect(ID_Quit, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainWindow::OnQuit));
	Connect(ID_Settings, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainWindow::OnSettings));
}

MainWindow::~MainWindow()
{
	delete m_timer;
}

void MainWindow::OnQuit(wxCommandEvent& WXUNUSED(event))
{
	Close(true);
}

void MainWindow::OnSettings(wxCommandEvent& WXUNUSED(event))
{
	wxMessageBox(	
		wxT("A settings window will be used instead of this messagebox."), 			
		wxT("Test message box"),			
		wxOK | wxICON_INFORMATION);
}

void MainWindow::onClose(wxCloseEvent& evt)
{
	m_timer->Stop();
	evt.Skip();
}
