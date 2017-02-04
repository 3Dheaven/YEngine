#include "CGraphicDriver.h"

CGraphicDriver::CGraphicDriver()
{
	vbIdCounter = 0;
}

CGraphicDriver::~CGraphicDriver()
{

}

template<typename T>
void createVertexBuffer(GLuint &vbId, GLuint vaoId, int vbIndex, unsigned int size, T &data)
{
	GLenum attribType = 3;

	if (std::is_same<T, glm::vec2>::value) 
	{ 
		GLenum attribType = 2;
	}
	
	// Create texture coordinates buffer
	glCreateBuffers(1, &vbId);

	// Initialize the second buffer with data
	glNamedBufferStorage(vbId, size * sizeof(T), &data, 0);
	glVertexArrayVertexBuffer(vaoId, vbIndex, vbId, 0, sizeof(T));
	glVertexArrayAttribFormat(vaoId, vbIndex, attribType, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(vaoId, vbIndex, vbIndex);
	glEnableVertexArrayAttrib(vaoId, vbIndex);
}

void
CGraphicDriver::init(CMesh *mesh)
{
	// Create the vertex array object
	glCreateVertexArrays(1, &mesh->mVao);

	// Create vertex buffer for positons
	createVertexBuffer(mesh->mVbuf, mesh->mVao, 0, mesh->vertices.size(), mesh->vertices[0]);
	
	if (mesh->mHasTexcoords)
	{
		// Create vertex buffer for texture coordinates
		createVertexBuffer(mesh->mUVbuf, mesh->mVao, 1, mesh->uvs.size(), mesh->uvs[0]);
	}

	if (mesh->mHasNormals)
	{
		// Create vertex buffer for normals
		createVertexBuffer(mesh->mNbuf, mesh->mVao, 1, mesh->normals.size(), mesh->normals[0]);
	}
}

void
CGraphicDriver::bindMaterial(CMesh *mesh, CShaderFactory * shader)
{
	glEnable(GL_TEXTURE_2D);

	if (mesh->mMaterial->mTextureDiffuse != NULL)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->mMaterial->mTextureDiffuse->mID);
		//glUniform1i(glGetUniformLocation(shader->shader->mProgramID, "texture_diffuse"), 0);
		shader->shader->setUniform("texture_diffuse", 0);
	}

	if (mesh->mMaterial->mTextureNormal != NULL)
	{
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, mesh->mMaterial->mTextureNormal->mID);
		glUniform1i(glGetUniformLocation(shader->shader->mProgramID, "texture_normal"), 1);
	}

	if (mesh->mMaterial->mTextureSpecular != NULL)
	{
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, mesh->mMaterial->mTextureSpecular->mID);
		glUniform1i(glGetUniformLocation(shader->shader->mProgramID, "texture_specular"), 2);
	}
	//m_customShader->shader->setUniform("projection_matrix", m_cam->projMatrix);
	//glUniform3i(glGetUniformLocation(shader->shader->mProgramID, "texDNS"), mTextureDiffuse->mID, mTextureNormal->mID, mTextureSpecular->mID);
	glUniform3f(glGetUniformLocation(shader->shader->mProgramID, "ambient"), mesh->mMaterial->mAmbientColor.r, mesh->mMaterial->mAmbientColor.g, mesh->mMaterial->mAmbientColor.b);
	glUniform3f(glGetUniformLocation(shader->shader->mProgramID, "diffuse"), mesh->mMaterial->mDiffuseColor.r, mesh->mMaterial->mDiffuseColor.g, mesh->mMaterial->mDiffuseColor.b);
	glUniform3f(glGetUniformLocation(shader->shader->mProgramID, "specular"), mesh->mMaterial->mSpecularColor.r, mesh->mMaterial->mSpecularColor.g, mesh->mMaterial->mSpecularColor.b);
}

void
CGraphicDriver::unbindMaterial(CMesh *mesh)
{
	if (mesh->mMaterial->mTextureDiffuse != NULL)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	if (mesh->mMaterial->mTextureNormal != NULL)
	{
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	if (mesh->mMaterial->mTextureSpecular != NULL)
	{
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	glDisable(GL_TEXTURE_2D);
}

void
CGraphicDriver::render(CMesh *mesh, CShaderFactory * shader)
{
	bindMaterial(mesh, shader);
	glBindVertexArray(mesh->mVao);
	glDrawArrays(GL_TRIANGLES, 0, 3 * mesh->vertices.size());
	glBindVertexArray(0);
	unbindMaterial(mesh);
}

void
CGraphicDriver::clean(CMesh *mesh)
{
	glDisableVertexAttribArray(0);

	if (mesh->mHasTexcoords)
	{
		glDisableVertexAttribArray(1);
	}

	if (mesh->mHasNormals)
	{
		glDisableVertexAttribArray(2);
	}

	glDisableVertexArrayAttrib(mesh->mVao, 0);
}
