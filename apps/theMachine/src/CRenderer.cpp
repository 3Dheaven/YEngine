#include "CRenderer.h"

CRenderer::CRenderer(/*CGraphicDriver *gdriver, E_MODULES_EXAMPLES renderingWorshopChoice, wxPanel *panel*/)
{
	/*mRenderingWorkshopChoice = renderingWorshopChoice;
	mGDriver = gdriver;
	mRightPanel = panel;
	loadModule();
	setupGraphics();*/
}

CRenderer::~CRenderer()
{
	/*if (mActiveModule != NULL)
	{
		delete mActiveModule;
	}*/
}

/*CCamera* 
CRenderer::getCam() 
{
	if (mActiveModule != NULL)
	{
		return mActiveModule->getCam();
	}

	return NULL;
}*/

void 
CRenderer::setupGraphics()
{
	/*if (mActiveModule != NULL)
	{
		mActiveModule->setupGraphics();
	}*/
}

void 
CRenderer::render()
{
	/*if (mActiveModule != NULL)
	{
		mActiveModule->render();
	}*/
}

/*void CRenderer::loadModule()
{
	switch (mRenderingWorkshopChoice)
	{
		case OBJ_LOADER:
		{
			mActiveModule = new ObjLoading(mGDriver, mRightPanel);
		}
		break;

		case CDLOD_TERRAIN:
		{
			mActiveModule = new TerrainCDLOD(mGDriver, mRightPanel);
		}
		break;

		default:
			mActiveModule = NULL;
	}
}*/