#include "CModel.h"

CModel::CModel(std::string path)
{
	mCurrentMesh = NULL;
	mCurrentMaterial = NULL;
	mCurrentObject = NULL; 
	mDefaultMaterial = new CMaterial("dafault-mat");
	mFilePath = path;

	auto filename = path.substr(path.find_last_of("/\\") + 1);
	mName = filename.substr(0, filename.find("."));
}

CModel::~CModel()
{
	delete mDefaultMaterial;
}
