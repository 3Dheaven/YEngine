#ifndef CGLSLSHADERFACTORY_H
#define CGLSLSHADERFACTORY_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "../CShader.h"

class CVKShaderFactory : public CShader
{
	public:
		CVKShaderFactory(const std::string vertexPath, const std::string fragmentPath)
		{
			
		}

		~CVKShaderFactory()
		{
		}

		void use() const
		{

		}

		void unuse() const
		{
		
		}

		void setUniform(const std::string &name, const glm::mat4 &m)
		{
	
		}

		void setUniform(const std::string &name, const glm::mat3 &m)
		{
		
		}

		void setUniform(const std::string &name, const glm::vec3 &v)
		{
		
		}

		void setUniform(const std::string &name, const glm::vec4 &v)
		{
		
		}

		void setUniform(const std::string &name, float f)
		{
		
		}

		void setUniform(const std::string &name, int i)
		{
		
		}
};

#endif