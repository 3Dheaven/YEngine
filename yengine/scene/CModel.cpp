#include "CModel.h"

CModel::CModel()
{

}

CModel::CModel(const char* path)
{
	m_filepath = path;
	loadModel(path);
}

void 
CModel::loadModel(const char* path)
{
	std::string ext = std::string(path).substr(std::string(path).find_last_of(".") + 1);

	if (ext == "obj")
	{
		std::cout << "> Format .obj supported ..." << std::endl;
		CObjectFile obj(path);
		obj.parse(m_meshes);

		for (auto &i : m_meshes)
		{
			i.get()->setupMesh(/*sh*/);
		}
	}
	else
	{
		std::cout << "> Files ." << ext << " not supported ..." << std::endl;
	}
}

void 
CModel::render(CShaderFactory *sh)
{
	for (auto &i : m_meshes)
	{
		i.get()->render(/*sh*/);
	}
}
