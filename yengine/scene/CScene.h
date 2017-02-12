#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

#include <GL/glew.h> 
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include "../core/CShader.h"
#include "CMaterial.h"
#include "CModel.h"
#include "CObjFile.h"
#include "../core/CGraphicDriver.h"

using namespace std;

class CScene 
{
public:
	
	CScene(CGraphicDriver *driver);
	~CScene();

	bool add(std::string modelPath);
	void render(CShader *shader);
	void setup();

private:

	CGraphicDriver *gDriver;
	std::string mName;
	std::vector<CModel*> mModels;
		
};