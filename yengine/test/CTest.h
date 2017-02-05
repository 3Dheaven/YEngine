#pragma once

#include "../scene/CScene.h"
#include "../CShaderFactory.h"
#include "../scene/CCamera.h"
#include "../core/CGraphicDriver.h"

#include <memory>
#include "wx/glcanvas.h"
#include "wx/wxprec.h"

#include <GL/glew.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <chrono>

using namespace glm;

class CTest
{
public:
	CTest(CGraphicDriver *gdriver);
	~CTest();

	void setupGraphics();
	void render();
	CCamera* getCam();

private:
	CCamera *mCam;
	CScene *mScene;
	CGraphicDriver *mGDriver;
};