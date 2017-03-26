#pragma once

#include <string>
#include <iostream>
#include <map>
#include <vector>
#include <GL/glew.h> 
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include "../image/CTextureManager.h"
#include "CMesh.h"

using namespace std;

class CModel
{
public:
	CModel(std::string path);
	CModel();
	~CModel();

	vector<CMesh*> mMeshes;
	vector<CMaterial*> mMaterials;

private:
	void processNode(aiNode* node, const aiScene* scene);
	void processMaterial(const aiScene* scene);
	
	std::string mName;
	std::string mFilePath;
	std::string mDirectory;
};

