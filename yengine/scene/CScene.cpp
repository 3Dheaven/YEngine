#include "CScene.h"

CScene::CScene()
{

}

CScene::~CScene()
{
	for (auto i : mModels)
	{
		delete i;
	}
}

bool 
CScene::add(std::string modelPath)
{
	std::string ext = modelPath.substr(modelPath.find_last_of(".") + 1);

	if (ext == "obj")
	{
		std::cout << "> Format .obj supported ..." << std::endl;

		CModel* model = new CModel(modelPath);

		CObjectFile* obj = new CObjectFile(model);
		if (obj->parse())
		{
			mModels.push_back(model);

			for (auto i : mModels)
			{
				for (auto o : i->mObjects)
				{
					for (auto m : o->mMeshes)
					{
						i->mMeshes[m]->setupMesh();
					}
				}
			}

		}	
		delete obj;
	}
	else
	{
		std::cout << "> Files with extension : ." << ext << " are not supported ..." << std::endl;
		return false;
	}

	return true;
}

void
CScene::render(CShaderFactory *shader)
{
	for (auto m : mModels)
	{
		for (auto o : m->mObjects)
		{
			for (auto i : o->mMeshes)
			{
				m->mMeshes[i]->render();
			}
		}
	}
}