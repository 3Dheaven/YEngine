#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

#include <GL/glew.h> 
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include "CMaterial.h"

using namespace std;

struct sVertex 
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texcoord;
};

class CMesh 
{
	public:

		std::vector<sVertex> mVertices;
		std::vector<unsigned int> mIndices;
		std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;

		int mNbVertices;
		int mNbFaces;

		GLuint mVao;
		GLuint mVbo;
		GLuint mEbo;

		bool mHasTexcoords;
		bool mHasNormals;
		bool mHasMaterial;

		bool isFinalized;
		std::string mName;
		CMaterial *mMaterial;

		CMesh(std::string name);
		~CMesh();


};