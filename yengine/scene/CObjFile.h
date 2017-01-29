#pragma once
#include <GL/glew.h>
#include <glm.hpp>
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <memory>
#include <fstream>
#include "CMesh.h"
#include "CModel.h"

class CModel;

class CObjectFile 
{

	public:
		CObjectFile();
		CObjectFile(CModel* model);
		~CObjectFile();

		bool parse();

	private:
		std::string getComment(std::string& buffer);
		glm::vec3 getVector3(std::string& buffer);
		glm::vec2 getVector2(std::string& buffer);
		float getFloat(std::string& buffer);
		void getFace(std::string& buffer, std::vector<unsigned int> &v, std::vector<unsigned int> &vn, std::vector<unsigned int> &vt);
		void finalizeModel();
		void parseMtl(std::string mtlpath);
		void getTexture(std::string& buffer);

	private:
		std::string dirPath;
		CModel* mModel;

}; 