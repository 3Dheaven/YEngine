#include "CRendererManager.h"

CRendererManager::CRendererManager(MainWindow *mainWindow) : mMainWindow(mainWindow)
{
	mModulesCombobox = new wxComboBox(mainWindow->mMainPanel, ID_MODULES_COMBOBOX, "", { mMainWindow->GetSize().GetWidth() - 215,0 }, { 200,20 });
	mModulesCombobox->AppendString("OBJ_LOADER");
	mModulesCombobox->AppendString("TERRAIN_CDLOD");
	mModulesCombobox->SetStringSelection("TERRAIN_CDLOD");



	mGDriver = NULL;
	mRenderer = NULL;

	gApi = API_OPENGL;
	E_MODULES_EXAMPLES ex = CDLOD_TERRAIN;//OBJ_LOADER;
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
		mRenderer = new CRenderer(mGDriver, ex, mMainWindow->getSettingsWindow()->getDynamicPanel());
		glcanvas->setGModule(mRenderer);
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
	auto s = mModulesCombobox->GetStringSelection();

	if (gApi == API_OPENGL && glcanvas != NULL)
	{
		delete mRenderer;
		delete mGDriver;

		mGDriver = new CGLDriver();
		if (s == "TERRAIN_CDLOD")
		{
			mRenderer = new CRenderer(mGDriver, CDLOD_TERRAIN, mMainWindow->getSettingsWindow()->getDynamicPanel());
		}
		else if (s == "OBJ_LOADER")
		{
			mRenderer = new CRenderer(mGDriver, OBJ_LOADER, mMainWindow->getSettingsWindow()->getDynamicPanel());
		}

		glcanvas->setGModule(mRenderer);
	}
}