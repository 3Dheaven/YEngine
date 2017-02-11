#pragma once

#include "../../../yengine/scene/CScene.h"
#include "../../../yengine/CShaderFactory.h"
#include "../../../yengine/scene/CCamera.h"
#include "../../../yengine/core/CGraphicDriver.h"

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
	CRenderer(CGraphicDriver *gdriver, E_MODULES_EXAMPLES renderingWorshopChoice);
	~CRenderer();

	void setupGraphics();
	void render();
	CCamera* getCam();
	CCamera *mCam;

private:

	CShaderFactory *mCustomShader;
	E_MODULES_EXAMPLES mRenderingWorshopChoice;
	CScene *mScene;
	CGraphicDriver *mGDriver;
};