#pragma once

class MainWindow : public wxFrame
{
public:
	MainWindow(wxWindow* parent, const std::wstring& title, const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize);
	virtual ~MainWindow();
	MainWindow(const MainWindow& tw) = delete;
	MainWindow(MainWindow&& tw) = delete;
	MainWindow& operator=(const MainWindow& tw) = delete;
	MainWindow& operator=(MainWindow&&) = delete;
};

