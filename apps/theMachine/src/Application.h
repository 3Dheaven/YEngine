#pragma once
#include "wx\msw\app.h"

// Should be placed in pch
#ifdef _DEBUG
#include <crtdbg.h>
#define DEBUG_NEW new(_NORMAL_BLOCK ,__FILE__, __LINE__)
#else
#define DEBUG_NEW new
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#endif 
//

class Application :
	public wxApp
{
public:
	Application();
	virtual ~Application();
	virtual bool OnInit() override;
};
