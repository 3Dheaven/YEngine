﻿#include "ObjLoadingTest.h"

ObjLoading::ObjLoading(CGraphicDriver *gdriver, wxPanel* panel)
{
	mCam = NULL;
	mGDriver = gdriver;
	mRightPanel = panel;
	loadGUI();
}

ObjLoading::~ObjLoading()
{
	delete mCam;
	delete mScene;
	cleanGUI();
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

	mGDriver->createShader(vsPath, fsPath);

	mCam = new CCamera(glm::vec3(0.0f, 0.0f, 5.0f), 
						glm::vec3(0.0f, 0.0f, 0.0f), 
						glm::vec3(0.0f, 1.0f, 0.0f));

	mScene = new CScene(mGDriver);
	mScene->add(projectPath + "//media//nanosuit//nanosuit.obj");

	mUniformColor = glm::vec4(1.0, 0.0, 0.0, 0.0);
	mGDriver->getShader()->setUniform("custom_color", mUniformColor);
	mUniformColorHasChanged = false;
}

void 
ObjLoading::render()
{
	mGDriver->getShader()->use();

	mCam->projMatrix = glm::perspective(mCam->getZoom(), (float)600 / (float)600, 0.1f, 100.0f);
	mGDriver->getShader()->setUniform("projection_matrix", mCam->projMatrix);
	mGDriver->getShader()->setUniform("view_matrix", mCam->getViewMatrix());
	// glm::vec3((sin(time * 1.0f) + 1.0f) / 2.0f, (sin(time * 0.5f) + 1.0f) / 2.0f, (cos(time * 0.25f) + 1.0f) / 2.0f)
	if (mUniformColorHasChanged)
	{
		mGDriver->getShader()->setUniform("custom_color", mUniformColor);
		mUniformColorHasChanged = false;
	}

	glm::mat4 model;
	//glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); 
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f)); 
	//model = glm::rotate(model, (float)time * 0.1f, glm::vec3(0.0f, 1.0f, 0.0f));
	mGDriver->getShader()->setUniform("model_matrix", model);

	mScene->render(mGDriver->getShader());
}

void 
ObjLoading::loadGUI()
{
	if (mRightPanel != NULL)
	{
		wxColourPickerCtrl* colourPickerCtrl = new wxColourPickerCtrl(mRightPanel, wxID_ANY, wxStockGDI::COLOUR_RED, { 40, 40 });
		colourPickerCtrl->Bind(wxEVT_COLOURPICKER_CHANGED, &ObjLoading::OnColourChanged, this);		
	}
}

void 
ObjLoading::cleanGUI()
{
	if (mRightPanel != NULL)
	{
		delete mRightPanel;
	}
}

void ObjLoading::OnColourChanged(wxColourPickerEvent& evt)
{
	mUniformColorHasChanged = true;
	auto newColor = evt.GetColour();
	mUniformColor.x = static_cast<float>(newColor.Red()) / 255.0f;
	mUniformColor.y = static_cast<float>(newColor.Green()) / 255.0f;
	mUniformColor.z = static_cast<float>(newColor.Blue()) / 255.0f;
	mUniformColor.w = 1.0f;
}