#include "CMesh.h"

void 
CMesh::setupMesh()
{
	// Create the vertex array object
	glCreateVertexArrays(1, &mVao);

	// Create vertex buffer
	glCreateBuffers(1, &mVbuf);

	// Initialize the first buffer with data
	glNamedBufferStorage(mVbuf, vertices.size() * sizeof(glm::vec3), &vertices[0], 0);
	glVertexArrayVertexBuffer(mVao, 0, mVbuf, 0, 3 * sizeof(GLfloat));
	glVertexArrayAttribFormat(mVao, 0, 3, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(mVao, 0, 0);
	glEnableVertexArrayAttrib(mVao, 0);

	if (hasTexcoords)
	{
		// Create texture coordinates buffer
		glCreateBuffers(1, &mUVbuf);

		// Initialize the second buffer with data
		glNamedBufferStorage(mUVbuf, uvs.size() * sizeof(glm::vec2), &uvs[0], 0);
		glVertexArrayVertexBuffer(mVao, 1, mUVbuf, 0, sizeof(glm::vec2));
		glVertexArrayAttribFormat(mVao, 1, 2, GL_FLOAT, GL_FALSE, 0);
		glVertexArrayAttribBinding(mVao, 1, 1);
		glEnableVertexArrayAttrib(mVao, 1);
	}

	if (hasNormals)
	{
		// Create texture coordinates buffer
		glCreateBuffers(1, &mNbuf);

		// Initialize the second buffer with data
		glNamedBufferStorage(mNbuf, normals.size() * sizeof(glm::vec3), &normals[0], 0);
		glVertexArrayVertexBuffer(mVao, 2, mNbuf, 0, sizeof(glm::vec3));
		glVertexArrayAttribFormat(mVao, 2, 3, GL_FLOAT, GL_FALSE, 0);
		glVertexArrayAttribBinding(mVao, 2, 2);
		glEnableVertexArrayAttrib(mVao, 2);
	}
}

void 
CMesh::render()
{
	glBindVertexArray(mVao);
	glDrawArrays(GL_TRIANGLES, 0, 3 * vertices.size());
	glBindVertexArray(0);
}

CMesh::CMesh()
{
	hasDatas = false;
	hasNormals = false;
	hasTexcoords = false;
	isFinalized = false;
	m_name = "unknown";
}

CMesh::~CMesh()
{
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexArrayAttrib(mVao, 0);
}