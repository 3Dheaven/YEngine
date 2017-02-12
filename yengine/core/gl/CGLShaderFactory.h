#ifndef CGLSLSHADERFACTORY_H
#define CGLSLSHADERFACTORY_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <GL/glew.h>

#include "../CShader.h"
#include "CGLSLShaderProgram.h"
#include "CGLSLShader.h"

class CGLShaderFactory : public CShader
{
private:
	CGLSLShaderProgram* mShader;

public:
	CGLShaderFactory(const string vertexPath, const string fragmentPath) : CShader()
	{
		CGLSLShader vertTerrain(vertexPath.c_str(), GL_VERTEX_SHADER, "VERTEX");
		CGLSLShader fragTerrain(fragmentPath.c_str(), GL_FRAGMENT_SHADER, "FRAGMENT");
		mShader = new CGLSLShaderProgram({ vertTerrain, fragTerrain });
	}

	~CGLShaderFactory()
	{
		if (mShader != NULL)
		{
			delete mShader;
		}
	}

	void use() const
	{
		if (mShader != NULL)
		{
			mShader->use();
		}
	}

	void unuse() const
	{
		if (mShader != NULL)
		{
			mShader->unuse();
		}
	}

	void setUniform(const std::string &name, const glm::mat4 &m)
	{
		if (mShader != NULL)
		{
			mShader->setUniform(name, m);
		}
	}

	void setUniform(const std::string &name, const glm::mat3 &m)
	{
		if (mShader != NULL)
		{
			mShader->setUniform(name, m);
		}
	}

	void setUniform(const std::string &name, const glm::vec3 &v)
	{
		if (mShader != NULL)
		{
			mShader->setUniform(name, v);
		}
	}

	void setUniform(const std::string &name, const glm::vec4 &v)
	{
		if (mShader != NULL)
		{
			mShader->setUniform(name, v);
		}
	}

	void setUniform(const std::string &name, float f)
	{
		if (mShader != NULL)
		{
			mShader->setUniform(name, f);
		}
	}

	void setUniform(const std::string &name, int i)
	{
		if (mShader != NULL)
		{
			mShader->setUniform(name, i);
		}
	}
};

#endif