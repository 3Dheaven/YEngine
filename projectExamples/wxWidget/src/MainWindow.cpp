#include "wx/wxprec.h"
#include "MainCanvas.h"
#include "MainWindow.h"

const int triCanvasID = 2000;			// TriangleCanvas widget ID

MainWindow::MainWindow(wxWindow* parent, const std::wstring& title, const wxPoint& pos,
	const wxSize& size)
	: wxFrame(parent, wxID_ANY, title, pos, size, wxMINIMIZE_BOX | wxCLOSE_BOX |
		wxSYSTEM_MENU | wxCAPTION | wxCLIP_CHILDREN)
{
	MainCanvas* canvas = new MainCanvas(this, triCanvasID, nullptr, { 0, 0 },
	{ 800, 800 });
	Fit();
	Centre();
}

MainWindow::~MainWindow()
{
}
