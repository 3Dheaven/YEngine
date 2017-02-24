#include "Grid.h"

Grid::Grid(int size) : 
	mSize(size) 
{
	int index = 0;
	mMeshes = new CMesh("2dGrid");
	
	// Generate GRID
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			glm::vec3 v1, v2, v3, v4, v5, v6;

			v1 = glm::vec3(i, 0.0f, j);
			mVertices.push_back(v1);
			index = index + 1;

			v4 = glm::vec3(i + 1.0f, 0.0f, j);
			mVertices.push_back(v4);
			index = index + 1;

			v5 = glm::vec3(i + 1.0f, 0.0f, j + 1.0f);
			mVertices.push_back(v5);
			index = index + 1;

			v6 = glm::vec3(i, 0.0f, j + 1.0f);
			mVertices.push_back(v6);

			mMeshes->vertexIndices.push_back(index); // 0
			mMeshes->vertexIndices.push_back(index - 1); // 2
			mMeshes->vertexIndices.push_back(index - 3); // 3

			mMeshes->vertexIndices.push_back(index - 3); // 0
			mMeshes->vertexIndices.push_back(index - 2); // 1
			mMeshes->vertexIndices.push_back(index - 1); // 2

			mMeshes->vertices.push_back(mVertices[index]);
			mMeshes->vertices.push_back(mVertices[index - 1]);
			mMeshes->vertices.push_back(mVertices[index - 3]);

			mMeshes->vertices.push_back(mVertices[index - 3]);
			mMeshes->vertices.push_back(mVertices[index - 2]);
			mMeshes->vertices.push_back(mVertices[index - 1]);

			index = index + 1;

			
		}
	}
};

int Grid::getSize()
{
	return mSize;
}
