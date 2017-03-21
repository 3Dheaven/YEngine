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
			sVertex v1, v2, v3, v4;

			v1 = { glm::vec3(i, 0.0f, j), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f) };
			mMeshes->mVertices.push_back(v1);
			index = index + 1;

			v2 = { glm::vec3(i + 1.0f, 0.0f, j), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f) };
			mMeshes->mVertices.push_back(v2);
			index = index + 1;

			v3 = { glm::vec3(i + 1.0f, 0.0f, j + 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f) };
			mMeshes->mVertices.push_back(v3);
			index = index + 1;

			v4 = { glm::vec3(i, 0.0f, j + 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f) };
			mMeshes->mVertices.push_back(v4);

			mMeshes->mIndices.push_back(index - 3); // 0
			mMeshes->mIndices.push_back(index - 1); // 2
			mMeshes->mIndices.push_back(index);		// 3
			mMeshes->mIndices.push_back(index - 3); // 0
			mMeshes->mIndices.push_back(index - 2); // 1
			mMeshes->mIndices.push_back(index - 1); // 2

			index = index + 1;		
		}
	}
};

int Grid::getSize()
{
	return mSize;
}
