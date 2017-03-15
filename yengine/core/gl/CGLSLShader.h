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
		std::string mName;
		CGLSLShader() { mName = "no-name"; };
		CGLSLShader(const GLchar* shaderFile, GLenum shaderType, std::string type);
		~CGLSLShader();
		GLuint getShaderID() const;
};

#endif