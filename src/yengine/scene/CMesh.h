#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

#include <GL/glew.h> 
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include "CMaterial.h"

#include <assimp/types.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


using namespace std;

struct sVertex 
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texcoord;
};

class CMesh 
{
	public:

		std::vector<sVertex> mVertices;
		std::vector<unsigned int> mIndices;

		int mNbVertices;
		int mNbFaces;

		GLuint mVao;
		GLuint mVbo;
		GLuint mEbo;

		bool mHasTexcoord0;

		std::string mName;
		std::string mDir;

		CMaterial* mMaterial;

		CMesh(std::string name);
		CMesh(aiMesh* mesh, const aiScene* scene, std::string dir)
		{
			mDir = dir;
			mNbVertices = mesh->mNumVertices;
			mNbFaces = mesh->mNumFaces;
			if(mesh->mTextureCoords[0])
				mHasTexcoord0 = true;

			// Fetch vertex
			for (GLuint i = 0; i < mesh->mNumVertices; i++)
			{
				sVertex vertex;

				// positions
				vertex.position.x = mesh->mVertices[i].x;
				vertex.position.y = mesh->mVertices[i].y;
				vertex.position.z = mesh->mVertices[i].z;

				// normals
				vertex.normal.x = mesh->mNormals[i].x;
				vertex.normal.y = mesh->mNormals[i].y;
				vertex.normal.z = mesh->mNormals[i].z;

				// texture coords
				// Does the mesh contain texture coordinates?
				if (mHasTexcoord0)
				{
					vertex.texcoord.x = mesh->mTextureCoords[0][i].x;
					vertex.texcoord.y = mesh->mTextureCoords[0][i].y;
				}
				else
				{
					vertex.texcoord = glm::vec2(0.0f, 0.0f);
				}

				mVertices.push_back(vertex);
			}

			// Process indices
			for (GLuint i = 0; i < mesh->mNumFaces; i++)
			{
				aiFace face = mesh->mFaces[i];
				for (GLuint j = 0; j < face.mNumIndices; j++)
				{
					mIndices.push_back(face.mIndices[j]);

				}
			}

		};
		~CMesh();


};