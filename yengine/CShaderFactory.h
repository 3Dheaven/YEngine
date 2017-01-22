#ifndef CGLSLSHADERFACTORY_H
#define CGLSLSHADERFACTORY_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <GL/glew.h>

#include "CGLSLShaderProgram.h"
#include "CGLSLShader.h"

class CShaderFactory
{
	public:
		CGLSLShaderProgram *shader;

		CShaderFactory(const GLchar* vertexPath, const GLchar* fragmentPath)
		{
			CGLSLShader vertTerrain(vertexPath, GL_VERTEX_SHADER, "VERTEX");
			CGLSLShader fragTerrain(fragmentPath, GL_FRAGMENT_SHADER, "FRAGMENT");
			shader = new CGLSLShaderProgram({ vertTerrain, fragTerrain });
		}

		~CShaderFactory()
		{
			delete shader;
		}
};

#endif