#pragma once

#include <memory>
#include "../../../yengine/core/gl/CGLdriver.h"
#include "../../../yengine/core/vk/CVKdriver.h"
#include "CVulkanCanvas.h"
#include "CGLCanvas.h"
#include "MainWindow.h"
#include "CRenderer.h"
#include "CVulkanCanvas.h"
#include "wx/wxprec.h"
#include <wx/combobox.h>
#include <wx/wx.h>
#include <wx/event.h>
#include "RendererFactory.h"

class CRenderer;
class CVulkanCanvas;
class MainWindow;

class CRendererManager
{
private:

	enum
	{
		ID_MODULES_COMBOBOX = wxID_HIGHEST + 1,
		ID_GL_CANVAS
	};

	enum E_API3D
	{
		API_VULKAN,
		API_OPENGL
	};

	MainWindow *mMainWindow;
	wxComboBox *mModulesCombobox;
	CRenderer* mRenderer;
	CGraphicDriver* mGDriver;
	CVulkanCanvas* vcanvas;
	CGLCanvas* glcanvas;
	E_API3D gApi;

	void OnModuleChanged(wxCommandEvent& event);

public:
	CRendererManager(MainWindow *parent);
	~CRendererManager();

	void onResize();
};