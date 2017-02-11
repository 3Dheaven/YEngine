#pragma once

#include "../../../yengine/scene/CCamera.h"

class CModuleBase
{
public:
	virtual void setupGraphics() = 0;
	virtual void render() = 0;
	virtual CCamera* getCam() = 0;
};