#include "CGLSLShaderProgram.h"

CGLSLShaderProgram::CGLSLShaderProgram() 
{

}

CGLSLShaderProgram::~CGLSLShaderProgram() 
{

	glDeleteProgram(mProgramID);

}

CGLSLShaderProgram::CGLSLShaderProgram(std::initializer_list<CGLSLShader> shaders) 
{
		
	mProgramID = glCreateProgram();

	if (!mProgramID)
		throw std::runtime_error("Impossible to create a new shader program.");

	std::cout << "size : " << shaders.size() << std::endl;

	for (auto &s : shaders) 
	{
		
		glAttachShader(mProgramID, s.getShaderID());
		std::cout << "ID : " << s.getShaderID() << std::endl;
	}

	glLinkProgram(mProgramID);

	GLint isLinked;
	glGetProgramiv(mProgramID, GL_LINK_STATUS, &isLinked);

	if (isLinked == GL_FALSE) 
	{

		GLsizei logsize = 0;
		glGetProgramiv(mProgramID, GL_INFO_LOG_LENGTH, &logsize);

		char* log = new char[logsize];
		glGetProgramInfoLog(mProgramID, logsize, &logsize, log);
		std::cout << "| ERROR::::PROGRAM-LINKING-ERROR of type: SHADER PROGRAM |\n" << log << "\n| -- --------------------------------------------------- -- |" << std::endl;
	
		return;
	}

	for (auto &s : shaders)
	{
		glDetachShader(mProgramID, s.getShaderID());
	}
}

void 
CGLSLShaderProgram::use() const 
{
	glUseProgram(mProgramID);
}

void 
CGLSLShaderProgram::unuse() const 
{
	glUseProgram(0);
}

GLuint 
CGLSLShaderProgram::getProgramID() const 
{

	return mProgramID;
}

void 
CGLSLShaderProgram::setUniform(const std::string &name, const glm::mat4 &m) {
	
	glUniformMatrix4fv(glGetUniformLocation(mProgramID, name.c_str()), 1, GL_FALSE, glm::value_ptr(m));
}

void 
CGLSLShaderProgram::setUniform(const std::string &name, const glm::mat3 &m) {
	glUniformMatrix3fv(glGetUniformLocation(mProgramID, name.c_str()), 1, GL_FALSE, glm::value_ptr(m));
}

void 
CGLSLShaderProgram::setUniform(const std::string &name, const glm::vec3 &v) 
{
	glUniform3fv(glGetUniformLocation(mProgramID, name.c_str()), 1, glm::value_ptr(v));
}

void 
CGLSLShaderProgram::setUniform(const std::string &name, const glm::vec4 &v) 
{
	glUniform4fv(glGetUniformLocation(mProgramID, name.c_str()), 1, glm::value_ptr(v));
}

void 
CGLSLShaderProgram::setUniform(const std::string &name, float f) 
{
	glUniform1f(glGetUniformLocation(mProgramID, name.c_str()), f);
}

void 
CGLSLShaderProgram::setUniform(const std::string &name, int i) 
{
	glUniform1i(glGetUniformLocation(mProgramID, name.c_str()), i);
}


GLint 
CGLSLShaderProgram::getAttributeID(const std::string &name) {

	GLint attribute = glGetAttribLocation(mProgramID, name.c_str());

	if (attribute == GL_INVALID_OPERATION || attribute < 0)
		std::cout << "Attribute [" << name << "] doesn't exist." << std::endl;

	return attribute;

}

void 
CGLSLShaderProgram::setAttribute(const std::string &name, GLint size, GLsizei stride, GLuint offset, GLboolean normalize, GLenum type) 
{

	GLint location = getAttributeID(name);
	glEnableVertexAttribArray(location);
	glVertexAttribPointer(location, size, type, normalize, stride, reinterpret_cast<void*>(offset));

}

void 
CGLSLShaderProgram::setAttribute(const std::string &name, GLint size, GLsizei stride, GLuint offset, GLboolean normalize) 
{
	setAttribute(name, size, stride, offset, normalize, GL_FLOAT);
}

void 
CGLSLShaderProgram::setAttribute(const std::string &name, GLint size, GLsizei stride, GLuint offset, GLenum type) 
{
	setAttribute(name, size, offset, false, type);
}

void 
CGLSLShaderProgram::setAttribute(const std::string &name, GLint size, GLsizei stride, GLuint offset) 
{
	setAttribute(name, size, stride, offset, false, GL_FLOAT);
}



