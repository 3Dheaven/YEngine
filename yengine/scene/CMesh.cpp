#include "CMesh.h"

CMesh::CMesh(std::string name)
{
	mHasNormals = false;
	mHasTexcoords = false;
	mHasMaterial = false;
	mName = name;
	mMaterial = NULL;
	mNbVertices = 0;
	mNbFaces = 0;
}

CMesh::~CMesh()
{
	/*if (mMaterial != NULL)
	{
		delete mMaterial;
	}*/
}