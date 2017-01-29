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
#include "CMesh.h"

using namespace std;

class CObject 
{
	public:

		std::string mName;
		std::vector<unsigned int> mMeshes;

		CObject(std::string name);
		~CObject();

};