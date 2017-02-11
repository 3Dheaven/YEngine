#include "ObjLoadingTest.h"
#include "../../../../yengine/helpers/StringsHelper.h"
#include "../../../../yengine/system/FileSystem.h"

ObjLoading::ObjLoading(CGraphicDriver *gdriver)
{
	mCam = NULL;
	mGDriver = gdriver;

}

ObjLoading::~ObjLoading()
{
	delete mCam;
	delete mCustomShader;
	delete mScene;
}

CCamera* 
ObjLoading::getCam()
{
	return mCam;
}

void 
ObjLoading::setupGraphics()
{
	std::vector<std::string> splittedPath = strh::split(sys::getExecutablePath(), '\\');
	std::string projectPath;
	for (auto i : splittedPath)
	{
		if (i != "YEngine")
		{
			projectPath += i + "\\";
		}
		else
		{
			projectPath += i;
			break;
		}
	}

	std::string vsPath = projectPath + "//apps//theMachine//workshop//obj_loading//shader.vert";
	std::string fsPath = projectPath + "//apps//theMachine//workshop//obj_loading//shader.frag";

	mCustomShader = new CShaderFactory(vsPath.c_str(), fsPath.c_str());

	mCam = new CCamera(glm::vec3(0.0f, 0.0f, 5.0f), 
						glm::vec3(0.0f, 0.0f, 0.0f), 
						glm::vec3(0.0f, 1.0f, 0.0f));

	mScene = new CScene(mGDriver);
	mScene->add(projectPath + "//media//nanosuit//nanosuit.obj");
	//mScene->add(projectPath + "//media//sphere.obj");
	//mScene->add(projectPath + "//media//shuttle.obj");
	//mScene->add(projectPath + "//media//axis//axisXYZ.obj");
	//mScene->add(projectPath + "//media//vis.obj");
}

void 
ObjLoading::render()
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
