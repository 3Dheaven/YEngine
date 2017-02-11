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

class CVKDriver : CGraphicDriver
{
	public:

		CVKDriver();
		~CVKDriver();

		void init(CMesh *mesh) const override;
		void bindMaterial(CMesh *mesh, CShaderFactory * shader) const override;
		void unbindMaterial(CMesh *mesh) const override;
		void render(CMesh *mesh, CShaderFactory * shader) const override;
		void clean(CMesh *mesh) const override;
};