#include "CModel.h"

CModel::CModel(std::string path) :
	mFilePath(path)
{
	auto filename = path.substr(path.find_last_of("/\\") + 1);
	mName = filename.substr(0, filename.find("."));

	std::cout << "Load file : " << filename << std::endl;

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return;
	}

	mDirectory = path.substr(0, path.find_last_of('/'));

	processMaterial(scene);

	processNode(scene->mRootNode, scene);
}

CModel::CModel()
{

}

CModel::~CModel()
{
	for (auto mtl : mMaterials)
	{
		delete mtl;
	}
}

void 
CModel::processNode(aiNode* node, const aiScene* scene)
{
	// Process all the node's meshes (if any)
	for (GLuint i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		CMesh *m = new CMesh(mesh, scene, mDirectory);
		
		// Link material
		// A mesh only contains an index to a material object
		if (mesh->mMaterialIndex >= 0)
		{
			m->mMaterial = mMaterials[mesh->mMaterialIndex];
		}

		mMeshes.push_back(m); 
	}
	// Then do the same for each of its children
	for (GLuint i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}

void 
CModel::processMaterial(const aiScene* scene)
{
	for (GLuint i = 0; i < scene->mNumMaterials; i++)
	{
		CMaterial* mtl = new CMaterial(scene->mMaterials[i], mDirectory);
		mMaterials.push_back(mtl);
	}
}
