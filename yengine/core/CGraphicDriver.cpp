#include "CGraphicDriver.h"

CGraphicDriver::CGraphicDriver() : mCustomShader(nullptr), vbIdCounter(0)
{
}

CGraphicDriver::~CGraphicDriver()
{
	if (mCustomShader != nullptr)
	{
		delete mCustomShader;
		mCustomShader = nullptr;
	}
}

CShader *CGraphicDriver::getShader() const
{
	return mCustomShader;
}