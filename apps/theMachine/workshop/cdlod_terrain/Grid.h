#ifndef GRID_H
#define GRID_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

#include <GL/glew.h> 
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <sys/types.h>

#include "../../../../yengine/scene/CMesh.h"

using namespace std;

class Grid 
{
	public:

		Grid(int size);
		int getSize();
		CMesh* mMeshes;

	private :

		int mSize;
};

#endif