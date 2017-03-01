#include "CRendererManager.h"

CRendererManager::CRendererManager(MainWindow *mainWindow) : mMainWindow(mainWindow)
{
	mModulesCombobox = new wxComboBox(mainWindow->mMainPanel, ID_MODULES_COMBOBOX, "", { mMainWindow->GetSize().GetWidth() - 215,0 }, { 200,20 });
	mModulesCombobox->AppendString("ObjLoading");
	mModulesCombobox->AppendString("TerrainCDLOD");
	/*ADD NEW RENDERERS HERE*/
	mModulesCombobox->SetStringSelection("TerrainCDLOD");

	map_type * rendererMap = RendererFactory::getMap();
	map_type::iterator it;
	for (it = rendererMap->begin(); it != rendererMap->end(); it++)
	{
		std::cout << "Renderer [] : " << it->first << std::endl;
	}
	

	mGDriver = NULL;
	mRenderer = RendererFactory::createInstance((std::string)mModulesCombobox->GetStringSelection());
	
	gApi = API_OPENGL;
	switch (gApi)
	{
	case API_VULKAN:
	{
		vcanvas = new CVulkanCanvas(mMainWindow->mMainPanel, wxID_ANY, { 0, 0 }, mMainWindow->GetSize());
		/*mGDriver = dynamic_cast<CGraphicDriver *>(new CVKDriver());
		mRenderer = new CRenderer(mGDriver);
		glcanvas->setGModule(mRenderer);*/
	}
	break;

	case API_OPENGL:
	{
		glcanvas = new CGLCanvas(mMainWindow->mMainPanel, ID_GL_CANVAS, nullptr, { 0, 0 }, mMainWindow->GetSize(), wxFULL_REPAINT_ON_RESIZE);
		mGDriver = new CGLDriver();
		mRenderer->init(mGDriver, mMainWindow->getSettingsWindow()->getDynamicPanel());
		glcanvas->setRenderer(mRenderer);
	}
	break;
	}

	mModulesCombobox->Bind(wxEVT_COMBOBOX, &CRendererManager::OnModuleChanged, this);
}

CRendererManager::~CRendererManager()
{
	if (mRenderer != NULL)
	{
		delete mRenderer;
		mRenderer = nullptr;
	}

	if (mGDriver != NULL)
	{
		delete mGDriver;
		mGDriver = nullptr;
	}
}

void 
CRendererManager::onResize()
{
	// Update position of the examples combobox in order to keep it on top-left
	mModulesCombobox->SetPosition({ mMainWindow->GetSize().GetWidth() - 215,0 });

	if (gApi == API_OPENGL && glcanvas != NULL)
	{
		glcanvas->SetSize(mMainWindow->GetSize());
		glcanvas->resize();
	}
}

void
CRendererManager::OnModuleChanged(wxCommandEvent& event)
{
	if (gApi == API_OPENGL && glcanvas != NULL)
	{
		//delete mRenderer;
		delete mGDriver;

		mGDriver = new CGLDriver();
		mRenderer = RendererFactory::createInstance((std::string)mModulesCombobox->GetStringSelection());

		mRenderer->init(mGDriver, mMainWindow->getSettingsWindow()->getDynamicPanel());
		glcanvas->setRenderer(mRenderer);
	}
}