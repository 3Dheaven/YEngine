#ifndef CSHADER_H
#define CSHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm.hpp>

class CShader
{
public:
	virtual ~CShader() {};
	virtual void use() const = 0;
	virtual void unuse() const = 0;
	virtual void setUniform(const std::string &name, const glm::mat4 &m) = 0;
	virtual void setUniform(const std::string &name, const glm::mat3 &m) = 0;
	virtual void setUniform(const std::string &name, const glm::vec3 &v) = 0;
	virtual void setUniform(const std::string &name, const glm::vec4 &v) = 0;
	virtual void setUniform(const std::string &name, float f) = 0;
	virtual void setUniform(const std::string &name, int i) = 0;
};

#endif