#include "CubeTest.h"

CubeTest::CubeTest()
{
	mGDriver = NULL;
	mRightPanel = NULL;
	mScene = NULL;
	fakeTime = 1;
}

CubeTest::CubeTest(CGraphicDriver *gdriver, wxPanel* panel)
{
	mGDriver = gdriver;
	mRightPanel = panel;
	mScene = NULL;
	fakeTime = 1;
	loadGUI();
}

void 
CubeTest::init(CGraphicDriver *gdriver, wxPanel* panel)
{
	mGDriver = gdriver;
	mRightPanel = panel;
	fakeTime = 1;
	setupGraphics();
	loadGUI();
}

CubeTest::~CubeTest()
{
	if (mScene != NULL)
	{
		delete mScene;
	}
	cleanGUI();
}

std::shared_ptr<CCamera>
CubeTest::getCam()
{
	return mCam;
}

void 
CubeTest::setupGraphics()
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

	std::string vsPath = projectPath + "//apps//theMachine//workshop//cube//gl//shader.vert";
	std::string fsPath = projectPath + "//apps//theMachine//workshop//cube/gl//shader.frag";


	mGDriver->createShader(vsPath, fsPath);

	mCam = std::make_shared<CCamera>(CCamera(glm::vec3(0.0f, 0.0f, 5.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f)));

	CMesh* mesh2 = new CMesh("cube");
	sVertex c1 = { glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0, 0.0) };
	sVertex c2 = { glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0, 0.0) };
	sVertex c3 = { glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0, 0.0) };
	sVertex c4 = { glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0, 0.0) };
	sVertex c5 = { glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0, 0.0) };
	sVertex c6 = { glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0, 0.0) };
	sVertex c7 = { glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0, 0.0) };
	sVertex c8 = { glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0, 0.0) };
	mesh2->mVertices = { c1, c2, c3, c4, c5, c6, c7, c8 };
	//					      front					  top					  back				 
	mesh2->mIndices = { 0, 1, 2, 2, 3, 0,		1, 5, 6, 6, 2, 1,		7, 6, 5, 5, 4, 7,
	//						  bottom				 left					  right
						4, 0, 3, 3, 7, 4,		4, 5, 1, 1, 0, 4,		3, 2, 6, 6, 7, 3 };

	mScene = new CScene(mGDriver);
	mScene->add(mesh2);
	
	mUniformColor = glm::vec4(1.0, 0.0, 0.0, 1.0);
	mGDriver->addUniform("custom_color", mUniformColor);
	mUniformColorHasChanged = true;

	glm::mat4 model;
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	glm::mat4 mvp = mCam->getProjectionMatrix() * mCam->getViewMatrix() * model;
	mGDriver->addUniform("mvp", mvp);

	mGDriver->finalizeSetup();
}

void 
CubeTest::render()
{
	if (mUniformColorHasChanged)
	{
		mGDriver->updateUniform("custom_color", mUniformColor);
		mUniformColorHasChanged = false;
	}

	glm::mat4 model;
	model = glm::rotate(model, fakeTime, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 mvp = mCam->getProjectionMatrix() * mCam->getViewMatrix() * model;
	mGDriver->updateUniform("mvp", mvp);

	mScene->render(mGDriver->getShader());

	fakeTime += 0.1;
	if (fakeTime > 1000000.0)
		fakeTime = 1.0;
}

void 
CubeTest::loadGUI()
{
	if (mRightPanel != NULL)
	{
		mRightPanel->DestroyChildren();
		mRightPanel->SetBackgroundColour(wxT("#ededed"));

		wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);

		wxBoxSizer *hbox1 = new wxBoxSizer(wxHORIZONTAL);
		wxStaticText *st1 = new wxStaticText(mRightPanel, wxID_ANY, wxT("Color uniform : "));
		hbox1->Add(st1, 0, wxRIGHT, 10);
		wxColourPickerCtrl* colourPickerCtrl = new wxColourPickerCtrl(mRightPanel, wxID_ANY, wxStockGDI::COLOUR_RED);
		colourPickerCtrl->Bind(	wxEVT_COLOURPICKER_CHANGED, 
								[&](wxColourPickerEvent& evt) 
								{
									mUniformColorHasChanged = true;
									auto newColor = evt.GetColour();
									mUniformColor.x = static_cast<float>(newColor.Red()) / 255.0f;
									mUniformColor.y = static_cast<float>(newColor.Green()) / 255.0f;
									mUniformColor.z = static_cast<float>(newColor.Blue()) / 255.0f;
									mUniformColor.w = 1.0f;
								});

		hbox1->Add(colourPickerCtrl, 2);
		vbox->Add(hbox1, 0, wxLEFT | wxRIGHT | wxTOP, 10);

		vbox->Add(-1, 25); // bottom space

		mRightPanel->SetSizer(vbox);
		mRightPanel->Layout();
	}
}

void 
CubeTest::cleanGUI()
{
	if (mRightPanel != NULL)
	{
		mRightPanel->DestroyChildren();
	}
}

RendererRegister<CubeTest> regCubeTest("CubeTest");