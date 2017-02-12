#ifndef CGLSLSHADERFACTORY_H
#define CGLSLSHADERFACTORY_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <GL/glew.h>

#include "CShaderFactory.h"
#include "CGLSLShaderProgram.h"
#include "CGLSLShader.h"

class CGLShaderFactory : public CShaderFactory
{
	public:
		CGLShaderFactory(const string vertexPath, const string fragmentPath) : CShaderFactory()
		{
			CGLSLShader vertTerrain(vertexPath.c_str(), GL_VERTEX_SHADER, "VERTEX");
			CGLSLShader fragTerrain(fragmentPath.c_str(), GL_FRAGMENT_SHADER, "FRAGMENT");
			mShader = dynamic_cast<CShaderProgram *>(new CGLSLShaderProgram({ vertTerrain, fragTerrain }));
		}

		~CGLShaderFactory()
		{
		}
};

#endif