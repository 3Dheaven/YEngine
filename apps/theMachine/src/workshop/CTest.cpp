#include "CTest.h"

CTest::CTest(CGraphicDriver *gdriver)
{
	mScene = NULL;
	mGDriver = gdriver;
	setupGraphics();
}

CTest::~CTest()
{
	delete mCam;
	delete mScene;
}

CCamera* 
CTest::getCam()
{
	return mCam;
}

void 
CTest::setupGraphics()
{
	mCam = new CCamera(glm::vec3(0.0f, 0.0f, 5.0f), 
						glm::vec3(0.0f, 0.0f, 0.0f), 
						glm::vec3(0.0f, 1.0f, 0.0f));

	//mScene = new CScene(mGDriver);
	//mScene->add("..//..//..//media//cube.obj");
}

void 
CTest::render()
{
	
}
