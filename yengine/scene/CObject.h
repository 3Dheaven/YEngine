#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include "CMesh.h"

using namespace std;

class CObject 
{
	public:

		std::string mName;
		std::vector<unsigned int> mMeshes;

		CObject(std::string name);
		~CObject();

};