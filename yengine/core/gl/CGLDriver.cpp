#include "CGLDriver.h"

CGLDriver::CGLDriver() : CGraphicDriver()
{
}

CGLDriver::~CGLDriver()
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
CGLDriver::init(CMesh *mesh) const
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
CGLDriver::bindMaterial(CMesh *mesh, CShaderFactory * shader) const
{
	if (mesh->mHasMaterial)
	{
		glEnable(GL_TEXTURE_2D);

		if (mesh->mMaterial->mTextureDiffuse != NULL)
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, mesh->mMaterial->mTextureDiffuse->mID);
			shader->shader->setUniform("texture_diffuse", 0);
		}

		if (mesh->mMaterial->mTextureNormal != NULL)
		{
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, mesh->mMaterial->mTextureNormal->mID);
			shader->shader->setUniform("texture_normal", 1);
		}

		if (mesh->mMaterial->mTextureSpecular != NULL)
		{
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, mesh->mMaterial->mTextureSpecular->mID);
			shader->shader->setUniform("texture_specular", 2);
		}

		shader->shader->setUniform("ambient", mesh->mMaterial->mAmbientColor);
		shader->shader->setUniform("diffuse", mesh->mMaterial->mDiffuseColor);
		shader->shader->setUniform("specular", mesh->mMaterial->mSpecularColor);
	}
}

void
CGLDriver::unbindMaterial(CMesh *mesh) const
{
	if (mesh->mHasMaterial)
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
}

void
CGLDriver::render(CMesh *mesh, CShaderFactory * shader) const
{
	bindMaterial(mesh, shader);
	glBindVertexArray(mesh->mVao);
	glDrawArrays(GL_TRIANGLES, 0, 3 * mesh->vertices.size());
	glBindVertexArray(0);
	unbindMaterial(mesh);
}

void
CGLDriver::clean(CMesh *mesh) const
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