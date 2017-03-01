#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include "QuadTreeNode.h"
#include "HeightMap.h"
#include "QuadTreeNode.h"
#include "Grid.h"
#include "HeightMap.h"
#include "../../src/CRenderer.h"
#include "../../src/RendererFactory.h"
#include "../../../../yengine/core/CGraphicDriver.h"
#include "../../../../yengine/scene/CCamera.h"
#include "../../../../yengine/scene/CScene.h"
#include "../../../../yengine/helpers/StringsHelper.h"
#include "../../../../yengine/system/FileSystem.h"
#include "wx/glcanvas.h"
#include "wx/wxprec.h"
#include <wx/clrpicker.h>
#include <wx/sizer.h>

class CRenderer;

class TerrainCDLOD : public CRenderer
{

private:

	int mLodDepth;
	std::vector<int> mRanges;
	HeightMap * mHeightMap;
	QuadTreeNode * mRootNode;
	int mapSize;
	Grid *grid;
	Grid *fullResGrid;
	Grid *halfResGrid;
	HeightMap *hmap;
	std::vector< std::vector<QuadTreeNode*> > gridmap;
	
	CScene *mScene;
	CGraphicDriver *mGDriver;
	CCamera *mCam;
	wxPanel* mRightPanel;

	void loadGUI();
	void cleanGUI();

private:
	bool mColorHasChanged;
	wxColour mColor;
	glm::vec4 mUniformColor;
	bool mUniformColorHasChanged;

public:
	TerrainCDLOD();
	TerrainCDLOD(CGraphicDriver *gdriver, wxPanel* panel = NULL);
	~TerrainCDLOD();

	void init(CGraphicDriver *gdriver, wxPanel* panel = NULL);

	void setupGraphics();
	void render();

	CCamera* getCam();

};
