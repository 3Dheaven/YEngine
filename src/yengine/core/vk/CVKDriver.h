#pragma once

//#include "YEngine_config.h"

#ifdef USE_VULKAN

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

#include <GL/glew.h> 
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include "../CGraphicDriver.h"
#include "CVulkan.h"

using namespace std;

class CVKDriver : public CGraphicDriver
{
	public:

		CVulkan mVulkan;

		CVKDriver();
		~CVKDriver();

		void updateUniform(const char* name, glm::vec4& value);
		void updateUniform(const char* name, glm::mat4& value);
		void addUniform(const char* name, glm::vec4& value);
		void addUniform(const char* name, glm::mat4& value);
		void init(CMesh *mesh) override;
		void init() override;
		void bindMaterial(CMesh *mesh, CShader* shader) const override;
		void unbindMaterial(CMesh *mesh) const override;
		void render(CMesh *mesh, CShader* shader) override;
		void render() override;
		void clean(CMesh *mesh) const override;
		void createShader(const string& vertexShader, const string& pixelShader);
		void finalizeSetup();
};

#endif