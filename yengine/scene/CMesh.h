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

using namespace std;

struct sVertex 
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoord;
};

class CMesh 
{
	public:
		std::vector< glm::vec3 > vertices;
		std::vector< glm::vec2 > uvs;
		std::vector< glm::vec3 > normals;

		std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
		std::vector< glm::vec3 > temp_vertices;
		std::vector< glm::vec2 > temp_uvs;
		std::vector< glm::vec3 > temp_normals;

		GLuint mVao;
		GLuint mVbuf;
		GLuint mUVbuf;
		GLuint mNbuf;

		bool mHasTexcoords;
		bool mHasNormals;

		bool isFinalized;
		std::string mName;
		CMaterial *mMaterial;

		CMesh(std::string name);
		~CMesh();
};