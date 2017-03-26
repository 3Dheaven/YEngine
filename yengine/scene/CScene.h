#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <GL/glew.h> 
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include "../core/CShader.h"
#include "CModel.h"
#include "../core/CGraphicDriver.h"
#include "../image/CTextureManager.h"

using namespace std;

class CScene 
{
public:
	
	CScene(CGraphicDriver *driver);
	~CScene();

	bool add(std::string modelPath);
	void add(CMesh* mesh);
	void render(CShader *shader);

private:

	CGraphicDriver *gDriver;
	std::string mName;
	std::vector<CModel*> mModels;
	CTextureManager mTexMngr;
		
};