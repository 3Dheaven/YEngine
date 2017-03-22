#pragma once

#include "../../src/CRenderer.h"
#include "../../src/RendererFactory.h"
#include "../../../yengine/scene/CScene.h"
#include "../../../yengine/scene/CCamera.h"
#include "../../../yengine/core/CGraphicDriver.h"
#include "../../../../yengine/helpers/StringsHelper.h"
#include "../../../../yengine/system/FileSystem.h"

#include <memory>
#include "wx/glcanvas.h"
#include "wx/wxprec.h"

#include <GL/glew.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <chrono>
#include <wx/combobox.h>
#include <wx/clrpicker.h>
#include <wx/sizer.h>


class CRenderer;
using namespace glm;

class ObjLoading : public CRenderer
{
public:
	ObjLoading();
	ObjLoading(CGraphicDriver *gdriver, wxPanel* panel = NULL);
	~ObjLoading();
	
	void init(CGraphicDriver *gdriver, wxPanel* panel = NULL);
	void setupGraphics();
	void render();
	std::shared_ptr<CCamera> getCam();
	
private:
	std::shared_ptr<CCamera> mCam;
	CScene *mScene;
	CGraphicDriver *mGDriver;

	glm::vec4 mUniformColor;
	bool mUniformColorHasChanged;

public:
	void loadGUI();
	void cleanGUI();

private:

	enum
	{
		IdColorPicker_1
	};

	wxPanel* mRightPanel;
	bool mColorHasChanged;
	wxColour mColor;
};
