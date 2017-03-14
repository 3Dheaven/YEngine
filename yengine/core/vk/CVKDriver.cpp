#include "CVKDriver.h"

CVKDriver::CVKDriver() : CGraphicDriver()
{
}

CVKDriver::~CVKDriver()
{
}

void CVKDriver::init(CMesh *mesh) const
{
}

void CVKDriver::init()
{
	//mVulkan.prepare(hwnd, size);
	mVulkan.prepare2();
}

void CVKDriver::bindMaterial(CMesh *mesh, CShader *shader) const
{
}

void CVKDriver::unbindMaterial(CMesh *mesh) const
{
}

void CVKDriver::render(CMesh *mesh, CShader *shader) const
{
	
}

void
CVKDriver::render()
{
	mVulkan.render();
}

void CVKDriver::clean(CMesh * mesh) const
{
}
