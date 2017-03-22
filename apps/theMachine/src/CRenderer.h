#pragma once

#include "../../../yengine/scene/CCamera.h"
#include "../../../yengine/core/CGraphicDriver.h"
#include "wx/wxprec.h"
#include <memory>
class CRenderer
{
public:

	CRenderer();
	virtual ~CRenderer() {};

	virtual void setupGraphics();
	virtual void render();
	virtual void init(CGraphicDriver *gdriver, wxPanel* panel) {};
	virtual std::shared_ptr<CCamera> getCam() { return NULL; };
};
