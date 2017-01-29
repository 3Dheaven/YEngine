#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

#include <GL/glew.h> 
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include "../CShaderFactory.h"
#include "CMaterial.h"
#include "CModel.h"
#include "CObjFile.h"
#include "../CShaderFactory.h"

using namespace std;

class CScene 
{
	public:

		std::string mName;
		std::vector<CModel*> mModels;

		CScene();
		~CScene();

		bool add(std::string modelPath);
		void render(CShaderFactory *shader);
		
};