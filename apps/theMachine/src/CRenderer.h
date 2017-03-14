#pragma once

#include "../../../yengine/scene/CCamera.h"
#include "../../../yengine/core/CGraphicDriver.h"
#include "wx/wxprec.h"

class CRenderer
{
public:

	CRenderer();
	~CRenderer();

	virtual void setupGraphics();
	virtual void render();
	virtual void init(CGraphicDriver *gdriver, wxPanel* panel) {};
	virtual CCamera* getCam() { return NULL; };
};
