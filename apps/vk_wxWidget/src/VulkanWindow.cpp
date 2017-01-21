#include "VulkanWindow.h"
#include "VulkanException.h"

BEGIN_EVENT_TABLE(VulkanWindow, wxFrame)
EVT_CLOSE(VulkanWindow::onClose)
EVT_COLOURPICKER_CHANGED(ID_ColorPicker,VulkanWindow::OnColourChanged)
END_EVENT_TABLE()

VulkanWindow::VulkanWindow(wxWindow* parent, const wxString &title, const wxPoint& pos, const wxSize& size)
	: wxFrame(parent, wxID_ANY, title, pos, size, wxMINIMIZE_BOX | wxCLOSE_BOX | wxSYSTEM_MENU | wxCAPTION | wxCLIP_CHILDREN)

{
	m_parent = new wxPanel(this, wxID_ANY);
	colorHasChanged = false;
    //Bind(wxEVT_SIZE, &VulkanWindow::OnResize, this);
    m_canvas = new VulkanCanvas(m_parent, wxID_ANY,  { 0, 0 }, { 600, 600 });

    //Fit();

	bottomPanel = new wxPanel(m_parent, wxID_ANY, { 0, 600 }, { 600, 200 });
	rightPanel = new wxPanel(m_parent, wxID_ANY, { 600, 0 }, { 400, 800 });

	wxButton *button = new wxButton(rightPanel, wxID_EXIT, wxT("Quit"), wxPoint(0, 600), { 400, 200 });

	// Create a wxColourPickerCtrl control
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

	Connect(wxID_EXIT, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(VulkanWindow::OnQuit));
	Connect(ID_Quit, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(VulkanWindow::OnQuit));
	Connect(ID_Settings, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(VulkanWindow::OnSettings));

	
	
}


VulkanWindow::~VulkanWindow()
{
}

void VulkanWindow::OnResize(wxSizeEvent& event)
{
    wxSize clientSize = GetClientSize();
    m_canvas->SetSize(clientSize);
}

wxTextCtrl* VulkanWindow::getDebugConsole()
{
	return m_console;
}

void VulkanWindow::OnQuit(wxCommandEvent& WXUNUSED(event))
{
	Close(true);
}

void VulkanWindow::OnSettings(wxCommandEvent& WXUNUSED(event))
{
	wxMessageBox(
		wxT("A settings window will be used instead of this messagebox."),
		wxT("Test message box"),
		wxOK | wxICON_INFORMATION);
}

void VulkanWindow::onClose(wxCloseEvent& evt)
{
	evt.Skip();
}

void VulkanWindow::OnColourChanged(wxColourPickerEvent& evt)
{
	// Use the wxColourPickerEvent::GetColour() function to get the selected
	// color and set the color of the text control accordingly.
	//m_textCtrl->SetForegroundColour(evt.GetColour());
	//m_textCtrl->Refresh();
	colorHasChanged = true;
	color = evt.GetColour();
}