#pragma once

#include "../../../yengine/scene/CScene.h"
#include "../../../yengine/CShaderFactory.h"
#include "../../../yengine/scene/CCamera.h"
#include "../../../yengine/core/CGraphicDriver.h"
#include "../CModuleBase.h"

#include <memory>
#include "wx/glcanvas.h"
#include "wx/wxprec.h"

#include <GL/glew.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <chrono>

using namespace glm;

class ObjLoading : public CModuleBase
{
public:
	ObjLoading(CGraphicDriver *gdriver);
	~ObjLoading();

	void setupGraphics();
	void render();
	CCamera* getCam();
	
private:
	CCamera *mCam;
	CShaderFactory *mCustomShader;
	CScene *mScene;
	CGraphicDriver *mGDriver;
};