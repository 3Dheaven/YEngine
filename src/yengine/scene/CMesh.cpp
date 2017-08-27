#include "CMesh.h"

CMesh::CMesh(std::string name)
{
	mName = name;
	mMaterial = NULL;
	mNbVertices = 0;
	mNbFaces = 0;
	mMaterial = NULL;
	mHasTexcoord0 = false;
}

CMesh::~CMesh()
{

}