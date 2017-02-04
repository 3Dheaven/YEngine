#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

#include <GL/glew.h> 
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include "../scene/CMesh.h"

using namespace std;

class CGraphicDriver
{
private:
	int vbIdCounter;

public:

	CGraphicDriver();
	~CGraphicDriver();

	void init(CMesh *mesh);
	void bindMaterial(CMesh *mesh, CShaderFactory * shader);
	void unbindMaterial(CMesh *mesh);
	void render(CMesh *mesh, CShaderFactory * shader);
	void clean(CMesh *mesh);
};