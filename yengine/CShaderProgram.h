#ifndef CGLSLSHADERPROGRAM_H
#define CGLSLSHADERPROGRAM_H

#include <GL/glew.h>
#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <initializer_list>
#include <vector>

#include "CShader.h"

class CShaderProgram 
{
	private:
		std::map<std::string, GLint> mAttributes;
		
	public:
			
		GLuint mProgramID;
		CShaderProgram();
		CShaderProgram(std::initializer_list<CShader> shaders);
		~CShaderProgram();

		void use() const;
		void unuse() const;
		GLuint getProgramID() const;

		GLint getUniformID(const std::string &name);
		void setUniform(const std::string &name, const glm::mat4 &m);
		void setUniform(const std::string &name, const glm::mat3 &m);
		void setUniform(const std::string &name, const glm::vec3 &v);
		void setUniform(const std::string &name, const glm::vec4 &v);
		void setUniform(const std::string &name, float f);
		void setUniform(const std::string &name, int i);

		GLint getAttributeID(const std::string &name);
		void setAttribute(const std::string &name, GLint size, GLsizei stride, GLuint offset, GLboolean normalize, GLenum type);
		void setAttribute(const std::string &name, GLint size, GLsizei stride, GLuint offset, GLboolean normalize);
		void setAttribute(const std::string &name, GLint size, GLsizei stride, GLuint offset, GLenum type);
		void setAttribute(const std::string &name, GLint size, GLsizei stride, GLuint offset);

};

#endif