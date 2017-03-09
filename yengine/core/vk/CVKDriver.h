#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

#include <GL/glew.h> 
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include "../CGraphicDriver.h"

using namespace std;

class CVKDriver : public CGraphicDriver
{
	public:

		CVKDriver();
		~CVKDriver();

		void init(CMesh *mesh) const override;
		void bindMaterial(CMesh *mesh, CShader* shader) const override;
		void unbindMaterial(CMesh *mesh) const override;
		void render(CMesh *mesh, CShader* shader) const override;
		void clean(CMesh *mesh) const override;
		void createShader(const string& vertexShader, const string& pixelShader) {};
};