#include "CMaterial.h"

CMaterial::CMaterial(std::string name) :
	mMaterialName(name),
	mDiffuseColor(glm::vec3(0.5, 0.5, 0.5)),
	mAlpha(1.0),
	mShineness(0.0),
	mIlluminationModel(1),
	mRefractionIndex(1.0),
	mTransparentColor(glm::vec3(1.0))
{
	mTextureAmbient = NULL;
	mTextureDiffuse = NULL;
	mTextureSpecular = NULL;
	mTextureEmissive = NULL;
	mTextureOpacity = NULL;
	mTextureBump = NULL;
	mTextureNormal = NULL;
	mTextureDisp = NULL;
}

CMaterial::~CMaterial()
{
	if (mTextureAmbient != NULL)
		delete mTextureAmbient;

	if (mTextureDiffuse != NULL)
		delete mTextureDiffuse;

	if (mTextureSpecular != NULL)
		delete mTextureSpecular;

	if (mTextureEmissive != NULL)
		delete mTextureEmissive;

	if (mTextureOpacity != NULL)
		delete mTextureOpacity;

	if (mTextureBump != NULL)
		delete mTextureBump;

	if (mTextureNormal != NULL)
		delete mTextureNormal;

	if (mTextureDisp != NULL)
		delete mTextureDisp;
}

void
CMaterial::bind(CShaderFactory * shader)
{
	glEnable(GL_TEXTURE_2D);

	if (mTextureDiffuse != NULL)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mTextureDiffuse->mID);
		//glUniform1i(glGetUniformLocation(shader->shader->mProgramID, "texture_diffuse"), 0);
		shader->shader->setUniform("texture_diffuse", 0);
	}

	if (mTextureNormal != NULL)
	{
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, mTextureNormal->mID);
		glUniform1i(glGetUniformLocation(shader->shader->mProgramID, "texture_normal"), 1);
	}

	if (mTextureSpecular != NULL)
	{
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, mTextureSpecular->mID);
		glUniform1i(glGetUniformLocation(shader->shader->mProgramID, "texture_specular"), 2);
	}
	//m_customShader->shader->setUniform("projection_matrix", m_cam->projMatrix);
	//glUniform3i(glGetUniformLocation(shader->shader->mProgramID, "texDNS"), mTextureDiffuse->mID, mTextureNormal->mID, mTextureSpecular->mID);
	glUniform3f(glGetUniformLocation(shader->shader->mProgramID, "ambient"), mAmbientColor.r, mAmbientColor.g, mAmbientColor.b);
	glUniform3f(glGetUniformLocation(shader->shader->mProgramID, "diffuse"), mDiffuseColor.r, mDiffuseColor.g, mDiffuseColor.b);
	glUniform3f(glGetUniformLocation(shader->shader->mProgramID, "specular"), mSpecularColor.r, mSpecularColor.g, mSpecularColor.b);
}

void
CMaterial::unbind()
{
	if (mTextureDiffuse != NULL)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	if (mTextureNormal != NULL)
	{
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	if (mTextureSpecular != NULL)
	{
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	glDisable(GL_TEXTURE_2D);
}