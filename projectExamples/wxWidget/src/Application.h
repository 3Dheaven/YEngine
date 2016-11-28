#pragma once
#include "C:\Users\yoan.audureau\Documents\GitHub\wxWidgets\include\wx\msw\app.h"

class Application :
	public wxApp
{
public:
	Application();
	virtual ~Application();
	virtual bool OnInit() override;
};
