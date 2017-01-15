#include "object.h"


object::object(const char * path) :mObjectPath(path)
{

}

object::~object()
{
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexArrayAttrib(mVao, 0);
}

bool object::loadOBJ()
{
	// Some temporary variables in which we will store the contents of the .obj 
	std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
	std::vector< glm::vec3 > temp_vertices;
	std::vector< glm::vec2 > temp_uvs;
	std::vector< glm::vec3 > temp_normals;

	std::unique_ptr<FILE, int(*)(FILE*)> file(fopen(mObjectPath, "r"), fclose);

	if (file == NULL){
		printf("Fail to open the file !\n");
		return false;
	}
	else
	{
		std::cout << "Success to open OBJ !" << std::endl;
	}

	// Read lines to extract vertex, uv, normals
	while (1){

		char lineHeader[128];
		// read the first word of the line
		int res = fscanf(file.get(), "%s", lineHeader);
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.

		// else : parse lineHeader
		if (strcmp(lineHeader, "v") == 0){
			glm::vec3 vertex;
			fscanf(file.get(), "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.push_back(vertex);

		}
		else if (strcmp(lineHeader, "vt") == 0){
			glm::vec2 uv;
			fscanf(file.get(), "%f %f\n", &uv.x, &uv.y);
			temp_uvs.push_back(uv);
		}
		else if (strcmp(lineHeader, "vn") == 0){
			glm::vec3 normal;
			fscanf(file.get(), "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0)
		{
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			//int matches = fscanf_s(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2], sizeof(vertexIndex) + sizeof(uvIndex) + sizeof(normalIndex));
			//if (matches != 9){
				//printf("File can't be read by our simple parser : ( Try exporting with other options\n");
			unsigned int vi, vti, vni;
			unsigned int vi1, vti1, vni1;
			unsigned int vi2, vti2, vni2;

			fscanf(file.get(), "%d %d %d\n", &vi, &vi1, &vi2);
			vertexIndex[0] = vi;
			vertexIndex[1] = vi1;
			vertexIndex[2] = vi2;
			fscanf(file.get(), "%s", lineHeader);
			fscanf(file.get(), "%d %d %d\n", &vni, &vni1, &vni2);
			normalIndex[0] = vni;
			normalIndex[1] = vni1;
			normalIndex[2] = vni2;
			
				//return false;
			//}

			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			/*uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);*/
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}
	}

	// Indexing

	// For each vertex of each triangle
	for (unsigned int i = 0; i<vertexIndices.size(); i++)
	{
		unsigned int vertexIndex = vertexIndices[i];
		glm::vec3 vertex = temp_vertices[vertexIndex - 1];
		vertices.push_back(vertex);
	}

	// For each uv of each triangle
	for (unsigned int i = 0; i<uvIndices.size(); i++)
	{
		unsigned int uvIndex = uvIndices[i];
		glm::vec2 uv = temp_uvs[uvIndex - 1];
		uvs.push_back(uv);
	}

	// For each normal of each triangle
	for (unsigned int i = 0; i<normalIndices.size(); i++)
	{
		unsigned int normalIndex = normalIndices[i];
		glm::vec3 normal = temp_normals[normalIndex - 1];
		normals.push_back(normal);
	}

	return true;

}

void object::initBuffer()
{
	// Create the vertex array object
	glCreateVertexArrays(1, &mVao);
	
	// Create vertex buffer
	glCreateBuffers(1, &mVbuf);

	// Initialize the first buffer with data
	glNamedBufferStorage(mVbuf, vertices.size() * sizeof(glm::vec3), &vertices[0], 0);
	glVertexArrayVertexBuffer(mVao, 0, mVbuf, 0, 3 *sizeof(GLfloat));
	glVertexArrayAttribFormat(mVao, 0, 3, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(mVao, 0, 0);
	glEnableVertexArrayAttrib(mVao, 0);

	// Create texture coordinates buffer
	/*glCreateBuffers(1, &mUVbuf);

	// Initialize the second buffer with data
	glNamedBufferStorage(mUVbuf, uvs.size() * sizeof(glm::vec2), &uvs[0], 0);
	glVertexArrayVertexBuffer(mVao, 1, mUVbuf, 0, sizeof(glm::vec2));
	glVertexArrayAttribFormat(mVao, 1, 2, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(mVao, 1, 1);
	glEnableVertexArrayAttrib(mVao, 1);*/

	// Create texture coordinates buffer
	glCreateBuffers(1, &mNbuf);

	// Initialize the second buffer with data
	glNamedBufferStorage(mNbuf, normals.size() * sizeof(glm::vec3), &normals[0], 0);
	glVertexArrayVertexBuffer(mVao, 2, mNbuf, 0, sizeof(glm::vec3));
	glVertexArrayAttribFormat(mVao, 2, 3, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(mVao, 2, 2);
	glEnableVertexArrayAttrib(mVao, 2);

}

bool object::render()
{
	glBindVertexArray(mVao);
	glDrawArrays(GL_TRIANGLES, 0, 3 * vertices.size());
	glBindVertexArray(0);

	return true;
}