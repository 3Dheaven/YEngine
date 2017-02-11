#pragma once

#include "../../../yengine/scene/CScene.h"
#include "../../../yengine/CShaderFactory.h"
#include "../../../yengine/scene/CCamera.h"
#include "../../../yengine/core/CGraphicDriver.h"
#include "../workshop/CModuleBase.h"
#include "../workshop/obj_loading/ObjLoadingTest.h"

#include <memory>
#include "wx/glcanvas.h"
#include "wx/wxprec.h"

#include "ERenderingWorkshop.h"

#include <GL/glew.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <chrono>

using namespace glm;

class CRenderer
{
public:
	CRenderer(CGraphicDriver *gdriver, E_MODULES_EXAMPLES renderingWorshopChoice, wxPanel *wxPanel = NULL);
	~CRenderer();

	void setupGraphics();
	void render();
	CCamera* getCam();
	void loadModule();

private:

	wxPanel* mRightPanel;
	CGraphicDriver* mGDriver;
	E_MODULES_EXAMPLES mRenderingWorkshopChoice;
	CModuleBase* mActiveModule;

};