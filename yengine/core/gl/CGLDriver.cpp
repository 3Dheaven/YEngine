#include "CGLDriver.h"
#include "CGLShaderFactory.h"

CGLDriver::CGLDriver() : CGraphicDriver()
{
}

CGLDriver::~CGLDriver()
{
}

void CGLDriver::createShader(const string &vertexShader, const string &pixelShader)
{
	if (mCustomShader == nullptr)
	{
		mCustomShader = new CGLShaderFactory(vertexShader, pixelShader);
		assert(mCustomShader != nullptr);
	}
}

template<typename T>
void createVertexBuffer(GLuint &vbId, GLuint vaoId, int vbIndex, unsigned int size, T &data)
{
	GLenum attribType = 3;

	if (std::is_same<T, glm::vec2>::value)
	{
		GLenum attribType = 2;
	}

	//glCreateBuffers(1, &vbId);
	glGenBuffers(1, &vbId);

    // Load data into vertex buffers
    glBindBuffer(GL_ARRAY_BUFFER, vbId);
    glBufferData(GL_ARRAY_BUFFER, size * sizeof(T), &data[0], GL_STATIC_DRAW);

	// Set the vertex attribute pointers
    glEnableVertexAttribArray(vbIndex);
    glVertexAttribPointer(0, attribType, GL_FLOAT, GL_FALSE, sizeof(T), (GLvoid*)0);
}

void
CGLDriver::init(CMesh *mesh)
{
	// Create vertex array object
	glGenVertexArrays(1, &mesh->mVao);
	glBindVertexArray(mesh->mVao);

	glGenBuffers(1, &mesh->mVbo);

	// Load data into vertex buffers
	glBindBuffer(GL_ARRAY_BUFFER, mesh->mVbo);
	glBufferData(GL_ARRAY_BUFFER, mesh->mVertices.size() * sizeof(sVertex), &mesh->mVertices[0], GL_STATIC_DRAW);

	// Vertex Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(sVertex), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Vertex Normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(sVertex), (GLvoid*)offsetof(sVertex, normal));

	// Vertex Texture Coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(sVertex), (GLvoid*)offsetof(sVertex, texcoord));
	
	// Create and fill element buffer object
	glGenBuffers(1, &mesh->mEbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->mEbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->mIndices.size() * sizeof(unsigned int), &mesh->mIndices[0], GL_STATIC_DRAW);
	
	glBindVertexArray(0);
}

void
CGLDriver::bindMaterial(CMesh *mesh, CShader* shader) const
{
	if (mesh->mHasMaterial)
	{
		glEnable(GL_TEXTURE_2D);

		if (mesh->mMaterial->mTextureDiffuse != NULL)
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, mesh->mMaterial->mTextureDiffuse->mID);
			shader->setUniform("texture_diffuse", 0);
		}

		if (mesh->mMaterial->mTextureNormal != NULL)
		{
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, mesh->mMaterial->mTextureNormal->mID);
			shader->setUniform("texture_normal", 1);
		}

		if (mesh->mMaterial->mTextureSpecular != NULL)
		{
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, mesh->mMaterial->mTextureSpecular->mID);
			shader->setUniform("texture_specular", 2);
		}

		shader->setUniform("ambient", mesh->mMaterial->mAmbientColor);
		shader->setUniform("diffuse", mesh->mMaterial->mDiffuseColor);
		shader->setUniform("specular", mesh->mMaterial->mSpecularColor);
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
CGLDriver::render(CMesh *mesh, CShader* shader)
{
	bindMaterial(mesh, shader);
	glBindVertexArray(mesh->mVao);
	glDrawElements(GL_TRIANGLES, mesh->mIndices.size(), GL_UNSIGNED_INT, (void*)0);
	glBindVertexArray(0);
	unbindMaterial(mesh);
}

void 
CGLDriver::render()
{

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

void 
CGLDriver::updateUniform(const char* name, glm::vec4& value)
{
	getShader()->use();
	getShader()->setUniform(name, value);
}

void 
CGLDriver::addUniform(const char* name, glm::vec4& value)
{
	getShader()->setUniform(name, value);
}

void 
CGLDriver::finalizeSetup()
{

}