#include "CRenderer.h"

CRenderer::CRenderer(CGraphicDriver *gdriver, E_MODULES_EXAMPLES renderingWorshopChoice)
{
	mCam = NULL;
	mGDriver = gdriver;
	setupGraphics();
	mRenderingWorshopChoice = renderingWorshopChoice;
}

CRenderer::~CRenderer()
{
	delete mCam;
	delete mCustomShader;
	delete mScene;
}

CCamera* 
CRenderer::getCam() 
{
	return mCam;
}

void 
CRenderer::setupGraphics()
{
	mCustomShader = new CShaderFactory("../shaders/shader.vert",
										"../shaders/shader.frag");

	mCam = new CCamera(glm::vec3(0.0f, 0.0f, 5.0f), 
						glm::vec3(0.0f, 0.0f, 0.0f), 
						glm::vec3(0.0f, 1.0f, 0.0f));

	mScene = new CScene(mGDriver);
	mScene->add("..//..//..//media//nanosuit//nanosuit.obj");
	//mScene->add("..//..//..//media//sphere.obj");
	//mScene->add("..//..//..//media//shuttle.obj");
	//mScene->add("..//..//..//media//axis//axisXYZ.obj");
	//mScene->add("..//..//..//media//vis.obj");
}

void 
CRenderer::render()
{
	mCustomShader->shader->use();

	mCam->projMatrix = glm::perspective(mCam->getZoom(), (float)600 / (float)600, 0.1f, 100.0f);
	mCustomShader->shader->setUniform("projection_matrix", mCam->projMatrix);
	mCustomShader->shader->setUniform("view_matrix", mCam->getViewMatrix());
	// glm::vec3((sin(time * 1.0f) + 1.0f) / 2.0f, (sin(time * 0.5f) + 1.0f) / 2.0f, (cos(time * 0.25f) + 1.0f) / 2.0f)
	mCustomShader->shader->setUniform("custom_color", glm::vec3(0.0,1.0,0.0));

	glm::mat4 model;
	//glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); 
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f)); 
	//model = glm::rotate(model, (float)time * 0.1f, glm::vec3(0.0f, 1.0f, 0.0f));
	mCustomShader->shader->setUniform("model_matrix", model);

	// draw the graphics
	mScene->render(mCustomShader);
}
