#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

#include <GL/glew.h> 
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include "CMesh.h"
#include "CObjFile.h"
#include "../CShaderFactory.h"

using namespace std;

class CModel
{
	private:
		vector<std::unique_ptr<CMesh>> m_meshes;
		const char* m_filepath;

	public:
		CModel();

		// Constructor, expects a filepath to a 3D model.
		CModel(const char* path);

		// Draws the model, and thus all its meshes
		void render(CShaderFactory *sh);

	private:
		void loadModel(const char* path);
};

