#pragma once

/*#include "../../../yengine/scene/CScene.h"

#include "../../../yengine/core/CGraphicDriver.h"
#include "../workshop/CModuleBase.h"
#include "../workshop/obj_loading/ObjLoadingTest.h"
#include "../workshop/cdlod_terrain/TerrainCDLOD.h"*/

//#include "RendererFactory.h"
#include "../../../yengine/scene/CCamera.h"
#include "../../../yengine/core/CGraphicDriver.h"
#include "wx/wxprec.h"
class CRenderer
{
public:
	CRenderer();
	~CRenderer();

	virtual void setupGraphics();
	virtual void render();
	virtual void init(CGraphicDriver *gdriver, wxPanel* panel) {};
	virtual CCamera* getCam() { return NULL; };
	//void loadModule();

//private:

	//wxPanel* mRightPanel;
	//CGraphicDriver* mGDriver;
	//E_MODULES_EXAMPLES mRenderingWorkshopChoice;
	//CModuleBase* mActiveModule;

};

//RendererRegister<CRenderer> regRendererBase("CRenderer");