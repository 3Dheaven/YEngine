#include "CScene.h"

CScene::CScene(CGraphicDriver *driver)
{
	gDriver = driver;
}

CScene::~CScene()
{
	for (auto model : mModels)
	{	
		for (auto mesh : model->mMeshes)
		{
			gDriver->clean(mesh);
			delete mesh;
		}
		delete model;
	}
}

bool 
CScene::add(std::string modelPath)
{
	std::string fileExt = modelPath.substr(modelPath.find_last_of(".") + 1);

	CModel* model = new CModel(modelPath);

	for (auto m : model->mMeshes)
	{
		gDriver->init(m);
	}
	
	mModels.push_back(model);

	return true;
}

void
CScene::add(CMesh* mesh)
{
	CModel* model = new CModel();

	model->mMeshes.push_back(mesh);

	for (auto m : model->mMeshes)
	{
		gDriver->init(m);
	}

	mModels.push_back(model);
}

void
CScene::render(CShader* shader)
{
	for (auto model : mModels)
	{
		for (auto mesh : model->mMeshes)
		{
			gDriver->render(mesh, shader);
		}
	}
}