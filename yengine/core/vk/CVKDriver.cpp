#include "CVKDriver.h"

CVKDriver::CVKDriver() : CGraphicDriver()
{
}

CVKDriver::~CVKDriver()
{
}

void CVKDriver::init(CMesh *mesh)
{
	mVulkan.prepareVertices();
}

void CVKDriver::init()
{
	//mVulkan.prepare(hwnd, size);
	
}

void CVKDriver::bindMaterial(CMesh *mesh, CShader *shader) const
{
}

void CVKDriver::unbindMaterial(CMesh *mesh) const
{
}

void CVKDriver::render(CMesh *mesh, CShader *shader)
{
	mVulkan.render();
}

void
CVKDriver::render()
{
	mVulkan.render();
}

void CVKDriver::clean(CMesh * mesh) const
{
}

void
CVKDriver::updateUniform(const char* name, glm::vec4& value)
{
	mVulkan.updateUniforms(value);
}

void
CVKDriver::addUniform(const char* name, glm::vec4& value)
{
	mVulkan.prepareUniformBuffers(value);
}

void
CVKDriver::finalizeSetup()
{
	mVulkan.finalizeSetup();
}