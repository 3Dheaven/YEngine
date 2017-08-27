#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include "CTexture.h"
#include <GL/glew.h> 
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <SOIL.h>

using namespace std;

class CTextureManager
{
	public:

		std::vector<CTexture> mTextures;
		CTextureManager();
		~CTextureManager() {};
		
};