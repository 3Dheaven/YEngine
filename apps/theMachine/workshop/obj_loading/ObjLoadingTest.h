#pragma once

#include "../../../yengine/scene/CScene.h"
#include "../../../yengine/scene/CCamera.h"
#include "../../../yengine/core/CGraphicDriver.h"
#include "../CModuleBase.h"
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

using namespace glm;

class ObjLoading : public CModuleBase
{
public:
	ObjLoading(CGraphicDriver *gdriver, wxPanel* panel = NULL);
	~ObjLoading();

	void setupGraphics();
	void render();
	CCamera* getCam();
	
private:
	CCamera *mCam;
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