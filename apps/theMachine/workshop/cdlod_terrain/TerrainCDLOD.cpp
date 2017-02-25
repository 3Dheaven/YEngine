#include "TerrainCDLOD.h"

TerrainCDLOD::TerrainCDLOD(CGraphicDriver *gdriver, wxPanel* panel)
{
	mCam = NULL;
	mGDriver = gdriver;
	mRightPanel = panel;
	loadGUI();
}

TerrainCDLOD::~TerrainCDLOD()
{
	delete mCam;
	delete mScene;

	if (grid != NULL) delete grid;
	if (hmap != NULL) delete hmap;
	if (mRootNode != NULL) delete mRootNode;
	cleanGUI();
}

CCamera*
TerrainCDLOD::getCam()
{
	return mCam;
}

void
TerrainCDLOD::setupGraphics()
{
	mCam = new CCamera(	glm::vec3(0.0f, 0.0f, 0.0f),	
						glm::vec3(1.0f, 0.0f, 1.0f),		
						glm::vec3(0.0f, 1.0f, 0.0f));	

	mCam->setYaw(45.0f);
	mCam->update();

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
	projectPath += "//apps//theMachine//workshop//cdlod_terrain//";

	std::string vsPath = projectPath + "shaders//terrainSimple.vs";
	std::string fsPath = projectPath + "shaders//terrainSimple.fs";
	mGDriver->createShader(vsPath, fsPath);

	mLodDepth = 8;
	grid = new Grid(8);
	fullResGrid = new Grid(32);
	halfResGrid = new Grid(16);

	hmap = new HeightMap(projectPath + "heightmap5.bmp");

	float rootNodeSize = hmap->getHeight();// pow(2, lodDepth - 1);

	for (int i = 0; i < mLodDepth; i++) 
	{
		mRanges.push_back(pow(2, i) * 10);
	}

	mRootNode = new QuadTreeNode(hmap, rootNodeSize, mLodDepth, 0, 0);

	mGDriver->init(fullResGrid->mMeshes);
}

void
TerrainCDLOD::render()
{
	mCam->projMatrix = glm::perspective(mCam->getZoom(), 1.0f, 0.1f, 1000.0f);

	std::stack<QuadTreeNode*> drawStack;

	mRootNode->LODSelect(mRanges, mLodDepth - 1, mCam, drawStack);

	mGDriver->getShader()->use();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, hmap->mTextureID);
	mGDriver->getShader()->setUniform("heightMapTexture", 0);
	
	mGDriver->getShader()->setUniform("projection", mCam->projMatrix);
	mGDriver->getShader()->setUniform("view", mCam->getViewMatrix());

	if (mUniformColorHasChanged)
	{
		mGDriver->getShader()->setUniform("custom_color", mUniformColor);
		mUniformColorHasChanged = false;
	}

	while (!drawStack.empty()) 
	{
		QuadTreeNode *current = drawStack.top();
		drawStack.pop();

		float s = current->mapSize;
		float l = current->fullResolution;

		float scale = current->mapSize / fullResGrid->getSize(); 
		if (scale < 1.0) scale = 1.0;
		float range = current->currentRange;

		glm::vec3 translation = glm::vec3(current->posX, 0.0, current->posY);

		mGDriver->getShader()->setUniform("translation", translation);
		mGDriver->getShader()->setUniform("scale", scale);
		mGDriver->getShader()->setUniform("range", range);
		mGDriver->getShader()->setUniform("gridDim", glm::vec3(32, 32, 0));

		mGDriver->render(fullResGrid->mMeshes, mGDriver->getShader());
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	mGDriver->getShader()->unuse();
}

void
TerrainCDLOD::loadGUI()
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

		colourPickerCtrl->Bind(wxEVT_COLOURPICKER_CHANGED,
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

		// bottom space
		//vbox->Add(-1, 25); 

		mRightPanel->SetSizer(vbox);
		mRightPanel->Layout();
	}
}

void
TerrainCDLOD::cleanGUI()
{
	if (mRightPanel != NULL)
	{
		mRightPanel->DestroyChildren();
	}
}

//void terraincdlod::renderarea(camera *cam) {
//
//	// build node list to render
//	std::stack<quadtreenode*> drawstack;
//	mrootnode->lodselect(mranges, mloddepth-1, cam, drawstack);
//
//	/*for (unsigned int i = 0; i < gridmap.size(); i++) {
//		for (unsigned int j = 0; j < gridmap[0].size(); j++) {
//			gridmap[i][j]->lodselect(mranges, mloddepth - 1, cam, drawstack);
//		}
//	}*/
//
//	terrainareashader->use();
//	terrainareashader->setuniform("projection", cam->projmatrix);
//	terrainareashader->setuniform("view", cam->getviewmatrix());
//
//	std::cout << "nb triangles : " << drawstack.size() * grid->msize * grid->msize * 2<< std::endl;
//
//	while (!drawstack.empty()) {
//
//		quadtreenode *current = drawstack.top();
//		drawstack.pop();
//		//std::cout << "node at lod " << current->lodlevel << " : pos(" << current->posx << ", 0, " << current->posy << ")" << std::endl;
//
//		float s = current->mapsize;
//		float l = current->fullresolution;
//		glm::vec4 color = glm::vec4(0.5, 0.5, 0.5, 1.0);
//		if (s < 2.0) {
//			color = glm::vec4(1.0);								// blanc
//			//if (!l) color = glm::vec4(0.5, 1.0, 0.5, 1.0);
//		}
//		else if (s < 4.0) {
//			color = glm::vec4(0.0, 1.0, 0.0, 1.0);				// green	
//			//if (!l) color = glm::vec4(0.0, 1.0, 1.0, 1.0);
//		}
//		else if (s < 8.0) {
//			color = glm::vec4(0.0, 0.0, 1.0, 1.0);				// blue		
//			//if (!l) color = glm::vec4(1.0, 0.0, 1.0, 1.0);
//		}
//		else if (s < 16.0) {
//			color = glm::vec4(1.0, 1.0, 0.0, 1.0);				// jaune		
//			//if (!l) color = glm::vec4(1.0, 0.5, 0.0, 1.0);
//		}
//		else if (s < 32.0) {
//			color = glm::vec4(0.0, 1.0, 1.0, 1.0);				// cyan			
//			//if (!l) color = glm::vec4(1.0, 0.0, 0.0, 1.0);
//
//		}
//		else if (s < 64.0) {
//			color = glm::vec4(1.0, 0.0, 1.0, 1.0);				// violet
//			//if (!l) color = glm::vec4(1.0, 0.0, 0.0, 1.0);
//
//		}
//		else if (s < 128.0) {
//			color = glm::vec4(0.25, 0.0, 0.5, 1.0);
//			//if (!l) color = glm::vec4(1.0, 0.0, 0.0, 1.0);
//
//		}
//
//
//		float scale = current->mapsize / grid->msize; //grid's size
//		if (scale < 1.0) scale = 1.0;
//		//std::cout << "current->mapsize : " << current->mapsize << std::endl;
//		//std::cout << "mapsize : " << mapsize << std::endl;
//		//std::cout << "scale : " << scale << std::endl;
//
//		glm::vec3 translation = glm::vec3(current->posx, 0.0, current->posy);
//		//std::cout << "translation : (" << translation.x << "," << translation.y << "," << translation.z << ")" << std::endl;
//
//		terrainareashader->setuniform("color", color);
//		terrainareashader->setuniform("translation", translation);
//		terrainareashader->setuniform("scale", scale);
//
//		grid->bindbuffers();
//		grid->render();
//
//		glbindvertexarray(0);
//
//	}
//
//	std::cout << std::endl;
//	terrainareashader->unuse();
//}

