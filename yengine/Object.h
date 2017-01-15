#pragma once
#include <GL/glew.h>
#include <glm.hpp>
#include <vector>
#include <iostream>
#include <string>
#include <memory>

class object 
{
public:

	std::vector< glm::vec3 > vertices;
	std::vector< glm::vec2 > uvs;
	std::vector< glm::vec3 > normals;

private:

	const char * mObjectPath;
	GLuint mVao;
	GLuint mVbuf;
	GLuint mUVbuf;
	GLuint mNbuf;

public:
	object(const char * path);
	~object();
	bool loadOBJ();
	void initBuffer();
	bool render();

};