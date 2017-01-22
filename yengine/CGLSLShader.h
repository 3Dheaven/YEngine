#ifndef CGLSLSHADER_H
#define CGLSLSHADER_H

#include <GL/glew.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class CGLSLShader 
{
	private:
		GLuint mShaderID;

	public:
		CGLSLShader() {};
		CGLSLShader(const GLchar* shaderFile, GLenum shaderType, std::string type);
		~CGLSLShader();
		GLuint getShaderID() const;
};

#endif