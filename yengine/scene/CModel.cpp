#include "CModel.h"

CModel::CModel(std::string path)
{
	mCurrentMesh = NULL;
	mCurrentMaterial = NULL;
	mCurrentObject = NULL; 
	mDefaultMaterial = NULL;
	mFilePath = path;

	auto filename = path.substr(path.find_last_of("/\\") + 1);
	mName = filename.substr(0, filename.find("."));
}

CModel::CModel()
{
	mCurrentMesh = NULL;
	mCurrentMaterial = NULL;
	mCurrentObject = NULL;
	mDefaultMaterial = NULL;
	mFilePath = "";
	mName = "unknown";
}

CModel::~CModel()
{
	if(mDefaultMaterial != NULL)
		delete mDefaultMaterial;
}

void
CModel::addObject(CObject* object, std::vector<CMesh*> meshes)
{
	mCurrentObject = object;
	mObjects.push_back(mCurrentObject);

	for (int i = 0; i < meshes.size(); i++)
	{
		mMeshes.push_back(meshes[i]);
		unsigned int meshId = static_cast<unsigned int>(mMeshes.size() - 1);
		mCurrentObject->mMeshes.push_back(meshId);
	}
}
