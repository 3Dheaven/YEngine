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
#include "CMaterial.h"
#include "CObject.h"

using namespace std;

class CModel
{
	public:
		
		//! Material map
		std::map<std::string, CMaterial*> mMaterialMap;
		//! All loaded position vertices
		std::vector<glm::vec3> mVertices;
		//! All loaded texture coordinates vertices
		std::vector<glm::vec2> mTexcoords;
		//! All loaded normals vertices
		std::vector<glm::vec3> mNormals;
		//! Model name
		std::string mName;
		//! List of objects which compose the model
		vector<CObject*> mObjects;
		//! List of all meshes
		vector<CMesh*> mMeshes;
		//! Pointer on current mesh
		CMesh* mCurrentMesh;
		//! Pointer on current material
		CMaterial* mCurrentMaterial;
		//! Pointer on current object
		CObject* mCurrentObject;
		//! Model path
		std::string mFilePath;
		//! Default material
		CMaterial* mDefaultMaterial;

	public:

		CModel(std::string path);
		~CModel();

		void render(CShaderFactory *sh);
};

