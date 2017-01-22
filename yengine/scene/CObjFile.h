#pragma once
#include <GL/glew.h>
#include <glm.hpp>
#include <vector>
#include <iostream>
#include <string>
#include <memory>
#include <fstream>
#include "CMesh.h"

class CObjectFile 
{
	private:
		const char * m_filePath;
		std::unique_ptr<CMesh> m_currentMesh;
	
		unsigned int cptPos, cptNormal, cptUv;
		unsigned int offsetPos, offsetNormal, offsetUv;

	public:
		CObjectFile(const char* path);
		~CObjectFile();

		bool parse(std::vector<std::unique_ptr<CMesh>>& meshes);

	private:
		std::string getComment(std::string& buffer);
		glm::vec3 getVector3(std::string& buffer);
		glm::vec2 getVector2(std::string& buffer);
		void getFace(std::string& buffer, unsigned int *v, unsigned int *vn, unsigned int *vt);
		bool createObject(std::string& buffer);
		void finalizeObject();
};