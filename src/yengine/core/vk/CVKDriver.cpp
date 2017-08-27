#include "CVKDriver.h"

#ifdef USE_VULKAN

CVKDriver::CVKDriver() : CGraphicDriver()
{
}

CVKDriver::~CVKDriver()
{
}

void CVKDriver::init(CMesh *mesh)
{
	mVulkan.prepareVertices(mesh->mVertices, mesh->mIndices);
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
	//mVulkan.prepareUniformBuffer(value);
	mVulkan.updateUniform(value, name);
}

void
CVKDriver::updateUniform(const char* name, glm::mat4& value)
{
	mVulkan.updateUniform(value, name);
}

void
CVKDriver::addUniform(const char* name, glm::vec4& value)
{
	mVulkan.prepareUniformBuffer(value, name);
}

void
CVKDriver::addUniform(const char* name, glm::mat4& value)
{
	mVulkan.prepareUniformBuffer(value, name);
}

void
CVKDriver::finalizeSetup()
{
	mVulkan.finalizeSetup();
}

void 
CVKDriver::createShader(const string& vertexShader, const string& pixelShader) 
{
	mVulkan.setupShader(vertexShader, pixelShader);
}

#endif