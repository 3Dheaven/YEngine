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
	CTexture(const char* filepath);
	CTexture();
	~CTexture();

	GLuint mID;
	std::string mFilePath;
};