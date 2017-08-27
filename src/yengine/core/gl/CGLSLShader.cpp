#include "CGLSLShader.h"

CGLSLShader::CGLSLShader(const GLchar* shaderFile, GLenum shaderType, std::string type) 
{

	// ----------------------------------------------------
	// ----- Retrieve the source code from input file -----
	// ----------------------------------------------------

	std::string code;
	std::ifstream shFile;

	// Ensures ifstream objects can throw exceptions:
	shFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try 
	{

		// Open files
		shFile.open(shaderFile);
		std::stringstream shaderStream;

		// Read file's buffer contents into streams
		shaderStream << shFile.rdbuf();

		// Close file handlers
		shFile.close();

		// Convert stream into string
		code = shaderStream.str();

	}
	catch (std::ifstream::failure e) 
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}

	const GLchar* shaderCode = code.c_str();

	// ----------------------------------------------------
	// ------------------ Compile shader ------------------
	// ----------------------------------------------------

	// Create shader
	mShaderID = glCreateShader(shaderType);
	glShaderSource(mShaderID, 1, &shaderCode, NULL);

	// Compilation
	glCompileShader(mShaderID);

	// Check compilation
	GLint compilationSuccess = 0;
	
	glGetShaderiv(mShaderID, GL_COMPILE_STATUS, &compilationSuccess);

	if (compilationSuccess == GL_FALSE) 
	{

		GLchar infoLog[1024];
		glGetShaderInfoLog(mShaderID, 1024, NULL, infoLog);
		std::cout << "| ERROR::::SHADER-COMPILATION-ERROR of type: " << type << "|\n" << infoLog << "\n| -- --------------------------------------------------- -- |" << std::endl;
	}
	else 
	{

		std::cout << type << " shader compiled successfully." << std::endl;

	}
}

CGLSLShader::~CGLSLShader()
{
	glDeleteShader(mShaderID);
}

GLuint CGLSLShader::getShaderID() const
{
	return mShaderID;
}