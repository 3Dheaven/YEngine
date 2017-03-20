#include "TwoDSquare.h"

TwoDSquare::TwoDSquare()
{
	mCam = NULL;
	mGDriver = NULL;
	mRightPanel = NULL;
}

TwoDSquare::TwoDSquare(CGraphicDriver *gdriver, wxPanel* panel)
{
	mCam = NULL;
	mGDriver = gdriver;
	mRightPanel = panel;
	loadGUI();
}

void 
TwoDSquare::init(CGraphicDriver *gdriver, wxPanel* panel)
{
	mGDriver = gdriver;
	mRightPanel = panel;
	setupGraphics();
	loadGUI();
}

TwoDSquare::~TwoDSquare()
{
	delete mCam;
	delete mScene;
	cleanGUI();
}

CCamera* 
TwoDSquare::getCam()
{
	return mCam;
}

void 
TwoDSquare::setupGraphics()
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

	std::string vsPath = projectPath + "//apps//theMachine//workshop//2d_square//gl//shader.vert";
	std::string fsPath = projectPath + "//apps//theMachine//workshop//2d_square/gl//shader.frag";


	mGDriver->createShader(vsPath, fsPath);
	
	mCam = new CCamera(glm::vec3(0.0f, 0.0f, 5.0f), 
						glm::vec3(0.0f, 0.0f, 0.0f), 
						glm::vec3(0.0f, 1.0f, 0.0f));

	CModel* model = new CModel();
	CObject* object = new CObject("square");
	std::vector<CMesh*> meshes;
	CMesh* mesh = new CMesh("quad");

	sVertex v0 = { glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0, 0.0) };
	sVertex v1 = { glm::vec3(0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0, 1.0) };
	sVertex v2 = { glm::vec3(0.5f, 0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0, 1.0) };
	sVertex v3 = { glm::vec3(-0.5f, 0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0, 0.0) };

	mesh->mVertices = {v0, v1, v2,v3};
	mesh->mIndices = { 0, 1, 2, 2, 3, 0 };//{ 0, 3, 2, 2, 1, 0 };

	meshes.push_back(mesh);

	model->addObject(object, meshes);

	// indexing
	/*for (auto o : model->mObjects)
	{
		for (auto m : o->mMeshes)
		{
			for (unsigned int i = 0; i < model->mMeshes[m]->vertexIndices.size(); i++)
			{
				unsigned int vertexIndex = model->mMeshes[m]->vertexIndices[i];
				glm::vec3 vertex = model->mVertices[vertexIndex];
				model->mMeshes[m]->vertices.push_back(vertex);
			}
		}
	}*/

	mScene = new CScene(mGDriver);
	mScene->add(model);
	
	mUniformColor = glm::vec4(1.0, 0.0, 0.0, 1.0);
	mGDriver->addUniform("custom_color", mUniformColor);
	mUniformColorHasChanged = true;

	mGDriver->finalizeSetup();
}

void 
TwoDSquare::render()
{
	if (mUniformColorHasChanged)
	{
		mGDriver->updateUniform("custom_color", mUniformColor);
		mUniformColorHasChanged = false;
	}

	mScene->render(mGDriver->getShader());
}

void 
TwoDSquare::loadGUI()
{
	// http://zetcode.com/gui/wxwidgets/layoutmanagement/

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
TwoDSquare::cleanGUI()
{
	if (mRightPanel != NULL)
	{
		mRightPanel->DestroyChildren();
	}
}

RendererRegister<TwoDSquare> regSquare("TwoDSquare");