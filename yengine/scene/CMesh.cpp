#include "CMesh.h"

CMesh::CMesh(std::string name)
{
	mHasNormals = false;
	mHasTexcoords = false;
	mName = name;
	mMaterial = NULL;
}

CMesh::~CMesh()
{
	delete mMaterial;
}