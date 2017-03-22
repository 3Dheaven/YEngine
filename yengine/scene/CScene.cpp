#include "CScene.h"

CScene::CScene(CGraphicDriver *driver)
{
	gDriver = driver;
}

CScene::~CScene()
{
	for (auto i : mModels)
	{
		for (auto o : i->mObjects)
		{
			for (auto m : o->mMeshes)
			{
				gDriver->clean(i->mMeshes[m]);
				if(i->mMeshes[m])
					delete i->mMeshes[m];
			}
			delete o;
		}
		delete i;
	}
}

void 
CScene::add(CModel* model)
{
	for (auto o : model->mObjects)
	{
		for (auto m : o->mMeshes)
		{
			gDriver->init(model->mMeshes[m]);
		}
	}
	mModels.push_back(model);
}

void 
CScene::addCustomModel(std::vector<sVertex> vertices, const char* name)
{

}

bool 
CScene::add(std::string modelPath)
{
	std::string ext = modelPath.substr(modelPath.find_last_of(".") + 1);

	if (ext == "obj")
	{
		std::cout << "> Format .obj supported ..." << std::endl;

		CModel* model = new CModel(modelPath);

		CObjectFile obj = CObjectFile(model);
		if (obj.parse())
		{
			for (auto o : model->mObjects)
			{
				for (auto m : o->mMeshes)
				{
					gDriver->init(model->mMeshes[m]);
				}
			}

			mModels.push_back(model);
		}	
	}
	else
	{
		std::cout << "> Files with extension : ." << ext << " are not supported ..." << std::endl;
		return false;
	}

	return true;
}

void
CScene::render(CShader* shader)
{
	for (auto i : mModels)
	{
		for (auto o : i->mObjects)
		{
			for (auto m : o->mMeshes)
			{
				gDriver->render(i->mMeshes[m], shader);
			}
		}
	}
}