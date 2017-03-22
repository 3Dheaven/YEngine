#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

#include <GL/glew.h> 
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <SOIL.h>

using namespace std;

class CTexture
{
	public:

		GLuint mID;
		std::string mFilePath;

		CTexture(const char* filepath);
		~CTexture();
		
};